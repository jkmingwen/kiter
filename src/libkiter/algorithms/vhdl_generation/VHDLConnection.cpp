/*
 * VHDLComponent.cpp
 *
 *  Created on: 15 June 2021
 *      Author: jkmingwen
 */

#include <models/Dataflow.h>
#include "VHDLConnection.h"

VHDLConnection::VHDLConnection(models::Dataflow* const dataflow, Edge e) {
  edge = e;
  connectionName = dataflow->getEdgeName(e);
  id = dataflow->getEdgeId(e);
  dataType = "float"; // assuming only data type is float for now
  bufferSize = dataflow->getPreload(e) + 1; // NOTE +1 of initial tokens is likely minimum buffer size; can replace with buffer size from XML after DSE in future
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

std::string VHDLConnection::printStatus()  const{
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
