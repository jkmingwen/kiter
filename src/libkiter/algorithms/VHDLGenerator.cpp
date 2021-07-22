/*
 * VHDLGenerator.h
 *
 *  Created on: 14 June 2021
 *      Author: jkmingwen
 */

#include <set>
#include <string>
#include <boost/filesystem.hpp>
#include "VHDLGenerator.h"
#include "VHDLComponent.h"
#include "VHDLConnection.h"
#include "VHDLCircuit.h"

// for signal name retrieval
#define VALID 0
#define READY 1
#define DATA 2

void algorithms::generateVHDL(models::Dataflow* const dataflow,
                              parameters_list_t param_list) {
  bool outputDirSpecified = false;
  std::string dirName = "./" + dataflow->getGraphName() + "_vhdl_gen/"; // default output directory
  std::string componentDir = dirName + "/components/";
  std::ofstream outputFile;
  VHDLCircuit circuit;
  circuit.setName(dataflow->getGraphName());

  // check for specified directory
  if (param_list.find("OUTPUT_DIR") != param_list.end()) {
    outputDirSpecified = true;
    dirName = param_list["OUTPUT_DIR"];
    componentDir = dirName + "/components/";
  }
  if (!boost::filesystem::is_directory(dirName)) {
    boost::filesystem::create_directory(dirName);
  }
  boost::filesystem::create_directory(componentDir);

  // populate circuit object with components and connections based on dataflow
  {ForEachVertex(dataflow, actor) {
      VHDLComponent newComp(dataflow, actor);
      circuit.addComponent(newComp);
    }}
  {ForEachEdge(dataflow, edge) {
      VHDLConnection newConn(dataflow, edge);
      circuit.addConnection(newConn);
    }}
  generateOperators(circuit, componentDir);
  generateCircuit(circuit, dirName);
  std::cout << circuit.printStatus() << std::endl;
  return;
}

void algorithms::generateOperators(VHDLCircuit &circuit, std::string compDir) {
  std::map<std::string, int> operatorMap = circuit.getOperatorMap();
  std::string compRefDir = "./vhdl_generation/";
  std::string bufferRefFileLoc = compRefDir + "axi_fifo.vhd";
  // Generate VHDL files for individual components
  for (auto const &op : operatorMap) {
    std::cout << "Generate VHDL component file for " << op.first << std::endl;
    generateOperator(circuit.getFirstComponentByType(op.first), compDir,
                     compRefDir);
  }
  generateAXIInterfaceComponents(compDir, compRefDir);

}

// Copy FloPoCo operator from reference file to project
void algorithms::generateFPCOperator(VHDLComponent comp, std:: string compDir,
                                     std::string referenceDir) {
  std::ofstream vhdlOutput;
  std::string operatorRefDir = referenceDir + "/operators/";
  std::string operatorFileName = "fp_" + comp.getType() + "_flopoco"; // TODO decide on naming convention (might be better to just name reference files the same as component types)
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
    std::cout << "Reference file for " << comp.getType()
              << " does not exist/not found!" << std::endl; // TODO turn into assert
  }
}

// Generate AXI interface for each FloPoCo operator
void algorithms::generateOperator(VHDLComponent comp, std::string compDir,
                                  std::string referenceDir) {
  std::ofstream vhdlOutput;
  std::string componentName = "fp_" + comp.getType();
  std::string wordsToReplace[] = {"$ENTITY_NAME", "$FLOPOCO_OP_NAME"};

  if (comp.getType() == "INPUT" || comp.getType() == "OUTPUT") {
    // TODO generate ports for top level component
  } else {
    generateFPCOperator(comp, compDir, referenceDir); // generate FloPoCo operator
    vhdlOutput.open(compDir + componentName + ".vhd"); // instantiate VHDL file
    std::ifstream operatorRef(referenceDir + "flopoco_axi_interface.vhd");
    std::string fileContent;
    std::string operatorName;
    // TODO clean up using some kind of switch statement and hash table (to handle strings)
    if (comp.getType() == "add") {
      operatorName = "FPAdd_8_23_F400_uid2";
    } else if (comp.getType() == "prod") {
      operatorName = "FPMult_8_23_8_23_8_23_uid2_F400_uid3";
    } else { // TODO replace with assert statement
      operatorName = "UNKNOWN_OPERATOR";
    }
    std::map<std::string, std::string> replacementWords;
    replacementWords["$ENTITY_NAME"] = componentName;
    replacementWords["$FLOPOCO_OP_NAME"] = operatorName;
    // replace keywords with parameters corresponding to operator used
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
      std::cout << "Reference file for flopoco_axi_interface does not exist/not found!"
                << std::endl;
    }
  }
}

