/*
 * VHDLCircuit.cpp
 *
 *  Created on: 16 June 2021
 *      Author: jkmingwen
 */

#include <models/Dataflow.h>
#include "VHDLCircuit.h"

VHDLCircuit::VHDLCircuit() {}

/**
   Insert a component into the circuit. Components with mixed input types will trigger a scan
   of the current components in the circuit to ensure that the right data types are provided.

   @param newComp VHDLComponent to add to the circuit.
 */
void VHDLCircuit::addComponent(VHDLComponent newComp) {
  newComp.setImplementationName(this->getOperatorImplementationName(newComp.getType()));
  this->componentMap.insert(std::make_pair(newComp.getActor(), newComp));
  // keep track of counts of operator types present in circuit
  if (this->operatorMap.count(newComp.getType())) {
    this->operatorMap[newComp.getType()]++;
  } else {
    this->operatorMap[newComp.getType()] = 1; // prevent count value initialising to anything other than 1
  }
  if (newComp.hasMixedType()) {
    this->refreshComponentMap();
  }
}

void VHDLCircuit::addInputPort(std::string portName,
                               const std::vector<std::string>& signalNames) {
  this->inputPorts.insert(std::make_pair(portName, signalNames));
}

void VHDLCircuit::addOutputPort(std::string portName,
                                const std::vector<std::string>& signalNames) {
  this->outputPorts.insert(std::make_pair(portName, signalNames));
}

int VHDLCircuit::getOperatorCount(const std::string &op) const {
    const auto &op_map = this->getOperatorMap();
    auto it = op_map.find(op);
    if   (it == op_map.end()) {
      VERBOSE_WARNING("The operator '" << op << "' is not inside the operator map of the VHDLCircuit.");
      return 0;
     }
    return it->second;
}

// iterate through component map and return first instance of component matching type from argument
const VHDLComponent& VHDLCircuit::getFirstComponentByType(const std::string &op) const {
  for (auto &comp : this->componentMap) {
    if (comp.second.getType() == op) {
      return comp.second;
    }
  }
  // if component of matching type not found
  std::cout << "No component of matching type found" << std::endl;
  // TODO return null VHDL component if nothing found
}

std::string VHDLCircuit::getOperatorImplementationName(const std::string &opType) const {
  if (this->implementationNames.count(opType)) {
    return this->implementationNames.at(opType);
  } else {
    VERBOSE_WARNING("No implementation name listed for " << opType
                    << ", check implementationNames in VHDLCircuit.h");
    return "UNIMPLEMENTED_OPERATOR_" + opType;
  }
}

// return input counts for each occurance of the given operator
std::map<int, int> VHDLCircuit::getNumInputs(const std::string &opType) const {
  std::map<int, int> inputCounts; // number of inputs, and their occurances
  for (auto &comp : this->componentMap) {
    if (comp.second.getType() == opType) {
      inputCounts[(comp.second.getInputPorts()).size()]++;
    }
  }

  return inputCounts;
}

// return output counts for each occurance of the given operator
std::map<int, int> VHDLCircuit::getNumOutputs(const std::string &opType) const {
  std::map<int, int> outputCounts; // number of outputs, and their occurances
  // std::vector<int> outputCounts;
  for (auto &comp : this->componentMap) {
    if (comp.second.getType() == opType) {
      outputCounts[(comp.second.getOutputPorts()).size()]++;
    }
  }
  // assert(outputCounts.size() == this->getOperatorCount(opType));
  return outputCounts;
}

// return name of the channel between two components
std::vector<std::string> VHDLCircuit::getConnectionNameFromComponents(const std::string& srcActorName,
                                                                      const std::string& dstActorName) const {
  std::vector<std::string> srcOutputEdges;
  std::vector<std::string> dstInputEdges;
  std::vector<std::string> connNames;

  for (auto& comp : this->componentMap) {
    if (comp.second.getUniqueName() == srcActorName) {
      srcOutputEdges = comp.second.getOutputEdges();
    } else if (comp.second.getUniqueName() == dstActorName) {
      dstInputEdges = comp.second.getInputEdges();
    }
  }
  std::vector<std::string> originalOrder = dstInputEdges; // save original ordering of input arguments
  std::sort(srcOutputEdges.begin(), srcOutputEdges.end()); // need to sort vectors for set_intersection to work correctly
  std::sort(dstInputEdges.begin(), dstInputEdges.end());
  std::vector<std::string>::iterator it;
  std::vector<std::string> matchingNames(std::max(srcOutputEdges.size(), dstInputEdges.size()));
  it = std::set_intersection(srcOutputEdges.begin(), srcOutputEdges.end(),
                             dstInputEdges.begin(), dstInputEdges.end(),
                             matchingNames.begin());
  matchingNames.resize(it - matchingNames.begin());
  for (auto i : originalOrder) { // restore original ordering of input signals; necessary when multiple input signals come from the same actor
    if (std::find(matchingNames.begin(), matchingNames.end(), i) != matchingNames.end()) {
      connNames.push_back(i);
    }
  }

  return connNames;
}

