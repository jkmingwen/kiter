/*
 * VHDLGenerator.h
 *
 *  Created on: 14 June 2021
 *      Author: jkmingwen
 */

#include <bitset>
#include <set>
#include <string>
#include <filesystem>
#include "VHDLGenerator.h"
#include "VHDLComponent.h"
#include "VHDLConnection.h"
#include "VHDLCircuit.h"
#include <algorithms/transformation/singleOutput.h>
#include <printers/SDF3Wrapper.h>

// for signal name retrieval
#define VALID 0
#define READY 1
#define DATA 2

VHDLCircuit generateCircuitObject(models::Dataflow* const dataflow, bool bufferless,
                                  int operatorFreq) {

  VHDLCircuit circuit;
  std::string circuitName = dataflow->getGraphName();
  // replace prohibited characters with underscores
  std::replace(circuitName.begin(), circuitName.end(), '-', '_');
  std::replace(circuitName.begin(), circuitName.end(), '.', '_');
  circuit.setName(circuitName);
  circuit.setOperatorFreq(operatorFreq);
  VERBOSE_DEBUG( "circuit name: " << circuitName );



  // populate circuit object with components and connections based on dataflow graph
  {ForEachVertex(dataflow, actor) {
      VHDLComponent newComp(dataflow, actor);
      circuit.addComponent(newComp);
      // update execution time in dataflow according to component operator type
      TIME_UNIT opLifespan = circuit.getOperatorLifespan(newComp.getType()); // TODO automatically set lifespan to 1 for operators without specified lifespan
      VERBOSE_INFO("operator lifespan (" << newComp.getType() << "): " << opLifespan);
      if (opLifespan == 0) { // i.e. no specified lifespan (e.g. const_value, delay, Proj operators)
        std::vector<TIME_UNIT> opLifespans{1};
        // input and output selectors have multiple phases of execution and so need a vector of lifespans
        if (newComp.getType() == "input_selector" || newComp.getType() == "output_selector") {
          opLifespans = dataflow->getVertexPhaseDuration(actor);
        }
        dataflow->setVertexDuration(actor, opLifespans);
      } else {
        std::vector<TIME_UNIT> opLifespans{opLifespan};
        dataflow->setVertexDuration(actor, opLifespans);
      }
    }}
  {ForEachEdge(dataflow, edge) {
      VHDLConnection newConn(dataflow, edge);
      circuit.addConnection(newConn);
    }}

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
                    << ") as a binary is not supported.");
  }

  return binaryRepresentation;
}

// Generates VHDL code in a specified output directory
void algorithms::generateVHDL(models::Dataflow* const dataflow, parameters_list_t param_list) {

  bool outputDirSpecified = false;
  bool bufferless = false;
  // default directories/parameters defined here
  // TODO remove unnecessary defaults
  std::string topDir      = "./" + dataflow->getGraphName() + "_vhdl_gen/"; // where VHDL files will be generated
  std::string componentDir = topDir + "/components/";
  std::string tbDir = topDir + "/testbenches/";
  std::string referenceDir = "./src/libkiter/algorithms/vhdl_generation/reference_files/";
  int operatorFreq = 125; // clock frequency VHDL operators are designed to run at (in MHz)

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
  if (param_list.find("BUFFERLESS") != param_list.end()) {
    VERBOSE_INFO("Bufferless mode activated");
    bufferless = true;
  } else {
    VERBOSE_INFO("Please use '-p BUFFERLESS=t'  to generate VHDL without FIFO buffers");
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
    VERBOSE_INFO("Default reference in used, you can use -p REFERENCE_DIR=/path/to/reference_directory/ to set reference directory");
  }

  VERBOSE_INFO ("Looking in " << referenceDir << " for VHDL generation reference files");
  if (!std::filesystem::is_directory(referenceDir)) {
      VERBOSE_ERROR("Reference directory is not found.");
      VERBOSE_FAILURE();
  }

  // VHDLCircuit object specifies operators and how they're connected
  VHDLCircuit tmp = generateCircuitObject(dataflow, bufferless, operatorFreq);
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

    VERBOSE_INFO("Generate updated circuit (with multi-output actors removed)"); // the dataflow now should only have actors with single outputs (with exceptions defined in singleOutput)
    tmp = generateCircuitObject(dataflow, bufferless, operatorFreq);
  }

  VERBOSE_ASSERT (tmp.getMultiOutActors().size() == 0, "Error while add Dups") ;
  VERBOSE_INFO("Output Circuit");


  if (outputDirSpecified) { // only produce actual VHDL files if output directory specified
    const auto copyOptions = std::filesystem::copy_options::update_existing
      | std::filesystem::copy_options::recursive;
    generateOperators(tmp, componentDir, referenceDir, bufferless, operatorFreq);
    generateCircuit(tmp, topDir, bufferless);
    generateAudioInterfaceWrapper(tmp, referenceDir, topDir);
    std::filesystem::copy(referenceDir + "/testbenches/", tbDir, copyOptions);
    printers::writeSDF3File(topDir + dataflow->getGraphName() + "_exectimes.xml",
                            dataflow);
    VERBOSE_INFO("VHDL files generated in: " << topDir);
  } else {
    VERBOSE_WARNING("No VHDL files created.");
  }

  std::cout << tmp.printStatus() << std::endl;

}

void algorithms::generateOperators(VHDLCircuit &circuit, std::string compDir,
                                   std::string compRefDir, bool isBufferless,
                                   int operatorFreq) {
  std::map<std::string, int> operatorMap = circuit.getOperatorMap();
  // Generate VHDL files for individual components
  for (auto const &op : operatorMap) {
    VERBOSE_INFO( "Generate VHDL component file for " << op.first );
    // track number of inputs/outputs for cases where operator can have different number of inputs/outputs
    std::map<int, int> inputCounts;
    std::map<int, int> outputCounts;
    inputCounts = circuit.getNumInputs(op.first);
    outputCounts = circuit.getNumOutputs(op.first);
    if (op.first == "Proj") {
      generateSplitterOperators(compDir, compRefDir, outputCounts);
    } else if (op.first == "const_value") {
      generateConstOperator(compDir, compRefDir, outputCounts);
    } else if (op.first == "select2") {
      generateRoutingOperators(circuit.getFirstComponentByType(op.first),
                               compDir, compRefDir);
    } else if (op.first == "fp_floor") {
      generateFloorOperator(circuit.getFirstComponentByType(op.first),
                            compDir, compRefDir, operatorFreq);
    } else if (op.first == "delay") {
      generateDelayOperator(circuit.getFirstComponentByType(op.first),
                            compDir, compRefDir, operatorFreq);
    } else if (op.first == "input_selector" || op.first == "output_selector") {
      generateInputOutputSelectorOperator(circuit.getFirstComponentByType(op.first),
                                          compDir, compRefDir, operatorFreq,
                                          inputCounts, outputCounts);
    } else {
      generateOperator(circuit.getFirstComponentByType(op.first),
                       compDir, compRefDir, operatorFreq);
    }

  }
  for (auto &conn : circuit.getConnectionMap()) {
    if (conn.second.getInitialTokenCount()) {
      isBufferless = false;
      break;
    }
  }
  generateHSInterfaceComponents(compDir, compRefDir, isBufferless);
  generateAudioInterfaceComponents(compDir, compRefDir, operatorFreq);

}

