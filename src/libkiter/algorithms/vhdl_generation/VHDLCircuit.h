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
  std::string getOperatorImplementationName(std::string opType);
  std::map<int, int> getNumOutputs(std::string opType);
  std::vector<std::string> getConnectionNameFromComponents(std::string srcActorName,
                                                           std::string dstActorName);
  std::vector<std::string> getDstPortBetweenComponents(std::string srcActorName,
                                                       std::string dstActorName);
  std::string getComponentFullName(std::string partialName);
  void setName(std::string);
  void setOperatorFreq(int freq);
  std::vector<std::string> getMultiOutActors();
  std::vector<VHDLComponent> getDstComponents(VHDLComponent srcComponent);
  void convConstIntToFloat(Vertex v);

 private:
  std::map<Vertex, VHDLComponent> componentMap;
  std::map<Edge, VHDLConnection> connectionMap;
  std::map<std::string, int> operatorMap; // key: operator type, value: occurances
  std::string graphName;
  int operatorFreq = 125; // default operating frequency of circuit is 125MHz
  // ports of top-level input and output signals
  std::map<std::string, std::vector<std::string>> inputPorts;
  std::map<std::string, std::vector<std::string>> outputPorts;
  /* Each component has a specific lifespan and name that needs to be defined in
     the generated AXI interface --- we track them using a predefined map where
     the keys are the operating frequencies. The "types" of the operators,
     which are taken from the actor types in the SDF file generated from the Faust
     application, then map to the number of clock cycles taken for them to complete */
  std::map<int, std::map<std::string, int>> operatorLifespans =
    {
      {50,
       {{"fp_add", 1}, {"fp_prod", 1}, {"fp_div", 3}, {"fp_sqrt", 1},
        {"fp_diff", 1}, {"fp_pow", 3}, {"int_add", 1}, {"int_prod", 1},
        {"int_diff", 1}, {"float2int", 1}, {"int2float", 1},
        // NOTE unimplemented operators from here:
        {"fp_floor", 1}, {"int_max", 1}, {"int_min", 1}}},
      {125,
       {{"fp_add", 3}, {"fp_prod", 1}, {"fp_div", 8}, {"fp_sqrt", 5},
        {"fp_diff", 3}, {"fp_pow", 8}, {"int_add", 1}, {"int_prod", 1},
        {"int_diff", 1}, {"float2int", 1}, {"int2float", 1},
        // NOTE unimplemented operators from here:
        {"fp_floor", 1}, {"int_max", 1}, {"int_min", 1}}},
      {250,
       {{"fp_add", 6}, {"fp_prod", 1}, {"fp_div", 18}, {"fp_sqrt", 10},
        {"fp_diff", 6}, {"fp_pow", 18}, {"int_add", 1}, {"int_prod", 1},
        {"int_diff", 1}, {"float2int", 2}, {"int2float", 3},
        // NOTE unimplemented operators from here:
        {"fp_floor", 1}, {"int_max", 1}, {"int_min", 1}}}
    };
  std::map<std::string, std::string> implementationNames = {{"fp_add", "fp_add_flopoco"},
                                                            {"fp_prod", "fp_prod_flopoco"},
                                                            {"fp_div", "fp_div_flopoco"},
                                                            {"fp_sqrt", "fp_sqrt_flopoco"},
                                                            {"fp_diff", "fp_diff_flopoco"},
                                                            {"fp_pow", "fp_pow_flopoco"},
                                                            {"int_add", "int_add_flopoco"},
                                                            {"int_diff", "int_diff_flopoco"},
                                                            {"int_prod", "int_prod_flopoco"},
                                                            {"float2int", "float2int_flopoco"},
                                                            {"int2float", "int2float_flopoco"},
                                                            {"fp_floor", "fp_floor"},
                                                            {"int_max", "int_max"},
                                                            {"int_min", "int_min"}};

};
#endif /* VHDL_CIRCUIT_H_ */
