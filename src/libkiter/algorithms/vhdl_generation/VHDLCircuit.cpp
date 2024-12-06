/*
 * VHDLCircuit.cpp
 *
 *  Created on: 16 June 2021
 *      Author: jkmingwen
 */

#include <models/Dataflow.h>
#include <string>
#include "VHDLCircuit.h"
#include "algorithms/vhdl_generation/VHDLCommons.h"

VHDLCircuit::VHDLCircuit() {}

VHDLCircuit::VHDLCircuit(implType t) {
  implementationType = t;
}

/**
   Insert a component into the circuit. Components with mixed input types will trigger a scan
   of the current components in the circuit to ensure that the right data types are provided.

   @param newComp VHDLComponent to add to the circuit.
 */
void VHDLCircuit::addComponent(VHDLComponent newComp) {
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

// Assign start times to component in component map with matching name
// Needed to use this roundabout method to modify component as getComponentMap is const
void VHDLCircuit::setCompStartTime(std::string name,
                                   std::vector<TIME_UNIT> times,
                                   std::vector<TIME_UNIT> popTime, // for buffers
                                   TIME_UNIT slack) {
  for (auto &[v, comp] : this->componentMap) {
    if (name == comp.getUniqueName()) {
      comp.setStartTimes(times, popTime, slack);
    }
  }
}

void VHDLCircuit::bypassBufferComponent(std::string name) {
  for (auto &[v, comp] : this->componentMap) {
    if (name == comp.getUniqueName()) {
      std::string port;
      if (comp.getType() == "shiftreg") {
        port = "depth";
      } else {
        port = "buffer_size";
      }
      comp.addPortMapping(port, std::to_string(0), "integer", "", true);
    }
  }
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
  implRefName = newName;
  portMapName = implRefName;
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
void VHDLCircuit::updateTopLevelPorts(implType t) {
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
            this->addInputPort(edgeName, signalNames);
            if (t == TT) {
              this->topLevelPorts[edgeName] = signalNames[2];
            } else { // need to account for HS interface even for top-level ports
              this->topLevelPorts[edgeName + "_VALID"] = signalNames[0];
              this->topLevelPorts[edgeName + "_READY"] = signalNames[1];
              this->topLevelPorts[edgeName + "_DATA"] = signalNames[2];
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
            this->addOutputPort(edgeName, signalNames);
            if (t == TT) {
              this->topLevelPorts[edgeName] = signalNames[2];
            } else { // need to account for HS interface even for top-level ports
              this->topLevelPorts[edgeName + "_VALID"] = signalNames[0];
              this->topLevelPorts[edgeName + "_READY"] = signalNames[1];
              this->topLevelPorts[edgeName + "_DATA"] = signalNames[2];
            }
          }
        }
      }
    }
  }

}

