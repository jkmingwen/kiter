/*
 * VHDLComponent.cpp
 *
 *  Created on: 15 June 2021
 *      Author: jkmingwen
 */

#include <models/Dataflow.h>
#include "VHDLComponent.h"

// returns binary representation of float
// adapted from https://www.codeproject.com/Questions/678447/Can-any-one-tell-me-how-to-convert-a-float-to-bina
std::string floatToBinary(float f)
{
  std::stringstream outputStream;
  size_t size = sizeof(f);
  unsigned char * p = (unsigned char *) &f;
  p += size-1;
  while (size--)
  {
    int n;
    for (n=0; n<8; n++)
    {
      char bit = ('0' + (*p & 128 ? 1 : 0));
      outputStream << bit;
      *p <<= 1;
    }
    p--;
  }

  return outputStream.str();
}

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
  // identify order of arguments for binary operators
  // adapted from: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
  std::string argFromName = dataflow->getVertexName(a);
  size_t pos = argFromName.find("_");
  argFromName.erase(0, pos + 1); // first ID before '_' is name of actor
  std::string argName;
  while ((pos = argFromName.find("_")) != std::string::npos) {
    argName = argFromName.substr(0, pos);
    argOrder.push_back(argName);
    argFromName.erase(0, pos + 1);
    if (argFromName.find("_") == std::string::npos) { // last element from split string
      argOrder.push_back(argFromName);
    }
  }
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
if (std::count(uiTypes.begin(), uiTypes.end(), componentType)) { // NOTE treat UI components as const_val for now
    isConstVal = true;
    numericValue = strtof((compType.substr(compType.find("_") + 1)).c_str(), &pEnd); // UI components have "type_defaultvalue" type naming convention; set value to default
    value = numericValue;
    std::string fpcFloatPrefix = (numericValue ? "01" : "00");
    binaryValue = fpcFloatPrefix + floatToBinary(numericValue);
  } else if (*pEnd && numericValue == 0) {
    isConstVal = false;
    value = 0;
  } else {
    isConstVal = true;
    componentType = "const_value";
    value = numericValue;
    std::string fpcFloatPrefix = (numericValue ? "01" : "00"); // NOTE might need to account for NaN (11) and Inf (10) values in the future
    binaryValue = fpcFloatPrefix + floatToBinary(numericValue);
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

bool VHDLComponent::isConst() {
  return this->isConstVal;
}

std::string VHDLComponent::getBinaryValue() {
  return this->binaryValue;
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
  if (argOrder.size()) {
    outputStream << "\tArgument actors:" << std::endl;
    int order = 1;
    for (auto& i : argOrder) {
      outputStream << "\t\t" << order << ". " << i << std::endl;
      order++;
    }
  }
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
    outputStream << " Y\n"
                 <<"\t\tValue: " << this->value << "\n"
                 << "\t\tBinary rep: " << this->binaryValue << std::endl;
  } else {
    outputStream << " N" << std::endl;
  }

  return outputStream.str();
}
