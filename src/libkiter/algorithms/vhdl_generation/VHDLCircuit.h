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
#include "VHDLCommons.h"

namespace models {
  class Dataflow;
}

class VHDLCircuit : public VHDLComponent {
 public:
  VHDLCircuit();
  VHDLCircuit(implType t);

  void addComponent(VHDLComponent newComp);
  void addConnection(VHDLConnection newConnect);
  void addInputPort(std::string portName, const std::vector<std::string> &signalNames);
  void addOutputPort(std::string portName, const std::vector<std::string> &signalNames);
  void setCompStartTime(std::string name, std::vector<TIME_UNIT> times,
                        std::vector<TIME_UNIT> popTime, TIME_UNIT slack = 0);
  void bypassBufferComponent(std::string name);

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

    const std::map<std::string, std::string> &getTopLevelPorts() const {
      return this->topLevelPorts;
    }

  const std::map<int, TIME_UNIT> &getComputeTimes() const {
      return this->computeTimes;
    }

  int getOperatorCount(const std::string &op) const;
  const VHDLComponent&  getFirstComponentByType(const std::string &op) const;

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
  std::string genBypassMapping(implType t) const;
  void setName(std::string);
  void refreshComponentMap();
  void updateTopLevelPorts(implType t);
  void portMappingInit();
  std::vector<std::string> generateDataSignalNames();
  std::vector<std::string> generateValidReadySignalNames();
  std::vector<std::string> generateHSSignalNames(std::string &name, bool isInputSig) const;
  std::string generateSignalNames(std::string &name, bool isInputSig) const;
  void addComputeTime(int id, TIME_UNIT time);
  void writeImplementation(std::ofstream &vhdlOutput);

 private:
  std::map<Vertex, VHDLComponent> componentMap;
  std::map<Edge, VHDLConnection> connectionMap;
  std::map<std::string, int> operatorMap; // key: operator type, value: occurances
  std::string graphName;
  // ports of top-level input and output signals
  std::map<std::string, std::vector<std::string>> inputPorts;
  std::map<std::string, std::vector<std::string>> outputPorts;
  std::map<std::string, std::string> topLevelPorts;
  std::map<int, TIME_UNIT> computeTimes;

}; // End of VHDLCircuit
#endif /* VHDL_CIRCUIT_H_ */