// return destination port of connection between two components
std::vector<std::string> VHDLCircuit::getDstPortBetweenComponents(const std::string &srcActorName,
                                                                  const std::string &dstActorName) const {
  const std::vector<std::string>& connNames = this->getConnectionNameFromComponents(srcActorName, dstActorName);
  std::vector<std::string> portNames;
  for (auto &name : connNames) {
    for (auto &conn : this->connectionMap) {
      if (conn.second.getName() == name) {
        portNames.push_back(conn.second.getDstPort());
      }
    }
  }

  return portNames;
}

// Looks for the component that starts with the given substring
// and returns its full name
// (helper function for 'getConnectionNameFromComponents').
// Naming convention of 'actor_arg1_arg2' for binary operators
// means that arg1 and arg2 would only contain the 'actor' part
// of the naming convention.
std::string VHDLCircuit::getComponentFullName(const std::string &partialName) const {
  std::vector<std::string> matchingNames;
  for (auto& comp : this->componentMap) {
    std::size_t found = comp.second.getUniqueName().find(partialName + "_"); // BRUNO Edit: Workaround to make sure it is really its name
    if ((found == 0) or (comp.second.getUniqueName() == partialName)) {
      matchingNames.push_back(comp.second.getUniqueName());
    }
  }

    VERBOSE_DEBUG("getComponentFullName '" << partialName << "' returns" << "'" << matchingNames[0] << "'");

  if(matchingNames.size() != 1) {
      VERBOSE_ERROR("Wrong name count for '" << partialName << "': " << commons::toString(matchingNames));
  }
  assert(matchingNames.size() == 1);
  return matchingNames.front();
}

void VHDLCircuit::setName(std::string newName) {
  this->graphName = newName;
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
  outputStream << "Operator, count (and lifespan):" << std::endl;
  for (auto &op : this->operatorMap) {
    outputStream << "\t" << op.first << ", "
                 << op.second // << " (" << this->getOperatorLifespan(op.first) // TODO add operator lifespan if possible and relevant
                 << ")\n";
    // if (op.first == "Proj" || op.first == "const_value") {
      outputStream << "\t\tOutput counts, occurances:" << std::endl;
      std::map<int, int> outputCounts = this->getNumOutputs(op.first);
      for (auto &out : outputCounts) {
        outputStream << "\t\t  " << out.first << ", " << out.second << std::endl;
      }
    // }
  }
  outputStream << std::endl;
  outputStream << "Top-level input port names:" << std::endl;
  for (auto &port : this->getInputPorts()) {
    outputStream << "\t" << port.first << std::endl;
  }
  outputStream << "Top-level output port names:" << std::endl;
  for (auto &port : this->getOutputPorts()) {
    outputStream << "\t" << port.first << std::endl;
  }

  return outputStream.str();
}

// return list of actor names who have more than their expected number of outputs
// this is for use in generating a bash script to distribute these outputs into Proj operators
std::vector<std::string> VHDLCircuit::getMultiOutActors() const {
  std::vector<std::string> actorNames;
  for (auto &comp : this->componentMap) {
    if ((comp.second).getType() != "Proj" && !(comp.second).isConst() &&
        (comp.second).getType() != "output_selector") {
      if ((comp.second).getOutputEdges().size() > 1) { // if operator has more than one output
        actorNames.push_back((comp.second).getUniqueName());
      }
    }
  }
  return actorNames;
}

