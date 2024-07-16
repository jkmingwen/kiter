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