void algorithms::generateConstOperator(std::string compDir,
                                       std::string referenceDir,
                                       std::map<int, int> outputCounts) {
  std::ofstream vhdlOutput;
  std::string operatorFileName = "const_value";
  std::string wordsToReplace[] = {"$N_OUTPUTS", "$PORT_LIST", "$PORT_CONNECTIONS"};
  // generate separate const_value components for given number of outputs required
  for (auto &i : outputCounts) {
    vhdlOutput.open(compDir + operatorFileName + "_" + std::to_string(i.first)
                    + ".vhd");
    std::stringstream portListStream;
    std::stringstream portConnStream;
    std::string portList;
    std::string portConn;
    std::string portListEnd = ";\n";
    // generate port names and behaviour
    for (int p = 0; p < i.first; p++) {
      if (p + 1 == i.first) {
        portListEnd = "";
      }
      portListStream << "out_ready_" << p << " : in std_logic;\n";
      portListStream << "out_valid_" << p << " : out std_logic;\n";
      portListStream << "out_data_" << p
                     << " : out std_logic_vector (33 downto 0) := (others => '0')"
                     << portListEnd << std::endl;
      portConnStream << "out_valid_" << p << " <= '1';\n";
      portConnStream << "out_data_" << p << " <= value;\n" << std::endl;
    }
    portList = portListStream.str();
    portConn = portConnStream.str();
    std::map<std::string, std::string> replacementWords = {{"$N_OUTPUTS", std::to_string(i.first)},
                                                           {"$PORT_LIST", portList},
                                                           {"$PORT_CONNECTIONS", portConn}};
    std::ifstream compReference(referenceDir + operatorFileName + "_n_outputs.vhd");
    std::string fileContent;
    if (compReference.is_open()) {
      while (std::getline(compReference, fileContent)) {
        for (const std::string &word : wordsToReplace) { // TODO account for multiple occurances in single line
          size_t pos = fileContent.find(word);
          if (pos != std::string::npos) {
            fileContent.replace(pos, word.length(),
                                replacementWords[word]);
          }
        }
        vhdlOutput << fileContent << std::endl;
      }
      compReference.close();
      vhdlOutput.close();
    } else {
        VERBOSE_ERROR("Reference file for " << operatorFileName << " does not exist/not found!"); // TODO turn into assert
    }
  }
}

// Generate splitter components for the Proj components in the circuit (defined by number of outputs)
void algorithms::generateSplitterOperators(std::string compDir, std::string referenceDir,
                                           std::map<int, int> outputCounts) {
  std::ofstream vhdlOutput;
  std::string operatorFileName = "hs_splitter";
  std::string wordsToReplace[] = {"$NUM_OUTPUTS", "$OUTPUT_PORTS", "$OUT_READY_SIGS",
                                  "$OUT_DATA_MAPPING", "$OUT_VALID_MAPPING"};
  for (auto &i : outputCounts) {
    std::string refFileName = operatorFileName + ".vhd";
    std::string outputFile = operatorFileName + "_" + std::to_string(i.first) + ".vhd";
    std::ifstream compReference(referenceDir + refFileName);
    std::string fileContent;
    std::stringstream outputPorts;
    std::stringstream outputReadySigs;
    std::stringstream outputDataMapping;
    std::stringstream outputValidMapping;

    for (int outCount = 0; outCount < i.first; outCount++) {
      std::string delimiter = ";\n";
      std::string boolAnd = " AND ";
      outputPorts << "out_ready_" << std::to_string(outCount) << " : in std_logic" << delimiter
                  << "out_valid_" << std::to_string(outCount) << " : out std_logic" << delimiter;
      outputDataMapping << "out_data_" << std::to_string(outCount) << " <= temp_data_0(bit_width-1 downto 0);\n";
      outputValidMapping << "out_valid_" << std::to_string(outCount) << " <= is_stored_0;\n";
      if (outCount + 1 == i.first) {
        delimiter = "\n";
        boolAnd = "";
      }
      outputPorts << "out_data_" << std::to_string(outCount) << " : out std_logic_vector(bit_width-1 downto 0)" << delimiter;
      outputReadySigs << "out_ready_" << std::to_string(outCount) << "='1'" << boolAnd;
    }
    std::map<std::string, std::string> replacementWords = {{"$NUM_OUTPUTS", std::to_string(i.first)},
                                                           {"$OUTPUT_PORTS", outputPorts.str()},
                                                           {"$OUT_READY_SIGS", outputReadySigs.str()},
                                                           {"$OUT_DATA_MAPPING", outputDataMapping.str()},
                                                           {"$OUT_VALID_MAPPING", outputValidMapping.str()}};
    vhdlOutput.open(compDir + outputFile);
    if (compReference.is_open()) {
      while (std::getline(compReference, fileContent)) {
        for (const std::string &word : wordsToReplace) { // TODO account for multiple occurances in single line
          size_t pos = fileContent.find(word);
          if (pos != std::string::npos) {
            fileContent.replace(pos, word.length(),
                                replacementWords[word]);
          }
        }
        vhdlOutput << fileContent << std::endl;
      }
      compReference.close();
      vhdlOutput.close();
    } else {
        VERBOSE_ERROR ("Reference file for " << refFileName  << " does not exist/not found!"); // TODO turn into assert
    }
  }

}

// Generate signal routing operators
// NOTE currently only supports select2 operator
void algorithms::generateRoutingOperators(VHDLComponent comp, std::string compDir,
                                          std::string referenceDir) {
  std::ofstream vhdlOutput;
  std::string entityName = comp.getType();
  std::string componentName = entityName + "_component"; // just need a distinct name for the component instantiation in the HS interface declaration in VHDL
  vhdlOutput.open(compDir + entityName + "_op.vhd"); // write operator implementation
  std::ifstream operatorRef(referenceDir + entityName + "_op.vhd");
  std::string fileContent;
  std::string wordsToReplace[] = {"$ENTITY_NAME", "$FLOPOCO_OP_NAME",
                                  "$COMPONENT_NAME", "$OP_LIFESPAN",
                                  "$HSM_TYPE"};
  std::string opInputCount = std::to_string(comp.getInputPorts().size());
  // generate select operator
  if (operatorRef.is_open()) {
    while (std::getline(operatorRef, fileContent)) {
      vhdlOutput << fileContent << std::endl;
    }
    operatorRef.close();
    vhdlOutput.close();
  } else {

      VERBOSE_ERROR ("Reference file for " << comp.getType()  << " does not exist/not found!"); // TODO turn into assert

  }
  std::string hsmType = "";
  if (comp.getType() == "select2") {
    hsmType = "_three";
  }
  std::map<std::string, std::string> replacementWords = {{"$ENTITY_NAME", entityName},
                                                         {"$FLOPOCO_OP_NAME", entityName + "_op"},
                                                         {"$COMPONENT_NAME", componentName},
                                                         {"$OP_LIFESPAN", std::to_string(1)}, // HS interface requires lifespan to be >0
                                                         {"$HSM_TYPE", hsmType}};
  // generate HS interface for select operator
  vhdlOutput.open(compDir + entityName + ".vhd");
  std::ifstream interfaceRef(referenceDir + "flopoco_hs_interface"
                             + "_" + opInputCount + ".vhd");
  if (interfaceRef.is_open()) {
    while (std::getline(interfaceRef, fileContent)) {
      for (const std::string &word : wordsToReplace) {
        size_t pos = fileContent.find(word);
        if (pos != std::string::npos) {
          fileContent.replace(pos, word.length(),
                              replacementWords[word]);
        }
      }
      vhdlOutput << fileContent << std::endl;
    }
    interfaceRef.close();
    vhdlOutput.close();
  } else {
     VERBOSE_ERROR ("HS interface file for " << "flopoco_hs_interface_" + opInputCount
                    << ".vhd does not exist/not found!"); // TODO turn into assert
  }
}

