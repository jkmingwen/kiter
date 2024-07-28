/*
 * VHDLComponent.cpp
 *
 *  Created on: 15 June 2021
 *      Author: jkmingwen
 */

#include <bitset>
#include <models/Dataflow.h>
#include <sstream>
#include <string>
#include "VHDLComponent.h"

/**
 * \brief Each instance represents a vertex to be implemented as an
 * operator in VHDL.
 * \param dataflow Dataflow graph that vertex is located in.
 * Required to query properties of the vertex.
 * \param a Vertex to instantiate as a VHDLComponent.
 * \param implType Optional specification of VHDL implementation for given
component {TT (default), DD}
 * \param freq Operating frequency (in MHz) of given component {250 (default), 125, 50}
 */
VHDLComponent::VHDLComponent(models::Dataflow* const dataflow, Vertex a, implType t, int freq) {
  actor = a;
  implementationType = t;
  opFreq = freq;
  uniqueName = dataflow->getVertexName(a); // unique ID per VHDLComponent
  implementationName = "default"; // name of top-level component for the given operator in VHDL
  componentType = dataflow->getVertexType(a); // vertex type denotes computation performed (sans data type)
  isMixedType = false; // some components (e.g. pow) take in a mix of data types as arguments

  // port names denote input/output data types --- check and track these
  {ForInputEdges(dataflow, a, inEdge) {
      inputPorts.push_back(dataflow->getEdgeOutputPortName(inEdge));
      inputEdges.push_back(dataflow->getEdgeName(inEdge));
    }}
  {ForOutputEdges(dataflow, a, outEdge) {
      outputPorts.push_back(dataflow->getEdgeInputPortName(outEdge));
      outputEdges.push_back(dataflow->getEdgeName(outEdge));
    }}
  // for when we want to use a single signal to send output to multiple actors (not data driven)
  if (dataflow->getVertexOutDegree(a)) {
    this->sharedOutputSignal = dataflow->getEdgeName(it2Edge(dataflow->getOutputEdges(a).first));
  }
  for (const auto& port : inputPorts) {
    std::string portName = port;
    if (portName.substr(portName.find_last_of('_') + 1) == "vect") { // ignore "vect" to find the actual data type
      portName.erase(portName.find_last_of('_'), std::string::npos);
    }
    inputTypes[portName.substr(portName.find_last_of('_') + 1)]++;
  }
  for (const auto& port : outputPorts) {
    std::string portName = port;
    if (portName.substr(portName.find_last_of('_') + 1) == "vect") { // ignore "vect" to find the actual data type
      portName.erase(portName.find_last_of('_'), std::string::npos);
    }
    outputTypes[portName.substr(portName.find_last_of('_') + 1)]++;
  }

  // IDs of input/output actors used for signal matching in generateVHDLArchitecture
  if (componentType.find("INPUT") != std::string::npos ||
      componentType.find("OUTPUT") != std::string::npos) {
    ioId = std::stoi(componentType.substr(componentType.find('_') + 1, std::string::npos));
    componentType = componentType.substr(0, componentType.find("_")); // necessary to strip ID from name (e.g. "INPUT_0") so we can parse it as a generic INPUT/OUTPUT component
  }

  /* Identify order of arguments for operators.
     Order of arguments for operators are denoted by their name (e.g.
     compName_arg0_arg1_arg2). Proj actors generated by the singleOutput
     function have the same naming convention so ignore these as they only
     have 1 input. */
  if (componentType != "Proj") {
    std::string argFromName = uniqueName;
    size_t pos = argFromName.rfind('_');
    std::string argName;
    while ((pos = argFromName.rfind('_')) != std::string::npos) { // start from last argument
      argName = argFromName.substr(pos + 1);
      argOrder.insert(argOrder.begin(), argName); // retain correct order of arguments
      argFromName.erase(pos);
    }
  }

  // Rearrange input ports and edges according to argument order
  if (this->argOrder.size() && this->getType() != "INPUT" &&
      this->getType() != "OUTPUT") {
    for (auto const &inputVertexName : this->argOrder) {
      Vertex inputVertex = dataflow->getVertexByName(
          getNameFromPartialName(dataflow, inputVertexName));
      {ForInputEdges(dataflow, this->actor, e) {
          if (dataflow->getEdgeSource(e) == inputVertex) {
            Edge signal = e;
            if (implementationType == TT) {
              if (this->getType() != "input_selector") {
                // share input sigals: use the first output edge from the source
                // vertex to ensure that the outputs of that vertex use the same
                // signal even if it has multiple output edges
                signal = it2Edge(dataflow->getOutputEdges(inputVertex).first);
              }
            }
            this->addInputSignal(dataflow, signal);
          }
        }}
    }
    VERBOSE_ASSERT(this->getInputSignals().size() == this->argOrder.size(),
                   this->getUniqueName() << ": input ports after reordering << (" << this->getInputSignals().size() << ") != argOrder size (" << this->argOrder.size() << ")");
  } else {
    {ForInputEdges(dataflow, this->actor, e) {
        this->addInputSignal(dataflow, e);
      }}
    VERBOSE_ASSERT(this->getInputSignals().size() == this->inputEdges.size(),
                   this->getUniqueName() << ": input signals != input edges");
  }

  {ForOutputEdges(dataflow, this->actor, e) {
      this->addOutputSignal(dataflow, e);
    }}
  // Rearrange output ports and edges according to output selector phases of execution
  if (this->getType() == "output_selector") {
    int outEdgeCount = 0;
    {ForOutputEdges(dataflow, this->actor, e) {
        std::vector<TOKEN_UNIT> inputVector = dataflow->getEdgeInVector(e);
        if (inputVector.size() > 1) {
          for (int exec = 0; exec < inputVector.size(); exec++) {
            if (inputVector.at(exec) == 1) {
              std::string name = dataflow->getEdgeName(e);
              if (dataflow->getPreload(e)) {
                name = dataflow->getEdgeInputPortName(e);
              }
              VERBOSE_INFO("Edge for exec " << exec << ": " << name);
              this->outputSignals.at(exec) = name;
            }
          }
        } else { // output selector is the broadcasting type
          this->outputSignals.at(outEdgeCount) = dataflow->getEdgeName(e);
          outEdgeCount++;
        }
      }}
    VERBOSE_ASSERT(this->getOutputSignals().size() == this->outputEdges.size(),
                   this->getUniqueName() << ": Output signals after reordering != output edges(" << this->getOutputSignals().size() << " != " << this->outputEdges.size() << ")");
  }

  /* TODO identify data type of component to select appropriate VHDL implementation
     data type produced by constant value and UI components determine their data type
     all other types of operators use input types */
  std::istringstream stringAsNumber(componentType); // just to test if strings are numbers
  float compTypeAsFloat;
  stringAsNumber >> std::noskipws >> compTypeAsFloat;
  if (std::count(uiTypes.begin(), uiTypes.end(), componentType) ||
      (stringAsNumber.eof() && !stringAsNumber.fail())) {
    if (outputTypes.size() > 1 || !outputTypes.size()) {
      VERBOSE_ERROR("UI/Constant value component "
                    << uniqueName
                    << " should have just one output data type");
    }
    if (outputTypes.begin()->first == "real") {
      dataType = "fp";
    } else if (outputTypes.begin()->first == "int") {
      dataType = "int";
    } else {
      VERBOSE_ERROR("Unsupported data type: " << dataType);
    }
  } else {
    if (inputTypes.empty() && componentType != "INPUT") {
      VERBOSE_ERROR(uniqueName << " (" << componentType << ")"
                                  << " should have at least 1 input");
    }
    // determine data type via input ports
    bool isInt = false, isFloat = false;
    for (auto &type : inputTypes) {
      if (type.first == "real") {
        isFloat = true;
      } else if (type.first == "int") {
        isInt = true;
      } else {
        VERBOSE_ERROR("\tUndefined input type detected: " << type.first);
      }
    }
    if (isInt && isFloat) {
      VERBOSE_WARNING("\tBoth Int and Float types detected for "
                      << uniqueName << "(" << componentType
                      << "), setting type to floating point");
      dataType = "fp";
      isMixedType = true;
    } else if (isFloat) {
      dataType = "fp";
    } else if (isInt) {
      dataType = "int";
    }
  }

  // data type added to componentType to determine appropriate VHDL implementation to use
  if (std::count(arithmeticTypes.begin(), arithmeticTypes.end(), componentType) ||
      std::count(numOperatorTypes.begin(), numOperatorTypes.end(), componentType)) {
    componentType = dataType + "_" + componentType;
  } else if ((stringAsNumber.eof() && !stringAsNumber.fail())) { // string is a number
    componentType = "const_value";
    if (dataType == "fp") {
      fpValue = compTypeAsFloat;
      std::string prefix = (fpValue ? "01" : "00");
      std::stringstream binaryRep;
      size_t size = sizeof(fpValue);
      unsigned char *p = (unsigned char *)&fpValue;
      p += size - 1;
      while (size--) {
        for (int n = 0; n < 8; n++) {
          char bit = ('0' + (*p & 128 ? 1 : 0));
          binaryRep << bit;
          *p <<=1;
        }
        p--;
      }
      binaryValue = prefix + binaryRep.str();
    } else {
      intValue = (int)compTypeAsFloat;
      binaryValue = std::bitset<34>(intValue).to_string(); // unsigned binary representation
    }
  }

  // Assign instance name and mappings
  if (componentType == "const_value" || componentType == "output_selector") {
    portMapName = componentType + "_" + std::to_string(outputPorts.size());
    implRefName = portMapName;
  } else if (componentType == "input_selector") {
    portMapName = componentType + "_" + std::to_string(inputPorts.size());
    implRefName = portMapName;
  } else if (componentType == "sbuffer") {
    portMapName = componentType;
    implRefName = componentType;
  } else if (componentType == "Proj") {
    portMapName = componentType + "_" + std::to_string(outputPorts.size());
    implRefName = implementationNames[componentType] + "_" + std::to_string(outputPorts.size());
  } else {
    portMapName = componentType;
    implRefName = implementationNames[componentType] + "_f" + std::to_string(opFreq);
    if (implementationType == DD) {
      implRefName = componentType;
    }

  }

  // Generic and port mappings are mostly set here
  // except for exec time mappings - set in setStartTimes()
  if (componentType == "INPUT" || componentType == "OUTPUT") { // expressed as top level ports
    std::string portName = dataflow->getGraphName();
    if (implementationType == TT) {
      if (componentType == "INPUT") {
        portName += "_in_data_" + std::to_string(ioId);
        addPortMapping(portName, portName, "std_logic_vector", "in");
      } else { // top-level output
        portName += "_out_data_" + std::to_string(ioId);
        addPortMapping(portName, portName, "std_logic_vector", "out");
      }
    } else {
      if (componentType == "INPUT") {
        std::string rdyPort = portName + "_in_ready_" + std::to_string(ioId);
        std::string vldPort = portName + "_in_valid_" + std::to_string(ioId);
        std::string dataPort = portName + "_in_data_" + std::to_string(ioId);
        addPortMapping(rdyPort, rdyPort, "std_logic", "out");
        addPortMapping(vldPort, vldPort, "std_logic", "in");
        addPortMapping(dataPort, dataPort, "std_logic_vector", "in");
      } else { // top-level output
        std::string rdyPort = portName + "_out_ready_" + std::to_string(ioId);
        std::string vldPort = portName + "_out_valid_" + std::to_string(ioId);
        std::string dataPort = portName + "_out_data_" + std::to_string(ioId);
        addPortMapping(rdyPort, rdyPort, "std_logic", "in");
        addPortMapping(vldPort, vldPort, "std_logic", "out");
        addPortMapping(dataPort, dataPort, "std_logic_vector", "out");
      }
    }
  } else {
    addPortMapping("clk", "clk", "std_logic", "in");
    if (componentType == "const_value") {
      addPortMapping("rst", "rst", "std_logic", "in");
      addPortMapping("value", "\"" + binaryValue + "\"", "std_logic_vector", "",
                     true);
      for (auto o = 0; o < outputSignals.size(); o++) {
        if (implementationType == DD) { // additional ports for HS protocol
          addPortMapping("out_ready_" + std::to_string(o),
                         outputSignals[o] + "_READY", "std_logic", "in");
          addPortMapping("out_valid_" + std::to_string(o),
                         outputSignals[o] + "_VALID", "std_logic", "out");
          addPortMapping("out_data_" + std::to_string(o),
                         outputSignals[o] + "_DATA", "std_logic_vector", "out");
        } else {
          addPortMapping("out_data_" + std::to_string(o), outputSignals[o], "std_logic_vector", "out");
        }
      }
    } else if (componentType == "input_selector") {
      if (implementationType == TT) {
        addPortMapping("ram_width", "ram_width", "integer", "", true);
        addPortMapping("rst", "rst", "std_logic", "in");
        addPortMapping("cycle_count", "cycle_count", "integer", "in");
        for (auto i = 0; i < inputSignals.size(); i++) {
          addPortMapping("in_data_" + std::to_string(i), inputSignals[i], "std_logic_vector", "in");
        }
        for (auto o = 0; o < outputSignals.size(); o++) {
          addPortMapping("out_data_" + std::to_string(o), outputSignals[o], "std_logic_vector", "out");
        }
      } else {
        addPortMapping("num_phases", std::to_string(inputSignals.size()),
                       "integer", "", true);
        addPortMapping("rst", "rst", "std_logic", "in");
        for (auto i = 0; i < inputSignals.size(); i++) {
          addPortMapping("op_in_ready_" + std::to_string(i),
                         inputSignals[i] + "_READY", "std_logic", "out");
          addPortMapping("op_in_valid_" + std::to_string(i),
                         inputSignals[i] + "_VALID", "std_logic", "in");
          addPortMapping("op_in_data_" + std::to_string(i),
                         inputSignals[i] + "_DATA", "std_logic_vector", "in");
        }
        for (auto o = 0; o < outputSignals.size(); o++) {
          addPortMapping("op_out_ready_" + std::to_string(o),
                         outputSignals[o] + "_READY", "std_logic", "in");
          addPortMapping("op_out_valid_" + std::to_string(o),
                         outputSignals[o] + "_VALID", "std_logic", "out");
          addPortMapping("op_out_data_" + std::to_string(o),
                         outputSignals[o] + "_DATA", "std_logic_vector", "out");
        }
      }

    } else if (componentType == "output_selector") {
      if (implementationType == TT) {
        addPortMapping("ram_width", "ram_width", "integer", "", true);
        addPortMapping("rst", "rst", "std_logic", "in");
        addPortMapping("cycle_count", "cycle_count", "integer", "in");
        for (auto i = 0; i < inputSignals.size(); i++) {
          addPortMapping("in_data_" + std::to_string(i), inputSignals[i], "std_logic_vector", "in");
        }
        for (auto o = 0; o < outputSignals.size(); o++) {
          addPortMapping("out_data_" + std::to_string(o), outputSignals[o], "std_logic_vector", "out");
        }
      } else {
        addPortMapping("num_phases", std::to_string(outputSignals.size()),
                       "integer", "", true);
        addPortMapping("rst", "rst", "std_logic", "in");
        for (auto i = 0; i < inputSignals.size(); i++) {
          addPortMapping("op_in_ready_" + std::to_string(i),
                         inputSignals[i] + "_READY", "std_logic", "out");
          addPortMapping("op_in_valid_" + std::to_string(i),
                         inputSignals[i] + "_VALID", "std_logic", "in");
          addPortMapping("op_in_data_" + std::to_string(i),
                         inputSignals[i] + "_DATA", "std_logic_vector", "in");
        }
        for (auto o = 0; o < outputSignals.size(); o++) {
          addPortMapping("op_out_ready_" + std::to_string(o),
                         outputSignals[o] + "_READY", "std_logic", "in");
          addPortMapping("op_out_valid_" + std::to_string(o),
                         outputSignals[o] + "_VALID", "std_logic", "out");
          addPortMapping("op_out_data_" + std::to_string(o),
                         outputSignals[o] + "_DATA", "std_logic_vector", "out");
        }
      }
    } else if (componentType == "sbuffer") {
      addPortMapping("ram_width", "ram_width", "integer", "", true);
      addPortMapping("buffer_size",
                     std::to_string(this->getSBufferInitTokens() + 1), "integer",
                     "", true);
      addPortMapping("init", std::to_string(this->getSBufferInitTokens()),
                     "integer", "", true);
      addPortMapping("rst", "rst", "std_logic", "in");
      addPortMapping("cycle_count", "cycle_count", "integer", "in");
      for (auto i : inputSignals) {
        addPortMapping("in_data", i, "std_logic_vector", "in");
      }
      for (auto o : outputSignals) {
        addPortMapping("out_data", o, "std_logic_vector", "out");
      }
    } else { // FPC operators
      if (implementationType == DD) { // HS protocol requires reset
        if (componentType == "Proj") {
          addPortMapping("bit_width", "ram_width", "integer", "", true);
        }
        addPortMapping("rst", "rst", "std_logic", "in");
        for (auto i = 0; i < inputSignals.size(); i++) {
          if (implementationType == DD) { // additional ports for HS protocol
            addPortMapping("op_in_ready_" + std::to_string(i),
                           inputSignals[i] + "_READY", "std_logic", "out");
            addPortMapping("op_in_valid_" + std::to_string(i),
                           inputSignals[i] + "_VALID", "std_logic", "in");
            addPortMapping("op_in_data_" + std::to_string(i),
                           inputSignals[i] + "_DATA", "std_logic_vector", "in");
          }
        }
        for (auto o = 0; o < outputSignals.size(); o++) {
          if (implementationType == DD) { // additional ports for HS protocol
            addPortMapping("op_out_ready_" + std::to_string(o),
                           outputSignals[o] + "_READY", "std_logic", "in");
            addPortMapping("op_out_valid_" + std::to_string(o),
                           outputSignals[o] + "_VALID", "std_logic", "out");
            addPortMapping("op_out_data_" + std::to_string(o),
                           outputSignals[o] + "_DATA", "std_logic_vector", "out");
          }
        }
      } else { // implementationType == TT
        for (auto i = 0; i < inputSignals.size(); i++) {
          std::vector<std::string> inPortNames = opInputPorts.at(componentType);
          addPortMapping(inPortNames[i], inputSignals[i], "std_logic_vector", "in");
        }
        for (auto o = 0; o < outputSignals.size(); o++) {
          std::vector<std::string> outPortNames = opOutputPorts.at(componentType);
          addPortMapping(outPortNames[o], outputSignals[o], "std_logic_vector", "out");
        }
      }
    }
  }
}

