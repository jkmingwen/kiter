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
  std::string compType = dataflow->getVertexType(a);
  std::string baseCompType = compType.substr(0, compType.find("_")); // NOTE assuming a naming convention of "type_id"
  lifespan = 0;
  implementationName = "default";
  isMixedType = false;
  // identify order of arguments for binary operators
  // adapted from: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
  if (baseCompType != "Proj") { // ignore Proj actors generated by merging script
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
  }

  // check and track input/output data types
  for (auto port : inputPorts) {
    std::string portName = port;
    if (portName.substr(portName.find_last_of("_") + 1) == "vect") { // we ignore "vect" to find the actual data type
      portName.erase(portName.find_last_of("_"), std::string::npos);
    }
    inputTypes[portName.substr(portName.find_last_of("_") + 1)]++;
  }
  for (auto port : outputPorts) {
    std::string portName = port;
    if (portName.substr(portName.find_last_of("_") + 1) == "vect") { // we ignore "vect" to find the actual data type
      portName.erase(portName.find_last_of("_"), std::string::npos);
    }
    outputTypes[portName.substr(portName.find_last_of("_") + 1)]++;
  }
  // check if component type is a float
  char* pEnd;
  float numericValue = strtof(baseCompType.c_str(), &pEnd);
  // pEnd will be a character if there are any characters in baseCompType (therefore not a float)
  // and numericValue will be 0 if strtof fails to convert string to float; check for both in case
  // we have a baseCompType of "0"
  if (std::count(uiTypes.begin(), uiTypes.end(), baseCompType)) { // NOTE treat UI components as const_val for now
    isConstVal = true;
    numericValue = strtof((compType.substr(compType.find("_") + 1)).c_str(), &pEnd); // UI components have "type_defaultvalue" type naming convention; set value to default
    fpValue = numericValue;
    std::string fpcFloatPrefix = (numericValue ? "01" : "00");
    binaryValue = fpcFloatPrefix + floatToBinary(numericValue);
    componentType = baseCompType;
  } else if (std::count(arithmeticTypes.begin(), arithmeticTypes.end(), baseCompType) ||
             std::count(numOperatorTypes.begin(), numOperatorTypes.end(), baseCompType)) {
    isConstVal = false;
    if (inputTypes.size() > 1) { // mixing input types might result in unintended behaviour
      VERBOSE_WARNING(inputTypes.size() << " input types on " << compType
                      << " (" << dataflow->getVertexName(a) << ")");
      bool isInt = false;
      bool isFloat = false;
      bool isUndefined = false;
      for (auto &type : inputTypes) {
        if (type.first == "real") {
          isFloat = true;
        } else if (type.first == "int") {
          isInt = true;
        } else {
          isUndefined = true;
        }
      }
      if (isUndefined) {
        VERBOSE_WARNING("\tUndefined input type detected");
        dataType = "undefined"; // TODO replace with assert
      } else if (isInt && isFloat) { // if both integer and float input types detected, default to FP
        VERBOSE_WARNING("\tBoth Int and Float types detected; setting type to floating point");
        dataType = "fp";
        isMixedType = true;
      } else if (isFloat) {
        dataType = "fp";
      } else if (isInt) {
        dataType = "int";
      }
    } else {
      // infer arithmetic operator data type based on first data type stated by input port
      if (inputTypes.begin()->first == "real") {
        dataType = "fp"; // TODO remove this workaround during refactoring
      } else if (inputTypes.begin()->first == "int") {
        dataType = inputTypes.begin()->first;
      } else {
        dataType = "undefined"; // TODO replace with assert
      }
    }
    std::string compTypePrefix = dataType + "_";
    componentType = compTypePrefix + baseCompType;
  } else if (std::count(routingTypes.begin(), routingTypes.end(), baseCompType)) {
    isConstVal = false;
    componentType = baseCompType;
  }
  else if (baseCompType == "float") { // conversion operator
    isConstVal = false;
    componentType = "int2float";
  } else if (baseCompType == "int") { // conversion operator
    isConstVal = false;
    componentType = "float2int";
  } else if (*pEnd && numericValue == 0) { // not a numeric value
    isConstVal = false;
    fpValue = 0;
    componentType = baseCompType;
  } else { // is constant value
    if (outputTypes.size() == 1) {
      dataType = outputTypes.begin()->first;
    }
    else {
      dataType = "undefined"; // TODO replace with assert
    }
    if (dataType == "real") { // const is of type float
      fpValue = numericValue;
      std::string fpcFloatPrefix = (numericValue ? "01" : "00"); // NOTE might need to account for NaN (11) and Inf (10) values in the future
      binaryValue = fpcFloatPrefix + floatToBinary(numericValue);
    } else if (dataType == "int") {
      std::string::size_type sz;
      intValue = std::stoi(baseCompType, &sz);
      binaryValue = "00" + std::bitset<32>(intValue).to_string(); // NOTE assuming unsigned binary representation here
    } // TODO add else for edge cases
    isConstVal = true;
    componentType = "const_value";
  }
}