// NOTE workaround for now to generate floor operator; integrate into generateOperator in the future
void algorithms::generateFloorOperator(VHDLComponent comp, std::string compDir,
                                       std::string referenceDir, int operatorFreq) {
  // TODO a lot of things just hardcoded here; need to change this --- just a really messy way of getting this done for now
  // copy in fp_floor_freq implementation file
  generateFPCOperator("fp_floor", compDir, referenceDir, operatorFreq);

  // copy in fp_floor wrapper file and change operator frequency
  std::ofstream floorWrapperCopy;
  floorWrapperCopy.open(compDir + "fp_floor.vhd");
  std::ifstream floorWrapper(referenceDir + "fp_floor.vhd");
  std::string content;
  std::string freq = "$FREQ";
  if (floorWrapper.is_open()) {
    while (std::getline(floorWrapper, content)) {
      size_t pos = content.find(freq);
      if (pos != std::string::npos) {
        content.replace(pos, freq.length(),
                        std::to_string(operatorFreq));
      }
      floorWrapperCopy << content << std::endl;
    }
    floorWrapperCopy.close();
  } else {
    VERBOSE_ERROR ("Reference file for " << "fp_floor.vhd does not exist/not found!"); // TODO turn into assert
  }
  // generate internal components that make up floor operator
  std::string internalComponents[] = {"float2int", "int2float"};
  for (const std::string &c : internalComponents) {
    std::ofstream vhdlOutput;
    std::string entityName = c;
    std::string componentName = entityName + "_implementation";
    std::string wordsToReplace[] = {"$ENTITY_NAME", "$FLOPOCO_OP_NAME",
                                    "$COMPONENT_NAME", "$OP_LIFESPAN",
                                    "$HSM_TYPE"};
    std::string implementationName = c + "_flopoco";
    std::string lifespan = "1";
    if (operatorFreq == 250) {
      if (c == "float2int") {
        lifespan = "2";
      } else if (c == "int2float") {
        lifespan = "3";
      }
    }
    generateFPCOperator(implementationName, compDir, referenceDir, operatorFreq);
    vhdlOutput.open(compDir + entityName + ".vhd"); // instantiate VHDL file
    std::ifstream operatorRef(referenceDir + "flopoco_hs_interface_1.vhd");
    std::string fileContent;
    std::string operatorName = implementationName + "_f" + std::to_string(operatorFreq);
    std::map<std::string, std::string> replacementWords = {{"$ENTITY_NAME", entityName},
                                                           {"$FLOPOCO_OP_NAME", operatorName},
                                                           {"$COMPONENT_NAME", componentName},
                                                           {"$OP_LIFESPAN", lifespan},
                                                           {"$HSM_TYPE", "_one"}};
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
      VERBOSE_ERROR ("Reference file for " << "flopoco_hs_interface_1.vhd does not exist/not found!"); // TODO turn into assert
    }
  }
}

void algorithms::generateDelayOperator(VHDLComponent comp, std::string compDir,
                                       std::string referenceDir, int operatorFreq) {
  generateFPCOperator(comp.getImplementationName(), compDir, referenceDir, operatorFreq);
}

void algorithms::generateInputOutputSelectorOperator(VHDLComponent comp, std::string compDir,
                                                     std::string referenceDir, int operatorFreq,
                                                     std::map<int, int> inputCounts,
                                                     std::map<int, int> outputCounts) {
  std::ofstream vhdlOutput;
  std::string entityName = comp.getType();
  VERBOSE_DEBUG("entity name: " << entityName);
  std::string componentName;
  std::string wordsToReplace[] = {"$COMPONENT_NAME", "$PORT_LIST", "$PROCESS_BEHAVIOUR",
                                  "$VALID_SIGNAL_ROUTING", "$DATA_SIGNAL_ROUTING",
                                  "$READY_SIGNAL_ROUTING"};
  int opPortCount = 0;
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
      vhdlOutput.open(compDir + componentName + ".vhd"); // instantiate VHDL file
      std::ifstream operatorRef(referenceDir + entityName + ".vhd");
      std::string fileContent;
      std::map<std::string, std::string> replacementWords = {{"$COMPONENT_NAME", componentName},
                                                             {"$PORT_LIST", portList},
                                                             {"$PROCESS_BEHAVIOUR", processBehaviour},
                                                             {"$VALID_SIGNAL_ROUTING", validSignalRouting},
                                                             {"$DATA_SIGNAL_ROUTING", dataSignalRouting},
                                                             {"$READY_SIGNAL_ROUTING", readySignalRouting}};
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
        // reset strings for next iteration
        portList.clear();
        processBehaviour.clear();
        validSignalRouting.clear();
        dataSignalRouting.clear();
        readySignalRouting.clear();
      } else {
        VERBOSE_ERROR ("Reference file for " << entityName + " with " + std::to_string(ports.first)
                       << " inputs does not exist/not found!"); // TODO turn into assert
      }
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
      vhdlOutput.open(compDir + componentName + ".vhd"); // instantiate VHDL file
      std::ifstream operatorRef(referenceDir + entityName + ".vhd");
      std::string fileContent;
      std::map<std::string, std::string> replacementWords = {{"$COMPONENT_NAME", componentName},
                                                             {"$PORT_LIST", portList},
                                                             {"$PROCESS_BEHAVIOUR", processBehaviour},
                                                             {"$VALID_SIGNAL_ROUTING", validSignalRouting},
                                                             {"$DATA_SIGNAL_ROUTING", dataSignalRouting},
                                                             {"$READY_SIGNAL_ROUTING", readySignalRouting}};
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
        // reset strings for next iteration
        portList.clear();
        processBehaviour.clear();
        validSignalRouting.clear();
        dataSignalRouting.clear();
        readySignalRouting.clear();
      } else {
        VERBOSE_ERROR ("Reference file for " << entityName + " with " + std::to_string(ports.first)
                       << " inputs does not exist/not found!"); // TODO turn into assert
      }
    }
  }
}

// Copy FloPoCo operator from reference file to project
void algorithms::generateFPCOperator(std::string compImplementationName, std:: string compDir,
                                     std::string referenceDir, int operatorFreq) {
  std::ofstream vhdlOutput;
  std::string operatorRefDir = referenceDir + "/operators/";
  std::string operatorFileName = compImplementationName + "_f" + std::to_string(operatorFreq);

  vhdlOutput.open(compDir + operatorFileName + ".vhd"); // instantiate VHDL file
  std::ifstream operatorRef(operatorRefDir + operatorFileName + ".vhdl");
  std::string fileContent;
  if (operatorRef.is_open()) {
    while (std::getline(operatorRef, fileContent)) {
      vhdlOutput << fileContent << std::endl;
    }
    operatorRef.close();
    vhdlOutput.close();
  } else {
    VERBOSE_ERROR ("Reference file for " <<  compImplementationName  << " does not exist/not found!"); // TODO turn into assert
  }
}

// Generate HS interface for each FloPoCo operator
void algorithms::generateOperator(VHDLComponent comp, std::string compDir,
                                  std::string referenceDir, int operatorFreq) {
  std::ofstream vhdlOutput;
  std::string entityName = comp.getType();
  std::string componentName = entityName + "_implementation";
  std::string wordsToReplace[] = {"$ENTITY_NAME", "$FLOPOCO_OP_NAME",
                                  "$COMPONENT_NAME", "$OP_LIFESPAN",
                                  "$HSM_TYPE"};
  std::string opInputCount = std::to_string(comp.getInputPorts().size());

  if (comp.getType() != "INPUT" && comp.getType() != "OUTPUT" && !comp.isConst()) {
    // generate flopoco operators
    generateFPCOperator(comp.getImplementationName(), compDir, referenceDir, operatorFreq); // generate FloPoCo operator
    vhdlOutput.open(compDir + entityName + ".vhd"); // instantiate VHDL file
    std::ifstream operatorRef(referenceDir + "flopoco_hs_interface" +
                              + "_" + opInputCount + ".vhd");
    std::string fileContent;
    std::string operatorName = comp.getImplementationName() + "_f" + std::to_string(operatorFreq);
    std::string hsmType = "";
    if (comp.getInputPorts().size() == 1) {
      hsmType = "_one";
    }
    std::map<std::string, std::string> replacementWords = {{"$ENTITY_NAME", entityName},
                                                           {"$FLOPOCO_OP_NAME", operatorName},
                                                           {"$COMPONENT_NAME", componentName},
                                                           {"$OP_LIFESPAN", std::to_string(comp.getLifespan())},
                                                           {"$HSM_TYPE", hsmType}};
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


        VERBOSE_ERROR ("Reference file for " << "flopoco_hs_interface_" + opInputCount
                                             << ".vhd does not exist/not found!"); // TODO turn into assert

    }
  }
}

