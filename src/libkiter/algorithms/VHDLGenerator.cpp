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

void algorithms::generateVHDL(models::Dataflow* const dataflow,
                              parameters_list_t param_list) {
  bool outputDirSpecified = false;
  std::string dirName = "./" + dataflow->getGraphName() + "_vhdl_gen/"; // default output directory
  std::string componentDir = dirName + "/components/";
  std::ofstream outputFile;
  VHDLCircuit circuit;

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

  return;
}

void algorithms::generateOperators(VHDLCircuit circuit, std::string compDir) {
  std::map<std::string, int> operatorMap = circuit.getOperatorMap();
  // Generate VHDL files for individual components
  // TODO generate buffer component
  for (auto const &op : operatorMap) {
    std::cout << "Generate VHDL component file for " << op.first << std::endl;
    generateOperator(circuit.getFirstComponentByType(op.first), compDir);
  }

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