Vertex VHDLComponent::getActor() const {
  return this->actor;
}

std::string VHDLComponent::getUniqueName() const {
  return this->uniqueName;
}

int VHDLComponent::getIOId() const {
  VERBOSE_ASSERT((this->getType() == "INPUT" || this->getType() == "OUTPUT"),
                 "Trying to get I/O ID for a component that hasn't been declared as type input/output.");
  return this->ioId;
}

const std::vector<std::string>& VHDLComponent::getInputPorts() const{
  return this->inputPorts;
}

const std::vector<std::string>& VHDLComponent::getOutputPorts() const{
  return this->outputPorts;
}

const std::vector<std::string>& VHDLComponent::getInputEdges() const{
  return this->inputEdges;
}

const std::vector<std::string>& VHDLComponent::getOutputEdges() const{
  return this->outputEdges;
}

std::string VHDLComponent::getType() const{
  return this->componentType;
}

/* NOTE in the future, if we intend to implement setting different frequencies
   for each operator, generating their component names based on compType and
   operatorFreq would be one way of going about it */
std::string VHDLComponent::getImplementationName() const{
  return this->implementationName;
}

const std::vector<std::string>& VHDLComponent::getArgOrder() const{
  return this->argOrder;
}

const std::map<std::string, int>& VHDLComponent::getInputTypes() const{
  return this->inputTypes;
}

