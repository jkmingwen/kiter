/*
 * VHDLComponent.cpp
 *
 *  Created on: 15 June 2021
 *      Author: jkmingwen
 */

#include <models/Dataflow.h>
#include "VHDLConnection.h"
#include "commons/verbose.h"

VHDLConnection::VHDLConnection(models::Dataflow* const dataflow, Edge e) {
  edge = e;
  connectionName = dataflow->getEdgeName(e);
  id = dataflow->getEdgeId(e);
  dataType = "float"; // assuming only data type is float for now
  bufferSize = dataflow->getPreload(e); // we set the buffer size to the amount of initial tokens as there's no separate notion of buffer size and initial tokens in SDF model
  initialTokenCount = dataflow->getPreload(e);
  dataTypeWidth = 34; // assume data type is float (using FloPoCo float representation)
  srcPort = dataflow->getEdgeInputPortName(e);
  dstPort = dataflow->getEdgeOutputPortName(e);
  inputVector = dataflow->getEdgeInVector(e);
  outputVector = dataflow->getEdgeOutVector(e);

  if (initialTokenCount) { // TODO check implementation type for type of
                           // intemediary component
    bufferInstanceName = "fifo";
    bufferImplRefName = "hs_fifo";
    // HS FIFO ports
    addPortMapping("clk", "clk", "std_logic", "in");
    addPortMapping("rst", "rst", "std_logic", "in");
    addPortMapping("ram_width", "ram_width", "integer", "", true);
    addPortMapping("ram_depth", std::to_string(bufferSize + 1), "integer", "",
                   true);
    addPortMapping("ram_init", std::to_string(initialTokenCount), "integer", "",
                   true);
    addPortMapping("buffer_in_ready", srcPort + "_READY", "std_logic", "out");
    addPortMapping("buffer_in_valid", srcPort + "_VALID", "std_logic", "in");
    addPortMapping("buffer_in_data", srcPort + "_DATA", "std_logic_vector",
                   "in");
    addPortMapping("buffer_out_ready", dstPort + "_READY", "std_logic", "in");
    addPortMapping("buffer_out_valid", dstPort + "_VALID", "std_logic", "out");
    addPortMapping("buffer_out_data", dstPort + "_DATA", "std_logic_vector", "out");
  }
}

Edge VHDLConnection::getEdge()  const{
  return this->edge;
}



ARRAY_INDEX VHDLConnection::getId()  const{
  return this->id;
}

std::string VHDLConnection::getSrcPort()  const{
  return this->srcPort;
}

std::string VHDLConnection::getDstPort()  const{
  return this->dstPort;
}

std::string VHDLConnection::getType()  const{
  return this->dataType;
}

TOKEN_UNIT VHDLConnection::getBufferSize()  const{
  return this->bufferSize;
}

TOKEN_UNIT VHDLConnection::getInitialTokenCount()  const{
  return this->initialTokenCount;
}

int VHDLConnection::getTypeWidth()  const{
  return this->dataTypeWidth;
}

std::vector<TOKEN_UNIT> VHDLConnection::getInputVector() const {
  return this->inputVector;
}

std::vector<TOKEN_UNIT> VHDLConnection::getOutputVector() const {
  return this->outputVector;
}

void VHDLConnection::setName(std::string newName) {
  this->connectionName = newName;
}

void VHDLConnection::setId(ARRAY_INDEX newId) {
  this->id = newId;
}

void VHDLConnection::setBufferSize(TOKEN_UNIT size) {
  this->bufferSize = size;
}

void VHDLConnection::setInitialTokenCount(TOKEN_UNIT count) {
  assert(count <= this->bufferSize);
  this->initialTokenCount = count;
}

/**
   Generate signal names for the given connection.

   @param t The implementation type determines the types (and number) of signals
   generated.

   @return A map of signal types to vectors of signal names.

 */
