/*
 * VHDLGenerator.cpp
 *
 *  Created on: 14 June 2021
 *      Author: jkmingwen
 */

#include <algorithm>
#include <bitset>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "VHDLGenerator.h"
#include "VHDLComponent.h"
#include "VHDLConnection.h"
#include "VHDLCircuit.h"
#include "algorithms/schedulings.h"
#include "algorithms/transformation/merge_operators.h"
#include "algorithms/transformation/merge_output.h"
#include "algorithms/vhdl_generation/VHDLWrapper.h"
#include "commons/KiterRegistry.h"
#include "commons/commons.h"
#include "commons/verbose.h"
#include <algorithms/transformation/singleOutput.h>
#include <algorithms/transformation/iterative_evaluation.h>
#include <printers/SDF3Wrapper.h>
#include "VHDLCommons.h"
#include "models/Dataflow.h"
#include "printers/stdout.h"

// for signal name retrieval
#define VALID 0
#define READY 1
#define DATA 2

// Configuration parameters for VHDL generation; initialised in generateVHDL
std::string topDir;
std::string componentDir;
std::string tbDir; // testbench directory
std::string referenceDir = "./src/libkiter/algorithms/vhdl_generation/reference_files/";
int operatorFreq = 250; // clock frequency (in MHz) VHDL operators are designed to run at
int bitWidth = 34;
bool dataDriven = false; // if VHDL design is data driven using HS protocol
implType implementationType = TT;
bool osBroadcast = false;
int systemPeriod = opFreqAndPeriod.at(operatorFreq);
int systemSlack = 1;   // lag given to audio interfacing (in cycles) after
                         // expected arrival of audio sample
int computeL = 0; // total compute time for left channel
int computeR = 0; // total compute time for right channel
std::string bufferImpl = "sbuffer"; // defines type of buffer to be implemented
                                    // in time triggered implementation

VHDLCircuit generateCircuitObject(models::Dataflow* const dataflow, int opFreq, implType t) {

  VHDLCircuit circuit(t);
  std::string circuitName = dataflow->getGraphName();
  // replace prohibited characters with underscores
  std::replace(circuitName.begin(), circuitName.end(), '-', '_');
  std::replace(circuitName.begin(), circuitName.end(), '.', '_');
  circuit.setName(circuitName);
  VERBOSE_DEBUG("circuit name: " << circuitName);

  // populate circuit object with components and connections based on dataflow graph
  {ForEachVertex(dataflow, actor) {
      VHDLComponent newComp(dataflow, actor, opFreq, t);
      circuit.addComponent(newComp);
      // update execution time in dataflow according to component operator type
      VERBOSE_INFO("operator lifespan ("
                   << newComp.getType() << "): "
                   << getOperatorLifespan(newComp.getType(), opFreq));
      if (newComp.getType() != "input_selector" &&
          newComp.getType() != "output_selector") { // input/output selector lifespans are already set in merge_operators
        std::vector<TIME_UNIT> opLifespans(
            dataflow->getVertexPhaseDuration(actor).size(),
            (TIME_UNIT)getOperatorLifespan(newComp.getType(), opFreq));
        dataflow->setVertexDuration(actor, opLifespans);
      }
    }}
  {ForEachEdge(dataflow, edge) {
      VHDLConnection newConn(dataflow, edge);
      circuit.addConnection(newConn);
    }}
  circuit.updateTopLevelPorts(implementationType);

  return circuit;
}