const std::map<std::string, int>& VHDLComponent::getOutputTypes() const{
  return this->outputTypes;
}

void VHDLComponent::setDataType(const std::string& newType) {
  this->dataType = newType;
}

std::string VHDLComponent::getDataType() const{
  return this->dataType;
}

bool VHDLComponent::isConst() const {
  return this->componentType == "const_value";
}

bool VHDLComponent::isUI() const {
  if (std::count(uiTypes.begin(), uiTypes.end(), this->componentType)) {
    return true;
  } else {
    return false;
  }
}

bool VHDLComponent::hasMixedType() const{
  return this->isMixedType;
}

void VHDLComponent::setFPValue(const float& newVal) {
  this->fpValue = newVal;
}

void VHDLComponent::setIntValue(const int& newVal) {
  this->intValue = newVal;
}

float VHDLComponent::getFPValue() const {
  return this->fpValue;
}

int VHDLComponent::getIntValue() const {
  return this->intValue;
}

void VHDLComponent::setUniqueName(const std::string& newName) {
  this->uniqueName = newName;
}

void VHDLComponent::setImplementationName(const std::string& newName) {
  this->implementationName = newName;
}

void VHDLComponent::setIOId(int id) {
  VERBOSE_ASSERT((this->getType() == "INPUT" || this->getType() == "OUTPUT"),
                 "Trying to set I/O ID for a component that hasn't been declared as type input/output.");
  this->ioId = id;
}