// return destination VHDLComponent of the given VHDLConnection
VHDLComponent VHDLCircuit::getDstComponent(const VHDLConnection &conn) const {
  VERBOSE_ASSERT(this->componentMap.size(), "Component map is empty, populate component map before searching for components");
  for (auto &comp : this->componentMap) {
    for (auto &inEdge : comp.second.getInputEdges()) {
      if (conn.getName() == inEdge) {
        return comp.second;
      }
    }
  }
  VERBOSE_ERROR("Connection " << conn.getName() << " has no destination component");
}

// return source VHDLComponent of the given VHDLConnection
VHDLComponent VHDLCircuit::getSrcComponent(const VHDLConnection &conn) const {
  VERBOSE_ASSERT(this->componentMap.size(), "Component map is empty, populate component map before searching for components");
  for (auto &comp : this->componentMap) {
    for (auto &outEdge : comp.second.getOutputEdges()) {
      if (conn.getName() == outEdge) {
        return comp.second;
      }
    }
  }
  VERBOSE_ERROR("Connection " << conn.getName() << " has no destination component");
}

std::vector<VHDLComponent> VHDLCircuit::getDstComponents(const VHDLComponent &srcComponent) const {
  VERBOSE_ASSERT(this->componentMap.size(), "Component map is empty, populate component map before searching for components");
  std::vector<VHDLComponent> dstComponents;
  const std::vector<std::string>& srcOutEdges = srcComponent.getOutputEdges();
  for (auto &comp : this->componentMap) {
    for (auto &inEdge : comp.second.getInputEdges()) {
      if (std::find(srcOutEdges.begin(), srcOutEdges.end(), inEdge) != srcOutEdges.end()) {
        dstComponents.push_back(comp.second);
        break; // just need to identify one matching edge to determine destination component
      }
    }
  }

  return dstComponents;
}

std::vector<VHDLComponent> VHDLCircuit::getSrcComponents(const VHDLComponent &dstComponent) const {
  VERBOSE_ASSERT(this->componentMap.size(), "Component map is empty, populate component map before searching for components");
  std::vector<VHDLComponent> srcComponents;
  const std::vector<std::string>& srcInEdges = dstComponent.getInputEdges();
  for (auto &comp : this->componentMap) {
    for (auto &inEdge : comp.second.getInputEdges()) {
      if (std::find(srcInEdges.begin(), srcInEdges.end(), inEdge) != srcInEdges.end()) {
        srcComponents.push_back(comp.second);
        break; // just need to identify one matching edge to determine source component
      }
    }
  }

  return srcComponents;
}

void VHDLCircuit::refreshComponentMap() {
  for (auto &dstComp : this->componentMap) {
    if (dstComp.second.hasMixedType() && dstComp.second.getDataType() == "fp") {
      // convert any input args of integer constant values to floating point
      const std::vector<std::string>& dstInEdges = dstComp.second.getInputEdges();
      for (auto &srcComp : this->componentMap) {
        for (auto &outEdge : srcComp.second.getOutputEdges()) {
          if (std::find(dstInEdges.begin(), dstInEdges.end(), outEdge) != dstInEdges.end()) {
            if (srcComp.second.isConst() && srcComp.second.getDataType() == "int") {
              srcComp.second.setFPValue(static_cast<float>(srcComp.second.getIntValue()));
              srcComp.second.setDataType("fp");
            }
          }
        }
      }
    }
  }
}