/**
   Generates VHDL code for the given dataflow graph

   @param dataflow HSDF/SDF graph; generate with Faust using `-sdf` flag.

   @param param_list Optional parameters to dictate configuration of VHDL
   generation. Parameters are as follows:
   - OUTPUT_DIR: Path to directory where VHDL code will be generated. Note that
   VHDL code will only be generated if this is specified.
   - REFERENCE_DIR: Path to directory containing VHDL reference files. Shouldn't
   be set unless you have your own directory of reference files.
   - FULLY_BUFFERED: 't' to activate; indicates whether every connection in the VHDL
   design should include a buffer. Set to false by default.
   - FREQUENCY: Clock cycle frequency (in MHz) of VHDL design.
   - SLACK: System slack given (in clock cycles) to exec time of scheduled
   operators; may be necessary to account for phase shifts due to mismatching
   operating frequency of audio interface clock and generated VHDL design.
   - NORMALISE_OUTPUTS: Enforce single outputs for all operators.
   - BROADCAST: Change output selector behaviour to simply broadcast input
   data and add buffers to each output edge.
   - BUFFER_TYPE: Use specified buffer type instead of scheduled buffers for
   buffer components in VHDL implementation. Options: shiftreg/sbuffer (default).

   @return void; VHDL code generated in location specified in OUTPUT_DIR
   parameter.
 */

