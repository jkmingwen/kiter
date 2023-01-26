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
  int getOperatorLifespan(std::string opType);
  std::string getOperatorFPCName(std::string opType);
  std::map<int, int> getNumOutputs(std::string opType);
  std::vector<std::string> getConnectionNameFromComponents(std::string srcActorName,
                                                           std::string dstActorName);
  std::vector<std::string> getDstPortBetweenComponents(std::string srcActorName,
                                                       std::string dstActorName);
  std::string getComponentFullName(std::string partialName);
  void setName(std::string);
  std::vector<std::string> getMultiOutActors();
  std::vector<VHDLComponent> getDstComponents(VHDLComponent srcComponent);
  void convConstIntToFloat(Vertex v);

 private:
  std::map<Vertex, VHDLComponent> componentMap;
  std::map<Edge, VHDLConnection> connectionMap;
  std::map<std::string, int> operatorMap; // key: operator type, value: occurances
  std::string graphName;
  // ports of top-level input and output signals
  std::map<std::string, std::vector<std::string>> inputPorts;
  std::map<std::string, std::vector<std::string>> outputPorts;
  /* Each component has a specific lifespan and name that needs to be defined in
     the generated AXI interface --- we track them using a predefined map where
     the keys are the "types" of the operators, which are taken from the actor
     types in the SDF file generated from the Faust application */
  std::map<std::string, int> operatorLifespans = {{"fp_add", 3}, {"fp_prod", 1},
                                                  {"fp_div", 8}, {"fp_sqrt", 5},
                                                  {"fp_diff", 3}, {"fp_pow", 8},
                                                  {"int_add", 1}, {"int_prod", 1},
                                                  {"float2int", 1}, {"int2float", 1}};
  std::map<std::string, std::string> FPCNames = {{"fp_add", "FPAdd_8_23_F125_uid2"},
                                                 {"fp_prod", "FPMult_8_23_8_23_8_23_uid2_F125_uid3"},
                                                 {"fp_div", "FPDiv_8_23_F125_uid2"},
                                                 {"fp_sqrt", "FPSqrt_8_23"},
                                                 {"fp_diff", "FPSub_8_23_F125_uid2"},
                                                 {"fp_pow", "FPPowr_8_23_F125_uid2"},
                                                 {"int_add", "IntAdder_34_F125_uid2"},
                                                 {"int_diff", "IntAdder_34_F125_uid2"},
                                                 {"int_prod", "IntMultiplier_F125_uid2"},
                                                 {"float2int", "FP2Fix_8_23_0_33_S_T_F125_uid2"},
                                                 {"int2float", "Fix2FP_0_33_S_8_23_F125_uid2"}};

};
#endif /* VHDL_CIRCUIT_H_ */