void algorithms::generateCircuit(VHDLCircuit &circuit, std::string outputDir) {
  std::ofstream vhdlOutput;
  std::string graphName = circuit.getName() + "_circuit"; // TODO decide on naming convention
  int numInputPorts = circuit.getOperatorCount("INPUT");
  int numOutputPorts = circuit.getOperatorCount("OUTPUT");
  std::map<std::string, int> operatorMap = circuit.getOperatorMap();

  vhdlOutput.open(outputDir + graphName + ".vhd"); // instantiate VHDL file
  // 1. Define libraries used
  vhdlOutput << "library ieee;\n"
             << "use ieee.std_logic_1164.all;\n"
             << "use ieee.numeric_std.all;\n" << std::endl;
  // 2. Port declarations
  vhdlOutput << "entity " << circuit.getName() << " is\n"
             << "generic (\n" // TODO add lifespans of different operators
             << "    " << "ram_width : natural := 16;\n"
             << "    " << "ram_depth : natural := 256";
  for (auto &op : operatorMap) {
    if (op.first == "INPUT" || op.first == "OUTPUT") {
      // do nothing
    } else {
      vhdlOutput << ";\n" << "    " << op.first << "_lifespan : integer := "
                 << circuit.getOperatorLifespan(op.first);
    }
  }
  vhdlOutput << ");\n"
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
  vhdlOutput << "architecture behaviour of " << circuit.getName() << " is\n" << std::endl;
  for (auto const &op : operatorMap) {
    if (op.first != "INPUT" && op.first != "OUTPUT") {
      vhdlOutput << generateComponent(circuit.getFirstComponentByType(op.first))
                 << std::endl;
    }
  }
  vhdlOutput << generateBufferComponent(circuit.getName()) << std::endl;
  // Track top-level input and output signals
  std::vector<std::string> inPortNames;
  std::vector<std::string> outPortNames;
  for (auto &op : circuit.getComponentMap()) {
    if (op.second.getType() == "INPUT") {
      for (auto &i : op.second.getOutputPorts()) {
        inPortNames.push_back(i);
      }
    } else if (op.second.getType() == "OUTPUT") {
      for (auto &i : op.second.getInputPorts()) {
        outPortNames.push_back(i);
      }
    }
  }
  // Define and generate signal names
  int inCount = 0;
  int outCount = 0;
  for (auto &connection : circuit.getConnectionMap()) {
    if (std::find(inPortNames.begin(), inPortNames.end(), connection.second.getSrcPort()) != inPortNames.end()) {
      std::vector<std::string> signalNames(3);
      signalNames[VALID] = circuit.getName() + "_in_valid_" + std::to_string(inCount);
      signalNames[READY] = circuit.getName() + "_in_ready_" + std::to_string(inCount);
      signalNames[DATA] = circuit.getName() + "_in_data_" + std::to_string(inCount);
      circuit.addInputPort(connection.second.getDstPort(), signalNames);
      inCount++;
    } else if (std::find(outPortNames.begin(), outPortNames.end(), connection.second.getDstPort()) != outPortNames.end()) {
      std::vector<std::string> signalNames(3);
      signalNames[VALID] = circuit.getName() + "_out_valid_" + std::to_string(outCount);
      signalNames[READY] = circuit.getName() + "_out_ready_" + std::to_string(outCount);
      signalNames[DATA] = circuit.getName() + "_out_data_" + std::to_string(outCount);
      circuit.addOutputPort(connection.second.getSrcPort(), signalNames);
      outCount++;
    }
  }

  std::vector<std::string> dataSignals;
  std::vector<std::string> validReadySignals;
  for (auto &connection : circuit.getConnectionMap()) {
    // only generate signal names for non-input/output ports
    if (!(circuit.getInputPorts().count(connection.second.getDstPort()) ||
          circuit.getOutputPorts().count(connection.second.getSrcPort()))) {
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
  // data signals
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
  vhdlOutput << generatePortMapping(circuit) << std::endl;
  vhdlOutput << "end behaviour;" << std::endl; // TODO don't hardcode architecture name

  vhdlOutput.close();
}

std::string algorithms::generateComponent(VHDLComponent comp) {
  std::stringstream outputStream;
  std::string componentName = "fp_" + comp.getType(); // TODO decide on naming convention
  int numInputPorts;
  int numOutputPorts;
  if (comp.getType() == "INPUT" || comp.getType() == "OUTPUT") { // assume that input and output actors always have same number of ports
    numInputPorts = 1;
    numOutputPorts = 1;
  } else {
    numInputPorts = comp.getInputPorts().size();
    numOutputPorts = comp.getOutputPorts().size();
  }

  // every component requires clock and reset ports
  outputStream << "component " << componentName << " is\n"
               << "generic (\n"
               << "    " << "ram_width : natural;\n"
               << "    " << "ram_depth : natural;\n"
               << "    " << "operator_lifespan : integer\n"
               << ");\n"
               << "port (\n"
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
  return outputStream.str();
}

std::string algorithms::generateBufferComponent(std::string circuitName) {
  std::stringstream outputStream;

  // every component requires clock and reset ports
  outputStream << "component axi_fifo is\n"
               << "generic (\n"
               << "    " << "ram_width : natural;\n"
               << "    " << "ram_depth : natural\n"
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

// Copy FloPoCo-AXI interface component specification from reference files to generated subdirectory
void algorithms::generateAXIInterfaceComponents(std::string compDir,
                                                std::string referenceDir) {
  std::ofstream vhdlOutput;
  // names of reference files required to copy into project; add/remove as required
  std::string componentNames[] = {"axi_merger", "delay", "store_send", "axi_fifo"};

  for (const std::string &component : componentNames) {
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
      std::cout << "Reference file for " << component
                << " does not exist/not found!" << std::endl; // TODO turn into assert
    }
  }
}

std::vector<std::string> algorithms::generateSendSigNames(std::string srcPort,
                                                          VHDLCircuit circuit) {
  std::vector<std::string> sendSignals(3);

  if (circuit.getOutputPorts().count(srcPort)) {
    sendSignals = circuit.getOutputPorts()[srcPort];
  } else {
    sendSignals[VALID] = "SEND_" + srcPort + "_VALID";
    sendSignals[READY] = "SEND_" + srcPort + "_READY";
    sendSignals[DATA] = "SEND_" + srcPort + "_DATA";
  }
  return sendSignals;
}

std::vector<std::string> algorithms::generateReceiveSigNames(std::string dstPort,
                                                             VHDLCircuit circuit) {
  std::vector<std::string> receiveSignals(3);

  if (circuit.getInputPorts().count(dstPort)) {
    receiveSignals = circuit.getInputPorts()[dstPort];
  } else {
    receiveSignals[VALID] = "RCV_" + dstPort + "_VALID";
    receiveSignals[READY] = "RCV_" + dstPort + "_READY";
    receiveSignals[DATA] = "RCV_" + dstPort + "_DATA";
  }

  return receiveSignals;
}

std::string algorithms::generatePortMapping(VHDLCircuit circuit) {
  std::stringstream outputStream;
  std::string operatorPrefix = "fp_";

  outputStream << "begin\n" << std::endl;
  // operator port mappings
  std::map<std::string, int> opCount(circuit.getOperatorMap());
  // initialise counts
  for (auto &c : opCount) {
    c.second = 0;
  }
  for (auto &op : circuit.getComponentMap()) {
    if (op.second.getType() != "INPUT" && op.second.getType() != "OUTPUT") {
      std::string opName = op.second.getType();
      // reset/clock mappings
      outputStream << opName << "_" + std::to_string(opCount[opName])
                   << " : " << operatorPrefix << opName << "\n"
                   << "generic map (\n"
                   << "    " << "ram_width => ram_width,\n"
                   << "    " << "ram_depth => ram_depth,\n"
                   << "    " << "operator_lifespan => " << opName << "_lifespan\n)\n"
                   << "port map (\n"
                   << "    " << "clk => " << "clk,\n"
                   << "    " << "rst => " << "rst,\n"
                   << std::endl;
      // input/output mappings
      int inPortCount = 0;
      int outPortCount = 0;
      // TODO define input operator mapping outside of input port for loop
      for (auto &inPort : op.second.getInputPorts()) {
        std::vector<std::string> receiveSigs(3);
        receiveSigs = generateReceiveSigNames(inPort, circuit);
        outputStream << "    " << "op_in_ready_" << std::to_string(inPortCount)
                     << " => " << receiveSigs[READY] << ",\n"
                     << "    " << "op_in_valid_" << std::to_string(inPortCount)
                     << " => " << receiveSigs[VALID] << ",\n"
                     << "    " << "op_in_data_" << std::to_string(inPortCount)
                     << " => " << receiveSigs[DATA] << ",\n" << std::endl;
        inPortCount++;
      }

      for (auto &outPort : op.second.getOutputPorts()) {
        std::vector<std::string> sendSigs(3);
        sendSigs = generateSendSigNames(outPort, circuit);
        outputStream << "    " << "op_out_ready_" << std::to_string(outPortCount)
                     << " => " << sendSigs[READY] << ",\n"
                     << "    " << "op_out_valid_" << std::to_string(outPortCount)
                     << " => " << sendSigs[VALID] << ",\n"
                     << "    " << "op_out_data_" << std::to_string(outPortCount)
                     << " => " << sendSigs[DATA] << std::endl; // TODO check for last entry
        outPortCount++;
      }

      outputStream << ");\n" << std::endl;
      opCount[opName]++;
    }
  }

  // buffer port mappings
  int bufferCount = 0;
  std::string bCompName = "axi_fifo";
  std::string bName = "buffer";
  for (auto &buffer : circuit.getConnectionMap()) {
     // only generate buffer between non-input/output components
    if (!(circuit.getInputPorts().count(buffer.second.getDstPort()) ||
          circuit.getOutputPorts().count(buffer.second.getSrcPort()))) {
      std::vector<std::string> sendSigs = generateSendSigNames(buffer.second.getSrcPort(), circuit);
      std::vector<std::string> receiveSigs = generateReceiveSigNames(buffer.second.getDstPort(), circuit);
      // ram width/depth, reset, and clock mappings
      outputStream << "fifo_" + std::to_string(bufferCount)
                   << " : " << bCompName << "\n"
                   <<"generic map (\n"
                   << "    " << "ram_width,\n"
                   << "    " << "ram_depth\n)\n"
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
  return outputStream.str();
}