std::map<std::string, std::vector<std::string>>
VHDLConnection::genSignalNames(implType t) const {
  std::map<std::string, std::vector<std::string>> names;
  if (t == TT) { // time triggered implementation only requires data signal
    std::string type = "std_logic_vector(" + std::to_string(dataTypeWidth - 1) + " downto 0)";
    names[type].push_back(connectionName);
  } else if (t == DD) {
    // generate data, valid, ready signal names
    std::string dataType =
      "std_logic_vector(" + std::to_string(dataTypeWidth - 1) + " downto 0)";
    std::string vldRdyType = "std_logic";
    // initial tokens means that FIFO is placed there; need intermediate signals;
    // generate names using ports instead
    if (initialTokenCount > 0) {
      names[dataType].push_back(srcPort + "_DATA");
      names[dataType].push_back(dstPort + "_DATA");
      names[vldRdyType].push_back(srcPort + "_VALID");
      names[vldRdyType].push_back(srcPort + "_READY");
      names[vldRdyType].push_back(dstPort + "_VALID");
      names[vldRdyType].push_back(dstPort + "_READY");
    } else {
      names[dataType].push_back(connectionName + "_DATA");
      names[vldRdyType].push_back(connectionName + "_VALID");
      names[vldRdyType].push_back(connectionName + "_READY");
    }
  } else {
    VERBOSE_ERROR("Invalid implementation type (" << t << ") specified for signal name generation.");
  }

  return names;
}

std::string VHDLConnection::printStatus() const {
  std::stringstream outputStream;

  outputStream << "\nEdge " << this->getName() << " (ID: " << this->getId()
               << ")" << std::endl;
  outputStream << "\tData type, width: " << this->getType()
               << ", " << this->getTypeWidth() << std::endl;
  outputStream << "\tBuffer size: " << this->getBufferSize() << std::endl;
  outputStream << "\tSource port, Destination port: "
               << this->getSrcPort() << ", " << this->getDstPort() << std::endl;

  return outputStream.str();
}

void VHDLConnection::addPortMapping(std::string port, std::string signal,
                                   std::string type, std::string direction,
                                   bool isGeneric, int dataWidth) {
 // slv type needs to have a defined width
  if (type == "std_logic_vector") {
    type += "(" + std::to_string(dataWidth - 1) + " downto 0)";
  }
  if (isGeneric) {
    genericMappings[port] = signal;
    genericPorts[port] = type;
  } else {
    portMappings[port] = signal;
    ports[port] = direction + " " + type;
  }
}

/**
   Generate VHDL port declarations according to a given port map where:
   [key: port name, value: signal/type].

   @param portMap Populated using addPortMapping().

   @param terminate Whether the port declarations should have a terminating
   delimiter (default = true).

   @param replacements Optional map of signal names that should be replaced
   (key), and their corresponding replacements (value).

   @param relation Defines the syntax for how the port relates to the signal
   (default = ":").

   @param delim Defines the delimiter between each port declaration
   (default = ";").

   @param term Defines the terminating character after the last port is declared
   (default = "").

   @return A string of port declarations.
 */
std::string
VHDLConnection::genPortList(std::map<std::string, std::string> portMap,
                            bool terminate,
                            std::map<std::string, std::string> replacements,
                            std::string relation,
                            std::string delim,
                            std::string term) const {
  std::stringstream codeOut;
  int padAmt = 4;
  std::string t(padAmt, ' ');
  std::string lineEnder = delim;
  std::string r = " " + relation + " "; // spaces for legibility
  for (auto &[port, sigType] : portMap) {
    std::string signal = sigType;
    if (replacements.count(sigType)) {
      signal = replacements[signal];
    }
    if (port == portMap.rbegin()->first && terminate) {
      lineEnder = term;
    }
    codeOut << t << port << r << signal << lineEnder << std::endl;
  }
  return codeOut.str();
}

// Generate the VHDL declaration code for the component
// - dataWidth (default: 34): bit width of data on the input/output ports
std::string VHDLConnection::genDeclaration() const {
  std::stringstream codeOut;
  std::string t = "    "; // indentation: 4 spaces
  codeOut << "component " << bufferImplRefName << " is"
          << std::endl;
    if (genericPorts.size()) {
      codeOut << "generic (" << std::endl;
      codeOut << genPortList(genericPorts);
      codeOut << ");" << std::endl;
    }
    if (ports.size()) {
      codeOut << "port (" << std::endl;
      codeOut << genPortList(ports);
      codeOut << ");" << std::endl;
    }
  codeOut << "end component;" << std::endl;

  return codeOut.str();
}

std::string VHDLConnection::genPortMapping(int id, std::map<std::string, std::string> replacements) const {
  std::stringstream codeOut;
  codeOut << bufferInstanceName << "_" << id << " : " << bufferImplRefName << std::endl;
  if (genericMappings.size()) {
    codeOut << "generic map (" << std::endl;
    codeOut << genPortList(genericMappings, true, replacements, "=>", ",");
    codeOut << ")" << std::endl;
  }
  if (portMappings.size()) {
    codeOut << "port map (" << std::endl;
    codeOut << genPortList(portMappings, true, replacements, "=>", ",");
    codeOut << ");" << std::endl;
  }

  return codeOut.str();
}