// converts a constant value int type to float
void VHDLComponent::convConstIntToFloat() {
  assert(this->isConst() && this->getDataType() == "int"); // only works on constant int value components
  this->fpValue = static_cast<float>(this->intValue);
  this->dataType = "fp";
  std::string fpcFloatPrefix = (this->fpValue ? "01" : "00");
  this->binaryValue = fpcFloatPrefix + floatToBinary(this->fpValue);
}

Vertex VHDLComponent::getActor() const {
  return this->actor;
}

std::string VHDLComponent::getName() const {
  return this->componentName;
}

ARRAY_INDEX VHDLComponent::getId() const {
  return this->id;
}

int VHDLComponent::getLifespan() const {
  return this->lifespan;
}

const std::vector<std::string>& VHDLComponent::getInputPorts() const{
  return this->inputPorts;
}

const std::vector<std::string>& VHDLComponent::getOutputPorts() const{
  return this->outputPorts;
}

const std::vector<std::string>& VHDLComponent::getInputEdges() const{
  return this->inputEdges;
}

const std::vector<std::string>& VHDLComponent::getOutputEdges() const{
  return this->outputEdges;
}

std::string VHDLComponent::getType() const{
  return this->componentType;
}

/* NOTE in the future, if we intend to implement setting different frequencies
   for each operator, generating their component names based on compType and
   operatorFreq would be one way of going about it */
std::string VHDLComponent::getImplementationName() const{
  return this->implementationName;
}

const std::vector<std::string>& VHDLComponent::getArgOrder() const{
  return this->argOrder;
}

const std::map<std::string, int>& VHDLComponent::getInputTypes() const{
  return this->inputTypes;
}

const std::map<std::string, int>& VHDLComponent::getOutputTypes() const{
  return this->outputTypes;
}

std::string VHDLComponent::getDataType() const{
  return this->dataType;
}

bool VHDLComponent::isConst() const {
  return this->isConstVal;
}

bool VHDLComponent::hasMixedType() const{
  return this->isMixedType;
}

std::string VHDLComponent::getBinaryValue() const{
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

void VHDLComponent::setImplementationName(std::string newName) {
  this->implementationName = newName;
}

std::string VHDLComponent::printStatus() const  {
  std::stringstream outputStream;

  outputStream << "\nActor " << this->getName() << " (ID: " << this->getId()
               << ")" << std::endl;
  outputStream << "\tType: " << this->getType() << std::endl;
  outputStream << "\tData type: " << this->getDataType() << std::endl;
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
  outputStream << "\tInput types: " << std::endl;
  for (auto inType : this->inputTypes) {
    outputStream << "\t\t" << inType.first << ", " << inType.second << std::endl;
  }
  outputStream << "\tOutput types: " << std::endl;
  for (auto outType : this->outputTypes) {
    outputStream << "\t\t" << outType.first << ", " << outType.second << std::endl;
  }
  outputStream << "\tIs constant?";
  if (this->isConstVal) {
    outputStream << " Y" << std::endl;
    if (this->dataType == "real") {
      outputStream <<"\t\tValue: " << this->fpValue << std::endl;
    } else if (this->dataType == "int") {
      outputStream <<"\t\tValue: " << this->intValue << std::endl;
    } // TODO add else for edge cases
    outputStream << "\t\tBinary rep: " << this->binaryValue << std::endl;
  } else {
    outputStream << " N" << std::endl;
  }

  return outputStream.str();
}