void algorithms::generateVHDL(models::Dataflow* const dataflow, parameters_list_t param_list) {

  // default directories/parameters defined here
  topDir      = "./" + dataflow->getGraphName() + "_vhdl_gen/"; // where VHDL files will be generated
  componentDir = topDir + "/components/"; // VHDL component implementations generated here
  tbDir = topDir + "/testbenches/";
  bool outputDirSpecified = false;

  // to store schedule for VHDL implementation
  std::map<std::string, std::vector<TIME_UNIT>>
      execTimes; // actor names -> execution times // TODO try with actor ID
                 // instead of names
  models::Scheduling res;
  // purely for latency computation
  std::map<std::string, std::vector<TIME_UNIT>> execTimesLatency;
  models::Scheduling resLatency;

  // check for specified VHDL output directory
  if (param_list.find("OUTPUT_DIR") != param_list.end()) {
    outputDirSpecified = true;
    topDir = param_list["OUTPUT_DIR"] + "/";
    componentDir = topDir + "/components/";
    tbDir = topDir + "/testbenches/";
    VERBOSE_INFO("Update output directory to " << topDir);

    // create topDir/componentDir directories if they don't already exist
    std::filesystem::create_directories(componentDir);
  } else {
    VERBOSE_WARNING("Please use '-p OUTPUT_DIR=topDir' to set output directory. No VHDL code will be generated without a specified output directory.");
  }

  // check if FIFO buffers should be generated in VHDL implementation
  if (param_list.find("FULLY_BUFFERED") != param_list.end() &&
      param_list["FULLY_BUFFERED"] == "t") {
    VERBOSE_INFO("Fully buffered implementation activated");
    {ForEachEdge(dataflow, e) {
        if (!dataflow->getPreload(e)) {
          dataflow->setPreload(e, 1);
        }
      }}
  }

  // check if operator frequencies have been specified
  if (param_list.find("FREQUENCY") != param_list.end()) {
    VERBOSE_INFO("Operator frequency set to " << param_list["FREQUENCY"]);
    operatorFreq = std::stoi(param_list["FREQUENCY"]);
    if (!opFreqAndPeriod.count(operatorFreq)) {
      VERBOSE_ERROR("Unsupported operator frequency requested: " << operatorFreq);
    }
    systemPeriod = opFreqAndPeriod.at(operatorFreq);
  } else {
    VERBOSE_INFO("Default operator frequency used (" << operatorFreq << "), you can use -p FREQUENCY=frequency_in_MHz to set the operator frequency");
  }

  // check if system slack defined
  if (param_list.find("SLACK") != param_list.end()) {
    VERBOSE_INFO("Operator frequency set to " << param_list["SLACK"]);
    systemSlack = std::stoi(param_list["SLACK"]);
  } else {
    VERBOSE_INFO("Default system slack used (" << systemSlack << "), you can use -p SLACK=slack_in_clock_cycles to set system slack");
  }

  // define location of VHDL generation reference files
  if (param_list.find("REFERENCE_DIR") != param_list.end()) {
    referenceDir = param_list["REFERENCE_DIR"] + "/";
    VERBOSE_INFO("Update reference directory to " << referenceDir);
  } else {
    VERBOSE_INFO("Default reference in use, you can use -p REFERENCE_DIR=/path/to/reference_directory/ to set reference directory");
  }

  VERBOSE_INFO ("Looking in " << referenceDir << " for VHDL generation reference files");
  if (!std::filesystem::is_directory(referenceDir)) {
      VERBOSE_ERROR("Reference directory is not found.");
      VERBOSE_FAILURE();
  }

  if (param_list.find("DATA_DRIVEN") != param_list.end()) {
    VERBOSE_INFO("Using data-driven implementation for VHDL design");
    dataDriven = true;
    implementationType = DD;
  }

  if (param_list.find("GS") != param_list.end()) {
    implementationType = GS;
  }

  if (param_list.find("BROADCAST") != param_list.end()) {
    VERBOSE_INFO("Add buffers to the output edges of output selectors");
    osBroadcast = true;
  }

  if (param_list.find("BUFFER_TYPE") != param_list.end()) {
    VERBOSE_INFO("Set buffer implementation to type: " << param_list["BUFFER_TYPE"]);
    bufferImpl = param_list["BUFFER_TYPE"];
  }

  while (getMultiOutputActors(dataflow).size() > 0) {
    VERBOSE_INFO("getMultiOutputActors is not empty");
    for (std::string actorName : getMultiOutputActors(dataflow)) {
      parameters_list_t parameters;
      parameters["name"] = actorName;
      VERBOSE_INFO("merge output for actor " << actorName);
      try {
        transformation::merge_output(dataflow, parameters);
      } catch (...) {
        VERBOSE_WARNING("actor missing!");
      }
    }
    VERBOSE_INFO("Regenerate Circuit");
  }

  // Generate schedule for given VHDL implementation
  if (osBroadcast) {
    /* use audio component artifacts for scheduling (scheduledDataflow),
       and broadcast transformed graph (dataflow) for VHDL implementation */
    /* in order to generate a schedule defining execution times of buffers on
       the output edges of broadcast actors, it's necessary to schedule the
       dataflow without those buffers to compute the start times of the output
       selector. These start times are then passed on to the buffers that are
       added afterwards. */
    models::Dataflow *scheduledDataflow = new models::Dataflow(*dataflow);
    algorithms::transformation::generate_audio_components(scheduledDataflow,
                                                          param_list);
    VERBOSE_ASSERT(computeRepetitionVector(scheduledDataflow),
                   "inconsistent graph");
    res = scheduling::CSDF_1PeriodicScheduling(scheduledDataflow, 0);
    for (const auto &item : res.getTaskSchedule()) {
      std::string actorBaseName =
        scheduledDataflow->getVertexName(scheduledDataflow->getVertexById(item.first));
      actorBaseName = getBaseName(actorBaseName);
      execTimes[actorBaseName] = item.second.periodic_starts.second;
    }
    // Workaround for latency computation
    models::Dataflow *latencyDataflow = new models::Dataflow(*dataflow);
    param_list["CODEC_PERIOD"] = "0";
    algorithms::transformation::generate_audio_components(latencyDataflow,
                                                          param_list);
    param_list.erase(param_list.find("CODEC_PERIOD"));
    VERBOSE_ASSERT(computeRepetitionVector(latencyDataflow),
                   "inconsistent graph");
    resLatency = scheduling::CSDF_1PeriodicScheduling(latencyDataflow, 0);
    for (const auto &item : resLatency.getTaskSchedule()) {
      std::string actorBaseName =
        latencyDataflow->getVertexName(latencyDataflow->getVertexById(item.first));
      actorBaseName = getBaseName(actorBaseName);
      execTimesLatency[actorBaseName] = item.second.periodic_starts.second;
    }
    algorithms::transformation::broadcast_os(dataflow, param_list);
  } else {
    models::Dataflow *scheduledDataflow = new models::Dataflow(*dataflow);
    algorithms::transformation::generate_audio_components(scheduledDataflow,
                                                          param_list);
    VERBOSE_ASSERT(computeRepetitionVector(scheduledDataflow),
                   "inconsistent graph");
    res = scheduling::CSDF_1PeriodicScheduling(scheduledDataflow, 0);
    for (const auto &item : res.getTaskSchedule()) {
      execTimes[scheduledDataflow->getVertexName(scheduledDataflow->getVertexById(item.first))] = item.second.periodic_starts.second;
    }
    // Workaround for latency computation
    models::Dataflow *latencyDataflow = new models::Dataflow(*dataflow);
    param_list["CODEC_PERIOD"] = "0";
    algorithms::transformation::generate_audio_components(latencyDataflow,
                                                          param_list);
    param_list.erase(param_list.find("CODEC_PERIOD"));
    VERBOSE_ASSERT(computeRepetitionVector(latencyDataflow),
                   "inconsistent graph");
    resLatency = scheduling::CSDF_1PeriodicScheduling(latencyDataflow, 0);
    for (const auto &item : resLatency.getTaskSchedule()) {
      execTimesLatency[latencyDataflow->getVertexName(latencyDataflow->getVertexById(item.first))] = item.second.periodic_starts.second;
    }
  }

  // Update placeholder buffer type to specified implementation after graph
  // transformations to generate circuit object for VHDL generation
  {ForEachVertex(dataflow, v) {
      std::string vertexType = dataflow->getVertexType(v);
      if (implementationType == TT || implementationType == GS) {
        if (vertexType == "buffer") {
          dataflow->setVertexType(v, bufferImpl);
        }
      } else {
        if (vertexType == "buffer") {
          dataflow->setVertexType(v, "hs_fifo");
        }
      }
      if (unsupportedOperators.find(vertexType) != unsupportedOperators.end()) {
        VERBOSE_WARNING("Unsupported operator "
                        << vertexType << " detected; unable to generate VHDL");
        return;
      }
    }}
  VHDLCircuit tmp = generateCircuitObject(dataflow, operatorFreq, implementationType); // VHDLCircuit object specifies operators and how they're connected
  VHDLScheduler schedule;
  schedule.setPeriod(systemPeriod);
  std::map<int, TIME_UNIT> inputStarts;
  std::map<int, TIME_UNIT> outputEnds;
  for (auto &[v, comp] : tmp.getComponentMap()) {
    std::string name = dataflow->getVertexName(v);
    if (osBroadcast) { name = getBaseName(name); }
    if (execTimes.count(name)) {
      // add execTimes element as actor exec time
      std::vector<TIME_UNIT> startTimes(execTimes[name]);
      std::vector<TIME_UNIT> latencyStartTimes(execTimesLatency[name]);
      std::vector<TIME_UNIT> bufferPopTime;
      if (comp.getType() == "INPUT") {
        VERBOSE_ASSERT(startTimes.size() == 1, "Input actor should only have 1 start time");
        tmp.addInExecTime(comp.getIOId(), startTimes.front());
        inputStarts[comp.getIOId()] = latencyStartTimes.front();
      }
      if (comp.getType() == "OUTPUT") {
        VERBOSE_ASSERT(startTimes.size() == 1, "Output actor should only have 1 start time");
        // Output buffers need to account for time taken to convert from fixed point to FP
        int conversionTime = getOperatorLifespan("fp2fix", operatorFreq) +
                             getOperatorLifespan("fp_prod", operatorFreq);
        tmp.addComputeTime(comp.getIOId(), startTimes.front() + conversionTime);
        outputEnds[comp.getIOId()] = latencyStartTimes.front() + conversionTime;
      }
      if (comp.getType() == "sbuffer" || comp.getType() == "shiftreg" || comp.getType() == "delay") {
        {ForOutputEdges(dataflow, v, outEdge) {
            // actor names after "_" redundant (only indicate order of args)
            Vertex dstActor = dataflow->getEdgeTarget(outEdge);
            std::string dstName = dataflow->getVertexName(dstActor);
            if (osBroadcast) { dstName = getBaseName(dstName); }
            std::vector<TIME_UNIT> dstActorStarts = execTimes.at(dstName);
            for (auto i = 0; i < dataflow->getEdgeOutPhasesCount(outEdge); i++) {
              if (dataflow->getEdgeOutVector(outEdge)[i] == 1) {
                bufferPopTime = {dstActorStarts[i]};
              }
            }
          }}
      }
      if (implementationType == TT) {
        tmp.setCompStartTime(comp.getUniqueName(), startTimes, bufferPopTime,
                             (TIME_UNIT)systemSlack);
      }
      if (implementationType == GS) {
        if (comp.getType() == "input_selector" ||
            comp.getType() == "output_selector") {
          tmp.addExecution(schedule, comp.getUniqueName(), "trigger_exec",
                           startTimes, systemSlack);
        } else if (comp.getType() == "sbuffer" || comp.getType() == "shiftreg" || comp.getType() == "delay") {
          tmp.addExecution(schedule, comp.getUniqueName(), "trigger_push",
                           startTimes, systemSlack);
          // Subtract 1 from pop time to account for 1 cycle delay between
          // pop time and data output
          tmp.addExecution(schedule, comp.getUniqueName(), "trigger_pop",
                           {bufferPopTime.front() - 1}, systemSlack);
        }
      }
    } else if ((comp.getType() == "sbuffer" || comp.getType() == "shiftreg") &&
               dataflow->getPhasesQuantity(v) > 1) {
      // buffers used for broadcasting OS signal have >1 exec phase (equal to
      // number of outputs of broadcast )
      // we use prod exec rates as a mask to identify exec time relevant to given buffer
      VERBOSE_ASSERT(comp.getInputEdges().size() == 1,
                     "buffers should only have 1 input");
      std::string srcName = dataflow->getVertexName(dataflow->getEdgeSource(
          dataflow->getEdgeByName(comp.getInputEdges().front())));
      srcName = getBaseName(srcName);
      std::vector<TIME_UNIT> srcOSStarts = execTimes.at(srcName);
      TIME_UNIT startTime = 0;
      TIME_UNIT dstStartTime = 0;
      {ForOutputEdges(dataflow, v, outEdge) {
          for (auto i = 0; i < dataflow->getEdgeInPhasesCount(outEdge); i++) {
            if (dataflow->getEdgeInVector(outEdge)[i] == 1) {
              startTime = srcOSStarts[i];
            }
          }
          Vertex dstActor = dataflow->getEdgeTarget(outEdge);
          std::string dstName =
              dataflow->getVertexName(dstActor);
          dstName = getBaseName(dstName);
          std::vector<TIME_UNIT> dstActorStarts = execTimes.at(dstName);
          for (auto i = 0; i < dataflow->getEdgeOutPhasesCount(outEdge); i++) {
            if (dataflow->getEdgeOutVector(outEdge)[i] == 1) {
              dstStartTime = dstActorStarts[i];
            }
          }
        }
      }
      srcOSStarts = {startTime};
      if (implementationType == TT) {
        tmp.setCompStartTime(comp.getUniqueName(), srcOSStarts, {dstStartTime},
                             (TIME_UNIT)systemSlack);
      } else if (implementationType == GS) {
        if (comp.getType() == "input_selector" ||
            comp.getType() == "output_selector") {
          tmp.addExecution(schedule, comp.getUniqueName(), "trigger_exec",
                           srcOSStarts, systemSlack);
        } else if (comp.getType() == "sbuffer" || comp.getType() == "shiftreg" || comp.getType() == "delay") {
          tmp.addExecution(schedule, comp.getUniqueName(), "trigger_push",
                           srcOSStarts, systemSlack);
          // Subtract 1 from pop time to account for 1 cycle delay between
          // pop time and data output
          tmp.addExecution(schedule, comp.getUniqueName(), "trigger_pop",
                           {dstStartTime - 1}, systemSlack);
        }
      }
      if (param_list.find("BUFFER_MIN") != param_list.end()) {
        if (startTime + 1 == dstStartTime) { // use output selector execution time (1)
          tmp.bypassBufferComponent(comp.getUniqueName());
        }
      }
    } else {
      VERBOSE_WARNING("No schedule generated for "
                      << comp.getUniqueName() << " (" << comp.getType() << ")");
    }
  }

  if (outputDirSpecified) { // only produce actual VHDL files if output directory specified
    const auto copyOptions = std::filesystem::copy_options::update_existing |
                             std::filesystem::copy_options::recursive;
    if (implementationType == TT || implementationType == DD) { // TODO merge these cases
      generateOperators(tmp);
      VHDLWrapper audioInterfaceWrapper =
        VHDLWrapper(tmp, implementationType, systemPeriod, systemSlack, topDir);
      std::ofstream vhdlOutput;
      vhdlOutput.open(topDir + tmp.getName() + "_top.vhdl");
      audioInterfaceWrapper.writeImplementation(vhdlOutput);
      vhdlOutput.close();
    } else if (implementationType == GS) {
      generateOperators(tmp);
      tmp.externalPortsInit();
      VHDLWrapper audioInterfaceWrapper = VHDLWrapper(
          tmp, schedule, implementationType, systemPeriod, systemSlack, topDir);
      // only instantiate circuit after instantiating wrapper as schedule width is updated
      std::ofstream vhdlOutput;
      vhdlOutput.open(topDir + tmp.getName() + "_top.vhdl");
      audioInterfaceWrapper.writeImplementation(vhdlOutput);
      vhdlOutput.close();
      audioInterfaceWrapper.writeSchedulerImplementation(topDir);
    }
    std::filesystem::copy(referenceDir + "/testbenches/", tbDir, copyOptions);
    printers::writeSDF3File(topDir + "exectimes.xml", dataflow);
    VERBOSE_INFO("VHDL files generated in: " << topDir);
    std::ofstream latencyOut;
    latencyOut.open(topDir + "latency.txt");
    latencyOut << computeLatency(inputStarts, outputEnds) << std::endl;
    latencyOut.close();
    // print schedule and corresponding signal graph
    // TODO fix schedule generation given separation of transformation and VHDL implementation
    // std::ofstream tikzFile;
    // tikzFile.open(topDir + dataflow->getGraphName() + "_schedule.tex");
    // tikzFile << generateTikzSchedule(res) << std::endl;
    // tikzFile.close();
    //// print diagram of graph that is being scheduled for the
    //// VHDL implementation (we don't use broadcastTimingModel
    //// graph here as it doesn't include the buffers)
    // param_list["filename"] =
    //     topDir + dataflow->getGraphName() + "_scheduledmodel.dot";
    // printers::printSigGraph(dataflowScheduled, param_list);
    // printers::writeSDF3File(topDir + dataflow->getGraphName() + "_scheduledmodel.xml", dataflowScheduled);
    // generate SDF XML and diagram of the graph after applying various
    // implementation strategies
    printers::writeSDF3File(
        topDir + dataflow->getGraphName() + "_implementation.xml", dataflow);
    param_list["filename"] =
      topDir + dataflow->getGraphName() + "_implementation.dot";
    printers::printSigGraph(dataflow, param_list);

    // // generate PIPO numbers
    // std::ofstream pipoCSV;
    // std::string nameAndMerge = "flat";
    // if (param_list.find("MERGE_STRATEGY") != param_list.end()) {
    //   nameAndMerge = param_list["MERGE_STRATEGY"];
    // }
    // nameAndMerge = tmp.getName() + "," + nameAndMerge + ",";
    // pipoCSV.open(topDir + "pipo_numbers.csv");
    // pipoCSV << "graph,merge,depth,load,period" << std::endl;
    // for (auto &[v, comp] : tmp.getComponentMap()) {
    //   if (comp.getType() == "shiftreg") {
    //     pipoCSV << nameAndMerge << comp.writePIPOCSV() << std::endl;
    //   }
    // }
    // pipoCSV.close();
  } else {
    VERBOSE_WARNING("No VHDL files created.");
  }

  std::cout << tmp.printStatus() << std::endl;

}