const std::vector<std::string> VHDLComponent::getHSInputSignals() const {
  return this->hsInputSignals;
}

const std::vector<std::string> VHDLComponent::getHSOutputSignals() const {
  return this->hsOutputSignals;
}

const std::vector<std::string> VHDLComponent::getInputSignals() const {
  return this->inputSignals;
}

const std::vector<std::string> VHDLComponent::getOutputSignals() const {
  if (this->getType() == "input_selector" ||
      this->getType() == "output_selector" ||
      this->getType() == "const_value") {
    return this->outputSignals;
  } else {
    return std::vector<std::string>(1, this->sharedOutputSignal);
  }

}

void VHDLComponent::addHSInputSignal(const std::string& signalName) {
  if (std::find(hsInputSignals.begin(),
                hsInputSignals.end(),
                signalName) == hsInputSignals.end()) {
    hsInputSignals.push_back(signalName);
  }
}

void VHDLComponent::addHSOutputSignal(const std::string& signalName) {
  if (std::find(hsOutputSignals.begin(),
                hsOutputSignals.end(),
                signalName) == hsOutputSignals.end()) {
    hsOutputSignals.push_back(signalName);
  }
}

void VHDLComponent::addInputSignal(models::Dataflow *const dataflow,
                                   const Edge e) {
  std::string signalName = dataflow->getEdgeName(e);
  if (implementationType == DD) {
    if (dataflow->getPreload(e)) {
      signalName = dataflow->getEdgeOutputPortName(e);
    }
  }
  // only add signal if it's not already in the vector
  if (std::find(inputSignals.begin(),
                inputSignals.end(),
                signalName) == inputSignals.end()) {
    inputSignals.push_back(signalName);
  }
}