void algorithms::generateCircuit(VHDLCircuit &circuit, std::string outputDir,
                                 bool isBufferless) {
  std::ofstream vhdlOutput;
  std::string graphName = circuit.getName() + "_circuit"; // TODO decide on naming convention
  int numInputPorts = circuit.getOperatorCount("INPUT");
  int numOutputPorts = circuit.getOperatorCount("OUTPUT");
  std::map<std::string, int> operatorMap = circuit.getOperatorMap();
  bool noOperators = operatorMap.size() == 2 && operatorMap.count("INPUT") && operatorMap.count("OUTPUT"); // edge case where Faust program has no operators (only inputs/outputs)

  // Open the stream
  vhdlOutput.open(outputDir + graphName + ".vhd"); // instantiate VHDL file

  // 1. Define libraries used
    generateVHDLHeader(vhdlOutput);
    // 2. Port declarations
    generateVHDLEntity(circuit, numInputPorts, numOutputPorts, vhdlOutput);

    // 3. Specify architecture (behaviour) of operator type
    generateVHDLArchitecture(circuit, isBufferless, operatorMap, noOperators, vhdlOutput);

    // Close the stream
    vhdlOutput.close();
}

void algorithms::generateVHDLHeader(std::ofstream &vhdlOutput) {
    vhdlOutput << "library ieee;\n"
                 << "use ieee.std_logic_1164.all;\n"
                 << "use ieee.numeric_std.all;\n" << std::endl;
}

void algorithms::generateVHDLEntity(VHDLCircuit &circuit, int numInputPorts, int numOutputPorts,
                                    std::ofstream &vhdlOutput) {
    vhdlOutput << "entity " << circuit.getName() << " is\n"
               << "generic (\n"
               << "    " << "ram_width : natural := 34;\n"
               << "    " << "ram_depth : natural := 2);\n" // buffer size
               << "port (\n"
               << "    " << "clk : in std_logic;\n"
               << "    " << "rst : in std_logic;\n"
               << std::endl;
    // Specify ready, valid, and data ports for each input port:
    for (auto i = 0; i < numInputPorts; i++) {
        // TODO define portName variable outside of loop to minimize unnecessary repetition
        //      (also, for loop should be outside if loop)
        std::string portName = "    " + circuit.getName() + "_in";
        vhdlOutput << portName + "_ready_" + std::to_string(i) + " : out std_logic;\n"
                   << portName + "_valid_" + std::to_string(i) + " : in std_logic;\n"
                   << portName + "_data_" + std::to_string(i) + " : in std_logic_vector("
                   << "ram_width - 1 downto 0) := (others => '0');\n"
                   << std::endl;
    }
    // Specify ready, valid, and data ports for each output port:
    for (auto i = 0; i < numOutputPorts; i++) {
        std::string portName = "    " + circuit.getName() + "_out";
        vhdlOutput << portName + "_ready_" + std::to_string(i) + " : in std_logic;\n"
                   << portName + "_valid_" + std::to_string(i) + " : out std_logic;\n";
        if (i + 1 == numOutputPorts) {
            vhdlOutput << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                       << "ram_width - 1 downto 0) := (others => '0')\n"
                       << std::endl; // last line of port declaration has no terminating semicolon
        } else {
            vhdlOutput << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                       << "ram_width - 1 downto 0) := (others => '0');\n"
                       << std::endl;
        }
    }
    vhdlOutput << ");\nend " << circuit.getName() << ";\n" << std::endl;
}

