/*
 * actor.h
 *
 *  Created on: 22 December 2020
 *      Author: jkmingwen
 */
#ifndef VHDL_CIRCUIT_H_
#define VHDL_CIRCUIT_H_

#include <models/Dataflow.h>
#include "VHDLComponent.h"
#include "VHDLConnection.h"

namespace models {
  class Dataflow;
}

class VHDLComponent;
class VHDLConnection;

class VHDLCircuit {
 public:
  VHDLCircuit(models::Dataflow* const dataflow);

  void addComponent(std::unique_ptr<VHDLComponent> newComp);
  void addConnection(std::unique_ptr<VHDLConnection> newConnect);
  std::map<Vertex, std::unique_ptr<VHDLComponent>> getComponentMap();
  std::map<Edge, std::unique_ptr<VHDLConnection>> getConnectionMap();
  std::string printStatus();
 private:
  models::Dataflow* dataflowRef;
  std::map<Vertex, std::unique_ptr<VHDLComponent>> componentMap;
  std::map<Edge, std::unique_ptr<VHDLConnection>> connectionMap;
};
#endif /* VHDL_CIRCUIT_H_ */