void algorithms::generateOperators(const VHDLCircuit &circuit) {
  std::map<std::string, int> operatorMap = circuit.getOperatorMap();
  std::map<std::string, int> trackImplementations; // to check if component has been implemented
  for (auto const &[v, comp] : circuit.getComponentMap()) {
    if (comp.getType() != "INPUT" && comp.getType() != "OUTPUT") {
      std::string name = comp.getPortMapName();
      if (!trackImplementations.count(name)) {
        trackImplementations[name] = 1;
        comp.genImplementation(referenceDir, componentDir);
      }
    }
  }
  generateAudioInterfaceComponents();
  if (dataDriven) { // include HS interface components for data driven operator execution
    generateHSInterfaceComponents();
  }
}

// Copy FloPoCo operator from reference file to project
void algorithms::generateFPCOperator(std::string compImplementationName) {
  std::string operatorRefDir = referenceDir + "/operators/";
  std::string operatorFileName = compImplementationName + "_f" + std::to_string(operatorFreq);
  const auto copyOptions = std::filesystem::copy_options::update_existing |
                           std::filesystem::copy_options::recursive;

  std::filesystem::copy(operatorRefDir + operatorFileName + ".vhdl",
                        componentDir + operatorFileName + ".vhdl", copyOptions);
}


void algorithms::generateCircuit(VHDLCircuit &circuit) {
  std::ofstream vhdlOutput;
  std::string graphName = circuit.getName() + "_circuit"; // TODO decide on naming convention

  vhdlOutput.open(topDir + graphName + ".vhdl"); // instantiate VHDL file
  circuit.writeImplementation(vhdlOutput);

}