void VHDLComponent::addOutputSignal(models::Dataflow *const dataflow,
                                    const Edge e) {
  std::string signalName = dataflow->getEdgeName(e);
  if (implementationType == DD) {
    if (dataflow->getPreload(e)) {
      signalName = dataflow->getEdgeInputPortName(e);
    }
  }
  // only add signal if it's not already in the vector
  if (std::find(outputSignals.begin(),
                outputSignals.end(),
                signalName) == outputSignals.end()) {
    outputSignals.push_back(signalName);
  }
}

int VHDLComponent::getSBufferInitTokens() const {
  VERBOSE_ASSERT(this->getType() == "sbuffer",
                 "Can only get initial tokens from SBuffers");
  std::string initTokens =
      this->getUniqueName().substr(this->getUniqueName().find("INIT") + 4);
  return std::stoi(initTokens);
}

void VHDLComponent::setStartTimes(std::vector<TIME_UNIT> times) {
  if (!times.size()) {
    VERBOSE_WARNING("Adding empty vector of start times for "
                    << this->getUniqueName());
  }
  this->startTimes = times;
  // execution time port mapping definition
  for (int n = 0; auto time : times) {
    if (componentType == "input_selector") {
      if (implementationType == TT) {
        VERBOSE_ASSERT(times.size() == inputPorts.size(),
                       "Number of start times and input ports don't match");
        addPortMapping("exec_time_" + std::to_string(n),
                       std::to_string((int)time), "integer", "", true);
      }
    } else if (componentType == "output_selector") {
      if (implementationType == TT) {
        VERBOSE_ASSERT(times.size() == outputPorts.size(),
                       "Number of start times and output ports don't match");
        addPortMapping("exec_time_" + std::to_string(n),
                       std::to_string((int)time), "integer", "", true);
      }
    } else if (componentType == "sbuffer") {
      addPortMapping("push_start", std::to_string((int)time), "integer", "",
                     true);
      addPortMapping("pop_start", std::to_string((int)time + 1), "integer", "",
                     true);
    }
    n++;
  }
}

