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
  if (param_list.find("OUTPUT_DIR") != param_list.end()) { // log output of DSE (includes pareto points and all search points)
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
  std::cout << circuit.printStatus() << std::endl;
  generateOperators(circuit, componentDir);
  generateCircuit(circuit, dirName);

  return;
}

void algorithms::generateOperators(VHDLCircuit circuit, std::string compDir) {
  std::map<std::string, int> operatorMap = circuit.getOperatorMap();
  std::string compRefDir = "./vhdl_generation/";
  std::string bufferRefFileLoc = compRefDir + "axi_fifo.vhd";
  // Generate VHDL files for individual components
  for (auto const &op : operatorMap) {
    std::cout << "Generate VHDL component file for " << op.first << std::endl;
    generateOperator(circuit.getFirstComponentByType(op.first), compDir,
                     compRefDir);
  }
  // generateBuffer(compDir, bufferRefFileLoc);
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
  std::string wordsToReplace[] = {"$ENTITY_NAME", "$FLOPOCO_OP_NAME", "$OP_LIFESPAN"};

  if (comp.getType() == "INPUT" || comp.getType() == "OUTPUT") {
    // TODO generate ports for top level component
  } else {
    generateFPCOperator(comp, compDir, referenceDir); // generate FloPoCo operator
    vhdlOutput.open(compDir + componentName + ".vhd"); // instantiate VHDL file
    std::ifstream operatorRef(referenceDir + "flopoco_axi_interface.vhd");
    std::string fileContent;
    std::string operatorName;
    std::string operatorLifespan;
    // TODO clean up using some kind of switch statement and hash table (to handle strings)
    if (comp.getType() == "add") {
      operatorName = "FPAdd_8_23_F400_uid2";
      operatorLifespan = "14";
    } else if (comp.getType() == "prod") {
      operatorName = "FPMult_8_23_8_23_8_23_uid2_F400_uid3";
      operatorLifespan = "3";
    } else { // TODO replace with assert statement
      operatorName = "UNKNOWN_OPERATOR";
      operatorLifespan = "1";
    }
    std::map<std::string, std::string> replacementWords;
    replacementWords["$ENTITY_NAME"] = componentName;
    replacementWords["$FLOPOCO_OP_NAME"] = operatorName;
    replacementWords["$OP_LIFESPAN"] = operatorLifespan;
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

void algorithms::generateCircuit(VHDLCircuit circuit, std::string outputDir) {
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
  // TODO create map of port names for behaviour specification in step 3
  // every component requires clock and reset ports
  vhdlOutput << "entity " << circuit.getName() << " is\n"
             << "generic (\n"
             << "    " << circuit.getName() << "_ram_width : natural := 16;\n"
             << "    " << circuit.getName() << "_ram_depth : natural := 256\n"
             << ");\n"
             << "port (\n"
             << "    " << circuit.getName() << "_clk : in std_logic;\n"
             << "    " << circuit.getName() << "_rst : in std_logic;\n"
             << std::endl;
  // Specify ready, valid, and data ports for each input port:
  for (auto i = 0; i < numInputPorts; i++) {
    // TODO define portName variable outside of loop to minimize unnecessary repetition
    //      (also, for loop should be outside if loop)
    std::string portName = "    " + circuit.getName() + "_in" + std::to_string(i);
    vhdlOutput << portName + "_ready : out std_logic;\n"
               << portName + "_valid : in std_logic;\n"
               << portName + "_data : in std_logic_vector("
               << circuit.getName() + "_ram_width - 1 downto 0) := (others => '0');\n"
               << std::endl;
  }
  // Specify ready, valid, and data ports for each output port:
  for (auto i = 0; i < numOutputPorts; i++) {
    std::string portName = "    " + circuit.getName() + "_out" + std::to_string(i);
    vhdlOutput << portName + "_ready : in std_logic;\n"
               << portName + "_valid : out std_logic;\n";
    if (i + 1 == numOutputPorts) {
      vhdlOutput << portName + "_data : out std_logic_vector("
                 << circuit.getName() + "_ram_width - 1 downto 0) := (others => '0')\n"
                 << std::endl; // last line of port declaration has no terminating semicolon
    } else {
      vhdlOutput << portName + "_data : out std_logic_vector("
                 << circuit.getName() + "_ram_width - 1 downto 0) := (others => '0');\n"
                 << std::endl;
    }
  }
  vhdlOutput << ");\nend;\n" << std::endl; // TODO check if name of entity necessary here

  // 3. Specify architecture (behaviour) of operator type
  vhdlOutput << "architecture behaviour of " << circuit.getName() << " is\n" << std::endl;
  for (auto const &op : operatorMap) {
    vhdlOutput << generateComponent(circuit.getFirstComponentByType(op.first), circuit.getName()) << std::endl;
  }
  vhdlOutput << generateBufferComponent(circuit.getName()) << std::endl;
  // Define and generate signal names
  std::vector<std::string> dataSignals;
  std::vector<std::string> validReadySignals;
  for (auto &connection : circuit.getConnectionMap()) {
    std::vector<std::string> sendSignals(generateSendSigNames(connection.second.getSrcPort()));
    std::vector<std::string> receiveSignals(generateReceiveSigNames(connection.second.getDstPort()));
    dataSignals.push_back(sendSignals[DATA]);
    dataSignals.push_back(receiveSignals[DATA]);
    validReadySignals.push_back(sendSignals[VALID]);
    validReadySignals.push_back(receiveSignals[VALID]);
    validReadySignals.push_back(sendSignals[READY]);
    validReadySignals.push_back(receiveSignals[READY]);
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
  vhdlOutput << " : std_logic_vector(" + circuit.getName() + "_ram_width - 1 downto 0);\n"
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
  // TODO specify behaviour
  vhdlOutput << generatePortMapping(circuit) << std::endl;
  vhdlOutput << "end behaviour;" << std::endl; // TODO don't hardcode architecture name

  vhdlOutput.close();
}

std::string algorithms::generateComponent(VHDLComponent comp, std::string circuitName) { // NOTE circuitName is here literally just for naming --- would be good to find a way to get rid of it
  std::stringstream outputStream;
  std::string componentName = comp.getType() + "_node"; // TODO decide on naming convention
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
               << "port (\n"
               << "    " << comp.getType() << "_clk : in std_logic;\n"
               << "    " << comp.getType() << "_rst : in std_logic;\n"
               << std::endl;
  // Specify ready, valid, and data ports for each input port:
  for (auto i = 0; i < numInputPorts; i++) {
    std::string portName = "    " + comp.getType() + "_in" + std::to_string(i);
    outputStream << portName + "_ready : out std_logic;\n"
                 << portName + "_valid : in std_logic;\n"
                 << portName + "_data : in std_logic_vector("
                 << circuitName + "_ram_width - 1 downto 0);\n"
                 << std::endl;
  }

  // Specify ready, valid, and data ports for each output port:
  for (auto i = 0; i < numOutputPorts; i++) {
    std::string portName = "    " + comp.getType() + "_out" + std::to_string(i);
    outputStream << portName + "_ready : in std_logic;\n"
                 << portName + "_valid : out std_logic;\n";
    if (i + 1 == numOutputPorts) {
      outputStream << portName + "_data : out std_logic_vector("
                   << circuitName + "_ram_width - 1 downto 0)\n" << std::endl; // last line of port declaration has no terminating semicolon
    } else {
      outputStream << portName + "_data : out std_logic_vector("
                   << circuitName + "_ram_width - 1 downto 0);\n" << std::endl;
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
               << circuitName + "_ram_width - 1 downto 0);\n"
               << std::endl;
  outputStream << "    " << "buffer_out_ready : in std_logic;\n"
               << "    " << "buffer_out_valid : out std_logic;\n"
               << "    " << "buffer_out_data : out std_logic_vector("
               << circuitName + "_ram_width - 1 downto 0)\n"
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

std::vector<std::string> algorithms::generateSendSigNames(std::string srcPort) {
  std::vector<std::string> sendSignals(3);

  sendSignals[VALID] = "SEND_" + srcPort + "_VALID";
  sendSignals[READY] = "SEND_" + srcPort + "_READY";
  sendSignals[DATA] = "SEND_" + srcPort + "_DATA";

  return sendSignals;
}
std::vector<std::string> algorithms::generateReceiveSigNames(std::string dstPort) {
  std::vector<std::string> receiveSignals(3);

  receiveSignals[VALID] = "RCV_" + dstPort + "_VALID";
  receiveSignals[READY] = "RCV_" + dstPort + "_READY";
  receiveSignals[DATA] = "RCV_" + dstPort + "_DATA";

  return receiveSignals;
}

std::string algorithms::generatePortMapping(VHDLCircuit circuit) {
  std::stringstream outputStream;
  std::string operatorSuffix = "_node";

  outputStream << "begin\n" << std::endl;
  // operator port mappings
  std::map<std::string, int> opCount(circuit.getOperatorMap());
  // initialise counts
  for (auto &c : opCount) {
    c.second = 0;
  }
  for (auto &op : circuit.getComponentMap()) {
    std::string opName = op.second.getType();
    // reset/clock mappings
    outputStream << opName << "_" + std::to_string(opCount[opName])
                 << " : " << opName << operatorSuffix << " PORT MAP (\n"
                 << "    " << opName << "_clk => " << circuit.getName() << "_clk,\n"
                 << "    " << opName << "_rst => " << circuit.getName() << "_rst,\n"
                 << std::endl;
    // input/output mappings
    int inPortCount = 0;
    int outPortCount = 0;
    // TODO define input operator mapping outside of input port for loop
    for (auto &inPort : op.second.getInputPorts()) {
      std::vector<std::string> receiveSigs(3);
      receiveSigs = generateReceiveSigNames(inPort);
      outputStream << "    " << opName << "_in" << std::to_string(inPortCount)
                   << "_ready => " << receiveSigs[READY] << ",\n"
                   << "    " << opName << "_in" << std::to_string(inPortCount)
                   << "_valid => " << receiveSigs[VALID] << ",\n"
                   << "    " << opName << "_in" << std::to_string(inPortCount)
                   << "_data => " << receiveSigs[DATA] << ",\n" << std::endl;
      inPortCount++;
    }
    // special case for input operators as they don't have input ports in SDF
    if (opName == "INPUT") {
      std::vector<std::string> receiveSigs(3);
      receiveSigs[READY] = circuit.getName() + "_in" + std::to_string(opCount[opName]) + "_ready";
      receiveSigs[VALID] = circuit.getName() + "_in" + std::to_string(opCount[opName]) + "_valid";
      receiveSigs[DATA] = circuit.getName() + "_in" + std::to_string(opCount[opName]) + "_data";
      outputStream << "    " << opName << "_in" << std::to_string(inPortCount)
                   << "_ready => " << receiveSigs[READY] << ",\n"
                   << "    " << opName << "_in" << std::to_string(inPortCount)
                   << "_valid => " << receiveSigs[VALID] << ",\n"
                   << "    " << opName << "_in" << std::to_string(inPortCount)
                   << "_data => " << receiveSigs[DATA] << ",\n" << std::endl;
      inPortCount++;
    }
    for (auto &outPort : op.second.getOutputPorts()) {
      std::vector<std::string> sendSigs(3);
      sendSigs = generateSendSigNames(outPort);
      outputStream << "    " << opName << "_out" << std::to_string(outPortCount)
                   << "_ready => " << sendSigs[READY] << ",\n"
                   << "    " << opName << "_out" << std::to_string(outPortCount)
                   << "_valid => " << sendSigs[VALID] << ",\n"
                   << "    " << opName << "_out" << std::to_string(outPortCount)
                   << "_data => " << sendSigs[DATA] << std::endl; // TODO check for last entry
      outPortCount++;
    }
    // special case for output operators as they don't have output ports in SDF
    if (opName == "OUTPUT") {
      std::vector<std::string> sendSigs(3);
      sendSigs[READY] = circuit.getName() + "_out" + std::to_string(opCount[opName]) + "_ready"; // TODO just always insert count
      sendSigs[VALID] = circuit.getName() + "_out" + std::to_string(opCount[opName]) + "_valid";
      sendSigs[DATA] = circuit.getName() + "_out" + std::to_string(opCount[opName]) + "_data";
      outputStream << "    " << opName << "_out" << std::to_string(outPortCount)
                   << "_ready => " << sendSigs[READY] << ",\n"
                   << "    " << opName << "_out" << std::to_string(outPortCount)
                   << "_valid => " << sendSigs[VALID] << ",\n"
                   << "    " << opName << "_out" << std::to_string(outPortCount)
                   << "_data => " << sendSigs[DATA] << std::endl; // TODO check for last entry
      outPortCount++;
    }
    outputStream << ");\n" << std::endl;
    opCount[opName]++;
  }

  // buffer port mappings
  int bufferCount = 0;
  std::string bCompName = "axi_fifo";
  std::string bName = "buffer";
  for (auto &buffer : circuit.getConnectionMap()) {
    std::vector<std::string> sendSigs = generateSendSigNames(buffer.second.getSrcPort());
    std::vector<std::string> receiveSigs = generateReceiveSigNames(buffer.second.getDstPort());
    // ram width/depth, reset, and clock mappings
    outputStream << "fifo_" + std::to_string(bufferCount)
                 << " : " << bCompName << " GENERIC MAP (\n"
                 << "    " << circuit.getName() << "_ram_width,\n"
                 << "    " << circuit.getName() << "_ram_depth)\n"
                 << std::endl;
    outputStream << "PORT MAP (\n"
                 << "    " << bName << "_clk => " << circuit.getName() << "_clk,\n"
                 << "    " << bName << "_rst => " << circuit.getName() << "_rst,\n"
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
  return outputStream.str();
}
