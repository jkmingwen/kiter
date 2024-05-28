/*
 * VHDLGenerator.cpp
 *
 *  Created on: 14 June 2021
 *      Author: jkmingwen
 */

#include <bitset>
#include <string>
#include <filesystem>
#include "VHDLGenerator.h"
#include "VHDLComponent.h"
#include "VHDLConnection.h"
#include "VHDLCircuit.h"
#include "commons/verbose.h"
#include <algorithms/transformation/singleOutput.h>
#include <printers/SDF3Wrapper.h>

// for signal name retrieval
#define VALID 0
#define READY 1
#define DATA 2

// Configuration parameters for VHDL generation; initialised in generateVHDL
std::string topDir;
std::string componentDir;
std::string tbDir; // testbench directory
std::string referenceDir = "./src/libkiter/algorithms/vhdl_generation/reference_files/";
int operatorFreq = 125; // clock frequency (in MHz) VHDL operators are designed to run at
bool isBufferless = false; // if VHDL design should include FIFO buffers along every connection
int bitWidth = 34;
bool dataDriven = false; // if VHDL design is data driven using HS protocol

/* Each component has a specific lifespan and name that needs to be defined in
   the generated AXI interface --- we track them using a predefined map where
   the keys are the operating frequencies. The "types" of the operators,
   which are taken from the actor types in the SDF file generated from the Faust
   application, then map to the number of clock cycles taken for them to complete */
std::map<int, std::map<std::string, int>> operatorLifespans =
  {
    {50,
     {{"fp_add", 1}, {"fp_prod", 1}, {"fp_div", 3}, {"fp_sqrt", 1},
      {"fp_diff", 1}, {"fp_pow", 3}, {"int_add", 1}, {"int_prod", 1},
      {"int_diff", 1}, {"float2int", 1}, {"int2float", 1}, {"sbuffer", 2},
      // NOTE unimplemented operators from here:
      {"fp_floor", 2}, {"int_max", 1}, {"int_min", 1}, {"fp_max", 1},
      {"fp_min", 1}, {"fp_abs", 1}, {"select2", 1},
      // {"select3", 1},
      {"attach", 1}, {"int_abs", 1}, {"vbargraph", 1}}},
    {125,
     {{"fp_add", 3}, {"fp_prod", 1}, {"fp_div", 8}, {"fp_sqrt", 5},
      {"fp_diff", 3}, {"fp_pow", 8}, {"int_add", 1}, {"int_prod", 1},
      {"int_diff", 1}, {"float2int", 1}, {"int2float", 1}, {"sbuffer", 2},
      // NOTE unimplemented operators from here:
      {"fp_floor", 2}, {"int_max", 1}, {"int_min", 1}, {"fp_max", 1},
      {"fp_min", 1}, {"fp_abs", 1}, {"select2", 1},
      // {"select3", 1},
      {"attach", 1}, {"int_abs", 1}, {"vbargraph", 1}}},
    {250,
     {{"fp_add", 6}, {"fp_prod", 1}, {"fp_div", 18}, {"fp_sqrt", 10},
      {"fp_diff", 6}, {"fp_pow", 18}, {"int_add", 1}, {"int_prod", 1},
      {"int_diff", 1}, {"float2int", 2}, {"int2float", 3}, {"sbuffer", 2},
      // NOTE unimplemented operators from here:
      {"fp_floor", 5}, {"int_max", 1}, {"int_min", 1}, {"fp_max", 1},
      {"fp_min", 1}, {"fp_abs", 1}, {"select2", 1},
      // {"select3", 1},
      {"attach", 1}, {"int_abs", 1}, {"vbargraph", 1}}}
  };

std::map<std::string, std::vector<std::string>>
    operatorImplementationInputPorts = {
        {"fp_add", {"X", "Y"}},
        {"fp_prod", {"X", "Y"}},
        {"fp_div", {"X", "Y"}},
        {"fp_sqrt", {"X"}},
        {"fp_diff", {"X", "Y"}},
        {"fp_pow", {"X", "Y"}},
        {"int_add", {"X", "Y"}},
        {"int_prod", {"X", "Y"}},
        {"int_diff", {"X", "Y"}},
        {"float2int", {"X"}},
        {"int2float", {"X"}},
        {"sbuffer", {"in_data"}},
        {"int_max", {"X", "Y"}},
        {"int_min", {"X", "Y"}},
        {"fp_max", {"X", "Y"}},
        {"fp_min", {"X", "Y"}},
        {"fp_abs", {"X"}},
        {"int_abs", {"X"}},
        {"select2", {"X", "Y", "Z"}},
        {"attach", {"X", "Y"}} // ,
                               // {"vbargraph", {"X"}},
                               // {"fp_floor", {"op_in_data_0"}}
};

std::map<std::string, std::vector<std::string>>
    operatorImplementationOutputPorts = {
        {"fp_add", {"R"}},
        {"fp_prod", {"R"}},
        {"fp_div", {"R"}},
        {"fp_sqrt", {"R"}},
        {"fp_diff", {"R"}},
        {"fp_pow", {"R"}},
        {"int_add", {"R"}},
        {"int_prod", {"R"}},
        {"int_diff", {"R"}},
        {"float2int", {"R"}},
        {"int2float", {"R"}},
        {"sbuffer", {"out_data"}},
        {"int_max", {"R"}},
        {"int_min", {"R"}},
        {"fp_max", {"R"}},
        {"fp_min", {"R"}},
        {"fp_abs", {"R"}},
        {"int_abs", {"R"}},
        {"select2", {"R"}},
        {"attach", {"R"}}// ,
        // {"vbargraph", {"X"}},
        // {"fp_floor", {"op_in_data_0"}}
};

VHDLCircuit generateCircuitObject(models::Dataflow* const dataflow) {

  VHDLCircuit circuit;
  std::string circuitName = dataflow->getGraphName();
  // replace prohibited characters with underscores
  std::replace(circuitName.begin(), circuitName.end(), '-', '_');
  std::replace(circuitName.begin(), circuitName.end(), '.', '_');
  circuit.setName(circuitName);
  VERBOSE_DEBUG( "circuit name: " << circuitName );

  // populate circuit object with components and connections based on dataflow graph
  {ForEachVertex(dataflow, actor) {
      VHDLComponent newComp(dataflow, actor);
      circuit.addComponent(newComp);
      // update execution time in dataflow according to component operator type
      VERBOSE_INFO("operator lifespan ("
                   << newComp.getType() << "): "
                   << getOperatorLifespan(newComp.getType(), operatorFreq));
      if (newComp.getType() != "input_selector" &&
          newComp.getType() != "output_selector") { // input/output selector lifespans are already set in merge_operators
        std::vector<TIME_UNIT> opLifespans(
            dataflow->getVertexPhaseDuration(actor).size(),
            (TIME_UNIT)getOperatorLifespan(newComp.getType(), operatorFreq));
        dataflow->setVertexDuration(actor, opLifespans);
      }
    }}
  {ForEachEdge(dataflow, edge) {
      VHDLConnection newConn(dataflow, edge);
      circuit.addConnection(newConn);
    }}
  circuit.updateTopLevelPorts();

  return circuit;
}

/**
   Generate the binary representation of a VHDLComponent's
   numerical value.

   @param comp Only works on a VHDLComponent of type "const_val".

   @return A string representing the binary form of the value
   transmitted by the given component. An empty string is
   returned if the given component has a data type that is
   not fp/real/int.
 */
std::string binaryValue(VHDLComponent const comp) {
  std::string binaryRepresentation;
  // adapted from https://www.codeproject.com/Questions/678447/Can-any-one-tell-me-how-to-convert-a-float-to-bina
  if (comp.getDataType() == "fp") {
    std::stringstream outputStream;
    float fpVal = comp.getFPValue();
    std::string fpcFloatPrefix = (fpVal ? "01" : "00"); // NOTE might need to account for NaN (11) and Inf (10) values in the future
    size_t size = sizeof(fpVal);
    unsigned char *p = (unsigned char *) &fpVal;
    p += size-1;
    while (size--) {
      int n;
      for (n=0; n<8; n++)
        {
          char bit = ('0' + (*p & 128 ? 1 : 0));
          outputStream << bit;
          *p <<= 1;
        }
      p--;
    }
    binaryRepresentation = fpcFloatPrefix + outputStream.str();
  } else if (comp.getDataType() == "int") {
    binaryRepresentation = std::bitset<34>(comp.getIntValue()).to_string(); // NOTE assuming unsigned binary representation here
  } else {
    VERBOSE_WARNING("Representing the value a VHDLComponent of type "
                    << comp.getType() << " (data type: " << comp.getDataType()
                    << ") as a binary value is not supported.");
  }

  return binaryRepresentation;
}

/**
   Generates a file based on a reference, optionally replacing words in the
   resulting file.

   @param refFile Path to reference file.
   @param dstFile Path to location of file to be generated in.
   @param replacementMap Words to match and their replacements.

   @return void
 */
void copyFileAndReplaceWords(
    std::string refFile, std::string dstFile,
    const std::map<std::string, std::string> &replacementMap) {
  // const auto copyOptions = std::filesystem::copy_options::update_existing |
  //   std::filesystem::copy_options::recursive;

  // std::filesystem::copy(refFile, dstFile, copyOptions);
  std::ifstream fileStream(refFile);
  std::ofstream fileCopy(dstFile);
  std::string lineContent;
  if (fileStream.is_open()) {
    while (std::getline(fileStream, lineContent)) {
      for (auto const &[match, replacement] : replacementMap) {
        size_t pos = 0;
        while ((pos = lineContent.find(match, pos)) != std::string::npos) {
          lineContent.replace(pos, match.length(), replacement);
          pos += replacement.length();
        }
      }
      fileCopy << lineContent << std::endl;
    }
    fileStream.close();
    fileCopy.close();
  } else {
    VERBOSE_ERROR("Failed to access " << refFile);
  }
}

int getOperatorLifespan(const std::string &opType, int operatorFreq) {
  if (operatorLifespans.at(operatorFreq).count(opType)) {
    return operatorLifespans.at(operatorFreq).at(opType);
  } else {
    // some components have no lifespan (e.g. Proj, const_val) but need a
    // positive lifespan for SDF to remain valid
    return 1;
  }
}

const std::vector<std::string> getImplementationInputPorts(std::string opType) {
  std::vector<std::string> inPortNames;
  if (operatorImplementationInputPorts.count(opType)) {
    inPortNames = operatorImplementationInputPorts.at(opType);
  } else {
    VERBOSE_WARNING("No input ports listed for given operator type " << opType <<
                    " --- check operatorImplementationInputPorts for entry.");
  }
  return inPortNames;
}