// Copy FloPoCo-HS interface component specification from reference files to generated subdirectory
void algorithms::generateHSInterfaceComponents() {
  // names of reference files required to copy into project; add/remove as required
  // TODO only produce the HS component files if necessary; right now, we're just writing every file
  std::vector<std::string> componentNames = {"hs_merger", "countdown",
                                             "store_send", "hs_merger_negate",
                                             "hs_merger_one", "hs_merger_three", "hs_merger_switch_ui"};

  for (const auto &component : componentNames) {
    const auto copyOptions = std::filesystem::copy_options::update_existing |
                             std::filesystem::copy_options::recursive;
    std::filesystem::copy(referenceDir + component + ".vhdl",
                          componentDir + component + ".vhdl", copyOptions);
  }
}

// Copy VHDL components necessary for interfacing with the audio codec from reference files to generated subdirectory
void algorithms::generateAudioInterfaceComponents() {
  // names of reference files required to copy into project; add/remove as required
  std::vector<std::string> componentNames;
  std::vector<std::string> operatorNames; // need separate path for FloPoCo operators as they're stored in different subdirectory
  if (implementationType == TT) {
    componentNames = {"fix2fp_and_scaledown", "fp2fix_and_scaleup",
                      "i2s_transceiver", "cycle_counter", "sbuffer",
                      "sbuffer_n", "sbuffer_bypass", "sbuffer_one"};
    // separate path for FloPoCo operators as they're stored in different subdirectory
    operatorNames = {"fix2fp_flopoco", "fp2fix_flopoco", "fp_prod_flopoco"};
  } else if (implementationType == GS) {
    componentNames = {"fix2fp_and_scaledown", "fp2fix_and_scaleup",
                      "i2s_transceiver", "cycle_counter", "sbuffer_t",
                      "sbuffer_t_n", "sbuffer_t_bypass", "sbuffer_t_one"};
    // separate path for FloPoCo operators as they're stored in different subdirectory
    operatorNames = {"fix2fp_flopoco", "fp2fix_flopoco", "fp_prod_flopoco"};
  } else {
    componentNames = {// send audio data in accordance to handshake protocol
      "input_interface", "output_interface",
      // convert and scale data coming to and from audio codec (fixed point to float)
      "i2s_to_fpc", "fpc_to_i2s", "fix2fp_and_scaledown",
      "fp2fix_and_scaleup",
      "i2s_transceiver"}; // expose data from ADC/DAC to PL
    // separate path for FloPoCo operators as they're stored in different subdirectory
    operatorNames =
        {"fix2fp_flopoco", "fp2fix_flopoco", "fp_prod_flopoco"};
  }
  std::map<std::string, std::string> replacementWords = {
    {"$OP_FREQ", std::to_string(operatorFreq)},
    {"$I2S_TO_FPC_LIFESPAN",
     std::to_string(getOperatorLifespan("fix2fp", operatorFreq) +
                    getOperatorLifespan("fp_prod", operatorFreq))},
    {"$FPC_TO_I2S_LIFESPAN", std::to_string(getOperatorLifespan("fp2fix", operatorFreq) + getOperatorLifespan("fp_prod", operatorFreq))}}; // name component according to operator frequency
  for (const auto &component : componentNames) {
    copyFileAndReplaceWords(referenceDir + component + ".vhdl",
                            componentDir + component + ".vhdl",
                            replacementWords);
  }
  for (const auto &op : operatorNames) {
    const auto copyOptions = std::filesystem::copy_options::update_existing |
      std::filesystem::copy_options::recursive;
    std::filesystem::copy(referenceDir + "/operators/" + op + "_f" +
                          std::to_string(operatorFreq) + ".vhdl",
                          componentDir + op + "_f" +
                          std::to_string(operatorFreq) + ".vhdl",
                          copyOptions);
  }
}


