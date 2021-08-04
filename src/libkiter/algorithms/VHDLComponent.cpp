/*
 * VHDLComponent.cpp
 *
 *  Created on: 15 June 2021
 *      Author: jkmingwen
 */

#include <models/Dataflow.h>
#include "VHDLComponent.h"

VHDLComponent::VHDLComponent(models::Dataflow* const dataflow, Vertex a) {
  actor = a;
  componentName = dataflow->getVertexName(a);
  id = dataflow->getVertexId(a);
  {ForInputEdges(dataflow, a, inEdge) {
      inputPorts.push_back(dataflow->getEdgeOutputPortName(inEdge));
      inputEdges.push_back(dataflow->getEdgeName(inEdge));
    }}
  {ForOutputEdges(dataflow, a, outEdge) {
      outputPorts.push_back(dataflow->getEdgeInputPortName(outEdge));
      outputEdges.push_back(dataflow->getEdgeName(outEdge));
    }}
  std::string compType = dataflow->getVertexType(a);
  componentType = compType.substr(0, compType.find("_")); // NOTE assuming a naming convention of "type_id"
  lifespan = 0;
  FPCName = "default";
  // check if component type is a float
  char* pEnd;
  float numericValue = strtof(componentType.c_str(), &pEnd);
  // pEnd will be a character if there are any characters in componentType (therefore not a float)
  // and numericValue will be 0 if strtof fails to convert string to float; check for both in case
  // we have a componentType of "0"
  if (*pEnd && numericValue == 0) {
    isConstVal = false;
    value = 0;
  } else {
    isConstVal = true;
    value = numericValue;
  }
}

Vertex VHDLComponent::getActor() {
  return this->actor;
}

std::string VHDLComponent::getName() {
  return this->componentName;
}

ARRAY_INDEX VHDLComponent::getId() {
  return this->id;
}

int VHDLComponent::getLifespan() {
  return this->lifespan;
}

std::vector<std::string> VHDLComponent::getInputPorts() {
  return this->inputPorts;
}

std::vector<std::string> VHDLComponent::getOutputPorts() {
  return this->outputPorts;
}

std::vector<std::string> VHDLComponent::getInputEdges() {
  return this->inputEdges;
}

std::vector<std::string> VHDLComponent::getOutputEdges() {
  return this->outputEdges;
}

std::string VHDLComponent::getType() {
  return this->componentType;
}

std::string VHDLComponent::getFPCName() {
  return this->FPCName;
}

void VHDLComponent::setName(std::string newName) {
  this->componentName = newName;
}

void VHDLComponent::setId(ARRAY_INDEX newId) {
  this->id = newId;
}

void VHDLComponent::setLifespan(int lifespan) {
  this->lifespan = lifespan;
}

void VHDLComponent::setFPCName(std::string newName) {
  this->FPCName = newName;
}

std::string VHDLComponent::printStatus() {
  std::stringstream outputStream;

  outputStream << "\nActor " << this->getName() << " (ID: " << this->getId()
               << ")" << std::endl;
  outputStream << "\tType: " << this->getType() << std::endl;
  outputStream << "\tInput ports: " << std::endl;
  for (auto portName : this->getInputPorts()) {
    outputStream << "\t\t" << portName << std::endl;
  }
  outputStream << "\tOutput ports: " << std::endl;
  for (auto portName : this->getOutputPorts()) {
    outputStream << "\t\t" << portName << std::endl;
  }
  outputStream << "\tInput edges: " << std::endl;
  for (auto portName : this->getInputEdges()) {
    outputStream << "\t\t" << portName << std::endl;
  }
  outputStream << "\tOutput edges: " << std::endl;
  for (auto portName : this->getOutputEdges()) {
    outputStream << "\t\t" << portName << std::endl;
  }
  outputStream << "\tIs constant?";
  if (this->isConstVal) {
    outputStream << " Y, value of " << this->value << std::endl;
  } else {
    outputStream << " N" << std::endl;
  }

  return outputStream.str();
}