std::vector<TIME_UNIT> VHDLComponent::getStartTimes() const {
  return this->startTimes;
}

void VHDLComponent::addPortMapping(std::string port, std::string signal,
                                   std::string type, std::string direction,
                                   bool isGeneric, int dataWidth) {
 // slv type needs to have a defined width
  if (type == "std_logic_vector") {
    type += "(" + std::to_string(dataWidth - 1) + " downto 0)";
  }
  if (isGeneric) {
    genericMappings[port] = signal;
    genericPorts[port] = type;
  } else {
    portMappings[port] = signal;
    ports[port] = direction + " " + type;
  }
}

std::string VHDLComponent::getPortMapName() const {
  return this->portMapName;
}

std::string VHDLComponent::printStatus() const  {
  std::stringstream outputStream;

  outputStream << "\nActor " << this->getUniqueName() << std::endl;
  outputStream << "\tType: " << this->getType() << std::endl;
  outputStream << "\tData type: " << this->getDataType() << std::endl;
  if (!argOrder.empty()) {
    outputStream << "\tArgument actors:" << std::endl;
    int order = 1;
    for (auto& i : argOrder) {
      outputStream << "\t\t" << order << ". " << i << std::endl;
      order++;
    }
  }
  outputStream << "\tInput ports: " << std::endl;
  for (auto portName : this->getInputPorts()) {
    outputStream << "\t\t" << portName << std::endl;
  }
  outputStream << "\tOutput ports: " << std::endl;
  for (auto portName : this->getOutputPorts()) {
    outputStream << "\t\t" << portName << std::endl;
  }
  outputStream << "\tInput edges: " << std::endl;
  for (auto portName : this->getInputEdges()) {
    outputStream << "\t\t" << portName << std::endl;
  }
  outputStream << "\tOutput edges: " << std::endl;
  for (auto portName : this->getOutputEdges()) {
    outputStream << "\t\t" << portName << std::endl;
  }
  outputStream << "\tInput types: " << std::endl;
  for (auto inType : this->inputTypes) {
    outputStream << "\t\t" << inType.first << ", " << inType.second << std::endl;
  }
  outputStream << "\tOutput types: " << std::endl;
  for (auto outType : this->outputTypes) {
    outputStream << "\t\t" << outType.first << ", " << outType.second << std::endl;
  }
  outputStream << "\tIs constant?";
  if (this->componentType == "const_value") {
    outputStream << " Y" << std::endl;
    if (this->dataType == "real") {
      outputStream <<"\t\tValue: " << this->fpValue << std::endl;
    } else if (this->dataType == "int") {
      outputStream <<"\t\tValue: " << this->intValue << std::endl;
    } // TODO add else for edge cases
  } else {
    outputStream << " N" << std::endl;
  }

  return outputStream.str();
}