void algorithms::printCircuitInfo(models::Dataflow* const dataflow,
                                  parameters_list_t param_list) {
  VHDLCircuit circuit;
  circuit.setName(dataflow->getGraphName());

  // populate circuit object with components and connections based on dataflow
  {ForEachVertex(dataflow, actor) {
      VHDLComponent newComp(dataflow, actor, operatorFreq, implementationType);
      circuit.addComponent(newComp);
    }}
  {ForEachEdge(dataflow, edge) {
      VHDLConnection newConn(dataflow, edge);
      circuit.addConnection(newConn);
    }}

  std::cout << circuit.printStatus() << std::endl;
  if (circuit.getMultiOutActors().size()) {
    std::cout << "Actors with abnormal number of outputs detected:\n" << std::endl;
  }
  return;
}

void algorithms::printOperatorCounts(models::Dataflow *const dataflow,
                                     parameters_list_t param_list) {
  std::stringstream csvOut;
  std::map<std::string, int> opCounts;
  std::string appName = dataflow->getGraphName();

  {ForEachVertex(dataflow, actor) {
      std::string opType = deriveOpCat(dataflow, actor);
      if (opCounts.count(opType)) {
        opCounts[opType]++;
      } else {
        opCounts[opType] = 1;
      }
    }}
  // no header so we can append counts to the same file
  for (auto const &[opName, count] : opCounts) {
    csvOut << appName << "," << opName << "," << count << std::endl;
  }
  std::cout << csvOut.str() << std::flush;

  return;
}

TIME_UNIT computeLatency(std::map<int, TIME_UNIT> inStarts, std::map<int, TIME_UNIT> outEnds) {
  std::set<TIME_UNIT> latencies{0};

  for (auto const [chId, outTime] : outEnds) {
    TIME_UNIT chLatency = 0;
    VERBOSE_DEBUG("Computing latency for channel " << chId);
    if (inStarts.count(chId)) {
      VERBOSE_DEBUG("\tInput start: " << inStarts.at(chId));
      VERBOSE_DEBUG("\tOutput end: " << outTime);
      if (inStarts.at(chId) > outTime) {
        chLatency = outTime;
      } else {
        chLatency = outTime - inStarts.at(chId);
      }
    } else {
      VERBOSE_DEBUG("\tNo input channel");
      VERBOSE_DEBUG("\tOutput end: " << outTime);
      chLatency = outTime;
    }
    VERBOSE_DEBUG("\tChannel latency: " << chLatency);
    latencies.insert(chLatency);
  }

  return *latencies.rbegin(); // return highest latency
}