void algorithms::generateVHDLArchitecture(VHDLCircuit &circuit, bool isBufferless, std::map<std::string, int> &operatorMap,
                                          bool noOperators, std::ofstream &vhdlOutput) {// component declaration
    vhdlOutput << "architecture behaviour of " << circuit.getName() << " is\n" << std::endl;
    if (!noOperators) { // no need to generate components other than buffers if there aren't any operators
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
    }
    // no need to generate buffer component if there aren't any operators between the input/output
    bool delayDetected = false;
    for (auto &conn : circuit.getConnectionMap()) {
      if (conn.second.getInitialTokenCount()) {
        delayDetected = true;
      }
    }
    if ((delayDetected &&
        (operatorMap.size() - (operatorMap.count("INPUT") + operatorMap.count("OUTPUT"))) > 1) || !isBufferless) { // we will always have FIFO buffers if the bufferless flag is false
      vhdlOutput << generateBufferComponent(circuit.getName()) << std::endl;
    }
    /* Track top-level input and output signals
       These are mapped such that
       input/output ID -> output/input signal name */
    // we expect each input/output component to only have 1 output/input signal respectively
    std::map<int, std::string> inSignalNames;
    std::map<int, std::string> outSignalNames;
    if (isBufferless) { // no FIFO buffers between components
      // NOTE Using a really roundabout method here to look for buffers with initial token counts; it's probably possible to reduce the number of nested loops
      for (auto &op : circuit.getComponentMap()) {
        if (op.second.getType() == "INPUT") {
          for (auto &i : op.second.getOutputEdges()) {
            for (auto &conn : circuit.getConnectionMap()) {
              if (conn.second.getName() == i) { // check if given edge has initial tokens
                if (conn.second.getInitialTokenCount()) {
                  inSignalNames[op.second.getIOId()] = conn.second.getSrcPort(); // use port names for signals if there are initial tokens as FIFO buffer will be used
                } else {
                  inSignalNames[op.second.getIOId()] = i; // if not, edge names will be used as signal names
                }
              }
            }
          }
        } else if (op.second.getType() == "OUTPUT") {
          for (auto &i : op.second.getInputEdges()) {
            for (auto &conn : circuit.getConnectionMap()) {
              if (conn.second.getName() == i) { // check if given edge has initial tokens
                if (conn.second.getInitialTokenCount()) {
                  outSignalNames[op.second.getIOId()] = conn.second.getDstPort(); // use port names for signals if there are initial tokens as FIFO buffer will be used
                } else {
                  outSignalNames[op.second.getIOId()] = i; // if not, edge names will be used as signal names
                }
              }
            }
          }
        }
      }
    } else { // use port names to generate signals to/from FIFO buffers
      for (auto &op : circuit.getComponentMap()) {
        if (op.second.getType() == "INPUT") {
          VERBOSE_ASSERT(op.second.getOutputPorts().size() == 1,
                         "Input component (" << op.second.getUniqueName() << ") should only have 1 output port");
          inSignalNames[op.second.getIOId()] = op.second.getOutputPorts().front();
        } else if (op.second.getType() == "OUTPUT") {
          VERBOSE_ASSERT(op.second.getInputPorts().size() == 1,
                         "Output component (" << op.second.getUniqueName() << ") should only have 1 input port");
          outSignalNames[op.second.getIOId()] = op.second.getInputPorts().front();
        }
      }
    }

    // Define and generate signal names
    bool isTopInput = false;
    bool isTopOutput = false;
    bool hasInputDelay;
    bool hasOutputDelay;
    for (auto &connection : circuit.getConnectionMap()) {
      int inputId = -1;
      int outputId = -1;
      std::string inActorSigName;
      std::string outActorSigName;
      // Identify when the connection is for a (top-level) input/output actor
      if (!isBufferless || connection.second.getInitialTokenCount()) {
        /* we use port names for signals when there's going to be
           a FIFO buffer placed along it - this happens when it is
           a buffered design or when the channel has initial tokens */
        inActorSigName = connection.second.getSrcPort();
        outActorSigName = connection.second.getDstPort();
      } else { // no FIFO buffer
        inActorSigName = connection.second.getName();
        outActorSigName = connection.second.getName();
      }
      for (auto &sig : inSignalNames) {
        if (inActorSigName == sig.second) {
          isTopInput = true;
          inputId = sig.first;
          break;
        }
      }
      for (auto &sig : outSignalNames) {
        if (outActorSigName == sig.second) {
          isTopOutput = true;
          outputId = sig.first;
          break;
        }
      }

      if (isTopInput) {
        VERBOSE_ASSERT (inputId > -1, "Top input connection found but ID not valid (ID: " << inputId << ")");
        std::vector<std::string> signalNames(3);
        signalNames[VALID] = circuit.getName() + "_in_valid_" + std::to_string(inputId);
        signalNames[READY] = circuit.getName() + "_in_ready_" + std::to_string(inputId);
        signalNames[DATA] = circuit.getName() + "_in_data_" + std::to_string(inputId);
        if (isBufferless && !noOperators) {
          if (connection.second.getInitialTokenCount()) {
            hasInputDelay = true;
            circuit.addInputPort(connection.second.getDstPort(), signalNames);
          } else {
            hasInputDelay = false;
            circuit.addInputPort(connection.second.getName(), signalNames);
          }
        } else if (isBufferless && noOperators) { // TODO can be reduced to fewer conditions
          circuit.addInputPort(connection.second.getSrcPort(), signalNames);
        } else {
          if (!noOperators) {
            circuit.addInputPort(connection.second.getDstPort(), signalNames);
          } else { // if there are only inputs/outputs, then the top-level input port is the output port of the INPUT operators
            circuit.addInputPort(connection.second.getSrcPort(), signalNames);
          }
        }
        isTopInput = false; // reset for next iteration
      }
      if (isTopOutput) {
        VERBOSE_ASSERT (outputId > -1, "Top output connection found but ID not valid (ID: " << inputId << ")");
        std::vector<std::string> signalNames(3);
        signalNames[VALID] = circuit.getName() + "_out_valid_" + std::to_string(outputId);
        signalNames[READY] = circuit.getName() + "_out_ready_" + std::to_string(outputId);
        signalNames[DATA] = circuit.getName() + "_out_data_" + std::to_string(outputId);
        if (isBufferless && !noOperators) {
          if (connection.second.getInitialTokenCount()) {
            hasOutputDelay = true;
            circuit.addOutputPort(connection.second.getSrcPort(), signalNames);
          } else {
            hasOutputDelay = false;
            circuit.addOutputPort(connection.second.getName(), signalNames);
          }
          circuit.addOutputPort(connection.second.getName(), signalNames);
        } else if (isBufferless && noOperators) { // TODO can be reduced to fewer conditions
          circuit.addOutputPort(connection.second.getDstPort(), signalNames);
        } else {
          if (!noOperators) {
            circuit.addOutputPort(connection.second.getSrcPort(), signalNames);
          } else { // if there are only inputs/outputs, then the top-level input port is the input port of the OUTPUT operators
            circuit.addOutputPort(connection.second.getDstPort(), signalNames);
          }
        }
        isTopOutput = false; // reset for next iteration
      }
    }

    std::vector<std::string> dataSignals;
    std::vector<std::string> validReadySignals;
    for (auto &connection : circuit.getConnectionMap()) {
      bool isNotTopInOut; // check if connection belongs to top level input/output
      if (isBufferless) {
        std::string topInSigName;
        std::string topOutSigName;
        if (hasInputDelay) {
          topInSigName = connection.second.getDstPort();
        } else {
          topInSigName = connection.second.getName();
        }
        if (hasOutputDelay) {
          topOutSigName = connection.second.getSrcPort();
        } else {
          topOutSigName = connection.second.getName();
        }
        isNotTopInOut = !(circuit.getInputPorts().count(topInSigName) ||
                          circuit.getOutputPorts().count(topOutSigName));
      } else {
        isNotTopInOut = !(circuit.getInputPorts().count(connection.second.getDstPort()) ||
                          circuit.getOutputPorts().count(connection.second.getSrcPort()));
      }
      // only generate signal names for non-input/output ports
      if (!noOperators) {
        if (isNotTopInOut) {
          if (!isBufferless || connection.second.getInitialTokenCount()) { // separate send/receive signals required due to buffers between components
            std::vector<std::string> sendSignals(generateSendSigNames(connection.second.getSrcPort(), circuit));
            std::vector<std::string> receiveSignals(generateReceiveSigNames(connection.second.getDstPort(), circuit));
            dataSignals.push_back(sendSignals[DATA]);
            dataSignals.push_back(receiveSignals[DATA]);
            validReadySignals.push_back(sendSignals[VALID]);
            validReadySignals.push_back(receiveSignals[VALID]);
            validReadySignals.push_back(sendSignals[READY]);
            validReadySignals.push_back(receiveSignals[READY]);
          } else {
            std::vector<std::string> signalNames(generateSendSigNames(connection.second.getName(), circuit));
            dataSignals.push_back(signalNames[DATA]);
            validReadySignals.push_back(signalNames[VALID]);
            validReadySignals.push_back(signalNames[READY]);
          }
        }
      } else {
        if (!isBufferless || connection.second.getInitialTokenCount()) { // separate send/receive signals required due to buffers between components
          std::vector<std::string> sendSignals(generateSendSigNames(connection.second.getSrcPort(), circuit));
          std::vector<std::string> receiveSignals(generateReceiveSigNames(connection.second.getDstPort(), circuit));
          dataSignals.push_back(sendSignals[DATA]);
          dataSignals.push_back(receiveSignals[DATA]);
          validReadySignals.push_back(sendSignals[VALID]);
          validReadySignals.push_back(receiveSignals[VALID]);
          validReadySignals.push_back(sendSignals[READY]);
          validReadySignals.push_back(receiveSignals[READY]);
        } else {
          std::vector<std::string> signalNames(generateSendSigNames(connection.second.getName(), circuit));
          dataSignals.push_back(signalNames[DATA]);
          validReadySignals.push_back(signalNames[VALID]);
          validReadySignals.push_back(signalNames[READY]);
        }
      }
    }
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
      vhdlOutput << generatePortMapping(circuit, isBufferless, noOperators) << std::endl;
      vhdlOutput << "end behaviour;" << std::endl;

      vhdlOutput.close();
    } else {
      vhdlOutput << generatePortMapping(circuit, isBufferless, noOperators) << std::endl;
      vhdlOutput << "end behaviour;" << std::endl;
    }
}