// Exists so that we can call genPortList() outside of the VHDLComponent class
// i.e. so that we can get top-level input/output ports
std::map<std::string, std::string> VHDLComponent::getPortMapping() const {
  return this->ports;
}

/**
   Generate VHDL port declarations according to a given port map where:
   [key: port name, value: signal/type].

   @param portMap Populated using addPortMapping().

   @param terminate Whether the port declarations should have a terminating
   delimiter (default = true).

   @param replacements Optional map of signal names that should be replaced
   (key), and their corresponding replacements (value).

   @param relation Defines the syntax for how the port relates to the signal
   (default = ":").

   @param delim Defines the delimiter between each port declaration
   (default = ";").

   @param term Defines the terminating character after the last port is declared
   (default = "").

   @return A string of port declarations.
 */
std::string
VHDLComponent::genPortList(std::map<std::string, std::string> portMap,
                           bool terminate,
                           std::map<std::string, std::string> replacements,
                           std::string relation,
                           std::string delim,
                           std::string term) const {
  std::stringstream codeOut;
  int padAmt = 4;
  std::string t(padAmt, ' ');
  std::string lineEnder = delim;
  std::string r = " " + relation + " "; // spaces for legibility
  for (auto &[port, sigType] : portMap) {
    std::string signal = sigType;
    if (replacements.count(sigType)) {
      signal = replacements[signal];
    }
    if (port == portMap.rbegin()->first && terminate) {
      lineEnder = term;
    }
    codeOut << t << port << r << signal << lineEnder << std::endl;
  }
  return codeOut.str();
}


