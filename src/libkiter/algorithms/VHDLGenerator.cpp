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
  std::string bufferRefDir = "./vhdl_generation/";
  std::string bufferRefFileLoc = bufferRefDir + "axi_fifo.vhd";
  // Generate VHDL files for individual components
  for (auto const &op : operatorMap) {
    std::cout << "Generate VHDL component file for " << op.first << std::endl;
    generateOperator(circuit.getFirstComponentByType(op.first), compDir);
  }
  generateBuffer(compDir, bufferRefFileLoc);

}

void algorithms::generateOperator(VHDLComponent comp, std::string compDir) {
  std::ofstream vhdlOutput;
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
  vhdlOutput.open(compDir + componentName + ".vhd"); // instantiate VHDL file
  // 1. Define libraries used
  vhdlOutput << "library ieee;\n"
             << "use ieee.std_logic_1164.all;\n"
             << "use ieee.numeric_std.all;\n" << std::endl;
  // 2. Port declarations
  // TODO create map of port names for behaviour specification in step 3
  // every component requires clock and reset ports
  vhdlOutput << "entity " << componentName << " is\n"
             << "port (\n"
             << "    " << comp.getType() << "_clk : in std_logic;\n"
             << "    " << comp.getType() << "_rst : in std_logic;\n"
             << std::endl;
  // Specify ready, valid, and data ports for each input port:
  if (numInputPorts > 1) {
    for (auto i = 0; i < numInputPorts; i++) {
      std::string portName = "    " + comp.getType() + "_in" + std::to_string(i);
      vhdlOutput << portName + "_ready : out std_logic;\n"
                 << portName + "_valid : in std_logic;\n"
                 << portName + "_data : in std_logic_vector;\n"
                 << std::endl;
    }
  } else {
    std::string portName = "    " + comp.getType() + "_in";
    vhdlOutput << portName + "_ready : out std_logic;\n"
               << portName + "_valid : in std_logic;\n"
               << portName + "_data : in std_logic_vector;\n"
               << std::endl;
  }
  // Specify ready, valid, and data ports for each output port:
  if (numOutputPorts > 1) {
    for (auto i = 0; i < numOutputPorts; i++) {
      std::string portName = "    " + comp.getType() + "_out" + std::to_string(i);
      vhdlOutput << portName + "_ready : in std_logic;\n"
                 << portName + "_valid : out std_logic;\n" << std::endl;
      if (i + 1 == numOutputPorts) {
        vhdlOutput << portName + "_data : out std_logic_vector\n" << std::endl; // last line of port declaration has no terminating semicolon
      }
    }
  } else {
    std::string portName = "    " + comp.getType() + "_out";
    vhdlOutput << portName + "_ready : in std_logic;\n"
               << portName + "_valid : out std_logic;\n"
               << portName + "_data : out std_logic_vector\n" // last line of port declaration has no terminating semicolon
               << std::endl;
  }
  vhdlOutput << ");\nend " << componentName << ";\n" << std::endl;
  // 3. Specify architecture (behaviour) of operator type
  vhdlOutput << "architecture behaviour of " << componentName << " is\n"
             << "    begin\n" << std::endl;
  // TODO specify behaviour
  vhdlOutput << "end architecture;" << std::endl;

  vhdlOutput.close();
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
             << "    " << circuit.getName() << "_ram_width : natural;\n"
             << "    " << circuit.getName() << "_ram_depth : natural;\n"
             << ");\n"
             << "port (\n"
             << "    " << circuit.getName() << "_clk : in std_logic;\n"
             << "    " << circuit.getName() << "_rst : in std_logic;\n"
             << std::endl;
  // Specify ready, valid, and data ports for each input port:
  if (numInputPorts > 1) {
    for (auto i = 0; i < numInputPorts; i++) {
      std::string portName = "    " + circuit.getName() + "_in" + std::to_string(i);
      vhdlOutput << portName + "_ready : out std_logic;\n"
                 << portName + "_valid : in std_logic;\n"
                 << portName + "_data : in std_logic_vector;\n"
                 << std::endl;
    }
  } else {
    std::string portName = "    " + circuit.getName() + "_in";
    vhdlOutput << portName + "_ready : out std_logic;\n"
               << portName + "_valid : in std_logic;\n"
               << portName + "_data : in std_logic_vector;\n"
               << std::endl;
  }
  // Specify ready, valid, and data ports for each output port:
  if (numOutputPorts > 1) {
    for (auto i = 0; i < numOutputPorts; i++) {
      std::string portName = "    " + circuit.getName() + "_out" + std::to_string(i);
      vhdlOutput << portName + "_ready : in std_logic;\n"
                 << portName + "_valid : out std_logic;\n" << std::endl;
      if (i + 1 == numOutputPorts) {
        vhdlOutput << portName + "_data : out std_logic_vector\n" << std::endl; // last line of port declaration has no terminating semicolon
      } else {
        vhdlOutput << portName + "_data : out std_logic_vector;\n" << std::endl;
      }
    }
  } else {
    std::string portName = "    " + circuit.getName() + "_out";
    vhdlOutput << portName + "_ready : in std_logic;\n"
               << portName + "_valid : out std_logic;\n"
               << portName + "_data : out std_logic_vector\n" // last line of port declaration has no terminating semicolon
               << std::endl;
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
    if (counter == dataSignals.size()) {
      delim = "";
    }
    vhdlOutput << signal << delim << std::endl;
  }
  vhdlOutput << " : std_logic;\n" << std::endl;
  // TODO specify behaviour
  vhdlOutput << "end;" << std::endl; // TODO check if name of entity necessary here

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
  if (numInputPorts > 1) {
    for (auto i = 0; i < numInputPorts; i++) {
      std::string portName = "    " + comp.getType() + "_in" + std::to_string(i);
      outputStream << portName + "_ready : out std_logic;\n"
                 << portName + "_valid : in std_logic;\n"
                 << portName + "_data : in std_logic_vector("
                 << circuitName + "_ram_width - 1 downto 0);\n"
                 << std::endl;
    }
  } else {
    std::string portName = "    " + comp.getType() + "_in";
    outputStream << portName + "_ready : out std_logic;\n"
                 << portName + "_valid : in std_logic;\n"
                 << portName + "_data : in std_logic_vector("
                 << circuitName + "_ram_width - 1 downto 0);\n"
                 << std::endl;
  }
  // Specify ready, valid, and data ports for each output port:
  if (numOutputPorts > 1) {
    for (auto i = 0; i < numOutputPorts; i++) {
      std::string portName = "    " + comp.getType() + "_out" + std::to_string(i);
      outputStream << portName + "_ready : in std_logic;\n"
                 << portName + "_valid : out std_logic;\n" << std::endl;
      if (i + 1 == numOutputPorts) {
        outputStream << portName + "_data : out std_logic_vector("
                   << circuitName + "_ram_width - 1 downto 0)\n" << std::endl; // last line of port declaration has no terminating semicolon
      } else {
        outputStream << portName + "_data : out std_logic_vector("
                   << circuitName + "_ram_width - 1 downto 0);\n" << std::endl;
      }
    }
  } else {
    std::string portName = "    " + comp.getType() + "_out";
    outputStream << portName + "_ready : in std_logic;\n"
               << portName + "_valid : out std_logic;\n"
               << portName + "_data : out std_logic_vector("
               << circuitName + "_ram_width - 1 downto 0)\n" << std::endl;// last line of port declaration has no terminating semicolon
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

// Copies buffer specification from reference file
// NOTE would be more flexible if we didn't just copy off a file
void algorithms::generateBuffer(std::string compDir,
                                std::string bufferRefFile) {
  std::ofstream vhdlOutput;
  std::ifstream bufferReference(bufferRefFile);
  std::string fileContent;
  std::string componentName = "axi_fifo";

  vhdlOutput.open(compDir + componentName + ".vhd");
  if (bufferReference.is_open()) {
    while (std::getline(bufferReference, fileContent)) {
      vhdlOutput << fileContent << std::endl;
    }
    bufferReference.close();
    vhdlOutput.close();
  } else {
    std::cout << "Buffer reference file doesn't exist/not found!" << std::endl;
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