void algorithms::generateAudioInterfaceWrapper(const VHDLCircuit &circuit, const std::string &referenceDir,
                                               const std::string &outputDir) {
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
  }
  if (numInputPorts) {
    componentDeclaration << generateInputInterfaceComponent(i2sBitWidth) << std::endl;
    componentDeclaration << generateI2SToFPCComponent(i2sBitWidth, fpcBitWidth) << std::endl;
    // Generate mappings for input/output interfaces and I2S transceivers
    for (auto i = 0; i < numAudioCodecs; i++) { //
      inputInterfaceSignals << generateInputInterfaceSignalNames(i, i2sBitWidth);
      inputInterfaceMapping << generateInputInterfaceMapping(i, i2sBitWidth);
      // need to generate these intermediate signals here as there are sometimes less less input ports
      // in the Faust component than there are input ports in the input interfaces
      I2SToFPCSignals << generateI2SToFPCSignalNames(i);
    }
  }
  if (numOutputPorts) {
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

  // Specify port listing according to number of inputs and outputs
  portListing << "generic (\n"
              << "    " << "ram_width : natural := " << std::to_string(fpcBitWidth) << ";\n"
              << "    " << "ram_depth : natural := 2);\n" // buffer size
              << "port (\n"
              << "    " << "clk : in std_logic;\n"
              << "    " << "rst : in std_logic;\n"
              << std::endl;
  entityMapping << entityName << "_0 : component " << entityName << "\n" // default to just a single component
                << "port map (\n"
                << "clk => sys_clk_sig,\n"
                << "rst => rst_sig," << std::endl;
  // Specify ready, valid, and data ports for each input port:
  for (auto i = 0; i < numInputPorts; i++) {
    std::string readyInPort = "    " + entityName + "_in" + "_ready_" + std::to_string(i);
    std::string validInPort = "    " + entityName + "_in" + "_valid_" + std::to_string(i);
    std::string dataInPort = "    " + entityName + "_in" + "_data_" + std::to_string(i);
    std::string portListingDelimiter = ";\n";
    std::string mappingDelimiter = ",\n";
    portListing << readyInPort << " : out std_logic" << portListingDelimiter
                << validInPort << " : in std_logic" << portListingDelimiter;
    entityMapping << readyInPort << " => " << entityName << "_in_ready_" << std::to_string(i)
                  << "_i2s_to_fpc_" << std::to_string(i) << "_op_out_ready_0" << mappingDelimiter
                  << validInPort << " => i2s_to_fpc_" << std::to_string(i) << "_op_out_valid_0" << mappingDelimiter;
    if ((i + 1 == numInputPorts) && (numOutputPorts == 0)) { // if dsp component only has inputs
      portListingDelimiter = "\n);";
      mappingDelimiter = "\n);"; // last line in port mapping should close bracket
    }
    portListing << dataInPort << " : in std_logic_vector(ram_width - 1 downto 0) := (others => '0')"
                << portListingDelimiter << std::endl;
    entityMapping << dataInPort << " => i2s_to_fpc_" << std::to_string(i) << "_op_out_data_0"
                  << mappingDelimiter << std::endl;
    // generate separate I2S to FPC component for each input port
    I2SToFPCInstantiation << generateI2SToFPCMapping(i, entityName);
    entitySignals << "signal " << entityName << "_in_ready_" << std::to_string(i)
                  << "_i2s_to_fpc_" << std::to_string(i) << "_op_out_ready_0 : std_logic;\n";
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
    portListing << readyOutPort << " : in std_logic" << portListingDelimiter
                << validOutPort << " : out std_logic" << portListingDelimiter;
    entityMapping << readyOutPort << " => " << "fpc_to_i2s_" << std::to_string(i) << "_op_in_ready_0" << mappingDelimiter
                  << validOutPort << " => " << entitySignalNames[VALID] << mappingDelimiter;
    if ((i + 1 == numOutputPorts)) {
      portListingDelimiter = "\n);";
      mappingDelimiter = "\n);"; // last line in port mapping should close bracket
    }
    portListing << dataOutPort << " : out std_logic_vector(ram_width - 1 downto 0) := (others => '0')"
                << portListingDelimiter << std::endl;
    entityMapping << dataOutPort << " => " << entitySignalNames[DATA]
                  << mappingDelimiter << std::endl;
    // generate separate FPC to I2S component for each output port
    FPCToI2SInstantiation << generateFPCToI2SMapping(i, entityName);
    // intermediate signals are named after a component's output ports
    entitySignals << "signal " << entitySignalNames[VALID] << " : std_logic;\n"
                  << "signal " << entitySignalNames[DATA] << " : std_logic_vector( 33 downto 0 );"
                  << std::endl;
  }

  // generate flopoco operators
  vhdlOutput.open(outputDir + entityName + "_top.vhd"); // instantiate VHDL file
  std::ifstream operatorRef(referenceDir + "audio_interface_wrapper.vhd");
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
    VERBOSE_ERROR( "Reference file for " << referenceDir << "audio_interface_wrapper.vhd"
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
               << id << "_r_data_rx" << vectorSize << ",\n"
               << "    " << "l_data_tx" << vectorSize << " => output_interface_"
               << id << "_l_data_out" << vectorSize << ",\n"
               << "    " << "r_data_tx" << vectorSize << " => output_interface_"
               << id << "_r_data_out" << vectorSize << "\n);";
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
  if (id % 2 == 1) { // odd ids assigned to R channel
    channel = "r";
  } else { // even ids assigned to L channel
    channel = "l";
  }
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
  return outputStream.str();
}

std::string algorithms::generateFPCToI2SMapping(int id, std::string entityName) {
  std::stringstream outputStream;
  std::string channel;
  if (id % 2 == 1) { // odd ids assigned to R channel
    channel = "r";
  } else { // even ids assigned to L channel
    channel = "l";
  }
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
  return outputStream.str();
}

std::string algorithms::generateI2STransceiverSignalNames(int id, int bitWidth) {
  std::stringstream outputStream;
  std::string vectorSize = "( " + std::to_string(bitWidth - 1) + " downto 0)";
  std::string componentName = "i2s_transceiver_" + std::to_string(id);
  outputStream << "signal " << componentName << "_l_data_rx : std_logic_vector " << vectorSize << ";\n"
               << "signal " << componentName << "_r_data_rx : std_logic_vector " << vectorSize << ";\n"
               << "signal " << componentName << "_sclk : std_logic;\n"
               << "signal " << componentName << "_sd_tx : std_logic;\n"
               << "signal " << componentName << "_ws : std_logic;\n"
               << "signal " << componentName << "_sd_rx : std_logic;\n";
  return outputStream.str();
}

std::string algorithms::generateInputInterfaceSignalNames(int id, int bitWidth) {
  std::stringstream outputStream;
  std::string vectorSize = "( " + std::to_string(bitWidth - 1) + " downto 0)";
  std::string componentName = "input_interface_" + std::to_string(id);
  outputStream << "signal " << componentName << "_l_data_out : std_logic_vector " << vectorSize << ";\n"
               << "signal " << componentName << "_l_valid : std_logic;\n"
               << "signal " << componentName << "_r_data_out : std_logic_vector " << vectorSize << ";\n"
               << "signal " << componentName << "_r_valid : std_logic;\n";
  return outputStream.str();
}

std::string algorithms::generateOutputInterfaceSignalNames(int id, int bitWidth) {
  std::stringstream outputStream;
  std::string vectorSize = "( " + std::to_string(bitWidth - 1) + " downto 0)";
  std::string componentName = "output_interface_" + std::to_string(id);
  outputStream << "signal " << componentName << "_l_data_out : std_logic_vector " << vectorSize << ";\n"
               << "signal " << componentName << "_l_ready : std_logic;\n"
               << "signal " << componentName << "_r_data_out : std_logic_vector " << vectorSize << ";\n"
               << "signal " << componentName << "_r_ready : std_logic;\n";
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
  std::string componentName;
  if (comp.getType() == "const_value") { // TODO verify if this check is still necessary since const_val is already checked prior to this function call
    componentName = comp.getType();
  } else {
    componentName = comp.getType();
  }
  int numInputPorts;
  int numOutputPorts;
  // TODO assert that component is not an INPUT/OUTPUT (not sure if still necessary given earlier checks)
  numInputPorts = comp.getInputPorts().size();
  numOutputPorts = comp.getOutputPorts().size();
  // every component requires clock and reset ports
  outputStream << "component " << componentName << " is" << std::endl;
  if (comp.getType() == "const_value") {
    outputStream << "generic (\n"
                 << "    " << "value : std_logic_vector(ram_width - 1 downto 0)\n"
                 << ");" << std::endl;
  }
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
                 << "ram_width - 1 downto 0);\n"
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
                   << "ram_width - 1 downto 0)\n" << std::endl; // last line of port declaration has no terminating semicolon
    } else {
      outputStream << portName + "_data_" + std::to_string(i) + " : out std_logic_vector("
                   << "ram_width - 1 downto 0);\n" << std::endl;
    }
  }

  outputStream << "); end component;\n" << std::endl;
  return outputStream.str();
}

