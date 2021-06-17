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
  this->componentMap.insert(std::make_pair(newComp.getActor(),
                                           newComp));
  this->operatorMap[newComp.getType()]++; // track operators used in circuit
}

std::map<std::string, int> VHDLCircuit::getOperatorMap() {
  return this->operatorMap;
}

int VHDLCircuit::getOperatorCount(std::string op) {
  std::map<std::string, int> opMap = this->getOperatorMap();
  return opMap[op];
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
  outputStream << "Operators:" << std::endl;
  for (auto &op : this->operatorMap) {
    outputStream << "\t" << op.first << ", " << op.second << std::endl;
  }

  return outputStream.str();
}