// Instantiate top level input/output port names and the corresponding signals
// they use
// To be called only after instantiating component and connection maps
void VHDLCircuit::updateTopLevelPorts() {
  std::map<int, std::string> inSignalNames;
  std::map<int, std::string> outSignalNames;
  // TODO check that component and connection maps have been instantiated
  // TODO check that input and output ports only have 1 output/input edge/port
  for (auto const &[v, comp] : this->getComponentMap()) {
    if (comp.getType() == "INPUT") {
      std::vector<std::string> signalNames(3);
      // 0: VALID, 1: READY, 2: DATA
      signalNames[0] = this->getName() + "_in_valid_" + std::to_string(comp.getIOId());
      signalNames[1] = this->getName() + "_in_ready_" + std::to_string(comp.getIOId());
      signalNames[2] = this->getName() + "_in_data_" + std::to_string(comp.getIOId());
      for (auto const &edgeName : comp.getOutputEdges()) {
        for (auto const &[e, conn] : this->getConnectionMap()) {
          if (conn.getName() == edgeName) {
            if (conn.getInitialTokenCount()) {
              this->addInputPort(conn.getSrcPort(), signalNames);
            } else {
              this->addInputPort(edgeName, signalNames);
            }
          }
        }
      }
    } else if (comp.getType() == "OUTPUT") {
      std::vector<std::string> signalNames(3);
      // 0: VALID, 1: READY, 2: DATA
      signalNames[0] = this->getName() + "_out_valid_" + std::to_string(comp.getIOId());
      signalNames[1] = this->getName() + "_out_ready_" + std::to_string(comp.getIOId());
      signalNames[2] = this->getName() + "_out_data_" + std::to_string(comp.getIOId());
      for (auto const &edgeName : comp.getInputEdges()) {
        for (auto const &[e, conn] : this->getConnectionMap()) {
          if (conn.getName() == edgeName) {
            if (conn.getInitialTokenCount()) {
              this->addOutputPort(conn.getDstPort(), signalNames);
            } else {
              this->addOutputPort(edgeName, signalNames);
            }
          }
        }
      }
    }
  }
}

std::vector<std::string> VHDLCircuit::generateDataSignalNames() {
  std::vector<std::string> signalNames;
  for (auto const &[e, conn] : this->getConnectionMap()) {
    // Connections directly between input and output have no intermediate signals
    if (conn.getInitialTokenCount()) {
      // 2 sets of signals to connect with FIFO buffer inbetween
      if (this->getSrcComponent(conn).getType() != "INPUT") {
        signalNames.push_back(conn.getSrcPort() + "_DATA");
      }
      if (this->getDstComponent(conn).getType() != "OUTPUT") {
        signalNames.push_back(conn.getDstPort() + "_DATA");
      }
    } else {
      if (!(this->getSrcComponent(conn).getType() == "INPUT" ||
            this->getDstComponent(conn).getType() == "OUTPUT")) {
        signalNames.push_back(conn.getName() + "_DATA");
      }
    }
  }

  return signalNames;
}

std::vector<std::string> VHDLCircuit::generateValidReadySignalNames() {
  std::vector<std::string> signalNames;
  for (auto const &[e, conn] : this->getConnectionMap()) {
    // Connections directly between input and output have no intermediate signals

    if (conn.getInitialTokenCount()) {
      // 2 sets of signals to connect with FIFO buffer inbetween
      if (this->getSrcComponent(conn).getType() != "INPUT") {
        signalNames.push_back(conn.getSrcPort() + "_VALID");
        signalNames.push_back(conn.getSrcPort() + "_READY");
      }
      if (this->getDstComponent(conn).getType() != "OUTPUT") {
        signalNames.push_back(conn.getDstPort() + "_VALID");
        signalNames.push_back(conn.getDstPort() + "_READY");
      }
    } else {
      if (!(this->getSrcComponent(conn).getType() == "INPUT" ||
            this->getDstComponent(conn).getType() == "OUTPUT")) {
        signalNames.push_back(conn.getName() + "_VALID");
        signalNames.push_back(conn.getName() + "_READY");
      }
    }
  }

  return signalNames;
}

std::vector<std::string> VHDLCircuit::generateHSSignalNames(std::string &name,
                                                            bool isInputSig) const {
  std::vector<std::string> signalNames(3);
  std::vector<std::string> topLevelPorts;
  if (isInputSig && this->getInputPorts().count(name)) {
    signalNames = this->inputPorts.at(name);
  } else if (!isInputSig && this->getOutputPorts().count(name)) {
    signalNames = this->outputPorts.at(name);
  } else {
    signalNames[0] = name + "_VALID";
    signalNames[1] = name + "_READY";
    signalNames[2] = name + "_DATA";
  }

  return signalNames;
}

// Replace signal name with top-level port when an input/output port encountered
// TODO don't hard code the iterator to the data signal name
std::string VHDLCircuit::generateSignalNames(std::string &name,
                                             bool isInputSig) const {
  std::string signalName;
  if (isInputSig && this->getInputPorts().count(name)) {
    signalName = this->inputPorts.at(name)[2];
  } else if (!isInputSig && this->getOutputPorts().count(name)) {
    signalName = this->outputPorts.at(name)[2];
  } else {
    signalName = name + "_DATA";
  }

  return signalName;
}