std::string algorithms::generateInputOutputSelectorComponent(VHDLComponent comp,
                                                             std::map<int, int> inputCounts,
                                                             std::map<int, int> outputCounts) {
  std::stringstream outputStream;
  std::string componentName = comp.getType();
  int numInputPorts;
  int numOutputPorts;
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
void algorithms::generateHSInterfaceComponents(std::string compDir,
                                                std::string referenceDir,
                                                bool isBufferless) {
  std::ofstream vhdlOutput;
  // names of reference files required to copy into project; add/remove as required
  // TODO only produce the HS component files if necessary; right now, we're just writing every file
  std::vector<std::string> componentNames = {"hs_merger", "countdown",
                                             "store_send", "hs_merger_negate",
                                             "hs_merger_one", "hs_merger_three"};
  if (!isBufferless) {
    componentNames.push_back("hs_fifo");
    componentNames.push_back("hs_fifo_n");
    componentNames.push_back("hs_fifo_one");
    componentNames.push_back("hs_fifo_zero");
  }

  for (const auto &component : componentNames) {
    vhdlOutput.open(compDir + component + ".vhd");
    std::ifstream compReference(referenceDir + component + ".vhd");
    std::string fileContent;
    if (compReference.is_open()) {
      while (std::getline(compReference, fileContent)) {
        vhdlOutput << fileContent << std::endl;
      }
      compReference.close();
      vhdlOutput.close();
    } else {

        VERBOSE_ERROR( "Reference file for " << component
                << " does not exist/not found!"); // TODO turn into assert

    }
  }
}

// Copy VHDL components necessary for interfacing with the audio codec from reference files to generated subdirectory
void algorithms::generateAudioInterfaceComponents(std::string compDir,
                                                  std::string referenceDir,
                                                  int operatorFreq) {
  std::ofstream vhdlOutput;
  // names of reference files required to copy into project; add/remove as required
  std::vector<std::string> componentNames =
    {"input_interface", "output_interface", // send audio data in accordance to handshake protocol
     "i2s_to_fpc", "fpc_to_i2s",
     "fix2fp_and_scaledown", "fp2fix_and_scaleup", // convert and scale data coming to and from the audio codec (fixed point to float)
     "i2s_transceiver"}; // expose data from ADC/DAC to PL
  std::vector<std::string> operatorNames = // need separate path for FloPoCo operators as they're stored in different subdirectory
    {"fix2fp_flopoco", "fp2fix_flopoco", "fp_prod_flopoco"};
  std::string wordsToReplace[] = {"$OP_FREQ"};
  std::map<std::string, std::string> replacementWords = {{"$OP_FREQ",
                                                            std::to_string(operatorFreq)}}; // name component according to operator frequency
    for (const auto &component : componentNames) {
      vhdlOutput.open(compDir + component + ".vhd");
      std::ifstream compReference(referenceDir + component + ".vhd");
      std::string fileContent;
      if (compReference.is_open()) {
        while (std::getline(compReference, fileContent)) {
          for (const std::string &word : wordsToReplace) { // TODO account for multiple occurances in single line
            size_t pos = fileContent.find(word);
            if (pos != std::string::npos) {
              fileContent.replace(pos, word.length(),
                                  replacementWords[word]);
            }
          }
          vhdlOutput << fileContent << std::endl;
        }
        compReference.close();
        vhdlOutput.close();
      } else {
        VERBOSE_ERROR( "Reference file for " << component
                       << " does not exist/not found!"); // TODO turn into assert
      }
    }
  for (const auto &op : operatorNames) {
    vhdlOutput.open(compDir + op + "_f" + std::to_string(operatorFreq) + ".vhd");
    std::ifstream opReference(referenceDir + "/operators/" + op +
                              "_f" + std::to_string(operatorFreq) + ".vhdl");
    std::string fileContent;
    if (opReference.is_open()) {
      while (std::getline(opReference, fileContent)) {
        vhdlOutput << fileContent << std::endl;
      }
      opReference.close();
      vhdlOutput.close();
    } else {
      VERBOSE_ERROR( "Reference file for " << op
                     << " does not exist/not found!"); // TODO turn into assert
    }
  }
}

std::vector<std::string> algorithms::generateSendSigNames(const std::string &srcPort,
                                                          const VHDLCircuit &circuit) {
  std::vector<std::string> sendSignals(3);

  if (circuit.getOutputPorts().count(srcPort)) {
    sendSignals = circuit.getOutputPorts().at(srcPort);
  } else {
    sendSignals[VALID] = srcPort + "_VALID";
    sendSignals[READY] = srcPort + "_READY";
    sendSignals[DATA] = srcPort + "_DATA";
  }
  return sendSignals;
}

std::vector<std::string> algorithms::generateReceiveSigNames(const std::string &dstPort,
                                                             const VHDLCircuit &circuit) {
  std::vector<std::string> receiveSignals(3);

  if (circuit.getInputPorts().count(dstPort)) {
    receiveSignals = circuit.getInputPorts().at(dstPort);
  } else {
    receiveSignals[VALID] = dstPort + "_VALID";
    receiveSignals[READY] = dstPort + "_READY";
    receiveSignals[DATA] = dstPort + "_DATA";
  }

  return receiveSignals;
}

// Generate port mapping for each operator in the circuit
std::string algorithms::generatePortMapping(const VHDLCircuit& circuit,
                                            bool isBufferless, bool noOperators) {
  std::stringstream outputStream;
  std::string componentName;
  std::map<std::string, int> opCount(circuit.getOperatorMap());

  outputStream << "begin\n" << std::endl;
  // initialise counts
  for (auto &c : opCount) {
    c.second = 0;
  }
  for (auto &op : circuit.getComponentMap()) {
    if (op.second.getType() != "INPUT" && op.second.getType() != "OUTPUT") {
      std::string opName = op.second.getType();
      if (op.second.isConst()) {
        opName = op.second.getType() + "_" + std::to_string((op.second).getOutputPorts().size());
        componentName = "const_value_" + std::to_string((op.second).getOutputPorts().size());
      } else if (op.second.getType() == "Proj") {
        opName = "hs_splitter_" + std::to_string((op.second).getOutputPorts().size());
        componentName = opName;
      } else if (op.second.getType() == "input_selector") {
        opName = "input_selector_" + std::to_string((op.second).getInputPorts().size());
        componentName = opName;
      } else if (op.second.getType() == "output_selector") {
        opName = "output_selector_" + std::to_string((op.second).getOutputPorts().size());
        componentName = opName;
      } else {
        componentName = opName;
      }
      // reset/clock mappings
      outputStream << opName << "_" + std::to_string(opCount[opName])
                   << " : " << componentName << "\n" << std::endl;
      if (op.second.isConst()) { // NOTE binary representation of const_val operators set here
        outputStream << "generic map (\n"
                     << "    " << "value => " << "\""
                     << binaryValue(op.second) << "\"" << "\n)\n"
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
      if (op.second.getType() == "input_selector") {
        outputStream << "generic map (\n"
                     << "    " << "num_phases => "
                     << std::to_string((op.second).getInputPorts().size())
                     << "\n)\n" << std::endl;
      } else if (op.second.getType() == "output_selector") {
        outputStream << "generic map (\n"
                     << "    " << "num_phases => "
                     << std::to_string((op.second).getOutputPorts().size())
                     << "\n)\n" << std::endl;
      }
      outputStream << "port map (\n"
                   << "    " << "clk => " << "clk,\n"
                   << "    " << "rst => " << "rst,\n"
                   << std::endl;
      // input/output mappings
      int inPortCount = 0;
      int outPortCount = 0;
      std::vector<std::string> inputSignals;
      std::vector<std::string> outputSignals;
      if (isBufferless) {
        for (auto &i : op.second.getInputEdges()) {
          for (auto &conn : circuit.getConnectionMap()) {
            if (conn.second.getName() == i) {
              if (conn.second.getInitialTokenCount()) {
                inputSignals.push_back(conn.second.getDstPort());
              } else {
                inputSignals.push_back(i);
              }
            }
          }
        }
        for (auto &i : op.second.getOutputEdges()) {
          for (auto &conn : circuit.getConnectionMap()) {
            if (conn.second.getName() == i) {
              if (conn.second.getInitialTokenCount()) {
                outputSignals.push_back(conn.second.getSrcPort());
              } else {
                outputSignals.push_back(i);
              }
            }
          }
        }
      } else {
        inputSignals = op.second.getInputPorts();
        outputSignals = op.second.getOutputPorts();
      }
      if (op.second.getArgOrder().size()) { // match input signals to argument ordering given by name
        assert(op.second.getArgOrder().size() == inputSignals.size()); // number of input signals must match number of arguments
        inputSignals.clear();
        for (auto i = 0; i < op.second.getArgOrder().size(); i++) {
          std::string srcActorName = circuit.getComponentFullName(op.second.getArgOrder()[i]);
          std::string dstActorName = op.second.getUniqueName();
          if (isBufferless) {
            std::vector<std::string> connNames = circuit.getConnectionNameFromComponents(srcActorName,
                                                                                         dstActorName);
            for (auto &name : connNames) {
              for (auto &conn : circuit.getConnectionMap()) {
                if (conn.second.getName() == name) {
                  if (conn.second.getInitialTokenCount()) {
                    if (std::find(inputSignals.begin(), inputSignals.end(), conn.second.getDstPort()) == inputSignals.end()) {
                      inputSignals.push_back(conn.second.getDstPort());
                    }
                  } else {
                    if (std::find(inputSignals.begin(), inputSignals.end(), name) == inputSignals.end()) {
                      inputSignals.push_back(name);
                    }
                  }
                }
              }
            }
          } else {
            std::vector<std::string> connNames = circuit.getDstPortBetweenComponents(srcActorName,
                                                                                     dstActorName);
            for (auto &name : connNames) {
              if (std::find(inputSignals.begin(), inputSignals.end(), name) == inputSignals.end()) {
                inputSignals.push_back(name);
              }
            }
          }
        }
        assert(op.second.getArgOrder().size() == inputSignals.size()); // make sure we retrieved the right number of inputSignal names
      }
      if (op.second.getType() == "output_selector") { // NOTE workaround to retain ordering of ports for output selector
        // we don't clear the outputSignal vector as we're just shuffling around the names
        for (auto &i : op.second.getOutputEdges()) {
          for (auto &conn : circuit.getConnectionMap()) { // find VHDL connection corresponding to edge
            if (conn.second.getName() == i) {
              for (int exec = 0; exec < conn.second.getInputVector().size(); exec++) {
                if (conn.second.getInputVector().at(exec) == 1) { // the position of the exec rate of 1 determines its order in the output signal vector
                  if (conn.second.getInitialTokenCount() || !isBufferless) {
                    outputSignals.at(exec) = conn.second.getSrcPort();
                  } else {
                    outputSignals.at(exec) = i;
                  }
                }
              }
            }
          }
        }
      }
      for (auto &inPort : inputSignals) {
        std::vector<std::string> receiveSigs(3);
        std::string sigPrefix = "op_";
        if (op.second.isConst() || op.second.getType() == "Proj") {
          sigPrefix = "";
        }
        receiveSigs = generateReceiveSigNames(inPort, circuit);
        outputStream << "    " << sigPrefix << "in_ready_" << std::to_string(inPortCount)
                     << " => " << receiveSigs[READY] << ",\n"
                     << "    " << sigPrefix << "in_valid_" << std::to_string(inPortCount)
                     << " => " << receiveSigs[VALID] << ",\n"
                     << "    " << sigPrefix << "in_data_" << std::to_string(inPortCount)
                     << " => " << receiveSigs[DATA] << ",\n" << std::endl;
        inPortCount++;
      }
      std::string lineEnder = ",";
      size_t numOutPorts = outputSignals.size();
      size_t iteration = 1;
      for (auto &outPort : outputSignals) {
        std::vector<std::string> sendSigs(3);
        std::string sigPrefix = "op_";
        if (op.second.isConst() || op.second.getType() == "Proj") {
          sigPrefix = "";
        }
        sendSigs = generateSendSigNames(outPort, circuit);
        if (iteration == numOutPorts) {
          lineEnder = "";  // last port mapping terminates without a comma
        }
        outputStream << "    " << sigPrefix << "out_ready_" << std::to_string(outPortCount)
                     << " => " << sendSigs[READY] << ",\n"
                     << "    " << sigPrefix << "out_valid_" << std::to_string(outPortCount)
                     << " => " << sendSigs[VALID] << ",\n"
                     << "    " << sigPrefix << "out_data_" << std::to_string(outPortCount)
                     << " => " << sendSigs[DATA] << lineEnder << "\n"
                     << std::endl;
        outPortCount++;
        iteration++;
      }

      outputStream << ");\n" << std::endl;
      opCount[opName]++;
    }
  }

  // buffer port mappings
  if (!isBufferless) {
    int bufferCount = 0;
    std::string bCompName = "hs_fifo";
    std::string bName = "buffer";
    for (auto &buffer : circuit.getConnectionMap()) {
      // only generate buffer between non-input/output components
      if (!(circuit.getInputPorts().count(buffer.second.getDstPort()) ||
            circuit.getOutputPorts().count(buffer.second.getSrcPort()))) {
        std::vector<std::string> sendSigs;
        std::vector<std::string> receiveSigs;
        if (!noOperators) {
          sendSigs = generateSendSigNames(buffer.second.getSrcPort(), circuit);
          receiveSigs = generateReceiveSigNames(buffer.second.getDstPort(), circuit);
        } else { // NOTE workaround to deal with case where only inputs/outputs in Faust program
          sendSigs = generateReceiveSigNames(buffer.second.getSrcPort(), circuit);
          receiveSigs = generateSendSigNames(buffer.second.getDstPort(), circuit);
        }
        // ram width/depth, reset, and clock mappings
        TOKEN_UNIT bufferPadding = 1;
        // if (buffer.second.getBufferSize() == 0) { bufferPadding = 1; } // buffers with size 0 would cause deadlocks
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
      }
    }
  } else {
    int bufferCount = 0;
    std::string bCompName = "hs_fifo";
    std::string bName = "buffer";
    for (auto &buffer : circuit.getConnectionMap()) {
      // only generate buffer between non-input/output components
      if (!(circuit.getInputPorts().count(buffer.second.getDstPort()) ||
            circuit.getOutputPorts().count(buffer.second.getSrcPort()))) {
        if (buffer.second.getInitialTokenCount()) { // only generate FIFO port mapping for necessary channels
          std::vector<std::string> sendSigs;
          std::vector<std::string> receiveSigs;
          if (!noOperators) {
            sendSigs = generateSendSigNames(buffer.second.getSrcPort(), circuit);
            receiveSigs = generateReceiveSigNames(buffer.second.getDstPort(), circuit);
          } else { // NOTE workaround to deal with case where only inputs/outputs in Faust program
            sendSigs = generateReceiveSigNames(buffer.second.getSrcPort(), circuit);
            receiveSigs = generateSendSigNames(buffer.second.getDstPort(), circuit);
          }
          // ram width/depth, reset, and clock mappings
          TOKEN_UNIT bufferPadding = 1;
          // if (buffer.second.getBufferSize() == 0) { bufferPadding = 1; } // buffers with size 0 would cause deadlocks
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
        }
      }
    }
    if (noOperators) { // just need to pass through data if no operators
      std::vector<std::string> sendSigs;
      std::vector<std::string> receiveSigs;
      for (auto &buffer : circuit.getConnectionMap()) {
        sendSigs = generateReceiveSigNames(buffer.second.getSrcPort(), circuit);
        receiveSigs = generateSendSigNames(buffer.second.getDstPort(), circuit);
        outputStream << receiveSigs[VALID] << "<=" << sendSigs[VALID] << ";\n";
        outputStream << receiveSigs[DATA] << "<=" << sendSigs[DATA] << ";\n";
        outputStream << sendSigs[READY] << "<=" << receiveSigs[READY] << ";\n";
      }
    }
  }

  return outputStream.str();
}

// generate a bash script to distribute the outputs of certain actors using additional Proj operators
void algorithms::generateMergingScript(std::vector<std::string> actorNames, std::string graphName,
                                          std::string dirName, std::string referenceDir) {
  std::ofstream bashOutput;
  std::string scriptName = "merge_outs";
  std::stringstream nameList;
  std::string wordsToReplace[] = {"#OUTPUT_DIR", "#GRAPH_NAME",
                                  "#MERGED_NAME", "#ACTOR_NAMES"};
  std::string delimiter = ",";
  size_t elemNumber = 0;
  for (auto& name : actorNames) {
    elemNumber++;
    if (elemNumber >= actorNames.size()) {
      delimiter = "";
    }
    nameList << name << delimiter;
  }
  std::map<std::string, std::string> replacementWords = {{"#OUTPUT_DIR", "\"" + dirName + "\""},
                                                         {"#GRAPH_NAME", "\"" + graphName + "_simplified.xml" + "\""},
                                                         {"#MERGED_NAME", "\"" + graphName + "_merged.xml" + "\""},
                                                         {"#ACTOR_NAMES", nameList.str()}};
  std::ifstream scriptRef(referenceDir + scriptName + ".sh");
  std::string scriptContent;
  bashOutput.open(dirName + scriptName + "_" + graphName + ".sh");
  if (scriptRef.is_open()) {
    while (std::getline(scriptRef, scriptContent)) {
      for (const std::string &word : wordsToReplace) {
        size_t pos = scriptContent.find(word);
        if (pos != std::string::npos) {
          scriptContent.replace(pos, word.length(),
                                replacementWords[word]);
        }
      }
      bashOutput << scriptContent << std::endl;
    }
    bashOutput.close();
  } else {
      VERBOSE_ERROR( "Reference file for " << scriptName
                << " does not exist/not found!" ); // TODO turn into assert
  }
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
