/*
 * VHDLCircuit.cpp
 *
 *  Created on: 16 June 2021
 *      Author: jkmingwen
 */

#include <models/Dataflow.h>
#include "VHDLCircuit.h"

VHDLCircuit::VHDLCircuit() {}

void VHDLCircuit::addComponent(VHDLComponent newComp) {
  newComp.setLifespan(this->getOperatorLifespan(newComp.getType()));
  newComp.setImplementationName(this->getOperatorImplementationName(newComp.getType()));
  this->componentMap.insert(std::make_pair(newComp.getActor(),
                                           newComp));
  this->operatorMap[newComp.getType()]++; // track operators used in circuit
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
  return this->getOperatorMap().at(op);
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


int VHDLCircuit::getOperatorLifespan(const std::string &opType) const {
  if (this->operatorLifespans.at(this->operatorFreq).count(opType)) {
    return this->operatorLifespans.at(this->operatorFreq).at(opType);
  } else {
    // NOTE default to 0 if lifespan not specified
    return 0;
  }
}

std::string VHDLCircuit::getOperatorImplementationName(const std::string &opType) const{
  if (this->implementationNames.count(opType)) {
    return this->implementationNames.at(opType);
  } else {
    VERBOSE_WARNING("No implementation name listed for " << opType
                    << ", check implemenatationNames in VHDLCircuit.h");
    return "UNIMPLEMENTED_OPERATOR_" + opType;
  }
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
    if (comp.second.getName() == srcActorName) {
      srcOutputEdges = comp.second.getOutputEdges();
    } else if (comp.second.getName() == dstActorName) {
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
    std::size_t found = comp.second.getName().find(partialName + "_"); // BRUNO Edit: Workaround to make sure it is really its name
    if ((found == 0) or (comp.second.getName() == partialName)) {
      matchingNames.push_back(comp.second.getName());
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

void VHDLCircuit::setOperatorFreq(int freq) {
  if (this->operatorLifespans.count(freq)) {
    this->operatorFreq = freq;
  } else {
    std::string supportedFrequencies;
    for (auto i : this->operatorLifespans) {
      supportedFrequencies += std::to_string(i.first) + "  ";
    }
    VERBOSE_ERROR("The selected operator frequency (" << freq
                  << ") is not supported. Supported frequencies: "
                  << supportedFrequencies);
  }
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
                 << op.second << " (" << this->getOperatorLifespan(op.first)
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
    if ((comp.second).getType() != "Proj" && !(comp.second).isConst()) {
      if ((comp.second).getOutputEdges().size() > 1) { // if operator has more than one output
        actorNames.push_back((comp.second).getName());
      }
    }
  }
  return actorNames;
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

// converts a constant value int type to float
void VHDLCircuit::convConstIntToFloat(Vertex v) {
  VERBOSE_ASSERT(this->componentMap.find(v) != this->componentMap.end(), "Specified constant value component doesn't exist in this circuit's component map");
  if (this->componentMap.at(v).getDataType() == "fp") {
    VERBOSE_WARNING("\t" << this->componentMap.at(v).getName() << " is already a float, bypassing conversion");
  } else {
    this->componentMap.at(v).convConstIntToFloat();
  }
}