// Generate the VHDL declaration code for the component
// - dataWidth (default: 34): bit width of data on the input/output ports
std::string VHDLComponent::genDeclaration() const {
  std::stringstream codeOut;
  std::string t = "    "; // indentation: 4 spaces
  codeOut << "component " << implRefName << " is"
          << std::endl;
    if (genericPorts.size()) {
      codeOut << "generic (" << std::endl;
      codeOut << genPortList(genericPorts);
      codeOut << ");" << std::endl;
    }
    if (ports.size()) {
      codeOut << "port (" << std::endl;
      codeOut << genPortList(ports);
      codeOut << ");" << std::endl;
    }
  codeOut << "end component;" << std::endl;

  return codeOut.str();
}



std::string VHDLComponent::genPortMapping(int id, std::map<std::string, std::string> replacements) const {
  std::stringstream codeOut;
  codeOut << portMapName << "_" << id << " : " << implRefName << std::endl;
  if (genericMappings.size()) {
    codeOut << "generic map (" << std::endl;
    codeOut << genPortList(genericMappings, true, replacements, "=>", ",");
    codeOut << ")" << std::endl;
  }
  if (portMappings.size()) {
    codeOut << "port map (" << std::endl;
    codeOut << genPortList(portMappings, true, replacements, "=>", ",");
    codeOut << ");" << std::endl;
  }

  return codeOut.str();
}

implType VHDLComponent::getImplType() const {
  return this->implementationType;
}

std::string getNameFromPartialName(models::Dataflow* const dataflow,
                                   const std::string &partialName) {
  std::vector<std::string> matchingNames;
  {ForEachVertex(dataflow, v) {
      std::string vertexName = dataflow->getVertexName(v);
      std::size_t found = vertexName.find(partialName + "_");
      if ((found == 0) || (vertexName == partialName)) {
        matchingNames.push_back(vertexName);
      }
    }}

  VERBOSE_DEBUG("getComponentFullName '" << partialName << "' returns '" << matchingNames[0] << "'");
  if(matchingNames.size() != 1) {
      VERBOSE_ERROR("Wrong name count for '" << partialName << "': " << commons::toString(matchingNames));
  }

  assert(matchingNames.size() == 1);
  return matchingNames.front();
}