const std::vector<std::string> getImplementationOutputPorts(std::string opType) {
  std::vector<std::string> outPortNames;
  if (operatorImplementationOutputPorts.count(opType)) {
    outPortNames = operatorImplementationOutputPorts.at(opType);
  } else {
    VERBOSE_WARNING("No output ports listed for given operator type " << opType <<
                    " --- check operatorImplementationOutputPorts for entry.");
  }
  return outPortNames;
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
   - BUFFERLESS: 't' to activate; indicates whether each connection in the VHDL
   design should include a FIFO buffer. Set to buffered (t) by default.
   - FREQUENCY: Clock cycle frequency of VHDL design.
   - NORMALISE_OUTPUTS: Enforce single outputs for all operators.

   @return void; VHDL code generated in location specified in OUTPUT_DIR
   parameter.
 */

void algorithms::generateVHDL(models::Dataflow* const dataflow, parameters_list_t param_list) {

  // default directories/parameters defined here
  topDir      = "./" + dataflow->getGraphName() + "_vhdl_gen/"; // where VHDL files will be generated
  componentDir = topDir + "/components/"; // VHDL component implementations generated here
  tbDir = topDir + "/testbenches/";
  bool outputDirSpecified = false;

  // check for specified VHDL output directory
  if (param_list.find("OUTPUT_DIR") != param_list.end()) {
    outputDirSpecified = true;
    topDir = param_list["OUTPUT_DIR"] + "/";
    componentDir = topDir + "/components/";
    tbDir = topDir + "/testbenches/";
    VERBOSE_INFO("Update output directory to " << topDir);

    // create both topDir and componentDir directories
    std::filesystem::create_directories(componentDir);
  } else {
    VERBOSE_WARNING("Please use '-p OUTPUT_DIR=topDir' to set output directory.");
  }

  // check if FIFO buffers should be generated in VHDL implementation
  if (param_list.find("BUFFERLESS") != param_list.end() &&
      param_list["BUFFERLESS"] == "t") {
    VERBOSE_INFO("Bufferless mode activated");
    isBufferless = true;
  } else {
    VERBOSE_INFO("Please use '-p BUFFERLESS=t' to generate VHDL without FIFO buffers");
  }

  // check if operator frequencies have been specified
  if (param_list.find("FREQUENCY") != param_list.end()) {
    VERBOSE_INFO("Operator frequency set to " << param_list["FREQUENCY"]);
    operatorFreq = std::stoi(param_list["FREQUENCY"]);
  } else {
    VERBOSE_INFO("Default operator frequency used (" << operatorFreq << "), you can use -p FREQUENCY=frequency_in_MHz to set the operator frequency");
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
  }

  if (!isBufferless) {
    {ForEachEdge(dataflow, e) {
        if (!dataflow->getPreload(e)) {
          dataflow->setPreload(e, 1);
        }
      }}
  }
  // VHDLCircuit object specifies operators and how they're connected
  VHDLCircuit tmp = generateCircuitObject(dataflow);
  if (param_list.find("NORMALISE_OUTPUTS") != param_list.end()) {
    while (tmp.getMultiOutActors().size() > 0) { // to simplify VHDL implementation, the operators are only supposed to have a single output

      VERBOSE_INFO("getMultiOutActors is not empty");

      /*  This block removes multiIO actors and replaces them with a router that splits their output */
      for (std::string actorName: tmp.getMultiOutActors()) {
        parameters_list_t parameters;
        parameters["name"] = actorName;
        VERBOSE_INFO("singleOutput actor " << actorName);
        try { // try-catch necessary as some actor in the multi-out list are removed as we iterate through the list
          algorithms::transformation::singleOutput(dataflow, parameters);
        } catch (...) {
          VERBOSE_WARNING("actor missing!");
        }
      }

      // the dataflow now should only have actors with single outputs (with exceptions defined in singleOutput)
      VERBOSE_INFO("Generate updated circuit (with multi-output actors removed)");
      tmp = generateCircuitObject(dataflow);
    }

    VERBOSE_ASSERT (tmp.getMultiOutActors().size() == 0, "Error while add Dups") ;
    VERBOSE_INFO("Output Circuit");
  }

  if (outputDirSpecified) { // only produce actual VHDL files if output directory specified
    const auto copyOptions = std::filesystem::copy_options::update_existing
      | std::filesystem::copy_options::recursive;
    generateOperators(tmp);
    generateCircuit(tmp);
    generateAudioInterfaceWrapper(tmp);
    std::filesystem::copy(referenceDir + "/testbenches/", tbDir, copyOptions);
    printers::writeSDF3File(topDir + dataflow->getGraphName() + "_exectimes.xml",
                            dataflow);
    VERBOSE_INFO("VHDL files generated in: " << topDir);
  } else {
    VERBOSE_WARNING("No VHDL files created.");
  }

  std::cout << tmp.printStatus() << std::endl;

}

void algorithms::generateOperators(VHDLCircuit &circuit) {
  std::map<std::string, int> operatorMap = circuit.getOperatorMap();
  // Generate VHDL files for individual components
  if (dataDriven) { // include HS interface components for data driven operator execution
    for (auto const &op : operatorMap) { // operatorMap <first: operator type, second: occurances>
      VERBOSE_INFO("Generate VHDL component file for " << op.first);
      // track number of inputs/outputs for cases where operator can have different number of inputs/outputs
      std::map<int, int> inputCounts;
      std::map<int, int> outputCounts;
      inputCounts = circuit.getNumInputs(op.first);
      outputCounts = circuit.getNumOutputs(op.first);
      if (op.first == "Proj") {
        generateSplitterOperators(outputCounts);
      } else if (op.first == "const_value") {
        generateConstOperator(outputCounts);
      } else if (op.first == "select2") {
        generateRoutingOperators(circuit.getFirstComponentByType(op.first));
      } else if (op.first == "fp_floor") {
        generateFloorOperator(circuit.getFirstComponentByType(op.first));
      } else if (op.first == "delay") {
        generateFPCOperator(circuit.getFirstComponentByType(op.first).getImplementationName());
      } else if (circuit.getFirstComponentByType(op.first).isUI()) {
        generateUIOperator(circuit.getFirstComponentByType(op.first));
      } else if (op.first == "input_selector" || op.first == "output_selector") {
        generateInputOutputSelectorOperator(circuit.getFirstComponentByType(op.first),
                                            inputCounts, outputCounts);
      } else {
        generateOperator(circuit.getFirstComponentByType(op.first));
      }
    }
    generateHSInterfaceComponents();
    generateAudioInterfaceComponents();
  } else {
    // operatorMap <key: operator type, value: occurances>
    for (auto const &op : operatorMap) {
      VERBOSE_INFO("Generate VHDL component file for " << op.first);
      // input/outputCounts for given operator <key: number of inputs/outputs, value: occurances>
      std::map<int, int> inputCounts;
      std::map<int, int> outputCounts;
      inputCounts = circuit.getNumInputs(op.first);
      outputCounts = circuit.getNumOutputs(op.first);
      if (op.first == "input_selector" || op.first == "output_selector") {
        VERBOSE_INFO("Generating for " << op.first);
        generateInputOutputSelectorImplementation(
            circuit.getFirstComponentByType(op.first), inputCounts,
            outputCounts);
      } else if (op.first == "const_value") {
        generateConstOperator(outputCounts);
      } else { // copy operator implementation from reference directory
        std::string operatorRefDir = referenceDir;
        std::string operatorFileName =
          circuit.getFirstComponentByType(op.first).getImplementationName();
        if (op.first == "sbuffer") {
          // do nothing
        } else if (circuit.getFirstComponentByType(op.first).isUI()) {
          operatorRefDir += "/ui/";
        } else {
          operatorRefDir += "/operators/";
          operatorFileName += "_f" + std::to_string(operatorFreq);
        }
        if (op.first != "INPUT" && op.first != "OUTPUT") {
          VERBOSE_INFO("Generating operator implementation for " << operatorFileName << " from " << operatorRefDir);
          const auto copyOptions = std::filesystem::copy_options::update_existing |
            std::filesystem::copy_options::recursive;
          std::filesystem::copy(operatorRefDir + operatorFileName + ".vhdl",
                                componentDir + operatorFileName + ".vhdl", copyOptions);
        }
      }
    }
    generateAudioInterfaceComponents();
  }
}

void algorithms::generateConstOperator(std::map<int, int> outputCounts) {
  std::string operatorFileName = "const_value";
  // generate separate const_value components for given number of outputs required
  for (auto const &[numOutputs, occurances] : outputCounts) {
    std::stringstream portListStream;
    std::stringstream portConnStream;
    std::string portList;
    std::string portConn;
    std::string portListEnd = ";\n";
    // generate port names and behaviour
    for (int p = 0; p < numOutputs; p++) {
      if (p + 1 == numOutputs) {
        portListEnd = "";
      }
      if (dataDriven) {
        portListStream << "out_ready_" << p << " : in std_logic;\n";
        portListStream << "out_valid_" << p << " : out std_logic;\n";
      }
      portListStream
          << "out_data_" << p
          << " : out std_logic_vector (33 downto 0) := (others => '0')"
          << portListEnd << std::endl;
      if (dataDriven) {
        portConnStream << "out_valid_" << p << " <= '1';\n";
      }
      portConnStream << "out_data_" << p << " <= value;\n" << std::endl;
    }
    portList = portListStream.str();
    portConn = portConnStream.str();
    std::map<std::string, std::string> replacementWords = {{"$N_OUTPUTS", std::to_string(numOutputs)},
                                                           {"$PORT_LIST", portList},
                                                           {"$PORT_CONNECTIONS", portConn}};
    copyFileAndReplaceWords(referenceDir + operatorFileName + "_n_outputs.vhdl",
                            componentDir + operatorFileName + "_" +
                                std::to_string(numOutputs) + ".vhdl",
                            replacementWords);
  }
}

// Generate splitter components for the Proj components in the circuit (defined by number of outputs)
void algorithms::generateSplitterOperators(std::map<int, int> outputCounts) {
  std::string operatorFileName = "hs_splitter";
  for (auto const &[numOutputs, occurances] : outputCounts) {
    std::string refFileName = operatorFileName + ".vhdl";
    std::string outputFile = operatorFileName + "_" + std::to_string(numOutputs) + ".vhdl";
    std::stringstream outputPorts;
    std::stringstream outputReadySigs;
    std::stringstream outputDataMapping;
    std::stringstream outputValidMapping;

    for (int outCount = 0; outCount < numOutputs; outCount++) {
      std::string delimiter = ";\n";
      std::string boolAnd = " AND ";
      outputPorts << "out_ready_" << std::to_string(outCount) << " : in std_logic" << delimiter
                  << "out_valid_" << std::to_string(outCount) << " : out std_logic" << delimiter;
      outputDataMapping << "out_data_" << std::to_string(outCount) << " <= temp_data_0(bit_width-1 downto 0);\n";
      outputValidMapping << "out_valid_" << std::to_string(outCount) << " <= is_stored_0;\n";
      if (outCount + 1 == numOutputs) {
        delimiter = "\n";
        boolAnd = "";
      }
      outputPorts << "out_data_" << std::to_string(outCount) << " : out std_logic_vector(bit_width-1 downto 0)" << delimiter;
      outputReadySigs << "out_ready_" << std::to_string(outCount) << "='1'" << boolAnd;
    }
    std::map<std::string, std::string> replacementWords = {{"$NUM_OUTPUTS", std::to_string(numOutputs)},
                                                           {"$OUTPUT_PORTS", outputPorts.str()},
                                                           {"$OUT_READY_SIGS", outputReadySigs.str()},
                                                           {"$OUT_DATA_MAPPING", outputDataMapping.str()},
                                                           {"$OUT_VALID_MAPPING", outputValidMapping.str()}};
    copyFileAndReplaceWords(referenceDir + refFileName,
                            componentDir + outputFile, replacementWords);
  }

}

// Generate signal routing operators
// NOTE currently only supports select2 operator
void algorithms::generateRoutingOperators(VHDLComponent comp) {
  std::string entityName = comp.getType();
  // need distinct name for component instantiation in the HS interface declaration in VHDL
  std::string componentName = entityName + "_component";
  std::string opInputCount = std::to_string(comp.getInputPorts().size());
  std::string hsmType = "";
  if (comp.getType() == "select2") {
    hsmType = "_three";
  }
  std::map<std::string, std::string> replacementWords = {{"$ENTITY_NAME", entityName},
                                                         {"$FLOPOCO_OP_NAME", entityName + "_op"},
                                                         {"$COMPONENT_NAME", componentName},
                                                         {"$OP_LIFESPAN", std::to_string(1)}, // HS interface requires lifespan to be >0
                                                         {"$HSM_TYPE", hsmType}};
  // generate select operator implementation
  const auto copyOptions = std::filesystem::copy_options::update_existing |
                           std::filesystem::copy_options::recursive;
  std::filesystem::copy(referenceDir + entityName + "_op.vhd",
                        componentDir + entityName + "_op.vhd", copyOptions);

  copyFileAndReplaceWords(referenceDir + "flopoco_hs_interface" + "_" +
                              opInputCount + ".vhdl",
                          componentDir + entityName + ".vhdl", replacementWords);
}

void algorithms::generateFloorOperator(VHDLComponent comp) {
  // copy in fp_floor_freq implementation file
  generateFPCOperator(comp.getImplementationName());

  // copy in fp_floor wrapper file and change operator frequency
  std::map<std::string, std::string> freqReplace;
  freqReplace["$FREQ"] = std::to_string(operatorFreq);
  copyFileAndReplaceWords(referenceDir + "fp_floor.vhdl",
                          componentDir + "fp_floor.vhdl", freqReplace);
  // generate internal components that make up floor operator
  std::string internalComponents[] = {"float2int", "int2float"};
  for (const std::string &entityName : internalComponents) {
    std::string componentName = entityName + "_implementation";
    std::string implementationName = entityName + "_flopoco";
    std::string lifespan = std::to_string(getOperatorLifespan(entityName,
                                                              operatorFreq));

    generateFPCOperator(implementationName);
    std::string operatorName = implementationName + "_f" + std::to_string(operatorFreq);
    std::map<std::string, std::string> replacementWords = {{"$ENTITY_NAME", entityName},
                                                           {"$FLOPOCO_OP_NAME", operatorName},
                                                           {"$COMPONENT_NAME", componentName},
                                                           {"$OP_LIFESPAN", lifespan},
                                                           {"$HSM_TYPE", "_one"}};
    copyFileAndReplaceWords(referenceDir + "flopoco_hs_interface_1.vhdl",
                            componentDir + entityName + ".vhdl",
                            replacementWords);
  }
}

void algorithms::generateInputOutputSelectorOperator(VHDLComponent comp,
                                                     std::map<int, int> inputCounts,
                                                     std::map<int, int> outputCounts) {
  std::string entityName = comp.getType();
  VERBOSE_DEBUG("entity name: " << entityName);
  std::string componentName;
  std::string portList;
  std::string processBehaviour;
  std::string validSignalRouting;
  std::string dataSignalRouting;
  std::string readySignalRouting;
  if (entityName == "input_selector") {
    for (auto &ports : inputCounts) {
      componentName = entityName + "_" + std::to_string(ports.first);
      for (int i = 0; i < ports.first; i++) {
        std::string phaseConditional = "if (current_phase = " + std::to_string(i) + ") then\n";
        std::string validSigRoute = "op_out_valid_0 <= op_in_valid_0 when (current_phase = 0) else\n";
        std::string dataSigRoute = "op_out_data_0 <= op_in_data_0 when (current_phase = 0) else\n";
        if (i > 0) {
          phaseConditional = "elsif (current_phase = " + std::to_string(i) + ") then\n";
          validSigRoute = "op_in_valid_" + std::to_string(i) + " when (current_phase = " + std::to_string(i) + ") else\n";
          dataSigRoute = "op_in_data_" + std::to_string(i) + " when (current_phase = " + std::to_string(i) + ") else\n";
          if (i + 1 == ports.first) {
            validSigRoute += "'0';\n";
            dataSigRoute += "\"0000000000000000000000000000000000\";\n";
          }
        }
        portList += "op_in_ready_" + std::to_string(i) + " : out std_logic;\n"
          + "op_in_valid_" + std::to_string(i) + " : in std_logic;\n"
          + "op_in_data_" + std::to_string(i) + " : in std_logic_vector(33 downto 0);\n";
        processBehaviour += phaseConditional
          + "        if (op_in_valid_" + std::to_string(i) + " = '1' and op_out_ready_0 = '1') then\n"
          + "            next_phase <= (current_phase + 1) mod num_phases;\n"
          + "        end if;\n";
        if (i + 1 == ports.first) {
          processBehaviour += "    end if;\n";
        }
        validSignalRouting += validSigRoute;
        dataSignalRouting += dataSigRoute;
        readySignalRouting += "op_in_ready_" + std::to_string(i)
          + " <= op_out_ready_0 when (current_phase = " + std::to_string(i) + ") else\n"
          + "'0';\n";
      }
      // generate input selector implementation
      std::map<std::string, std::string> replacementWords = {{"$COMPONENT_NAME", componentName},
                                                             {"$PORT_LIST", portList},
                                                             {"$PROCESS_BEHAVIOUR", processBehaviour},
                                                             {"$VALID_SIGNAL_ROUTING", validSignalRouting},
                                                             {"$DATA_SIGNAL_ROUTING", dataSignalRouting},
                                                             {"$READY_SIGNAL_ROUTING", readySignalRouting}};
        copyFileAndReplaceWords(referenceDir + entityName + ".vhdl",
                                componentDir + componentName + ".vhdl",
                                replacementWords);
        // reset strings for next iteration
        portList.clear();
        processBehaviour.clear();
        validSignalRouting.clear();
        dataSignalRouting.clear();
        readySignalRouting.clear();
    }
  } else {
    for (auto &ports : outputCounts) {
      componentName = entityName + "_" + std::to_string(ports.first);
      for (int i = 0; i < ports.first; i++) {
        std::string phaseConditional = "if (current_phase = " + std::to_string(i) + ") then\n";
        std::string readySignalRoute = "op_in_ready_0 <= op_out_ready_0 when (current_phase = 0) else\n";
        if (i > 0) {
          phaseConditional = "elsif (current_phase = " + std::to_string(i) + ") then\n";
          readySignalRoute = "op_out_ready_" + std::to_string(i) + " when (current_phase = " + std::to_string(i) + ") else\n";
          if (i + 1 == ports.first) {
            readySignalRoute += "'0';\n";
          }
        }
        portList += "op_out_ready_" + std::to_string(i) + " : in std_logic;\n"
          + "op_out_valid_" + std::to_string(i) + " : out std_logic;\n"
          + "op_out_data_" + std::to_string(i) + " : out std_logic_vector(33 downto 0)";
        processBehaviour += phaseConditional
          + "        if (op_in_valid_0 = '1' and op_out_ready_" + std::to_string(i) + " = '1') then\n"
          + "            next_phase <= (current_phase + 1) mod num_phases;\n"
          + "        end if;\n";
        validSignalRouting += "op_out_valid_" + std::to_string(i)
          + " <= op_in_valid_0 when (current_phase = " + std::to_string(i) + ") else\n"
          + "'0';\n";
        dataSignalRouting += "op_out_data_" + std::to_string(i)
          + " <= op_in_data_0 when (current_phase = " + std::to_string(i) + ") else\n"
          + "\"0000000000000000000000000000000000\";\n";
        readySignalRouting += readySignalRoute;
        if (i + 1 == ports.first) { // last output port
          portList += "\n";
          processBehaviour += "end if;\n";
        } else {
          portList += ";\n";
        }
      }
      // generate output selector implementation
      std::map<std::string, std::string> replacementWords = {{"$COMPONENT_NAME", componentName},
                                                             {"$PORT_LIST", portList},
                                                             {"$PROCESS_BEHAVIOUR", processBehaviour},
                                                             {"$VALID_SIGNAL_ROUTING", validSignalRouting},
                                                             {"$DATA_SIGNAL_ROUTING", dataSignalRouting},
                                                             {"$READY_SIGNAL_ROUTING", readySignalRouting}};
        copyFileAndReplaceWords(referenceDir + entityName + ".vhdl",
                                componentDir + componentName + ".vhdl",
                                replacementWords);
        // reset strings for next iteration
        portList.clear();
        processBehaviour.clear();
        validSignalRouting.clear();
        dataSignalRouting.clear();
        readySignalRouting.clear();
    }
  }
}

/**
   Generate VHDL implementation for the input/output selector. Will generate a
   separate implementation file for each instance of a different number of
   input/output ports required.

   @param inputCounts Map with key = number of inputs, value = occurances

   @param outputCounts Map with key = number of outputs, value = occurances
*/
void algorithms::generateInputOutputSelectorImplementation(VHDLComponent comp,
                                                           std::map<int, int> inputCounts,
                                                           std::map<int, int> outputCounts) {
  std::string entityName = comp.getType();
  std::string componentName;
  std::string execTimePorts;
  std::string portList;
  std::string processBehaviour;
  std::string validSignalRouting;
  std::string dataSignalRouting;
  std::string readySignalRouting;
  std::string resetBehaviour;
  if (entityName == "input_selector") {
    for (auto &ports : inputCounts) {
      componentName = entityName + "_" + std::to_string(ports.first);
      for (int i = 0; i < ports.first; i++) {
        execTimePorts += "exec_time_" + std::to_string(i) + " : integer := 0";
        if (i + 1 == ports.first) {
          execTimePorts += "\n";
        } else {
          execTimePorts += ";\n";
        }
        portList += "in_data_" + std::to_string(i) + " : in std_logic_vector(ram_width-1 downto 0);\n";
        processBehaviour += "when exec_time_" + std::to_string(i) +
                            " => out_data_0 <= in_data_" + std::to_string(i) +
                            ";\n";
      }
      // generate input selector implementation
      std::map<std::string, std::string> replacementWords = {{"$COMPONENT_NAME", componentName},
                                                             {"$EXEC_TIME_PORTS", execTimePorts},
                                                             {"$INPUT_PORTS", portList},
                                                             {"$PROCESS_BEHAVIOUR", processBehaviour}};
        copyFileAndReplaceWords(referenceDir + "s_" + entityName + ".vhdl",
                                componentDir + componentName + ".vhdl",
                                replacementWords);
        // reset strings for next iteration
        execTimePorts.clear();
        portList.clear();
        processBehaviour.clear();
    }
  } else {
    for (auto &ports : outputCounts) {
      componentName = entityName + "_" + std::to_string(ports.first);
      for (int i = 0; i < ports.first; i++) {
        execTimePorts += "exec_time_" + std::to_string(i) + " : integer := 0";
        if (i + 1 == ports.first) {
          execTimePorts += "\n";
        } else {
          execTimePorts += ";\n";
        }
        portList += "out_data_" + std::to_string(i) + " : out std_logic_vector(ram_width-1 downto 0);\n";
        processBehaviour += "when exec_time_" + std::to_string(i) +
                            " => out_data_" + std::to_string(i) +
                            " <= in_data_0;\n";
        resetBehaviour += "out_data_" + std::to_string(i) + " <= (others => '0');\n";
      }
      // generate output selector implementation
      std::map<std::string, std::string> replacementWords = {{"$COMPONENT_NAME", componentName},
                                                             {"$EXEC_TIME_PORTS", execTimePorts},
                                                             {"$OUTPUT_PORTS", portList},
                                                             {"$PROCESS_BEHAVIOUR", processBehaviour},
                                                             {"$RESET_BEHAVIOUR", resetBehaviour}};
        copyFileAndReplaceWords(referenceDir + "s_" + entityName + ".vhdl",
                                componentDir + componentName + ".vhdl",
                                replacementWords);
        // reset strings for next iteration
        execTimePorts.clear();
        portList.clear();
        processBehaviour.clear();
        resetBehaviour.clear();
    }
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

void algorithms::generateUIOperator(VHDLComponent comp) {
  const auto copyOptions = std::filesystem::copy_options::update_existing |
    std::filesystem::copy_options::recursive;
  std::filesystem::copy(
      referenceDir + "/ui/" + comp.getImplementationName() + ".vhdl",
      componentDir + comp.getImplementationName() + ".vhdl", copyOptions);
}

/**
   Generate corresponding VHDL code for the component.

   @param comp VHDL code generated based on this.

   @return void
 */
void algorithms::generateOperator(VHDLComponent comp) {
  if (comp.getType() != "INPUT" && comp.getType() != "OUTPUT" && !comp.isConst()) {
    std::string entityName = comp.getType();
    std::string componentName = entityName + "_implementation";
    std::string opInputCount = std::to_string(comp.getInputPorts().size());
    std::string operatorName = comp.getImplementationName() + "_f" + std::to_string(operatorFreq);
    std::string hsmType = "";
    if (comp.getInputPorts().size() == 1) {
      hsmType = "_one";
    }
    std::map<std::string, std::string> replacementWords = {
        {"$ENTITY_NAME", entityName},
        {"$FLOPOCO_OP_NAME", operatorName},
        {"$COMPONENT_NAME", componentName},
        {"$OP_LIFESPAN", std::to_string(getOperatorLifespan(comp.getType(), operatorFreq))},
        {"$HSM_TYPE", hsmType}};

    // generate VHDL implementation of component's operation
    generateFPCOperator(comp.getImplementationName());
    // generate the appropriate HS interface
    copyFileAndReplaceWords(
        referenceDir + "flopoco_hs_interface_" + opInputCount + ".vhdl",
        componentDir + entityName + ".vhdl", replacementWords);
  }
}

void algorithms::generateCircuit(VHDLCircuit &circuit) {
  std::ofstream vhdlOutput;
  std::string graphName = circuit.getName() + "_circuit"; // TODO decide on naming convention
  std::map<std::string, int> operatorMap = circuit.getOperatorMap();
  bool noOperators = operatorMap.size() == 2 && operatorMap.count("INPUT") && operatorMap.count("OUTPUT"); // edge case where Faust program has no operators (only inputs/outputs)

  // Open the stream
  vhdlOutput.open(topDir + graphName + ".vhd"); // instantiate VHDL file

  // 1. Define libraries used
  generateVHDLHeader(vhdlOutput);

  // 2. Port declarations
  generateVHDLEntity(circuit, vhdlOutput);

  // 3. Specify architecture (behaviour) of operator type
  generateVHDLArchitecture(circuit, operatorMap, noOperators, vhdlOutput);

  // Close the stream
  vhdlOutput.close();
}

void algorithms::generateVHDLHeader(std::ofstream &vhdlOutput) {
    vhdlOutput << "library ieee;\n"
                 << "use ieee.std_logic_1164.all;\n"
                 << "use ieee.numeric_std.all;\n" << std::endl;
}

/**
   Generates VHDL code for the entity declaration of the VHDL component that
   will contain the port mapping of the rest of the constituent components that
   make up the circuit.

   @param circuit Circuit object to declare as VHDL entity.

   @param numInputPorts Number of input ports of entity.

   @param numOutputPorts Number of output ports of entity.

   @param vhdlOutput Output stream to write VHDL code to.
 */
void algorithms::generateVHDLEntity(VHDLCircuit &circuit, std::ofstream &vhdlOutput) {
    int numInputPorts = circuit.getOperatorCount("INPUT");
    int numOutputPorts = circuit.getOperatorCount("OUTPUT");
    vhdlOutput << "entity " << circuit.getName() << " is\n"
               << "generic (\n"
               << "    " << "ram_width : natural := 34;\n"
               << "    " << "ram_depth : natural := 2);\n" // buffer size
               << "port (\n"
               << "    " << "clk : in std_logic;\n"
               << "    " << "rst : in std_logic;\n"
               << std::endl;
    // Specify ready, valid, and data ports for each input port:
    std::string portName = "    " + circuit.getName();
    if (!dataDriven) {
      vhdlOutput << "    " << "cycle_count : in integer;\n" << std::endl;
      for (auto i = 0; i < numInputPorts; i++) {
        vhdlOutput << portName + "_in_data_" + std::to_string(i) + " : in std_logic_vector("
                   << "ram_width - 1 downto 0) := (others => '0');\n"
                   << std::endl;
      }
      // Specify ready, valid, and data ports for each output port:
      for (auto i = 0; i < numOutputPorts; i++) {
        if (i + 1 == numOutputPorts) {
          vhdlOutput << portName + "_out_data_" + std::to_string(i) + " : out std_logic_vector("
                     << "ram_width - 1 downto 0) := (others => '0')\n"
                     << std::endl; // last line of port declaration has no terminating semicolon
        } else {
          vhdlOutput << portName + "_out_data_" + std::to_string(i) + " : out std_logic_vector("
                     << "ram_width - 1 downto 0) := (others => '0');\n"
                     << std::endl;
        }
      }
    } else {
      for (auto i = 0; i < numInputPorts; i++) {
        vhdlOutput << portName + "_in_ready_" + std::to_string(i) + " : out std_logic;\n"
                   << portName + "_in_valid_" + std::to_string(i) + " : in std_logic;\n"
                   << portName + "_in_data_" + std::to_string(i) + " : in std_logic_vector("
                   << "ram_width - 1 downto 0) := (others => '0');\n"
                   << std::endl;
      }
      // Specify ready, valid, and data ports for each output port:
      for (auto i = 0; i < numOutputPorts; i++) {
        vhdlOutput << portName + "_out_ready_" + std::to_string(i) + " : in std_logic;\n"
                   << portName + "_out_valid_" + std::to_string(i) + " : out std_logic;\n";
        if (i + 1 == numOutputPorts) {
          vhdlOutput << portName + "_out_data_" + std::to_string(i) + " : out std_logic_vector("
                     << "ram_width - 1 downto 0) := (others => '0')\n"
                     << std::endl; // last line of port declaration has no terminating semicolon
        } else {
          vhdlOutput << portName + "_out_data_" + std::to_string(i) + " : out std_logic_vector("
                     << "ram_width - 1 downto 0) := (others => '0');\n"
                     << std::endl;
        }
      }
    }
    vhdlOutput << ");\nend " << circuit.getName() << ";\n" << std::endl;
}


void algorithms::generateVHDLArchitecture(VHDLCircuit &circuit, std::map<std::string, int> &operatorMap,
                                          bool noOperators, std::ofstream &vhdlOutput) {// component declaration
  // top level intermediate signal names stored in these vectors
  std::vector<std::string> dataSignals;
  std::vector<std::string> validReadySignals;
  bool uiDetected = false;

  // 1. Instantiate components for each operator in circuit
  vhdlOutput << "architecture behaviour of " << circuit.getName() << " is\n" << std::endl;
  if (!noOperators) { // only generate components if there are operators
    if (!dataDriven) {
      std::map<int, int> constOutputs;
      for (auto const &op : operatorMap) {
        if (op.first != "INPUT" && op.first != "OUTPUT") {
          if (op.first == "input_selector" || op.first == "output_selector") {
            vhdlOutput << generateInputOutputSelectorComponent(
                circuit.getFirstComponentByType(op.first),
                circuit.getNumInputs(op.first), circuit.getNumOutputs(op.first))
                       << std::endl;
          } else if (op.first == "sbuffer") {
            vhdlOutput << generateSBufferComponent() << std::endl;
          } else if (op.first == "const_value" ||
                     circuit.getFirstComponentByType(op.first).isConst()) { // current workaround for UI components (we classify them as const value)
            constOutputs = circuit.getNumOutputs(op.first);
          }
          else {
            vhdlOutput << generateComponent(circuit.getFirstComponentByType(op.first))
                       << std::endl;
          }
        }
      }
      if (constOutputs.size()) {
        vhdlOutput << generateConstComponents(constOutputs) << std::endl;
      }
    } else {
      std::map<int, int> constOutputs;
      for (auto const &op : operatorMap) {
        if (op.first != "INPUT" && op.first != "OUTPUT") {
          std::map<int, int> inputCounts;
          std::map<int, int> outputCounts;
          inputCounts = circuit.getNumInputs(op.first);
          outputCounts = circuit.getNumOutputs(op.first);
          if (op.first == "Proj") {
            vhdlOutput << generateSplitterComponents(outputCounts) << std::endl;
          } else if (op.first == "delay") {
            vhdlOutput << generateDelayComponent(circuit.getFirstComponentByType(op.first))
                       << std::endl;
          } else if (op.first == "const_value" ||
                     circuit.getFirstComponentByType(op.first).isConst()) {
            constOutputs.insert(outputCounts.begin(), outputCounts.end()); // workaround for UI components
          } else if (circuit.getFirstComponentByType(op.first).isUI()) {
            vhdlOutput << generateUIComponents(circuit.getFirstComponentByType(op.first)) << std::endl;
            uiDetected = true;
          } else if (op.first == "input_selector" || op.first == "output_selector") {
            vhdlOutput << generateInputOutputSelectorComponent(circuit.getFirstComponentByType(op.first),
                                                               inputCounts, outputCounts)
                       << std::endl;
          } else {
            vhdlOutput << generateComponent(circuit.getFirstComponentByType(op.first))
                       << std::endl;
          }

        }
      }
      if (constOutputs.size()) {
        vhdlOutput << generateConstComponents(constOutputs) << std::endl;
      }
      // FIFO component instantiation
      for (auto &conn : circuit.getConnectionMap()) {
        if (conn.second.getInitialTokenCount()) {
          VERBOSE_INFO("Generating HS FIFO buffer implementations");
          vhdlOutput << generateBufferComponent(circuit.getName()) << std::endl;
          std::vector<std::string> fifoBufferComps = {
              "hs_fifo", "hs_fifo_n", "hs_fifo_one", "hs_fifo_zero"};
          for (const auto &component : fifoBufferComps) {
            const auto copyOptions = std::filesystem::copy_options::update_existing |
              std::filesystem::copy_options::recursive;
            std::filesystem::copy(referenceDir + component + ".vhdl",
                                  componentDir + component + ".vhdl", copyOptions);
          }
          break;
        }
      }

      if (uiDetected) {
        for (auto &op : circuit.getOperatorMap()) {
          // NOTE workaround for UI components' pin out that isn't reflected in SDF
          if (circuit.getFirstComponentByType(op.first).isUI()) { // TODO push this workaround down into VHDLComponent class
            for (int i = 0; i < op.second; i++) {
              dataSignals.push_back(op.first + "_" + std::to_string(i) + "_data_in");
            }
          }
        }
      }
    }
  }

  // 2. Generate intermediate signal names
  if (!dataDriven) {
    for (auto const &[e, conn] : circuit.getConnectionMap()) {
      if (!(circuit.getSrcComponent(conn).getType() == "INPUT" ||
            circuit.getDstComponent(conn).getType() == "OUTPUT")) {
        dataSignals.push_back(conn.getName() + "_DATA");
      }
    }
  } else {
    dataSignals = circuit.generateDataSignalNames();
    validReadySignals = circuit.generateValidReadySignalNames();
  }
    // 3. Write signal names to VHDL output
    // data signals
    if (!noOperators && dataSignals.size()) { // no need to use internal signals if no operators nor internal signals
      vhdlOutput << "signal ";
      std::string delim = ",";
      int counter = 0;
      for (auto &signal : dataSignals) {
        counter++;
        if (counter == dataSignals.size()) {
          delim = "";
        }
        vhdlOutput << signal << delim << std::endl;
      }
      vhdlOutput << " : std_logic_vector(ram_width - 1 downto 0);\n"
                 << std::endl;
      // valid and ready signals
      if (dataDriven) {
        vhdlOutput << "signal ";
        delim = ",";
        counter = 0;
        for (auto &signal : validReadySignals) {
          counter++;
          if (counter == validReadySignals.size()) {
            delim = "";
          }
          vhdlOutput << signal << delim << std::endl;
        }
        vhdlOutput << " : std_logic;\n" << std::endl;
      }
      vhdlOutput << generatePortMapping(circuit) << std::endl;
      vhdlOutput << "end behaviour;" << std::endl;

      vhdlOutput.close();
    } else {
      vhdlOutput << generatePortMapping(circuit) << std::endl;
      vhdlOutput << "end behaviour;" << std::endl;
    }

}



void algorithms::generateAudioInterfaceWrapper(const VHDLCircuit &circuit) {
  int i2sBitWidth = 24;
  int fpcBitWidth = 34;
  std::ofstream vhdlOutput;
  std::string entityName = circuit.getName();
  std::stringstream componentDeclaration;
  std::stringstream portListing;
  std::stringstream entityMapping;
  std::stringstream audioInterfacePortListing;
  std::stringstream audioInterfaceMapping;
  std::stringstream i2sTransceiverMapping;
  std::stringstream inputInterfaceMapping;
  std::stringstream outputInterfaceMapping;
  std::stringstream entitySignals;
  std::stringstream i2sTransceiverSignals;
  std::stringstream inputInterfaceSignals;
  std::stringstream outputInterfaceSignals;
  std::stringstream I2SToFPCInstantiation;
  std::stringstream FPCToI2SInstantiation;
  std::stringstream I2SToFPCSignals;
  std::stringstream FPCToI2SSignals;
  // TODO merge words to replace (signals, mapping)
  std::string wordsToReplace[] = {"$ENTITY_NAME", "$ENTITY_PORTS", "$COMPONENT_DECLARATION",
                                  "$AUDIO_INTERFACE_WRAPPER_PORTS",
                                  "$ENTITY_SIGNALS", "$I2STOFPC_SIGNALS", "$FPCTOI2S_SIGNALS",
                                  "$I2S_TRANSCEIVER_SIGNALS", "$INPUT_INTERFACE_SIGNALS",
                                  "$OUTPUT_INTERFACE_SIGNALS",
                                  "$AUDIO_INTERFACE_WRAPPER_MAPPING",
                                  "$INPUT_INTERFACE_MAPPING",
                                  "$OUTPUT_INTERFACE_MAPPING",
                                  "$I2S_TRANSCEIVER_MAPPING",
                                  "$ENTITY_MAPPING",
                                  "$I2STOFPC_COMPONENTS", "$FPC2I2S_COMPONENTS"};
  int numInputPorts = circuit.getOperatorCount("INPUT");
  int numOutputPorts = circuit.getOperatorCount("OUTPUT");
  int numInputInterfaces = 0;
  int numOutputInterfaces = 0;
  int numAudioCodecs = 0;
  // one input/output interface for each pair of input/output ports; round up if odd counts
  if (numInputPorts % 2) {
    numInputInterfaces = (numInputPorts + 1) / 2;
  } else {
    numInputInterfaces = numInputPorts / 2;
  }
  if (numOutputPorts % 2) {
    numOutputInterfaces = (numOutputPorts + 1) / 2;
  } else {
    numOutputInterfaces = numOutputPorts / 2;
  }
  if (numInputPorts >= numOutputPorts) { // match number of audio codecs to input/output interfaces
    numAudioCodecs = numInputInterfaces;
  } else {
    numAudioCodecs = numOutputInterfaces;
  }
  VERBOSE_ASSERT (numAudioCodecs > 0, "At least 1 audio codec required");

  /* NOTE I2S transceiver declaration is already in audio interface wrapper by default as
     it's necessary to interface with the audio codec */
  for (auto i = 0; i < numAudioCodecs; i++) {
    audioInterfacePortListing << generateAudioInterfaceWrapperPorts(i);
    audioInterfaceMapping << generateAudioInterfaceWrapperMapping(i);
    if (i + 1 == numAudioCodecs) {
      audioInterfacePortListing << "sys_clk : in std_logic;\n"
                                << "reset : in std_logic;\n"
                                << "mclk : in std_logic\n";
      audioInterfaceMapping << "sys_clk_sig <= sys_clk;\n"
                            << "mclk_sig <= mclk;\n"
                            << "rst_sig <= reset;";
    }
    i2sTransceiverSignals << generateI2STransceiverSignalNames(i, i2sBitWidth);
    i2sTransceiverMapping << generateI2STransceiverMapping(i, i2sBitWidth);
    if (!dataDriven) {
      componentDeclaration << generateSBufferComponent() << std::endl;
      componentDeclaration << generateCycleCounterComponent() << std::endl;
    }
  }
  if (numInputPorts) {
    if (!dataDriven) {
      componentDeclaration << generateInputScalingComponents(i2sBitWidth, fpcBitWidth);
      // Generate mappings for input/output interfaces and I2S transceivers
      for (auto i = 0; i < numAudioCodecs; i++) {
        inputInterfaceSignals << generateInputInterfaceSignalNames(i, i2sBitWidth);
        // inputInterfaceMapping << generateInputInterfaceMapping(i, i2sBitWidth);
        // TODO add rx sbuffer mapping
        // TODO add fix2fp mapping
      }
    } else {
      componentDeclaration << generateInputInterfaceComponent(i2sBitWidth) << std::endl;
      componentDeclaration << generateI2SToFPCComponent(i2sBitWidth, fpcBitWidth) << std::endl;
      // Generate mappings for input/output interfaces and I2S transceivers
      for (auto i = 0; i < numAudioCodecs; i++) {
        inputInterfaceSignals << generateInputInterfaceSignalNames(i, i2sBitWidth);
        inputInterfaceMapping << generateInputInterfaceMapping(i, i2sBitWidth);
        // need to generate these intermediate signals here as there are sometimes less input ports
        // in the Faust component than there are input ports in the input interfaces
        I2SToFPCSignals << generateI2SToFPCSignalNames(i);
      }
    }
  }
  if (numOutputPorts) {
    if (!dataDriven) {
      componentDeclaration << generateOutputScalingComponents(i2sBitWidth, fpcBitWidth);
      for (auto i = 0; i < numAudioCodecs; i++) {
        outputInterfaceSignals << generateOutputInterfaceSignalNames(i, i2sBitWidth);
      }
    } else {
      componentDeclaration << generateOutputInterfaceComponent(i2sBitWidth) << std::endl;
      componentDeclaration << generateFPCToI2SComponent(fpcBitWidth, i2sBitWidth) << std::endl;
      for (auto i = 0; i < numAudioCodecs; i++) {
        outputInterfaceSignals << generateOutputInterfaceSignalNames(i, i2sBitWidth);
        outputInterfaceMapping << generateOutputInterfaceMapping(i, i2sBitWidth);
        // need to generate these intermediate signals here as there are sometimes less less output ports
        // in the Faust component than there are output ports in the output interfaces
        FPCToI2SSignals << generateFPCToI2SSignalNames(i);
      }
    }
  }

  // Specify port listing according to number of inputs and outputs
  portListing << "generic (\n"
              << "    " << "ram_width : natural := " << std::to_string(fpcBitWidth) << ";\n"
              << "    " << "ram_depth : natural := 2);\n" // buffer size
              << "port (\n"
              << "    " << "clk : in std_logic;\n"
              << "    " << "rst : in std_logic;"
              << std::endl;
  entityMapping << entityName << "_0 : component " << entityName << "\n" // default to just a single component
                << "port map (\n"
                << "clk => sys_clk_sig,\n"
                << "rst => rst_sig," << std::endl;
  if (!dataDriven) {
    portListing << "    " << "cycle_count : in integer;" << std::endl;
    entityMapping << "cycle_count => counter_sig," << std::endl;
  }
  // Specify ready, valid, and data ports for each input port:
  for (auto i = 0; i < numInputPorts; i++) {
    std::string readyInPort = "    " + entityName + "_in" + "_ready_" + std::to_string(i);
    std::string validInPort = "    " + entityName + "_in" + "_valid_" + std::to_string(i);
    std::string dataInPort = "    " + entityName + "_in" + "_data_" + std::to_string(i);
    std::string portListingDelimiter = ";\n";
    std::string mappingDelimiter = ",\n";
    if (dataDriven) {
      portListing << readyInPort << " : out std_logic" << portListingDelimiter
                  << validInPort << " : in std_logic" << portListingDelimiter;
      entityMapping << readyInPort << " => " << entityName << "_in_ready_" << std::to_string(i)
                    << "_i2s_to_fpc_" << std::to_string(i) << "_op_out_ready_0" << mappingDelimiter
                    << validInPort << " => i2s_to_fpc_" << std::to_string(i) << "_op_out_valid_0" << mappingDelimiter;
    }
    if ((i + 1 == numInputPorts) && (numOutputPorts == 0)) { // if dsp component only has inputs
      portListingDelimiter = "\n);";
      mappingDelimiter = "\n);"; // last line in port mapping should close bracket
    }
    portListing << dataInPort << " : in std_logic_vector(ram_width - 1 downto 0) := (others => '0')"
                << portListingDelimiter << std::endl;
    if (!dataDriven) {
      entityMapping << dataInPort << " => " << entityName << "_in_data_" << std::to_string(i)
                    << mappingDelimiter << std::endl;
    } else {
      entityMapping << dataInPort << " => i2s_to_fpc_" << std::to_string(i) << "_op_out_data_0"
                    << mappingDelimiter << std::endl;
    }
    // generate separate I2S to FPC component for each input port
    I2SToFPCInstantiation << generateI2SToFPCMapping(i, entityName);
    if (!dataDriven) {
      entitySignals << "signal " << entityName << "_in_data_" << std::to_string(i)
                    << " : std_logic_vector(33 downto 0);\n";
    } else {
      entitySignals << "signal " << entityName << "_in_ready_" << std::to_string(i)
                    << "_i2s_to_fpc_" << std::to_string(i) << "_op_out_ready_0 : std_logic;\n";
    }
  }
  // Specify ready, valid, and data ports for each output port:
  for (auto i = 0; i < numOutputPorts; i++) {
    std::string readyOutPort = "    " + entityName + "_out" + "_ready_" + std::to_string(i);
    std::string validOutPort = "    " + entityName + "_out" + "_valid_" + std::to_string(i);
    std::string dataOutPort = "    " + entityName + "_out" + "_data_" + std::to_string(i);
    std::string portListingDelimiter = ";\n";
    std::string mappingDelimiter = ",\n";
    std::vector<std::string> entitySignalNames(3);
    entitySignalNames[VALID] = entityName + "_out_valid_" + std::to_string(i) +
      "_fpc_to_i2s_" + std::to_string(i) + "_op_in_valid_0";
    entitySignalNames[READY] = entityName + "_in_ready_" + std::to_string(i) +
      "_i2s_to_fpc_" + std::to_string(i) + "_op_out_ready_0";
    entitySignalNames[DATA] = entityName + "_out_data_" + std::to_string(i) +
      "_fpc_to_i2s_" + std::to_string(i) + "_op_in_data_0";
    if (dataDriven) {
      portListing << readyOutPort << " : in std_logic" << portListingDelimiter
                  << validOutPort << " : out std_logic" << portListingDelimiter;
      entityMapping << readyOutPort << " => "
                    << "fpc_to_i2s_" << std::to_string(i) << "_op_in_ready_0"
                    << mappingDelimiter << validOutPort << " => "
                    << entitySignalNames[VALID] << mappingDelimiter;
    }
    if ((i + 1 == numOutputPorts)) {
      portListingDelimiter = "\n);";
      mappingDelimiter = "\n);"; // last line in port mapping should close bracket
    }
    portListing << dataOutPort << " : out std_logic_vector(ram_width - 1 downto 0) := (others => '0')"
                << portListingDelimiter << std::endl;
    if (!dataDriven) {
      entityMapping << dataOutPort << " => " << entityName << "_out_data_" << std::to_string(i)
                    << mappingDelimiter << std::endl;
    } else {
      entityMapping << dataOutPort << " => " << entitySignalNames[DATA]
                    << mappingDelimiter << std::endl;
    }
    // generate separate FPC to I2S component for each output port
    FPCToI2SInstantiation << generateFPCToI2SMapping(i, entityName);
    // intermediate signals are named after a component's output ports
    if (!dataDriven) {
      entitySignals << "signal " << entityName << "_out_data_"
                    << std::to_string(i)
                    << " : std_logic_vector( 33 downto 0 );" << std::endl;
    } else {
      entitySignals << "signal " << entitySignalNames[VALID] << " : std_logic;\n"
                    << "signal " << entitySignalNames[DATA] << " : std_logic_vector( 33 downto 0 );"
                    << std::endl;
    }
  }
  // audio interface constants TODO add proper compute time
  if (!dataDriven) {
    int slack = 100;
    int compute = 25;
    int period = 5209;
    entitySignals << "signal counter_sig : integer;" << std::endl;
    entitySignals << "constant SLACK : integer := " << std::to_string(slack) << ";" << std::endl;
    entitySignals << "constant COMPUTE : integer := " << std::to_string(compute) << ";" << std::endl;
    // cycle counter mapping declared here:
    entityMapping << "counter : component cycle_counter\n"
                  << "    generic map (period => " << std::to_string(period)
                  << ")\n"
                  << "    port map (clk => sys_clk_sig,\n"
                  << "              rst => rst_sig,\n"
                  << "              count => counter_sig);" << std::endl;
  }
  // generate flopoco operators
  vhdlOutput.open(topDir + entityName + "_top.vhd"); // instantiate VHDL file
  std::ifstream operatorRef(referenceDir + "audio_interface_wrapper.vhdl");
  std::string fileContent;
  std::map<std::string, std::string> replacementWords = {{"$ENTITY_NAME", entityName},
                                                         {"$ENTITY_PORTS", portListing.str()},
                                                         {"$AUDIO_INTERFACE_WRAPPER_PORTS", audioInterfacePortListing.str()},
                                                         {"$AUDIO_INTERFACE_WRAPPER_MAPPING", audioInterfaceMapping.str()},
                                                         {"$I2S_TRANSCEIVER_MAPPING", i2sTransceiverMapping.str()},
                                                         {"$INPUT_INTERFACE_MAPPING", inputInterfaceMapping.str()},
                                                         {"$OUTPUT_INTERFACE_MAPPING", outputInterfaceMapping.str()},
                                                         {"$COMPONENT_DECLARATION", componentDeclaration.str()},
                                                         {"$I2S_TRANSCEIVER_SIGNALS", i2sTransceiverSignals.str()},
                                                         {"$INPUT_INTERFACE_SIGNALS", inputInterfaceSignals.str()},
                                                         {"$OUTPUT_INTERFACE_SIGNALS", outputInterfaceSignals.str()},
                                                         {"$ENTITY_SIGNALS", entitySignals.str()},
                                                         {"$ENTITY_MAPPING", entityMapping.str()},
                                                         {"$I2STOFPC_SIGNALS", I2SToFPCSignals.str()},
                                                         {"$FPCTOI2S_SIGNALS", FPCToI2SSignals.str()},
                                                         {"$I2STOFPC_COMPONENTS", I2SToFPCInstantiation.str()},
                                                         {"$FPC2I2S_COMPONENTS", FPCToI2SInstantiation.str()}};
  if (operatorRef.is_open()) {
    while (std::getline(operatorRef, fileContent)) {
      for (const std::string &word : wordsToReplace) { // TODO account for multiple occurances in single line
        size_t pos = fileContent.find(word);
        if (pos != std::string::npos) {
          fileContent.replace(pos, word.length(),
                              replacementWords[word]);
        }
      }
      vhdlOutput << fileContent << std::endl;
    }
    operatorRef.close();
    vhdlOutput.close();
  } else {
    VERBOSE_ERROR( "Reference file for " << referenceDir << "audio_interface_wrapper.vhdl"
                   << " does not exist/not found!"); // TODO turn into assert
  }
}

std::string algorithms::generateInputInterfaceComponent(int bitWidth) {
  std::stringstream outputStream;
  outputStream << "component input_interface is\n"
               << "  port ("
               << "    clk : in std_logic;\n"
               << "    ws : in std_logic;\n"
               << "    rst : in std_logic;\n"
               << "    l_valid : out std_logic;\n"
               << "    r_valid : out std_logic;\n"
               << "    l_ready : in std_logic;\n"
               << "    r_ready : in std_logic;\n"
               << "    l_data_in : in std_logic_vector ( "
               << std::to_string(bitWidth - 1) << " downto 0 );\n"
               << "    r_data_in : in std_logic_vector ( "
               << std::to_string(bitWidth - 1) << " downto 0 );\n"
               << "    l_data_out : out std_logic_vector ( "
               << std::to_string(bitWidth - 1) << " downto 0 );\n"
               << "    r_data_out : out std_logic_vector ( "
               << std::to_string(bitWidth - 1)  << " downto 0 )\n);\n"
               << "end component input_interface;";
  return outputStream.str();
}

std::string algorithms::generateOutputInterfaceComponent(int bitWidth) {
  std::stringstream outputStream;
  outputStream << "component output_interface is\n"
               << "  port ("
               << "    clk : in std_logic;\n"
               << "    ws : in std_logic;\n"
               << "    rst : in std_logic;\n"
               << "    l_valid : in std_logic;\n"
               << "    r_valid : in std_logic;\n"
               << "    l_ready : out std_logic;\n"
               << "    r_ready : out std_logic;\n"
               << "    l_data_in : in std_logic_vector ( "
               << std::to_string(bitWidth - 1) << " downto 0 );\n"
               << "    r_data_in : in std_logic_vector ( "
               << std::to_string(bitWidth - 1) << " downto 0 );\n"
               << "    l_data_out : out std_logic_vector ( "
               << std::to_string(bitWidth - 1) << " downto 0 );\n"
               << "    r_data_out : out std_logic_vector ( "
               << std::to_string(bitWidth - 1)  << " downto 0 )\n);\n"
               << "end component output_interface;";
  return outputStream.str();
}

std::string algorithms::generateI2SToFPCComponent(int inputBitWidth,
                                                  int outputBitWidth) {
  std::stringstream outputStream;
  outputStream << "component i2s_to_fpc is\n"
               << "  port ("
               << "    clk : in std_logic;\n"
               << "    rst : in std_logic;\n"
               << "    op_in_ready_0 : out std_logic;\n"
               << "    op_in_valid_0 : in std_logic;\n"
               << "    op_in_data_0 : in std_logic_vector ( "
               << std::to_string(inputBitWidth - 1) << " downto 0 );\n"
               << "    op_out_ready_0 : in std_logic;\n"
               << "    op_out_valid_0 : out std_logic;\n"
               << "    op_out_data_0 : out std_logic_vector ( "
               << std::to_string(outputBitWidth - 1) << " downto 0 )\n);\n"
               << "end component i2s_to_fpc;";
  return outputStream.str();
}

std::string algorithms::generateFPCToI2SComponent(int inputBitWidth, int outputBitWidth) {
  std::stringstream outputStream;
  outputStream << "component fpc_to_i2s is\n"
               << "  port ("
               << "    clk : in std_logic;\n"
               << "    rst : in std_logic;\n"
               << "    op_in_ready_0 : out std_logic;\n"
               << "    op_in_valid_0 : in std_logic;\n"
               << "    op_in_data_0 : in std_logic_vector ( "
               << std::to_string(inputBitWidth - 1) << " downto 0 );\n"
               << "    op_out_ready_0 : in std_logic;\n"
               << "    op_out_valid_0 : out std_logic;\n"
               << "    op_out_data_0 : out std_logic_vector ( "
               << std::to_string(outputBitWidth - 1) << " downto 0 )\n);\n"
               << "end component fpc_to_i2s;";
  return outputStream.str();
}

std::string algorithms::generateAudioInterfaceWrapperPorts(int id) {
  // NOTE sys_clk, mclk, and reset ports are generated outside of this function as they're shared
  std::stringstream outputStream;
  outputStream << "ac_bclk_" << std::to_string(id) << " : out std_logic;\n"
               << "ac_pbdat_" << std::to_string(id) << " : out std_logic;\n"
               << "ac_recdat_" << std::to_string(id) << " : in std_logic;\n"
               << "ac_pblrc_" << std::to_string(id) << " : out std_logic;\n"
               << "ac_reclrc_" << std::to_string(id) << " : out std_logic;\n";
  return outputStream.str();
}

std::string algorithms::generateAudioInterfaceWrapperMapping(int id) {
  std::stringstream outputStream;
  std::string i2sTransceiver = "i2s_transceiver_" + std::to_string(id);
  outputStream << "ac_bclk_" << std::to_string(id) << " <= " << i2sTransceiver << "_sclk;\n"
               << "ac_pbdat_" << std::to_string(id) << " <= " << i2sTransceiver << "_sd_tx;\n"
               << i2sTransceiver << "_sd_rx" << " <= " << "ac_recdat_" << std::to_string(id) << ";\n"
               << "ac_pblrc_" << std::to_string(id) << " <= " << i2sTransceiver << "_ws;\n"
               << "ac_reclrc_" << std::to_string(id) << " <= " << i2sTransceiver << "_ws;\n";
  return outputStream.str();
}

std::string algorithms::generateI2STransceiverMapping(int id, int bitWidth) {
  std::stringstream outputStream;
  std::string vectorSize = "( " + std::to_string(bitWidth - 1) + " downto 0 )";
  outputStream << "i2s_transceiver_" << std::to_string(id) << ": component i2s_transceiver\n"
               << "    " << "port map (\n"
               << "    " << "mclk => mclk_sig,\n"
               << "    " << "reset_n => rst_sig,\n"
               << "    " << "sclk => i2s_transceiver_" << id << "_sclk,\n"
               << "    " << "ws => i2s_transceiver_" << id << "_ws,\n"
               << "    " << "sd_rx => ac_recdat_" << id << ",\n"
               << "    " << "sd_tx => i2s_transceiver_" << id << "_sd_tx,\n"
               << "    " << "l_data_rx" << vectorSize << " => i2s_transceiver_"
               << id << "_l_data_rx" << vectorSize << ",\n"
               << "    " << "r_data_rx" << vectorSize << " => i2s_transceiver_"
               << id << "_r_data_rx" << vectorSize << "," << std::endl;
  if (!dataDriven) {
    outputStream << "    " << "l_data_tx" << vectorSize << " => i2s_transceiver_"
                 << id << "_l_data_tx" << vectorSize << ",\n"
                 << "    " << "r_data_tx" << vectorSize << " => i2s_transceiver_"
                 << id << "_r_data_tx" << vectorSize << "\n);";
  } else {
    outputStream << "    " << "l_data_tx" << vectorSize << " => output_interface_"
                 << id << "_l_data_out" << vectorSize << ",\n"
                 << "    " << "r_data_tx" << vectorSize << " => output_interface_"
                 << id << "_r_data_out" << vectorSize << "\n);";
  }
  return outputStream.str();
}

std::string algorithms::generateInputInterfaceMapping(int id, int bitWidth) {
  std::stringstream outputStream;
  std::string vectorSize = "( " + std::to_string(bitWidth - 1) + " downto 0 )";
  std::string inputInterface = "input_interface_" + std::to_string(id);
  std::string i2sTransceiver = "i2s_transceiver_" + std::to_string(id);
    outputStream << inputInterface << ": component input_interface\n"
                 << "    " << "port map (\n"
                 << "    " << "clk => sys_clk_sig,\n"
                 << "    " << "rst => rst_sig,\n"
                 << "    " << "ws => " << i2sTransceiver << "_ws,\n"
                 << "    " << "l_data_in" << vectorSize << " => "
                 << i2sTransceiver << "_l_data_rx" << vectorSize << ",\n"
                 << "    " << "l_data_out" << vectorSize << " => "
                 << inputInterface << "_l_data_out" << vectorSize << ",\n"
                 << "    " << "l_ready => i2s_to_fpc_" << std::to_string(id) << "_op_in_ready_0,\n"
                 << "    " << "l_valid => " << inputInterface << "_l_valid,\n"
                 << "    " << "r_data_in" << vectorSize << " => "
                 << i2sTransceiver << "_r_data_rx" << vectorSize << ",\n"
                 << "    " << "r_data_out" << vectorSize << " => "
                 << inputInterface << "_r_data_out" << vectorSize << ",\n"
      // NOTE right channel connected to other i2s_to_fpc component
                 << "    " << "r_ready => i2s_to_fpc_" << std::to_string(id + 1) << "_op_in_ready_0,\n"
                 << "    " << "r_valid => " << inputInterface << "_r_valid\n);";

  return outputStream.str();
}

std::string algorithms::generateOutputInterfaceMapping(int id, int bitWidth) {
  std::stringstream outputStream;
  std::string vectorSize = "( " + std::to_string(bitWidth - 1) + " downto 0 )";
  std::string outputInterface = "output_interface_" + std::to_string(id);
  std::string i2sTransceiver = "i2s_transceiver_" + std::to_string(id);
    outputStream << outputInterface << ": component output_interface\n"
                 << "    " << "port map (\n"
                 << "    " << "clk => sys_clk_sig,\n"
                 << "    " << "rst => rst_sig,\n"
                 << "    " << "ws => " << i2sTransceiver << "_ws,\n"
                 << "    " << "l_data_in" << vectorSize << " => fpc_to_i2s_"
                 << std::to_string(id) << "_op_out_data_0" << vectorSize << ",\n"
                 << "    " << "l_data_out" << vectorSize << " => "
                 << outputInterface << "_l_data_out" << vectorSize << ",\n"
                 << "    " << "l_ready => " << outputInterface << "_l_ready,\n"
                 << "    " << "l_valid => fpc_to_i2s_"
                 << std::to_string(id) << "_op_out_valid_0,\n"
                 << "    " << "r_data_in" << vectorSize << " => fpc_to_i2s_"
                 << std::to_string(id + 1) << "_op_out_data_0" << vectorSize << ",\n"
                 << "    " << "r_data_out" << vectorSize << " => "
                 << outputInterface << "_r_data_out" << vectorSize << ",\n"
      // NOTE right channel connected to other i2s_to_fpc component
                 << "    " << "r_ready => " << outputInterface << "_r_ready,\n"
                 << "    " << "r_valid => fpc_to_i2s_"
                 << std::to_string(id + 1) << "_op_out_valid_0\n);";

  return outputStream.str();
}

std::string algorithms::generateI2SToFPCMapping(int id, std::string entityName) {
  std::stringstream outputStream;
  std::string channel;
  int halfPeriod = 2604;
  std::string pushStart;
  std::string popStart;
  if (id % 2 == 1) { // odd ids assigned to R channel
    channel = "r";
    pushStart = "SLACK";
    popStart = "SLACK + 1";
  } else { // even ids assigned to L channel
    channel = "l";
    pushStart = std::to_string(halfPeriod) + " + SLACK";
    popStart = std::to_string(halfPeriod) + " + SLACK + 1";
  }
  if (!dataDriven) {
    outputStream << "rx_buffer_" << std::to_string(id) << " : component sbuffer\n"
                 << "    " << "generic map (\n"
                 << "    " << "ram_width => 24,\n"
                 << "    " << "buffer_size => 1,\n"
                 << "    " << "push_start => " << pushStart << ",\n"
                 << "    " << "pop_start => " << popStart << ",\n"
                 << "    " << "init => 0)\n"
                 << "    " << "port map (\n"
                 << "    " << "clk => sys_clk_sig,\n"
                 << "    " << "rst => rst_sig,\n"
                 << "    " << "cycle_count => counter_sig,\n"
                 << "    " << "in_data => " << "i2s_transceiver_" << std::to_string(id)
                 << "_" << channel << "_data_rx,\n"
                 << "    " << "out_data => fix2fp_" << std::to_string(id)
                 << "_" << channel << "_data_in);"
                 << std::endl;
    outputStream << channel << "_fix2fp : fix2fp_and_scaledown\n"
                 << "    " << "port map (\n"
                 << "    " << "clk => sys_clk_sig,\n"
                 << "    " << "rst => rst_sig,\n"
                 << "    " << "i2s_in => fix2fp_" << std::to_string(id)
                 << "_" << channel << "_data_in,\n"
                 << "    " << "fp_out => " << entityName << "_in_data_"
                 << std::to_string(id) << ");" << std::endl;
  } else {
    outputStream << "i2s_to_fpc_" << id << ": component i2s_to_fpc\n"
                 << "    " << "port map (\n"
                 << "    " << "clk => sys_clk_sig,\n"
                 << "    " << "rst => rst_sig,\n"
                 << "    " << "op_in_data_0(23 downto 0) => input_interface_0_"
                 << channel << "_data_out(23 downto 0),\n"
                 << "    " << "op_in_ready_0 => i2s_to_fpc_"
                 << id << "_op_in_ready_0,\n"
                 << "    " << "op_in_valid_0 => input_interface_0_"
                 << channel << "_valid,\n"
                 << "    " << "op_out_data_0(33 downto 0) => i2s_to_fpc_"
                 << id << "_op_out_data_0(33 downto 0),\n"
                 << "    " << "op_out_ready_0 => " << entityName << "_in_ready_"
                 << id << "_i2s_to_fpc_" << id << "_op_out_ready_0,\n"
                 << "    " << "op_out_valid_0 => i2s_to_fpc_"
                 << id << "_op_out_valid_0\n"
                 << ");" << std::endl;
  }

  return outputStream.str();
}

std::string algorithms::generateFPCToI2SMapping(int id, std::string entityName) {
  std::stringstream outputStream;
  std::string channel;
  int halfPeriod = 2604;
  std::string pushStart;
  std::string popStart;
  if (id % 2 == 1) { // odd ids assigned to R channel
    channel = "r";
    pushStart = "2*SLACK + COMPUTE";
    popStart = "2*SLACK + COMPUTE + 1";
  } else { // even ids assigned to L channel
    channel = "l";
    pushStart = std::to_string(halfPeriod) + " + 2*SLACK + COMPUTE";
    popStart = std::to_string(halfPeriod) + " + 2*SLACK + COMPUTE + 1";
  }
  if (!dataDriven) {
    outputStream << "tx_buffer_" << std::to_string(id) << " : component sbuffer\n"
                 << "    " << "generic map (\n"
                 << "    " << "ram_width => 24,\n"
                 << "    " << "buffer_size => 1,\n"
                 << "    " << "push_start => " << pushStart << ",\n"
                 << "    " << "pop_start => " << popStart << ",\n"
                 << "    " << "init => 0)\n"
                 << "    " << "port map (\n"
                 << "    " << "clk => sys_clk_sig,\n"
                 << "    " << "rst => rst_sig,\n"
                 << "    " << "cycle_count => counter_sig,\n"
                 << "    " << "in_data => fp2fix_" << std::to_string(id)
                 << "_" << channel << "_data_out,\n"
                 << "    " << "out_data => " << "i2s_transceiver_" << std::to_string(id)
                 << "_" << channel << "_data_tx);\n"
                 << std::endl;
    outputStream << channel << "_fp2fix : fp2fix_and_scaleup\n"
                 << "    " << "port map (\n"
                 << "    " << "clk => sys_clk_sig,\n"
                 << "    " << "rst => rst_sig,\n"
                 << "    " << "fp_in => " << entityName << "_out_data_"
                 << std::to_string(id) << ",\n"
                 << "    " << "i2s_out => fp2fix_" << std::to_string(id)
                 << "_" << channel << "_data_out);" << std::endl;
  } else {
    outputStream << "fpc_to_i2s_" << id << ": component fpc_to_i2s\n"
                 << "    " << "port map (\n"
                 << "    " << "clk => sys_clk_sig,\n"
                 << "    " << "rst => rst_sig,\n"
                 << "    " << "op_in_data_0(33 downto 0) => "
                 << entityName << "_out_data_" << id << "_fpc_to_i2s_" << id
                 << "_op_in_data_0,\n"
                 << "    " << "op_in_valid_0 => "
                 << entityName << "_out_valid_" << id << "_fpc_to_i2s_" << id
                 << "_op_in_valid_0,\n"
                 << "    " << "op_in_ready_0 => fpc_to_i2s_"
                 << id << "_op_in_ready_0,\n"
                 << "    " << "op_out_data_0(23 downto 0) => fpc_to_i2s_"
                 << id << "_op_out_data_0,\n"
                 << "    " << "op_out_ready_0 => " << "output_interface_0_"
                 << channel << "_ready,\n"
                 << "    " << "op_out_valid_0 => fpc_to_i2s_"
                 << id << "_op_out_valid_0\n"
                 << ");" << std::endl;
  }

  return outputStream.str();
}

std::string algorithms::generateI2STransceiverSignalNames(int id, int bitWidth) {
  std::stringstream outputStream;
  std::string vectorSize = "( " + std::to_string(bitWidth - 1) + " downto 0)";
  std::string componentName = "i2s_transceiver_" + std::to_string(id);
  outputStream << "signal " << componentName << "_l_data_rx : std_logic_vector " << vectorSize << ";\n"
               << "signal " << componentName << "_r_data_rx : std_logic_vector " << vectorSize << ";\n"
    // we only use i2s_transceiver data tx signals in non-data driven implementation
               << "signal " << componentName << "_l_data_tx : std_logic_vector " << vectorSize << ";\n"
               << "signal " << componentName << "_r_data_tx : std_logic_vector " << vectorSize << ";\n"
               << "signal " << componentName << "_sclk : std_logic;\n"
               << "signal " << componentName << "_sd_tx : std_logic;\n"
               << "signal " << componentName << "_ws : std_logic;\n"
               << "signal " << componentName << "_sd_rx : std_logic;\n";
  return outputStream.str();
}

std::string algorithms::generateInputInterfaceSignalNames(int id, int bitWidth) {
  std::stringstream outputStream;
  std::string vectorSize = "( " + std::to_string(bitWidth - 1) + " downto 0)";
  std::string componentName;
  if (!dataDriven) {
    componentName = "fix2fp_" + std::to_string(id);
    outputStream << "signal " << componentName << "_l_data_in : std_logic_vector " << vectorSize << ";\n"
                 << "signal " << componentName << "_r_data_in : std_logic_vector " << vectorSize << ";"
                 << std::endl;
  } else {
    componentName = "input_interface_" + std::to_string(id);
    outputStream << "signal " << componentName << "_l_data_out : std_logic_vector " << vectorSize << ";\n"
                 << "signal " << componentName << "_l_valid : std_logic;\n"
                 << "signal " << componentName << "_r_data_out : std_logic_vector " << vectorSize << ";\n"
                 << "signal " << componentName << "_r_valid : std_logic;" << std::endl;
  }
  return outputStream.str();
}

std::string algorithms::generateOutputInterfaceSignalNames(int id, int bitWidth) {
  std::stringstream outputStream;
  std::string vectorSize = "( " + std::to_string(bitWidth - 1) + " downto 0)";
  std::string componentName;
  if (!dataDriven) {
    componentName = "fp2fix_" + std::to_string(id);
    outputStream << "signal " << componentName << "_l_data_out : std_logic_vector " << vectorSize << ";\n"
                 << "signal " << componentName << "_r_data_out : std_logic_vector " << vectorSize << ";"
                 << std::endl;
  } else {
    componentName = "output_interface_" + std::to_string(id);
    outputStream << "signal " << componentName << "_l_data_out : std_logic_vector " << vectorSize << ";\n"
                 << "signal " << componentName << "_l_ready : std_logic;\n"
                 << "signal " << componentName << "_r_data_out : std_logic_vector " << vectorSize << ";\n"
                 << "signal " << componentName << "_r_ready : std_logic;" << std::endl;
  }
  return outputStream.str();
}

// generate signal names for I2S to FPC converter component of given ID
// signals for both L and R channels are generated simultaneously to avoid
// instances of unconnected ports
std::string algorithms::generateI2SToFPCSignalNames(int id) {
  std::stringstream outputStream;
  std::string componentNameL = "i2s_to_fpc_" + std::to_string(id*2);
  std::string componentNameR = "i2s_to_fpc_" + std::to_string((id*2)+1);
  std::string fpcVectorSize = "( 33 downto 0 )";
  outputStream << "signal " << componentNameL << "_op_out_valid_0 : std_logic;\n"
               << "signal " << componentNameL << "_op_in_ready_0 : std_logic;\n"
               << "signal " << componentNameL << "_op_out_data_0 : std_logic_vector"
               << fpcVectorSize << ";\n"
               << "signal " << componentNameR << "_op_out_valid_0 : std_logic;\n"
               << "signal " << componentNameR << "_op_in_ready_0 : std_logic;\n"
               << "signal " << componentNameR << "_op_out_data_0 : std_logic_vector"
               << fpcVectorSize << ";" << std::endl;
  return outputStream.str();
}

// generate signal names for FPC to I2S converter component of given ID
// signals for both L and R channels are generated simultaneously to avoid
// instances of unconnected ports
std::string algorithms::generateFPCToI2SSignalNames(int id) {
  std::stringstream outputStream;
  std::string componentNameL = "fpc_to_i2s_" + std::to_string(id*2);
  std::string componentNameR = "fpc_to_i2s_" + std::to_string((id*2)+1);
  std::string i2sVectorSize = "( 23 downto 0 )";
  outputStream << "signal " << componentNameL << "_op_out_valid_0 : std_logic;\n"
               << "signal " << componentNameL << "_op_in_ready_0 : std_logic;\n"
               << "signal " << componentNameL << "_op_out_data_0 : std_logic_vector"
               << i2sVectorSize << ";\n"
               << "signal " << componentNameR << "_op_out_valid_0 : std_logic;\n"
               << "signal " << componentNameR << "_op_in_ready_0 : std_logic;\n"
               << "signal " << componentNameR << "_op_out_data_0 : std_logic_vector"
               << i2sVectorSize << ";" << std::endl;
  return outputStream.str();
}

std::string algorithms::generateComponent(VHDLComponent comp) {
  std::stringstream outputStream;
  std::string componentName = comp.getType();
  int numInputPorts = comp.getInputPorts().size();
  int numOutputPorts = comp.getOutputPorts().size();

  if (!dataDriven) {
    std::vector<std::string> inPortNames =
        getImplementationInputPorts(comp.getType());
    std::vector<std::string> outPortNames =
        getImplementationOutputPorts(comp.getType());
    // every component requires clock and reset ports
    outputStream << "component "
                 << comp.getImplementationName() + "_f" +
                        std::to_string(operatorFreq)
                 << " is" << std::endl;
    outputStream << "port (\n"
                 << "    " << "clk : in std_logic;"
                 << std::endl;
    // Specify ready, valid, and data ports for each input port:
    for (auto i = 0; i < inPortNames.size(); i++) {
      outputStream << "    " + inPortNames[i] + " : in std_logic_vector("
                   << "ram_width - 1 downto 0);"
                   << std::endl;
    }

    // Specify ready, valid, and data ports for each output port:
    for (auto i = 0; i < outPortNames.size(); i++) {
      if (i + 1 == numOutputPorts) {
        outputStream << "    " + outPortNames[i] + " : out std_logic_vector("
                     << "ram_width - 1 downto 0)" << std::endl; // last line of port declaration has no terminating semicolon
      } else {
        outputStream << "    " + outPortNames[i] + " : out std_logic_vector("
                     << "ram_width - 1 downto 0);" << std::endl;
      }
    }

    outputStream << "); end component;" << std::endl;
  } else {
    componentName = comp.getType();
    // every component requires clock and reset ports
    outputStream << "component " << componentName << " is" << std::endl;
    outputStream << "port (\n"
                 << "    " << "clk : in std_logic;\n"
                 << "    " << "rst : in std_logic;\n"
                 << std::endl;
    // Specify ready, valid, and data ports for each input port:
    for (auto i = 0; i < numInputPorts; i++) {
      std::string portName = "    op_in";
      outputStream << portName + "_ready_" + std::to_string(i) + " : out std_logic;\n"
                   << portName + "_valid_" + std::to_string(i) + " : in std_logic;\n"
                   << portName + "_data_" + std::to_string(i) + " : in std_logic_vector("
                   << "ram_width - 1 downto 0);\n"
                   << std::endl;
    }

    // Specify ready, valid, and data ports for each output port:
    for (auto i = 0; i < numOutputPorts; i++) {
      std::string portName = "    op_out";
      outputStream << portName + "_ready_" + std::to_string(i) + " : in std_logic;\n"
                   << portName + "_valid_" + std::to_string(i) + " : out std_logic;\n";
      if (i + 1 == numOutputPorts) {
        outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                     << "ram_width - 1 downto 0)\n" << std::endl; // last line of port declaration has no terminating semicolon
      } else {
        outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                     << "ram_width - 1 downto 0);\n" << std::endl;
      }
    }

    outputStream << "); end component;\n" << std::endl;
  }
  return outputStream.str();
}

std::string algorithms::generateInputOutputSelectorComponent(VHDLComponent comp,
                                                             std::map<int, int> inputCounts,
                                                             std::map<int, int> outputCounts) {
  std::stringstream outputStream;
  std::string componentName = comp.getType();
  if (!dataDriven) {
    if (comp.getType() == "input_selector") {
      for (auto &i : inputCounts) {
        int numInputPorts = i.first;
        std::string compNumberedName = componentName + "_" + std::to_string(numInputPorts);
        // every component requires clock and reset ports
        outputStream << "component " << compNumberedName << " is" << std::endl;
        outputStream << "generic ( ram_width : integer;" << std::endl;
        for (auto port = 0; port < numInputPorts; port++) {
          outputStream << "          "
                       << "exec_time_" << std::to_string(port)
                       << " : integer := 0";
          if (port + 1 == numInputPorts) {
            outputStream << ");" << std::endl;
          } else {
            outputStream << ";" << std::endl;
          }
        }
        outputStream << "port (\n"
                     << "    " << "clk : in std_logic;\n"
                     << "    " << "rst : in std_logic;\n"
                     << "    " << "cycle_count : in integer;"
                     << std::endl;
        for (auto port = 0; port < numInputPorts; port++) {
          outputStream << "    "
                       << "in_data_" << std::to_string(port)
                       << " : in std_logic_vector(ram_width-1 downto 0);"
                       << std::endl;
        }
        outputStream
            << "    "
            << "out_data_0 : out std_logic_vector(ram_width-1 downto 0)"
            << std::endl;
        outputStream << "); end component;\n" << std::endl;
      }
    } else { // output_selector
      for (auto &i : outputCounts) {
        int numOutputPorts = i.first;
        std::string compNumberedName = componentName + "_" + std::to_string(numOutputPorts);
        // every component requires clock and reset ports
        outputStream << "component " << compNumberedName << " is" << std::endl;
        outputStream << "generic ( ram_width : integer;" << std::endl;
        for (auto port = 0; port < numOutputPorts; port++) {
          outputStream << "          "
                       << "exec_time_" << std::to_string(port)
                       << " : integer := 0" << std::endl;
          if (port + 1 == numOutputPorts) {
            outputStream << ");" << std::endl;
          } else {
            outputStream << ";" << std::endl;
          }
        }
        outputStream << "port (\n"
                     << "    " << "clk : in std_logic;\n"
                     << "    " << "rst : in std_logic;\n"
                     << "    " << "cycle_count : in integer;" << std::endl;
        for (auto port = 0; port < numOutputPorts; port++) {
          outputStream << "    "
                       << "out_data_" << std::to_string(port)
                       << " : out std_logic_vector(ram_width-1 downto 0);"
                       << std::endl;
        }
        outputStream
          << "    "
          << "in_data_0 : in std_logic_vector(ram_width-1 downto 0)"
          << std::endl;
        outputStream << "); end component;\n" << std::endl;
      }
    }
  } else {
    if (componentName == "input_selector") {
      for (auto &i : inputCounts) {
        int numInputPorts = i.first;
        int numOutputPorts = 1; // NOTE we assume that all input selectors have 1 output
        std::string compNumberedName = componentName + "_" + std::to_string(numInputPorts);
        // every component requires clock and reset ports
        outputStream << "component " << compNumberedName << " is" << std::endl;
        outputStream << "generic ( num_phases : integer );" << std::endl;
        outputStream << "port (\n"
                     << "    " << "clk : in std_logic;\n"
                     << "    " << "rst : in std_logic;\n"
                     << std::endl;
        // Specify ready, valid, and data ports for each input port:
        for (auto i = 0; i < numInputPorts; i++) {
          std::string portName = "    op_in";
          outputStream << portName + "_ready_" + std::to_string(i) + " : out std_logic;\n"
                       << portName + "_valid_" + std::to_string(i) + " : in std_logic;\n"
                       << portName + "_data_" + std::to_string(i) + " : in std_logic_vector("
                       << "ram_width - 1 downto 0);\n"
                       << std::endl;
        }

        // Specify ready, valid, and data ports for each output port:
        for (auto i = 0; i < numOutputPorts; i++) {
          std::string portName = "    op_out";
          outputStream << portName + "_ready_" + std::to_string(i) + " : in std_logic;\n"
                       << portName + "_valid_" + std::to_string(i) + " : out std_logic;\n";
          if (i + 1 == numOutputPorts) {
            outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                         << "ram_width - 1 downto 0)\n" << std::endl; // last line of port declaration has no terminating semicolon
          } else {
            outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                         << "ram_width - 1 downto 0);\n" << std::endl;
          }
        }
        outputStream << "); end component;\n" << std::endl;
      }
    } else if (componentName == "output_selector") { // generate for output selector
      for (auto &i : outputCounts) {
        int numInputPorts = 1; // NOTE we assume that all output selectors have 1 input
        int numOutputPorts = i.first;
        std::string compNumberedName = componentName + "_" + std::to_string(numOutputPorts);
        // every component requires clock and reset ports
        outputStream << "component " << compNumberedName << " is" << std::endl;
        outputStream << "generic ( num_phases : integer );" << std::endl;
        outputStream << "port (\n"
                     << "    " << "clk : in std_logic;\n"
                     << "    " << "rst : in std_logic;\n"
                     << std::endl;
        // Specify ready, valid, and data ports for each input port:
        for (auto i = 0; i < numInputPorts; i++) {
          std::string portName = "    op_in";
          outputStream << portName + "_ready_" + std::to_string(i) + " : out std_logic;\n"
                       << portName + "_valid_" + std::to_string(i) + " : in std_logic;\n"
                       << portName + "_data_" + std::to_string(i) + " : in std_logic_vector("
                       << "ram_width - 1 downto 0);\n"
                       << std::endl;
        }

        // Specify ready, valid, and data ports for each output port:
        for (auto i = 0; i < numOutputPorts; i++) {
          std::string portName = "    op_out";
          outputStream << portName + "_ready_" + std::to_string(i) + " : in std_logic;\n"
                       << portName + "_valid_" + std::to_string(i) + " : out std_logic;\n";
          if (i + 1 == numOutputPorts) {
            outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                         << "ram_width - 1 downto 0)\n" << std::endl; // last line of port declaration has no terminating semicolon
          } else {
            outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                         << "ram_width - 1 downto 0);\n" << std::endl;
          }
        }
        outputStream << "); end component;\n" << std::endl;
      }
    } else {
      VERBOSE_ERROR("Invalid component: " << componentName);
    }
  }

  return outputStream.str();
}

std::string algorithms::generateDelayComponent(VHDLComponent comp) {
  std::stringstream outputStream;
  std::string componentName = comp.getType();
  int numInputPorts = comp.getInputPorts().size();
  int numOutputPorts = comp.getOutputPorts().size();

  outputStream << "component delay is\n"
               << "generic (\n"
               << "    " << "ram_width : natural;\n"
               << "    " << "ram_depth : natural;\n"
               << "    " << "ram_init : natural\n"
               << ");" << std::endl;
  outputStream << "port (\n"
               << "    " << "clk : in std_logic;\n"
               << "    " << "rst : in std_logic;\n"
               << std::endl;
  // Specify ready, valid, and data ports for each input port:
  for (auto i = 0; i < numInputPorts; i++) {
    std::string portName = "    op_in";
    if (comp.getType() == "const_value") {
      portName = "    in";
    }
    outputStream << portName + "_ready_" + std::to_string(i) + " : out std_logic;\n"
                 << portName + "_valid_" + std::to_string(i) + " : in std_logic;\n"
                 << portName + "_data_" + std::to_string(i) + " : in std_logic_vector("
                 << "33 downto 0);\n"
                 << std::endl;
  }
  // Specify ready, valid, and data ports for each output port:
  for (auto i = 0; i < numOutputPorts; i++) {
    std::string portName = "    op_out";
    if (comp.getType() == "const_value") {
      portName = "    out";
    }
    outputStream << portName + "_ready_" + std::to_string(i) + " : in std_logic;\n"
                 << portName + "_valid_" + std::to_string(i) + " : out std_logic;\n";
    if (i + 1 == numOutputPorts) {
      outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                   << "33 downto 0)\n" << std::endl; // last line of port declaration has no terminating semicolon
    } else {
      outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                   << "33 downto 0);\n" << std::endl;
    }
  }

  outputStream << "); end component;\n" << std::endl;
  return outputStream.str();
}

std::string algorithms::generateBufferComponent(std::string circuitName) {
  std::stringstream outputStream;

  // every component requires clock and reset ports
  outputStream << "component hs_fifo is\n"
               << "generic (\n"
               << "    " << "ram_width : natural;\n"
               << "    " << "ram_depth : natural;\n"
               << "    " << "ram_init : natural\n"
               << ");\n"
               << "port (\n"
               << "    " << "buffer_clk : in std_logic;\n"
               << "    " << "buffer_rst : in std_logic;\n"
               << std::endl;
  outputStream << "    " << "buffer_in_ready : out std_logic;\n"
               << "    " << "buffer_in_valid : in std_logic;\n"
               << "    " << "buffer_in_data : in std_logic_vector("
               << "ram_width - 1 downto 0);\n"
               << std::endl;
  outputStream << "    " << "buffer_out_ready : in std_logic;\n"
               << "    " << "buffer_out_valid : out std_logic;\n"
               << "    " << "buffer_out_data : out std_logic_vector("
               << "ram_width - 1 downto 0)\n"
               << std::endl;
  outputStream << "); end component;\n" << std::endl;
  return outputStream.str();
}

std::string algorithms::generateSBufferComponent() {
  std::stringstream outputStream;
  // every component requires clock and reset ports
  outputStream << "component sbuffer is\n"
               << "generic (\n"
               << "    " << "ram_width : integer;\n"
               << "    " << "buffer_size : integer;\n"
               << "    " << "push_start : integer;\n"
               << "    " << "pop_start : integer;\n"
               << "    " << "init : integer);\n"
               << "port (\n"
               << "    " << "clk : in std_logic;\n"
               << "    " << "rst : in std_logic;\n"
               << "    " << "cycle_count : in integer;\n"
               << "    " << "in_data : in std_logic_vector(ram_width-1 downto 0);\n"
               << "    " << "out_data : out std_logic_vector(ram_width-1 downto 0)\n"
               << "); end component;\n" << std::endl;
  return outputStream.str();
}

std::string algorithms::generateCycleCounterComponent() {
  std::stringstream outputStream;
  // every component requires clock and reset ports
  outputStream << "component cycle_counter is\n"
               << "generic (period : integer := 0);\n"
               << "port (\n"
               << "    " << "clk : in std_logic;\n"
               << "    " << "rst : in std_logic;\n"
               << "    " << "count : out integer range - 1 to period-1 := 0\n"
               << "); end component;\n" << std::endl;
  return outputStream.str();
}

std::string algorithms::generateInputScalingComponents(int i2sBitWidth,
						       int fpcBitWidth) {
  std::stringstream outputStream;
  outputStream << "component fix2fp_and_scaledown is\n"
               << "generic (\n"
               << "    " << "i2s_bit_width : natural := " << std::to_string(i2sBitWidth) << ";\n"
               << "    " << "fp_bit_width : natural := " << std::to_string(fpcBitWidth) << ";\n"
               << "    " << "bit_depth : natural := 2;\n"
               << "    " <<  "scale_factor : std_logic_vector(33 downto 0) := \"0100110100000000000000000000000001\"); -- 1 / (2^23 - 1)\n"
               << "port (\n"
               << "    " << "clk : in std_logic;\n"
               << "    " << "rst : in std_logic;\n"
               << "    " << "i2s_in : in std_logic_vector(i2s_bit_width-1 downto 0) := (others => '0');\n"
               << "    " << "fp_out : out std_logic_vector(fp_bit_width-1 downto 0) := (others => '0')\n"
               << "); end component;\n" << std::endl;
  return outputStream.str();
}

std::string algorithms::generateOutputScalingComponents(int i2sBitWidth,
							int fpcBitWidth) {
  std::stringstream outputStream;
  outputStream << "component fp2fix_and_scaleup is\n"
               << "generic (\n"
               << "    " << "i2s_bit_width : natural := " << std::to_string(i2sBitWidth) << ";\n"
               << "    " << "fp_bit_width : natural := " << std::to_string(fpcBitWidth) << ";\n"
               << "    " << "bit_depth : natural := 2;\n"
               << "    " << "scale_factor : std_logic_vector(33 downto 0) := \"0101001010111111111111111111111110\"); -- 2^23 - 1\n"
               << "port (\n"
               << "    " << "clk : in std_logic;\n"
               << "    " << "rst : in std_logic;\n"
               << "    " << "fp_in : in std_logic_vector(fp_bit_width-1 downto 0) := (others => '0');\n"
               << "    " << "i2s_out : out std_logic_vector(i2s_bit_width-1 downto 0) := (others => '0')\n"
               << "); end component;\n" << std::endl;
  return outputStream.str();
}

std::string algorithms::generateConstComponents(std::map<int, int> outputCounts) {
  std::stringstream outputStream;
  std::string componentName;
  for (auto &i : outputCounts) {
    componentName = "const_value_" + std::to_string(i.first);
    int numOutputPorts = i.first;
    // every component requires clock and reset ports
    outputStream << "component " << componentName << " is\n"
                 << "generic (\n"
                 << "    " << "value : std_logic_vector(ram_width - 1 downto 0)\n"
                 << ");\n"
                 << "port (\n"
                 << "    " << "clk : in std_logic;\n"
                 << "    " << "rst : in std_logic;\n"
                 << std::endl;
    // Specify ready, valid, and data ports for each output port:
    for (auto i = 0; i < numOutputPorts; i++) {
      std::string portName = "    out";
      if (dataDriven) {
        outputStream << portName + "_ready_" + std::to_string(i) + " : in std_logic;\n"
                     << portName + "_valid_" + std::to_string(i) + " : out std_logic;\n";
      }
      if (i + 1 == numOutputPorts) {
        outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                     << "ram_width - 1 downto 0)\n" << std::endl; // last line of port declaration has no terminating semicolon
      } else {
        outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                     << "ram_width - 1 downto 0);\n" << std::endl;
      }
    }

    outputStream << "); end component;\n" << std::endl;
  }
  return outputStream.str();
}

std::string algorithms::generateUIComponents(VHDLComponent comp) { // TODO combine with generateComponent
  std::stringstream outputStream;
  std::string componentName = comp.getType();
  int numInputPorts = comp.getInputPorts().size();
  int numOutputPorts = comp.getOutputPorts().size();
  // every component requires clock and reset ports
  outputStream << "component " << componentName << " is\n";
  if (componentName == "hslider" || componentName == "vslider" ||
      componentName == "button" || componentName == "checkbox" ||
      componentName == "nentry") {
    outputStream << "generic (\n"
                 << "    " << "default_value : std_logic_vector(ram_width - 1 downto 0)\n"
                 << ");\n";
  }
  outputStream << "port (\n"
               << "    " << "clk : in std_logic;\n"
               << "    " << "rst : in std_logic;\n" << std::endl;
  if (componentName == "hslider" || componentName == "vslider" ||
      componentName == "button" || componentName == "checkbox" ||
      componentName == "nentry") {
    // additional input port required to connect UI component to pin out (even though it's not reflected in SDF)
    outputStream << "    " << "op_in_data_0 : in std_logic_vector(ram_width - 1 downto 0);\n"
                 << std::endl;
  }
  // Specify ready, valid, and data ports for each input port:
  for (auto i = 0; i < numInputPorts; i++) {
    std::string portName = "    op_in";
    outputStream << portName + "_ready_" + std::to_string(i) + " : out std_logic;\n"
                 << portName + "_valid_" + std::to_string(i) + " : in std_logic;\n"
                 << portName + "_data_" + std::to_string(i) + " : in std_logic_vector("
                 << "ram_width - 1 downto 0);\n"
                 << std::endl;
  }
  // Specify ready, valid, and data ports for each output port:
  for (auto i = 0; i < numOutputPorts; i++) {
    std::string portName = "    op_out";
    outputStream << portName + "_ready_" + std::to_string(i) + " : in std_logic;\n"
                 << portName + "_valid_" + std::to_string(i) + " : out std_logic;\n";
    if (i + 1 == numOutputPorts) {
      outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                   << "ram_width - 1 downto 0)\n" << std::endl; // last line of port declaration has no terminating semicolon
    } else {
      outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                   << "ram_width - 1 downto 0);\n" << std::endl;
    }

  }
  outputStream << "); end component;\n" << std::endl;

  return outputStream.str();
}

// generate one HS splitter component for each corresponding Proj component (defined by number of outputs)
// TODO find a way to generalise splitter component and generation
std::string algorithms::generateSplitterComponents(std::map<int, int> outputCounts) {
  std::stringstream outputStream;
  std::string componentName;
  for (auto &i : outputCounts) {
    componentName = "hs_splitter_" + std::to_string(i.first);
    int numInputPorts = 1; // NOTE hardcoded here that HS splitter components all have 1 input
    int numOutputPorts = i.first;
    // every component requires clock and reset ports
    outputStream << "component " << componentName << " is\n"
                 << "generic (\n"
                 << "    " << "bit_width : natural := ram_width\n"
                 << ");\n"
                 << "port (\n"
                 << "    " << "clk : in std_logic;\n"
                 << "    " << "rst : in std_logic;\n"
                 << std::endl;
    // Specify ready, valid, and data ports for each input port:
    for (auto i = 0; i < numInputPorts; i++) {
      std::string portName = "    in";
      outputStream << portName + "_ready_" + std::to_string(i) + " : out std_logic;\n"
                   << portName + "_valid_" + std::to_string(i) + " : in std_logic;\n"
                   << portName + "_data_" + std::to_string(i) + " : in std_logic_vector("
                   << "bit_width - 1 downto 0);\n"
                   << std::endl;
    }

    // Specify ready, valid, and data ports for each output port:
    for (auto i = 0; i < numOutputPorts; i++) {
      std::string portName = "    out";
      outputStream << portName + "_ready_" + std::to_string(i) + " : in std_logic;\n"
                   << portName + "_valid_" + std::to_string(i) + " : out std_logic;\n";
      if (i + 1 == numOutputPorts) {
        outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                     << "bit_width - 1 downto 0)\n" << std::endl; // last line of port declaration has no terminating semicolon
      } else {
        outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                     << "bit_width - 1 downto 0);\n" << std::endl;
      }
    }

    outputStream << "); end component;\n" << std::endl;
  }
  return outputStream.str();
}

// Copy FloPoCo-HS interface component specification from reference files to generated subdirectory
void algorithms::generateHSInterfaceComponents() {
  // names of reference files required to copy into project; add/remove as required
  // TODO only produce the HS component files if necessary; right now, we're just writing every file
  std::vector<std::string> componentNames = {"hs_merger", "countdown",
                                             "store_send", "hs_merger_negate",
                                             "hs_merger_one", "hs_merger_three"};

  for (const auto &component : componentNames) {
    const auto copyOptions = std::filesystem::copy_options::update_existing |
                             std::filesystem::copy_options::recursive;
    std::filesystem::copy(referenceDir + component + ".vhdl",
                          componentDir + component + ".vhdl", copyOptions);
  }
}

// Copy VHDL components necessary for interfacing with the audio codec from reference files to generated subdirectory
void algorithms::generateAudioInterfaceComponents() {
  // names of reference files required to copy into project; add/remove as
  // required
  std::vector<std::string> componentNames;
  std::vector<std::string> operatorNames; // need separate path for FloPoCo operators as they're stored in different subdirectory
  if (!dataDriven) {
    componentNames = {"fix2fp_and_scaledown", "fp2fix_and_scaleup",
                      "i2s_transceiver", "cycle_counter", "sbuffer"};
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
  std::map<std::string, std::string> replacementWords = {{"$OP_FREQ",
                                                            std::to_string(operatorFreq)}}; // name component according to operator frequency
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

// Generate port mapping for each operator in the circuit
std::string algorithms::generatePortMapping(const VHDLCircuit &circuit) {
  std::stringstream outputStream;
  std::string componentName;
  std::map<std::string, int> opCount(circuit.getOperatorMap());

  outputStream << "begin\n" << std::endl;
  if (!dataDriven) {
    // track operator counts to give them unique names for each instance we need
    // to perform port mapping for
    for (auto &c : opCount) {
      c.second = 0;
    }
    for (auto &[v, comp] : circuit.getComponentMap()) {
      if (comp.getType() != "INPUT" && comp.getType() != "OUTPUT") {
        std::string opName = comp.getType();
        if (comp.isConst()) {
          opName = comp.getType() + "_" + std::to_string(comp.getOutputPorts().size());
          componentName = "const_value_" + std::to_string(comp.getOutputPorts().size());
        } else if (comp.getType() == "input_selector") {
          opName = "input_selector_" + std::to_string(comp.getInputPorts().size());
          componentName = opName;
        } else if (comp.getType() == "output_selector") {
          opName = "output_selector_" + std::to_string(comp.getOutputPorts().size());
          componentName = opName;
        } else if (comp.getType() == "sbuffer") {
          componentName = comp.getImplementationName();
        } else { // workaround to use implementation instead of HS interface wrapper
          componentName = comp.getImplementationName() + "_f" + std::to_string(operatorFreq);
        }
        // reset/clock mappings
        outputStream << opName << "_" + std::to_string(opCount[opName]) << " : "
                     << componentName << "\n"
                     << std::endl;
        if (comp.getType() == "const_value") {
          // NOTE binary representation of const_val operators set here
          outputStream << "generic map (\n"
                       << "    " << "value => " << "\""
                       << binaryValue(comp) << "\"" << "\n)\n"
                       << std::endl;
        } else if (comp.getType() == "input_selector") {
          outputStream << "generic map (\n"
                       << "    " << "ram_width => ram_width," << std::endl;
          for (int i = 0; i < comp.getInputPorts().size(); i++) {
            int execTime = 0; // TODO set exec time
            std::string lineEnd = ",";
            if (i + 1 == comp.getInputPorts().size()) {
              lineEnd = ")";
            }
            outputStream << "    "
                         << "exec_time_" << std::to_string(i) << " => "
                         << execTime << lineEnd << std::endl;
          }
        } else if (comp.getType() == "output_selector") {
          outputStream << "generic map (\n"
                       << "    " << "ram_width => ram_width," << std::endl;
          for (int i = 0; i < comp.getOutputPorts().size(); i++) {
            int execTime = 0; // TODO set exec time
            std::string lineEnd = ",";
            if (i + 1 == comp.getOutputPorts().size()) {
              lineEnd = ")";
            }
            outputStream << "    "
                         << "exec_time_" << std::to_string(i) << " => "
                         << execTime << lineEnd << std::endl;
          }
        } else if (comp.getType() == "sbuffer") {
          int bufferSz = 0;
          int pushStart = 0;
          int popStart = 0;
          int initTokens = comp.getSBufferInitTokens();
          outputStream << "generic map (\n"
                       << "    "
                       << "ram_width => ram_width,\n"
                       << "    "
                       << "buffer_size => " << bufferSz << ",\n"
                       << "    "
                       << "push_start => " << pushStart << ",\n"
                       << "    "
                       << "pop_start => " << popStart << ",\n"
                       << "    "
                       << "init => " << initTokens << ")" << std::endl;
        }
        outputStream << "port map (\n"
                     << "    " << "clk => " << "clk," << std::endl;
        if (!operatorLifespans.at(operatorFreq).count(comp.getType()) ||
            comp.getType() == "sbuffer") { // hacky workaround to avoid use of reset port for FloPoCo implemented operators
          outputStream << "    " << "rst => " << "rst," << std::endl;
        }
        if (comp.getType() == "sbuffer" || comp.getType() == "input_selector" ||
            comp.getType() == "output_selector") {
          outputStream << "    " << "cycle_count => cycle_count," << std::endl;
        }
        if (opName == "hslider" || opName == "vslider" || opName == "nentry" ||
            opName == "checkbox" || opName == "button") { // TODO check for all UI components
          outputStream << "    "
                       << "op_in_data_0 => " << opName
                       << "_"
                       << std::to_string(opCount[opName]) << "_data_in,\n"
                       << std::endl;
        }
        // input/output mappings
        std::vector<std::string> inputSignals = comp.getInputSignals();
        std::vector<std::string> outputSignals = comp.getOutputSignals();
        unsigned inIt = 0;
        unsigned outIt = 0;
        std::string lineEnder = ",";
        for (auto s : inputSignals) {
          std::string sigName = circuit.generateSignalNames(s, true);
          if (comp.getType() == "input_selector" ||
              comp.getType() == "output_selector") {
            outputStream << "    "
                         << "in_data_" << std::to_string(inIt) << " => "
                         << sigName << lineEnder << std::endl;
          } else {
            outputStream << "    "
                         << getImplementationInputPorts(comp.getType())[inIt]
                         << " => " << sigName << lineEnder << std::endl;
          }
          inIt++;
        }
        for (auto s : outputSignals) {
          std::string sigName = circuit.generateSignalNames(s, false);
          if (outIt + 1 == outputSignals.size()) {
            lineEnder = "";
          }
          if (comp.getType() == "input_selector" ||
              comp.getType() == "output_selector" ||
              comp.getType() == "const_value") {
            outputStream << "    "
                         << "out_data_" << std::to_string(outIt) << " => "
                         << sigName << lineEnder << std::endl;
          } else {
            outputStream << "    "
                         << getImplementationOutputPorts(comp.getType())[outIt]
                         << " => " << sigName << lineEnder << std::endl;
          }
          outIt++;
        }

        outputStream << ");\n" << std::endl;
        opCount[opName]++;
      }
    }
  } else {
    // initialise counts
    for (auto &c : opCount) {
      c.second = 0;
    }
    for (auto &[v, comp] : circuit.getComponentMap()) {
      if (comp.getType() != "INPUT" && comp.getType() != "OUTPUT") {
        std::string opName = comp.getType();
        if (comp.isConst()) {
          opName = comp.getType() + "_" + std::to_string(comp.getOutputPorts().size());
          componentName = "const_value_" + std::to_string(comp.getOutputPorts().size());
        } else if (comp.getType() == "Proj") {
          opName = "hs_splitter_" + std::to_string(comp.getOutputPorts().size());
          componentName = opName;
        } else if (comp.getType() == "input_selector") {
          opName = "input_selector_" + std::to_string(comp.getInputPorts().size());
          componentName = opName;
        } else if (comp.getType() == "output_selector") {
          opName = "output_selector_" + std::to_string(comp.getOutputPorts().size());
          componentName = opName;
        } else {
          componentName = opName;
        }
        // reset/clock mappings
        outputStream << opName << "_" + std::to_string(opCount[opName])
                     << " : " << componentName << "\n" << std::endl;
        if (comp.isConst()) { // NOTE binary representation of const_val operators set here
          outputStream << "generic map (\n"
                       << "    " << "value => " << "\""
                       << binaryValue(comp) << "\"" << "\n)\n"
                       << std::endl;
        }
        if (opName == "hslider" || opName == "vslider" || opName == "nentry" ||
            opName == "checkbox" || opName == "button") { // TODO check for all UI components
          outputStream << "generic map (\n"
                       << "    " << "default_value =>"
                       << "\"" << "0000000000000000010000000000000000" << "\"" << "\n)\n" // TODO get default value
                       << std::endl;
        }
        if (opName == "delay") {
          int ramInitVal = 0;
          int ramDepthVal = 2048;
          outputStream << "generic map (\n"
                       << "    " << "ram_width => ram_width,\n"
                       << "    " << "ram_init => " << std::to_string(ramInitVal) << ",\n"
                       << "    " << "ram_depth => " << std::to_string(ramDepthVal)
                       << "\n)\n" << std::endl;
        }
        if (comp.getType() == "input_selector") {
          outputStream << "generic map (\n"
                       << "    " << "num_phases => "
                       << std::to_string(comp.getInputPorts().size())
                       << "\n)\n" << std::endl;
        } else if (comp.getType() == "output_selector") {
          outputStream << "generic map (\n"
                       << "    " << "num_phases => "
                       << std::to_string(comp.getOutputPorts().size())
                       << "\n)\n" << std::endl;
        }
        outputStream << "port map (\n"
                     << "    " << "clk => " << "clk,\n"
                     << "    " << "rst => " << "rst,\n"
                     << std::endl;
        if (opName == "hslider" || opName == "vslider" || opName == "nentry" ||
            opName == "checkbox" || opName == "button") { // TODO check for all UI components
          outputStream << "    "
                       << "op_in_data_0 => " << opName
                       << "_"
                       << std::to_string(opCount[opName]) << "_data_in,\n"
                       << std::endl;
        }
        // input/output mappings
        int inPortCount = 0;
        int outPortCount = 0;
        for (auto s : comp.getHSInputSignals()) {
          std::vector<std::string> sigNames = circuit.generateHSSignalNames(s, true);
          std::string sigPrefix = "op_";
          if (comp.isConst() || comp.getType() == "Proj") {
            sigPrefix = "";
          }
          outputStream << "    " << sigPrefix << "in_ready_" << std::to_string(inPortCount)
                       << " => " << sigNames[READY] << ",\n"
                       << "    " << sigPrefix << "in_valid_" << std::to_string(inPortCount)
                       << " => " << sigNames[VALID] << ",\n"
                       << "    " << sigPrefix << "in_data_" << std::to_string(inPortCount)
                       << " => " << sigNames[DATA] << ",\n" << std::endl;
          inPortCount++;
        }
        std::string lineEnder = ",";
        size_t numOutPorts = comp.getHSOutputSignals().size();
        size_t iteration = 1;
        for (auto s : comp.getHSOutputSignals()) {
          std::vector<std::string> sigNames = circuit.generateHSSignalNames(s, false);
          std::string sigPrefix = "op_";
          if (comp.isConst() || comp.getType() == "Proj") {
            sigPrefix = "";
          }
          if (iteration == numOutPorts) {
            lineEnder = "";  // last port mapping terminates without a comma
          }
          outputStream << "    " << sigPrefix << "out_ready_" << std::to_string(outPortCount)
                       << " => " << sigNames[READY] << ",\n"
                       << "    " << sigPrefix << "out_valid_" << std::to_string(outPortCount)
                       << " => " << sigNames[VALID] << ",\n"
                       << "    " << sigPrefix << "out_data_" << std::to_string(outPortCount)
                       << " => " << sigNames[DATA] << lineEnder << "\n"
                       << std::endl;
          outPortCount++;
          iteration++;
        }

        outputStream << ");\n" << std::endl;
        opCount[opName]++;
      }
    }
    // Buffer port mappings
    int bufferCount = 0;
    std::string bCompName = "hs_fifo";
    std::string bName = "buffer";
    for (auto &buffer : circuit.getConnectionMap()) {
      if (buffer.second.getInitialTokenCount()) { // only generate FIFO port mapping for necessary channels
        auto src = buffer.second.getSrcPort();
        auto dst = buffer.second.getDstPort();
        std::vector<std::string> sendSigs = circuit.generateHSSignalNames(src, 1);
        std::vector<std::string> receiveSigs = circuit.generateHSSignalNames(dst, 0);
        // ram width/depth, reset, and clock mappings
        TOKEN_UNIT bufferPadding = 1; // buffers with size 0 would cause deadlocks
        outputStream << "fifo_" + std::to_string(bufferCount)
                     << " : " << bCompName << "\n"
                     <<"generic map (\n"
                     << "    " << "ram_width => ram_width,\n"
                     << "    " << "ram_depth => " << buffer.second.getBufferSize() + bufferPadding << ",\n"
                     << "    " << "ram_init => " << buffer.second.getInitialTokenCount() << "\n)\n"
                     << std::endl;
        outputStream << "port map (\n"
                     << "    " << bName << "_clk => " << "clk,\n"
                     << "    " << bName << "_rst => " << "rst,\n"
                     << std::endl;
        // input/output mappings
        outputStream << "    " << bName << "_in_ready => " << sendSigs[READY] << ",\n"
                     << "    " << bName << "_in_valid => " << sendSigs[VALID] << ",\n"
                     << "    " << bName << "_in_data => " << sendSigs[DATA] << ",\n"
                     << std::endl;
        outputStream << "    " << bName << "_out_ready => " << receiveSigs[READY] << ",\n"
                     << "    " << bName << "_out_valid => " << receiveSigs[VALID] << ",\n"
                     << "    " << bName << "_out_data => " << receiveSigs[DATA] << "\n);\n"
                     << std::endl;
        bufferCount++;
      } else {
        // simply pass through data if no operators and no initial tokens
        if (circuit.getSrcComponent(buffer.second).getType() == "INPUT" &&
            circuit.getDstComponent(buffer.second).getType() == "OUTPUT") {
          auto edgeName = buffer.second.getName();
          std::vector<std::string> sendSigs = circuit.generateHSSignalNames(edgeName, 1);
          std::vector<std::string> receiveSigs  = circuit.generateHSSignalNames(edgeName, 0);
          outputStream << receiveSigs[VALID] << "<=" << sendSigs[VALID] << ";\n";
          outputStream << receiveSigs[DATA] << "<=" << sendSigs[DATA] << ";\n";
          outputStream << sendSigs[READY] << "<=" << receiveSigs[READY] << ";\n";
        }
      }
    }
  }

  return outputStream.str();
}


void algorithms::printCircuitInfo(models::Dataflow* const dataflow,
                                  parameters_list_t param_list) {
  VHDLCircuit circuit;
  circuit.setName(dataflow->getGraphName());

  // populate circuit object with components and connections based on dataflow
  {ForEachVertex(dataflow, actor) {
      VHDLComponent newComp(dataflow, actor);
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
