/*
 * VHDLCircuit.cpp
 *
 *  Created on: 16 June 2021
 *      Author: jkmingwen
 */

#include <models/Dataflow.h>
#include "VHDLCircuit.h"

VHDLCircuit::VHDLCircuit(models::Dataflow* const dataflow) {
  dataflowRef = new models::Dataflow(*dataflow);
}

void VHDLCircuit::addComponent(std::unique_ptr<VHDLComponent> newComp) {
  this->componentMap.insert(std::pair(dataflowRef->getVertexById(newComp->getId()),
                                      newComp));
}

void VHDLCircuit::addConnection(std::unique_ptr<VHDLConnection> newConnect) {
  this->connectionMap.insert(std::pair(dataflowRef->getEdgeById(newConnect->getId()),
                                       newConnect));
}

std::map<Vertex, std::unique_ptr<VHDLComponent>> VHDLCircuit::getComponentMap() {
  return this->componentMap;
}

std::map<Edge, std::unique_ptr<VHDLConnection>> VHDLCircuit::getConnectionMap() {
  return this->connectionMap;
}

// std::string VHDLCircuit::printStatus() {
//   std::stringstream outputStream;

//   outputStream << "\nComponents: " << std::endl;
//   for (auto &comps : this->componentMap) {
//     outputStream << "\t" << (comps.second)->printStatus() << std::endl;
//   }
//   outputStream << "\nConnections: " << std::endl;
//   for (auto &conns : this->connectionMap) {
//     outputStream << "\t" << (conns.second)->printStatus() << std::endl;
//   }

//   return outputStream.str();
// }
