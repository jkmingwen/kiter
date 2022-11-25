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

  std::string printStatus() const ;
  const std::map<Vertex, VHDLComponent>& getComponentMap()const;
  const std::map<Edge, VHDLConnection>& getConnectionMap()const;
  const std::map<std::string, int>& getOperatorMap()const;
  const std::map<std::string, std::vector<std::string>>& getInputPorts()const;
  const std::map<std::string, std::vector<std::string>>& getOutputPorts()const;
  int getOperatorCount(std::string op)const;
  VHDLComponent getFirstComponentByType(std::string op)const;
  std::string getName()const;
  int getOperatorLifespan(std::string opType)const;
  std::string getOperatorFPCName(std::string opType)const;
  std::map<int, int> getNumOutputs(std::string opType)const;
  std::vector<std::string> getConnectionNameFromComponents(std::string srcActorName,
                                                           std::string dstActorName)const;
  std::vector<std::string> getDstPortBetweenComponents(std::string srcActorName,
                                                       std::string dstActorName)const;
  std::string getComponentFullName(std::string partialName)const;
  void setName(std::string);
  std::vector<std::string> getMultiOutActors()const;
  std::vector<VHDLComponent> getDstComponents(VHDLComponent srcComponent)const;
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
  std::map<std::string, int> operatorLifespans = {{"fp_add", 14}, {"fp_prod", 3},
                                                  {"fp_div", 29}, {"fp_sqrt", 22},
                                                  {"fp_diff", 14}, {"fp_pow", 37},
                                                  {"int_add", 1}, {"int_prod", 2},
                                                  {"float2int", 4}, {"int2float", 4}};
  std::map<std::string, std::string> FPCNames = {{"fp_add", "FPAdd_8_23_F400_uid2"},
                                                 {"fp_prod", "FPMult_8_23_8_23_8_23_uid2_F400_uid3"},
                                                 {"fp_div", "FPDiv_8_23_F400_uid2"},
                                                 {"fp_sqrt", "FPSqrt_8_23"},
                                                 {"fp_diff", "FPAdd_8_23_F400_uid2"},
                                                 {"fp_pow", "FPPowr_8_23_F400_uid2"},
                                                 {"int_add", "IntAdder_34_F400_uid2"},
                                                 {"int_diff", "IntAdder_34_F400_uid2"},
                                                 {"int_prod", "IntMultiplier_F400_uid2"},
                                                 {"float2int", "FP2Fix_8_23_0_33_S_NT_F400_uid2"},
                                                 {"int2float", "Fix2FP_0_33_S_8_23_F400_uid2"}};

};
#endif /* VHDL_CIRCUIT_H_ */
