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
  void addInputPort(std::string portName, const std::vector<std::string> &signalNames);
  void addOutputPort(std::string portName, const std::vector<std::string> &signalNames);

  std::string printStatus();

    const std::string &getName() const {
        return this->graphName;
    }

    const std::map<Vertex, VHDLComponent>& getComponentMap() const {
        return this->componentMap;
    }

    const std::map<Edge, VHDLConnection>& getConnectionMap() const {
        return this->connectionMap;
    }

    const std::map<std::string, int>& getOperatorMap() const {
        return this->operatorMap;
    }

    const std::map<std::string, std::vector<std::string>>& getInputPorts() const {
        return this->inputPorts;
    }

    const std::map<std::string, std::vector<std::string>>& getOutputPorts() const {
        return this->outputPorts;
    }
  int getOperatorCount(const std::string &op) const;
  const VHDLComponent&  getFirstComponentByType(const std::string &op) const;

  int getOperatorLifespan(const std::string &opType) const;
  std::string getOperatorImplementationName(const std::string &opType) const;
  std::map<int, int> getNumInputs(const  std::string &opType) const;
  std::map<int, int> getNumOutputs(const  std::string &opType) const;
  std::vector<std::string> getConnectionNameFromComponents(const std::string &srcActorName,
                                                           const std::string &dstActorName) const;
  std::vector<std::string> getDstPortBetweenComponents(const std::string &srcActorName,
                                                       const std::string &dstActorName) const;
  std::string getComponentFullName(const std::string &partialName) const;
    std::vector<std::string> getMultiOutActors() const;
    std::vector<VHDLComponent> getDstComponents(const VHDLComponent &srcComponent) const;

  void setName(std::string);
  void setOperatorFreq(int freq);
  void refreshComponentMap();

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
        {"fp_floor", 1}, {"int_max", 1}, {"int_min", 1}, {"fp_max", 1},
        {"fp_min", 1}, {"fp_abs", 1}, {"select2", 1},
        // {"select3", 1},
        {"attach", 1}, {"int_abs", 1}, {"vbargraph", 1}}},
      {125,
       {{"fp_add", 3}, {"fp_prod", 1}, {"fp_div", 8}, {"fp_sqrt", 5},
        {"fp_diff", 3}, {"fp_pow", 8}, {"int_add", 1}, {"int_prod", 1},
        {"int_diff", 1}, {"float2int", 1}, {"int2float", 1},
        // NOTE unimplemented operators from here:
        {"fp_floor", 1}, {"int_max", 1}, {"int_min", 1}, {"fp_max", 1},
        {"fp_min", 1}, {"fp_abs", 1}, {"select2", 1},
        // {"select3", 1},
        {"attach", 1}, {"int_abs", 1}, {"vbargraph", 1}}},
      {250,
       {{"fp_add", 6}, {"fp_prod", 1}, {"fp_div", 18}, {"fp_sqrt", 10},
        {"fp_diff", 6}, {"fp_pow", 18}, {"int_add", 1}, {"int_prod", 1},
        {"int_diff", 1}, {"float2int", 2}, {"int2float", 3},
        // NOTE unimplemented operators from here:
        {"fp_floor", 1}, {"int_max", 1}, {"int_min", 1}, {"fp_max", 1},
        {"fp_min", 1}, {"fp_abs", 1}, {"select2", 1},
        // {"select3", 1},
        {"attach", 1}, {"int_abs", 1}, {"vbargraph", 1}}}
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
                                                            {"int_min", "int_min"},
                                                            {"fp_max", "fp_max"},
                                                            {"fp_min", "fp_min"},
                                                            {"fp_abs", "fp_abs"},
                                                            {"select2", "select2"},
                                                            // {"select3", "select3"},
                                                            {"attach", "attach"},
                                                            {"delay", "delay"}, // NOTE delay doesn't have a fixed lifespan, which is why it's not in the operatorLifespan map
                                                            {"int_abs", "int_abs"},
                                                            {"vbargraph", "vbargraph"}
  };

}; // End of VHDLCircuit
#endif /* VHDL_CIRCUIT_H_ */