void VHDLCircuit::portMappingInit() {
  addGenericMapping("ram_width", "34", "natural", 34, "34");
  addGenericMapping("ram_depth", "2", "natural", 2, "2");
  addPortMapping("clk", "sys_clk_sig", "std_logic", "in");
  addPortMapping("rst", "rst_sig", "std_logic", "in");
  if (implementationType == TT) {
    addPortMapping("cycle_count", "counter_sig", "integer", "in");
    for (auto i = 0; i < getOperatorCount("INPUT"); i++) {
      std::string portName = graphName + "_in_data_" + std::to_string(i);
      addPortMapping(portName, portName, "std_logic_vector", "in");
    }
    for (auto o = 0; o < getOperatorCount("OUTPUT"); o++) {
      std::string portName = graphName + "_out_data_" + std::to_string(o);
      addPortMapping(portName, portName, "std_logic_vector", "out");
    }
  } else if (implementationType == DD) {
    std::string portName = graphName;
    for (auto i = 0; i < getOperatorCount("INPUT"); i++) {
      std::string inId = std::to_string(i);
      std::string rdyPort = portName + "_in_ready_" + inId;
      std::string vldPort = portName + "_in_valid_" + inId;
      std::string dataPort = portName + "_in_data_" + inId;
      std::string inConvName = "i2s_to_fpc_" + inId;
      std::string rdySig = rdyPort + "_" + inConvName + "_op_out_ready_0";
      std::string vldSig = inConvName + "_op_out_valid_0";
      std::string dataSig = inConvName + "_op_out_data_0";
      addPortMapping(rdyPort, rdySig, "std_logic", "out");
      addPortMapping(vldPort, vldSig, "std_logic", "in");
      addPortMapping(dataPort, dataSig, "std_logic_vector", "in");
    }
    for (auto o = 0; o < getOperatorCount("OUTPUT"); o++) {
      std::string outId = std::to_string(o);
      std::string rdyPort = portName + "_out_ready_" + outId;
      std::string vldPort = portName + "_out_valid_" + outId;
      std::string dataPort = portName + "_out_data_" + outId;
      std::string outConvName = "fpc_to_i2s_" + outId;
      std::string rdySig = outConvName + "_op_in_ready_0";
      std::string vldSig = vldPort + "_" + outConvName + "_op_in_valid_0";
      std::string dataSig = dataPort + "_" + outConvName + "_op_in_data_0";
      addPortMapping(rdyPort, rdySig, "std_logic", "in");
      addPortMapping(vldPort, vldSig, "std_logic", "out");
      addPortMapping(dataPort, dataSig, "std_logic_vector", "out");
    }
  } else {
    VERBOSE_ERROR("Implementation type " << implementationType << " not supported");
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

std::string VHDLCircuit::genBypassMapping(implType t) const {
  std::stringstream codeOut;

  for (auto const &[v, comp] : componentMap) {
    if (comp.getType() == "INPUT") {
      int inId = comp.getIOId();
      for (auto const &[vTarget, compTarget] : componentMap) {
        if (compTarget.getType() == "OUTPUT") {
          if (this->getConnectionNameFromComponents(comp.getUniqueName(),
                                                    compTarget.getUniqueName()).size()) {
            int outId = compTarget.getIOId();
            if (t == TT) {
              codeOut << this->getName() << "_out_data_" << outId
                      << " <= " << this->getName() << "_in_data_" << inId << ";"
                      << std::endl;
            } else if (t == DD) {
              codeOut << this->getName() << "_out_data_" << outId
                      << " <= " << this->getName() << "_in_data_" << inId << ";"
                      << std::endl;
              codeOut << this->getName() << "_out_valid_" << outId
                      << " <= " << this->getName() << "_in_valid_" << inId
                      << ";" << std::endl;
              codeOut << this->getName() << "_in_ready_" << outId
                      << " <= " << this->getName() << "_out_ready_" << inId << ";"
                      << std::endl;
            } else {
              VERBOSE_ERROR("Implementation type " << t << " is not supported.");
            }
          }
        }
      }
    }
  }

  return codeOut.str();
}

void VHDLCircuit::writeImplementation(std::ofstream &vhdlOutput) {
  // account for case where Faust program has no only inputs/outputs
  bool noOperators = operatorMap.size() == 2 && operatorMap.count("INPUT") &&
                     operatorMap.count("OUTPUT");
  // VHDL header
  vhdlOutput << "library ieee;\n"
             << "use ieee.std_logic_1164.all;\n"
             << "use ieee.numeric_std.all;\n"
             << std::endl;

  // Initialise top level ports (necessary before running genEntityDecl)
  portMappingInit();
  vhdlOutput << genEntityDecl() << std::endl;

  // House constituent components of circuit
  // 1. Instantiate components for each operator in circuit
  vhdlOutput << "architecture behaviour of " << this->getName() << " is\n"
             << std::endl;
  if (!noOperators) { // only generate components if there are operators
    std::map<std::string, int> trackDeclarations; // only need 1 declaration per component type so use this to check if component has been declared
    for (auto const &[v, comp] : componentMap) {
      if (comp.getType() != "INPUT" && comp.getType() != "OUTPUT") {
        std::string name = comp.getPortMapName();
        if (!trackDeclarations.count(name)) {
          trackDeclarations[name] = 1;
          vhdlOutput << comp.genDeclaration() << std::endl;
        }
      }
    }
  }

  // 2. Generate intermediate signal names
  std::map<std::string, std::vector<std::string>> signalNames;
  for (auto const &[e, conn] : connectionMap) {
    std::map<std::string, std::vector<std::string>> newNames;
    VERBOSE_INFO("Generate signal name for " << conn.getName());
    // if (!(circuit.getSrcComponent(conn).getType() == "INPUT" ||
    //       circuit.getDstComponent(conn).getType() == "OUTPUT")) {
    //   // Check the implementation types of both sources and destination to allow
    //   // for future implementations with mixed implementations
    //   if (circuit.getSrcComponent(conn).getImplType() == TT &&
    //       circuit.getDstComponent(conn).getImplType() == TT) {
    //     VERBOSE_INFO("\tGenerating signal names for " << conn.getName());
    //     newNames = conn.genSignalNames(TT);
    //   } else if (circuit.getSrcComponent(conn).getImplType() == DD &&
    //              circuit.getDstComponent(conn).getImplType() == DD) {
    //     newNames = conn.genSignalNames(DD);
    //   } else {
    //     VERBOSE_ERROR("Implementation type for signal name generation not yet supported");
    //   }
    //   for (auto const &[type, name] : newNames) {
    //     signalNames[type].insert(signalNames[type].end(), name.begin(),
    //                              name.end());
    //   }
    // }

    // NOTE this is a workaround --- previously we'd check for the types of the
    // src/dst components of each connection so we know if its necessary to
    // generate signals for them (input/output connections don't require
    // internal signals), but this leads to a seg fault when trying to generate
    // for matrix.dsp. This workaround means that we'll always have excess
    // signals, but shouldn't affect resource utilization since they're never
    // used. The previous implementation was pretty unelegant, so it might be
    // worth trying to modify this instead.
    newNames = conn.genSignalNames(implementationType);
    for (auto const &[type, name] : newNames) {
        signalNames[type].insert(signalNames[type].end(), name.begin(),
                                 name.end());
      }
  }

  // 2a. Write signal names to VHDL output
  for (auto const &[type, sigNames] : signalNames) {
    std::string delim = ",\n";
    vhdlOutput << "signal ";
    for (auto const &name : sigNames) {
      if (name == sigNames.back()) { delim = ""; }
      vhdlOutput << name << delim;
    }
    vhdlOutput << " : " << type << ";\n" << std::endl;
  }

    // 3. Generate port mapping
  vhdlOutput << "begin\n" << std::endl;
  std::map<std::string, int> opCounts; // track counts of operators for instantiation in port mapping
  std::map<std::string, int> bufferCounts;
  std::map<std::string, std::string> replacementSigs = this->getTopLevelPorts();

  for (auto &[v, comp] : componentMap) {
    if (comp.getType() != "INPUT" && comp.getType() != "OUTPUT") {
      std::string opName = comp.getPortMapName();
      if (opCounts.count(opName)) {
        opCounts[opName]++;
      } else {
        opCounts[opName] = 0;
      }
      vhdlOutput << comp.genPortMapping(opCounts[opName], replacementSigs) << std::endl;
    }
  }

  // Edge case where there are only input/outputs
  if (noOperators) {
    bool routeInToOut = true;
    for (auto const &[e, conn] : connectionMap) {
      if (conn.getInitialTokenCount()) {
        routeInToOut = false;
        break;
      }
    }
    if (routeInToOut) {
      vhdlOutput << genBypassMapping(implementationType);
    }
  }

  vhdlOutput << "end behaviour;" << std::endl;

  vhdlOutput.close();
}

void VHDLCircuit::addComputeTime(int id, TIME_UNIT time) {
  this->computeTimes[id] = time;
}
