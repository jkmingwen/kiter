/*
 * VHDLCircuit.cpp
 *
 *  Created on: 16 June 2021
 *      Author: jkmingwen
 */

#include <models/Dataflow.h>
#include "VHDLCircuit.h"

VHDLCircuit::VHDLCircuit() {}

void VHDLCircuit::addComponent(VHDLComponent newComp) {
  newComp.setLifespan(this->getOperatorLifespan(newComp.getType()));
  newComp.setFPCName(this->getOperatorFPCName(newComp.getType()));
  this->componentMap.insert(std::make_pair(newComp.getActor(),
                                           newComp));
  this->operatorMap[newComp.getType()]++; // track operators used in circuit
}

void VHDLCircuit::addInputPort(std::string portName,
                               std::vector<std::string> signalNames) {
  this->inputPorts.insert(std::make_pair(portName, signalNames));
}

void VHDLCircuit::addOutputPort(std::string portName,
                                std::vector<std::string> signalNames) {
  this->outputPorts.insert(std::make_pair(portName, signalNames));
}

std::map<Vertex, VHDLComponent> VHDLCircuit::getComponentMap() {
  return this->componentMap;
}

std::map<Edge, VHDLConnection> VHDLCircuit::getConnectionMap() {
  std::map<Edge, VHDLConnection> connMap(this->connectionMap);
  return connMap;
}

std::map<std::string, int> VHDLCircuit::getOperatorMap() {
  return this->operatorMap;
}

std::map<std::string, std::vector<std::string>> VHDLCircuit::getInputPorts() {
  return this->inputPorts;
}

std::map<std::string, std::vector<std::string>> VHDLCircuit::getOutputPorts() {
  return this->outputPorts;
}

int VHDLCircuit::getOperatorCount(std::string op) {
  std::map<std::string, int> opMap = this->getOperatorMap();
  return opMap[op];
}

// iterate through component map and return first instance of component matching type from argument
VHDLComponent VHDLCircuit::getFirstComponentByType(std::string op) {
  for (auto &comp : this->componentMap) {
    if (comp.second.getType() == op) {
      return comp.second;
    }
  }
  // if component of matching type not found
  std::cout << "No component of matching type found" << std::endl;
  // TODO return null VHDL component if nothing found
}

std::string VHDLCircuit::getName() {
  return this->graphName;
}

int VHDLCircuit::getOperatorLifespan(std::string opType) {
  if (this->operatorLifespans.count(opType)) {
    return this->operatorLifespans[opType];
  } else {
    // NOTE default to 0 if lifespan not specified
    return 0;
  }
}

std::string VHDLCircuit::getOperatorFPCName(std::string opType) {
  if (this->FPCNames.count(opType)) {
    return this->FPCNames[opType];
  } else {
    return "UNKNOWN_OPERATOR";
  }
}

void VHDLCircuit::setName(std::string newName) {
  this->graphName = newName;
}

void VHDLCircuit::addConnection(VHDLConnection newConnect) {
  this->connectionMap.insert(std::make_pair(newConnect.getEdge(),
                                            newConnect));
}

std::string VHDLCircuit::printStatus() {
  std::stringstream outputStream;

  outputStream << "\nVHDL Circuit components and connections:\n" << std::endl;
  outputStream << "Components:" << std::endl;
  for (auto &comps : this->componentMap) {
    outputStream << "\t" << (comps.second).printStatus() << std::endl;
  }
  outputStream << "Connections:" << std::endl;
  for (auto &conns : this->connectionMap) {
    outputStream << "\t" << (conns.second).printStatus() << std::endl;
  }
  outputStream << "Operator, count (and lifespan):" << std::endl;
  for (auto &op : this->operatorMap) {
    outputStream << "\t" << op.first << ", "
                 << op.second << " (" << this->getOperatorLifespan(op.first)
                 << ")\n";
  }
  outputStream << std::endl;
  outputStream << "Top-level input port names:" << std::endl;
  for (auto &port : this->getInputPorts()) {
    outputStream << "\t" << port.first << std::endl;
  }
  outputStream << "Top-level output port names:" << std::endl;
  for (auto &port : this->getOutputPorts()) {
    outputStream << "\t" << port.first << std::endl;
  }

  return outputStream.str();
}
