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
  VHDLCircuit();

  void addComponent(VHDLComponent newComp);
  void addConnection(VHDLConnection newConnect);
  void addInputPort(std::string portName, std::vector<std::string> signalNames);
  void addOutputPort(std::string portName, std::vector<std::string> signalNames);
  std::string printStatus();
  std::map<Vertex, VHDLComponent> getComponentMap();
  std::map<Edge, VHDLConnection> getConnectionMap();
  std::map<std::string, int> getOperatorMap();
  std::map<std::string, std::vector<std::string>> getInputPorts();
  std::map<std::string, std::vector<std::string>> getOutputPorts();
  int getOperatorCount(std::string op);
  VHDLComponent getFirstComponentByType(std::string op);
  std::string getName();
  void setName(std::string);

 private:
  std::map<Vertex, VHDLComponent> componentMap;
  std::map<Edge, VHDLConnection> connectionMap;
  std::map<std::string, int> operatorMap; // key: operator type, value: occurances
  std::string graphName;
  // ports of top-level input and output signals
  std::map<std::string, std::vector<std::string>> inputPorts;
  std::map<std::string, std::vector<std::string>> outputPorts;
};
#endif /* VHDL_CIRCUIT_H_ */
