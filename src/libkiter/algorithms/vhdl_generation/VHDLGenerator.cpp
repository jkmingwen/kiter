/*
 * VHDLGenerator.h
 *
 *  Created on: 14 June 2021
 *      Author: jkmingwen
 */

#include <set>
#include <string>
#include <filesystem>
#include "VHDLGenerator.h"
#include "VHDLComponent.h"
#include "VHDLConnection.h"
#include "VHDLCircuit.h"
#include <algorithms/transformation/singleOutput.h>

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



    // populate circuit object with components and connections based on dataflow
    {ForEachVertex(dataflow, actor) {
            VHDLComponent newComp(dataflow, actor);
            circuit.addComponent(newComp);
        }}
    {ForEachEdge(dataflow, edge) {
            VHDLConnection newConn(dataflow, edge);
            circuit.addConnection(newConn);
        }}
    for (auto &comp : circuit.getComponentMap()) { // convert any int constant values that are supplying fp components into fp representation
        if (comp.second.isConst() && comp.second.getDataType() == "int") {
            std::vector<VHDLComponent> dstComps = circuit.getDstComponents(comp.second);
            for (auto &i : dstComps) {
                if (i.hasMixedType() && i.getDataType() == "fp") { // operators with mixed inputs are set to FP by default
                    VERBOSE_WARNING("The destination component, " << i.getName()
                                                                  << " has mixed input types; casting the binary representation of the value provided by "
                                                                  << comp.second.getName() << " as a float");
                    circuit.convConstIntToFloat(comp.first);
                }
            }
        }
    }

    return circuit;

}
void algorithms::generateVHDL(models::Dataflow* const dataflow, parameters_list_t param_list) {


  bool outputDirSpecified = false;
  bool bufferless = false;

  std::string topDir      = "./" + dataflow->getGraphName() + "_vhdl_gen/"; // default output directory
  std::string componentDir = topDir + "/components/";
  std::string referenceDir = "./src/libkiter/algorithms/vhdl_generation/reference_files/";
  int operatorFreq = 125; // default target operator frequency is 125MHz

  // check for specified directory
  if (param_list.find("OUTPUT_DIR") != param_list.end()) {

    outputDirSpecified = true;
    topDir = param_list["OUTPUT_DIR"] + "/";
    componentDir = topDir + "/components/";
    VERBOSE_INFO("Update output directory to " << topDir);

    // This creates both topDir and componentDir directories
    std::filesystem::create_directories(componentDir);
  } else {
      VERBOSE_WARNING("Please use '-p OUTPUT_DIR=topDir' to set output directory.");
  }

  // check if FIFO buffers should be generated
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

    VHDLCircuit tmp = generateCircuitObject(dataflow, bufferless, operatorFreq);
    while (tmp.getMultiOutActors().size() > 0) {

        VERBOSE_INFO("getMultiOutActors is not empty");

        /*  This block remove multiIO actors and replace them  */
        for (std::string actorName: tmp.getMultiOutActors()) {
            parameters_list_t parameters;
            parameters["name"] = actorName;
            VERBOSE_INFO("singleOutput actor " << actorName);
            try {
                algorithms::transformation::singleOutput(dataflow, parameters);
            } catch (...) {
                VERBOSE_WARNING("actor missing!");
            }
        }

        VERBOSE_INFO("Regenerate Circuit");
        tmp = generateCircuitObject(dataflow, bufferless, operatorFreq);
    }

    VERBOSE_ASSERT (tmp.getMultiOutActors().size() == 0, "Error while add Dups") ;
    VERBOSE_INFO("Output Circuit");


    if (outputDirSpecified) { // only produce actual VHDL files if output directory specified
        generateOperators(tmp, componentDir, referenceDir, bufferless, operatorFreq);
        generateCircuit(tmp, topDir, bufferless);
        generateAudioInterfaceWrapper(tmp, referenceDir, topDir);
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
    // track number of outputs for cases where operator can have different number of outputs
    std::map<int, int> outputCounts;
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
  generateAXIInterfaceComponents(compDir, compRefDir, isBufferless);
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

// Generate type conversion operators
void algorithms::generateConversionOperators(VHDLComponent comp, std::string compDir,
                                             std::string referenceDir) {
  std::ofstream vhdlOutput;
  std::string entityName = comp.getType();
  std::string axiInterfaceName = "axi_" + entityName;
  vhdlOutput.open(compDir + entityName + ".vhd");
  std::ifstream operatorRef(referenceDir + entityName + ".vhd");
  std::string fileContent;
  // generate conversion operator
  if (operatorRef.is_open()) {
    while (std::getline(operatorRef, fileContent)) {
      vhdlOutput << fileContent << std::endl;
    }
    operatorRef.close();
    vhdlOutput.close();
  } else {
      VERBOSE_ERROR ("Reference file for " << comp.getType()  << " does not exist/not found!"); // TODO turn into assert
  }
  // generate AXI interface for conversion operator
  vhdlOutput.open(compDir + axiInterfaceName + ".vhd");
  std::ifstream interfaceRef(referenceDir + axiInterfaceName + ".vhd");
  if (interfaceRef.is_open()) {
    while (std::getline(interfaceRef, fileContent)) {
      vhdlOutput << fileContent << std::endl;
    }
    interfaceRef.close();
    vhdlOutput.close();
  } else {
      VERBOSE_ERROR ("Reference file for " << comp.getType()  << " does not exist/not found!"); // TODO turn into assert
  }
}

// Generate AXI splitter components for the Proj components in the circuit (defined by number of outputs)
void algorithms::generateSplitterOperators(std::string compDir, std::string referenceDir,
                                           std::map<int, int> outputCounts) {
  std::ofstream vhdlOutput;
  std::string operatorFileName = "axi_splitter";
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
  std::string componentName = entityName + "_component"; // just need a distinct name for the component instantiation in the AXI interface declaration in VHDL
  vhdlOutput.open(compDir + entityName + "_op.vhd"); // write operator implementation
  std::ifstream operatorRef(referenceDir + entityName + "_op.vhd");
  std::string fileContent;
  std::string wordsToReplace[] = {"$ENTITY_NAME", "$FLOPOCO_OP_NAME",
                                  "$COMPONENT_NAME", "$OP_LIFESPAN",
                                  "$AXM_TYPE"};
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
  std::string axmType = "";
  if (comp.getType() == "select2") {
    axmType = "_three";
  }
  std::map<std::string, std::string> replacementWords = {{"$ENTITY_NAME", entityName},
                                                         {"$FLOPOCO_OP_NAME", entityName + "_op"},
                                                         {"$COMPONENT_NAME", componentName},
                                                         {"$OP_LIFESPAN", std::to_string(1)}, // AXI interface requires lifespan to be >0
                                                         {"$AXM_TYPE", axmType}};
  // generate AXI interface for select operator
  vhdlOutput.open(compDir + entityName + ".vhd");
  std::ifstream interfaceRef(referenceDir + "flopoco_axi_interface"
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
     VERBOSE_ERROR ("AXI interface file for " << "flopoco_axi_interface_" + opInputCount
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
                                    "$AXM_TYPE"};
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
    std::ifstream operatorRef(referenceDir + "flopoco_axi_interface_1.vhd");
    std::string fileContent;
    std::string operatorName = implementationName + "_f" + std::to_string(operatorFreq);
    std::map<std::string, std::string> replacementWords = {{"$ENTITY_NAME", entityName},
                                                           {"$FLOPOCO_OP_NAME", operatorName},
                                                           {"$COMPONENT_NAME", componentName},
                                                           {"$OP_LIFESPAN", lifespan},
                                                           {"$AXM_TYPE", "_one"}};
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
      VERBOSE_ERROR ("Reference file for " << "flopoco_axi_interface_1.vhd does not exist/not found!"); // TODO turn into assert
    }
  }
}

void algorithms::generateDelayOperator(VHDLComponent comp, std::string compDir,
                                       std::string referenceDir, int operatorFreq) {
  generateFPCOperator(comp.getImplementationName(), compDir, referenceDir, operatorFreq);
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

// Generate AXI interface for each FloPoCo operator
void algorithms::generateOperator(VHDLComponent comp, std::string compDir,
                                  std::string referenceDir, int operatorFreq) {
  std::ofstream vhdlOutput;
  std::string entityName = comp.getType();
  std::string componentName = entityName + "_implementation";
  std::string wordsToReplace[] = {"$ENTITY_NAME", "$FLOPOCO_OP_NAME",
                                  "$COMPONENT_NAME", "$OP_LIFESPAN",
                                  "$AXM_TYPE"};
  std::string opInputCount = std::to_string(comp.getInputPorts().size());

  if (comp.getType() != "INPUT" && comp.getType() != "OUTPUT" && !comp.isConst()) {
    // generate flopoco operators
    generateFPCOperator(comp.getImplementationName(), compDir, referenceDir, operatorFreq); // generate FloPoCo operator
    vhdlOutput.open(compDir + entityName + ".vhd"); // instantiate VHDL file
    std::ifstream operatorRef(referenceDir + "flopoco_axi_interface" +
                              + "_" + opInputCount + ".vhd");
    std::string fileContent;
    std::string operatorName = comp.getImplementationName() + "_f" + std::to_string(operatorFreq);
    std::string axmType = "";
    if (comp.getInputPorts().size() == 1) {
      axmType = "_one";
    }
    std::map<std::string, std::string> replacementWords = {{"$ENTITY_NAME", entityName},
                                                           {"$FLOPOCO_OP_NAME", operatorName},
                                                           {"$COMPONENT_NAME", componentName},
                                                           {"$OP_LIFESPAN", std::to_string(comp.getLifespan())},
                                                           {"$AXM_TYPE", axmType}};
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


        VERBOSE_ERROR ("Reference file for " << "flopoco_axi_interface_" + opInputCount
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

  vhdlOutput.open(outputDir + graphName + ".vhd"); // instantiate VHDL file
  // 1. Define libraries used
  vhdlOutput << "library ieee;\n"
             << "use ieee.std_logic_1164.all;\n"
             << "use ieee.numeric_std.all;\n" << std::endl;
  // 2. Port declarations
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

  // 3. Specify architecture (behaviour) of operator type
  // component declaration
  vhdlOutput << "architecture behaviour of " << circuit.getName() << " is\n" << std::endl;
  if (!noOperators) { // no need to generate components other than buffers if there aren't any operators
    std::map<int, int> constOutputs;
    for (auto const &op : operatorMap) {
      if (op.first != "INPUT" && op.first != "OUTPUT") {
        std::map<int, int> outputCounts;
        outputCounts = circuit.getNumOutputs(op.first);
        if (op.first == "Proj") {
          vhdlOutput << generateSplitterComponents(outputCounts) << std::endl;
        } else if (op.first == "delay") {
          vhdlOutput << generateDelayComponent(circuit.getFirstComponentByType(op.first))
                     << std::endl;
        } else if (op.first == "const_value" ||
                   circuit.getFirstComponentByType(op.first).isConst()) {
          constOutputs.insert(outputCounts.begin(), outputCounts.end()); // workaround for UI components
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
  // Track top-level input and output signals
  std::vector<std::string> inSignalNames;
  std::vector<std::string> outSignalNames;
  if (isBufferless) { // no FIFO buffers between components
    // NOTE Using a really roundabout method here to look for buffers with initial token counts; it's probably possible to reduce the number of nested loops
    for (auto &op : circuit.getComponentMap()) {
      if (op.second.getType() == "INPUT") {
        for (auto &i : op.second.getOutputEdges()) {
          for (auto &conn : circuit.getConnectionMap()) {
            if (conn.second.getName() == i) { // check if given edge has initial tokens
              if (conn.second.getInitialTokenCount()) {
                inSignalNames.push_back(conn.second.getSrcPort()); // use port names for signals if there are initial tokens as FIFO buffer will be used
              } else {
                inSignalNames.push_back(i); // if not, edge names will be used as signal names
              }
            }
          }
        }
      } else if (op.second.getType() == "OUTPUT") {
        for (auto &i : op.second.getInputEdges()) {
          for (auto &conn : circuit.getConnectionMap()) {
            if (conn.second.getName() == i) { // check if given edge has initial tokens
              if (conn.second.getInitialTokenCount()) {
                outSignalNames.push_back(conn.second.getDstPort()); // use port names for signals if there are initial tokens as FIFO buffer will be used
              } else {
                outSignalNames.push_back(i); // if not, edge names will be used as signal names
              }
            }
          }
        }
      }
    }
  } else { // use port names to generate signals to/from FIFO buffers
    for (auto &op : circuit.getComponentMap()) {
      if (op.second.getType() == "INPUT") {
        for (auto &i : op.second.getOutputPorts()) {
          inSignalNames.push_back(i);
        }
      } else if (op.second.getType() == "OUTPUT") {
        for (auto &i : op.second.getInputPorts()) {
          outSignalNames.push_back(i);
        }
      }
    }
  }

  // Define and generate signal names
  int inCount = 0;
  int outCount = 0;
  bool isTopInput;
  bool isTopOutput;
  bool hasInputDelay;
  bool hasOutputDelay;
  for (auto &connection : circuit.getConnectionMap()) {
    if (isBufferless) {
      if (connection.second.getInitialTokenCount()) { // account for different signal names when channel has initial token count
        isTopInput = std::find(inSignalNames.begin(), inSignalNames.end(),
                               connection.second.getSrcPort()) != inSignalNames.end();
        isTopOutput = std::find(outSignalNames.begin(), outSignalNames.end(),
                                connection.second.getDstPort()) != outSignalNames.end();
      } else {
        isTopInput = std::find(inSignalNames.begin(), inSignalNames.end(),
                               connection.second.getName()) != inSignalNames.end();
        isTopOutput = std::find(outSignalNames.begin(), outSignalNames.end(),
                                connection.second.getName()) != outSignalNames.end();
      }
    } else {
      isTopInput = std::find(inSignalNames.begin(), inSignalNames.end(),
                             connection.second.getSrcPort()) != inSignalNames.end();
      isTopOutput = std::find(outSignalNames.begin(), outSignalNames.end(),
                              connection.second.getDstPort()) != outSignalNames.end();
    }

    if (isTopInput) {
      std::vector<std::string> signalNames(3);
      signalNames[VALID] = circuit.getName() + "_in_valid_" + std::to_string(inCount);
      signalNames[READY] = circuit.getName() + "_in_ready_" + std::to_string(inCount);
      signalNames[DATA] = circuit.getName() + "_in_data_" + std::to_string(inCount);
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
      inCount++;
    }
    if (isTopOutput) {
      std::vector<std::string> signalNames(3);
      signalNames[VALID] = circuit.getName() + "_out_valid_" + std::to_string(outCount);
      signalNames[READY] = circuit.getName() + "_out_ready_" + std::to_string(outCount);
      signalNames[DATA] = circuit.getName() + "_out_data_" + std::to_string(outCount);
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
      outCount++;
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
        if (isBufferless) {
          if (connection.second.getInitialTokenCount()) { // if there are initial tokens, separate send/receive signals required due to buffers between components
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
        } else {
          // separate send/receive signals required due to buffers between components
          std::vector<std::string> sendSignals(generateSendSigNames(connection.second.getSrcPort(), circuit));
          std::vector<std::string> receiveSignals(generateReceiveSigNames(connection.second.getDstPort(), circuit));
          dataSignals.push_back(sendSignals[DATA]);
          dataSignals.push_back(receiveSignals[DATA]);
          validReadySignals.push_back(sendSignals[VALID]);
          validReadySignals.push_back(receiveSignals[VALID]);
          validReadySignals.push_back(sendSignals[READY]);
          validReadySignals.push_back(receiveSignals[READY]);
        }
      }
    } else {
      if (isBufferless) { // TODO I suspect this is redundant --- check to see if this is the case
        if (connection.second.getInitialTokenCount()) { // if there are initial tokens, separate send/receive signals required due to buffers between components
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
      } else {
        // separate send/receive signals required due to buffers between components
        std::vector<std::string> sendSignals(generateSendSigNames(connection.second.getSrcPort(), circuit));
        std::vector<std::string> receiveSignals(generateReceiveSigNames(connection.second.getDstPort(), circuit));
        dataSignals.push_back(sendSignals[DATA]);
        dataSignals.push_back(receiveSignals[DATA]);
        validReadySignals.push_back(sendSignals[VALID]);
        validReadySignals.push_back(receiveSignals[VALID]);
        validReadySignals.push_back(sendSignals[READY]);
        validReadySignals.push_back(receiveSignals[READY]);
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

// Generate AXI interface for each FloPoCo operator
void algorithms::generateAudioInterfaceWrapper(VHDLCircuit &circuit, std::string referenceDir,
                                               std::string outputDir) {
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
    for (auto i = 0; i < numInputInterfaces; i++) {
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
    for (auto i = 0; i < numOutputInterfaces; i++) {
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
  if (comp.getType() == "const_value") {
    componentName = comp.getType();
  } else {
    componentName = comp.getType();
  }
  int numInputPorts;
  int numOutputPorts;
  // TODO assert that component is not an INPUT/OUTPUT
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
  outputStream << "component axi_fifo is\n"
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

// generate one AXI splitter component for each corresponding Proj component (defined by number of outputs)
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

// generate one AXI splitter component for each corresponding Proj component (defined by number of outputs)
// TODO find a way to generalise splitter component and generation
std::string algorithms::generateSplitterComponents(std::map<int, int> outputCounts) {
  std::stringstream outputStream;
  std::string componentName;
  for (auto &i : outputCounts) {
    componentName = "axi_splitter_" + std::to_string(i.first);
    int numInputPorts = 1; // NOTE hardcoded here that AXI splitter components all have 1 input
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

// Copy FloPoCo-AXI interface component specification from reference files to generated subdirectory
void algorithms::generateAXIInterfaceComponents(std::string compDir,
                                                std::string referenceDir,
                                                bool isBufferless) {
  std::ofstream vhdlOutput;
  // names of reference files required to copy into project; add/remove as required
  // TODO only produce the AXI component files if necessary; right now, we're just writing every file
  std::vector<std::string> componentNames = {"axi_merger", "countdown",
                                             "store_send", "axi_merger_negate",
                                             "axi_merger_one", "axi_merger_three"};
  if (!isBufferless) {
    componentNames.push_back("axi_fifo");
    componentNames.push_back("axi_fifo_n");
    componentNames.push_back("axi_fifo_one");
    componentNames.push_back("axi_fifo_zero");
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

std::vector<std::string> algorithms::generateSendSigNames(std::string srcPort,
                                                          VHDLCircuit circuit) {
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

std::vector<std::string> algorithms::generateReceiveSigNames(std::string dstPort,
                                                             VHDLCircuit circuit) {
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
std::string algorithms::generatePortMapping(VHDLCircuit circuit,
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
        opName = "axi_splitter_" + std::to_string((op.second).getOutputPorts().size());
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
                     << op.second.getBinaryValue() << "\"" << "\n)\n"
                     << std::endl;
      }
      if (opName == "delay") {
        int ramInitVal = 0;
        int ramDepthVal = 1;
        outputStream << "generic map (\n"
                     << "    " << "ram_width => ram_width,\n"
                     << "    " << "ram_init => " << std::to_string(ramInitVal) << ",\n"
                     << "    " << "ram_depth => " << std::to_string(ramDepthVal)
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
          std::string dstActorName = op.second.getName();
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
                     << " => " << sendSigs[DATA] << lineEnder
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
    std::string bCompName = "axi_fifo";
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
        outputStream << "fifo_" + std::to_string(bufferCount)
                     << " : " << bCompName << "\n"
                     <<"generic map (\n"
                     << "    " << "ram_width => ram_width,\n"
                     << "    " << "ram_depth => " << buffer.second.getBufferSize() << ",\n"
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
    std::string bCompName = "axi_fifo";
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
          outputStream << "fifo_" + std::to_string(bufferCount)
                       << " : " << bCompName << "\n"
                       <<"generic map (\n"
                       << "    " << "ram_width => ram_width,\n"
                       << "    " << "ram_depth => " << buffer.second.getBufferSize() << ",\n"
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
