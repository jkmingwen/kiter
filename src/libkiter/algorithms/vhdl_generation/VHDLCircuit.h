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

  std::string getOperatorImplementationName(const std::string &opType) const;
  std::map<int, int> getNumInputs(const  std::string &opType) const;
  std::map<int, int> getNumOutputs(const  std::string &opType) const;
  std::vector<std::string> getConnectionNameFromComponents(const std::string &srcActorName,
                                                           const std::string &dstActorName) const;
  std::vector<std::string> getDstPortBetweenComponents(const std::string &srcActorName,
                                                       const std::string &dstActorName) const;
  std::string getComponentFullName(const std::string &partialName) const;
  std::vector<std::string> getMultiOutActors() const;
  VHDLComponent getDstComponent(const VHDLConnection &conn) const;
  VHDLComponent getSrcComponent(const VHDLConnection &conn) const;
  std::vector<VHDLComponent> getDstComponents(const VHDLComponent &srcComponent) const;
  std::vector<VHDLComponent> getSrcComponents(const VHDLComponent &dstComponent) const;

  void setName(std::string);
  void refreshComponentMap();
  void updateTopLevelPorts();
  std::vector<std::string> generateDataSignalNames();
  std::vector<std::string> generateValidReadySignalNames();
  std::vector<std::string> generateHSSignalNames(std::string &name, bool isInputSig) const;
  std::string generateSignalNames(std::string &name, bool isInputSig) const;

 private:
  std::map<Vertex, VHDLComponent> componentMap;
  std::map<Edge, VHDLConnection> connectionMap;
  std::map<std::string, int> operatorMap; // key: operator type, value: occurances
  std::string graphName;
  // ports of top-level input and output signals
  std::map<std::string, std::vector<std::string>> inputPorts;
  std::map<std::string, std::vector<std::string>> outputPorts;
  std::map<std::string, std::string> implementationNames = {
      {"fp_add", "fp_add_flopoco"},
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
      {"attach", "attach"},
      {"delay", "delay"}, // NOTE delay doesn't have a fixed lifespan, which is
                          // why it's not in the operatorLifespan map
      {"int_abs", "int_abs"},
      {"vbargraph", "vbargraph"},
      {"hbargraph", "hbargraph"},
      {"hslider", "hslider"},
      {"vslider", "vslider"},
      {"nentry", "nentry"},
      {"checkbox", "checkbox"},
      {"button", "button"},
      {"sbuffer", "sbuffer"}
  };

}; // End of VHDLCircuit
#endif /* VHDL_CIRCUIT_H_ */
