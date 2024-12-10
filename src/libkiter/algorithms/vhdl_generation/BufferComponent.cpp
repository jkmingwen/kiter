/*
 * BufferComponent.cpp
 *
 *  Created on: 5 December 2024
 *      Author: jkmingwen
 */

#include "VHDLCommons.h"
#include "commons/verbose.h"
#include "BufferComponent.h"

BufferComponent::BufferComponent(std::string bufferType, implType t) {
  // TODO generate different buffers based on buffer and implementation type
  this->setType(bufferType);
  portMapName = bufferType;
  implRefName = bufferType;
  implementationType = t;
  portMappingInit();
}

void BufferComponent::portMappingInit() {
  if (implementationType == TT) {
    addGenericMapping("ram_width", std::to_string(dataWidth), "integer");
    addGenericMapping("buffer_size", std::to_string(bufferSize), "integer");
    addGenericMapping("push_start", std::to_string(pushStart), "integer");
    addGenericMapping("pop_start", std::to_string(popStart), "integer");
    addGenericMapping("init", std::to_string(numInitialTokens), "integer");
    addPortMapping("clk", "sys_clk_sig", "std_logic", "in");
    addPortMapping("rst", "rst_sig", "std_logic", "in");
    addPortMapping("cycle_count", "counter_sig", "integer", "in");
    addPortMapping("in_data", inputSignalName, "std_logic_vector", "in", dataWidth);
    addPortMapping("out_data", outputSignalName, "std_logic_vector", "out", dataWidth);
  } else if (implementationType == GS) {
    addGenericMapping("ram_width", std::to_string(dataWidth), "integer");
    addGenericMapping("buffer_size", std::to_string(bufferSize), "integer");
    addPortMapping("trigger_push",
                   schedSignalName + "(" + std::to_string(pushId) + ")",
                   "std_logic", "in");
    addPortMapping("trigger_pop",
                   schedSignalName + "(" + std::to_string(popId) + ")",
                   "std_logic", "in");
    addGenericMapping("init", std::to_string(numInitialTokens), "integer");
    addPortMapping("clk", "sys_clk_sig", "std_logic", "in");
    addPortMapping("rst", "rst_sig", "std_logic", "in");
    addPortMapping("in_data", inputSignalName, "std_logic_vector", "in", dataWidth);
    addPortMapping("out_data", outputSignalName, "std_logic_vector", "out", dataWidth);
  } else {
    VERBOSE_WARNING("Implementation type " << implementationType << " not supported");
  }
}

void BufferComponent::setPushStart(int cycle) {
  pushStart = cycle;
}

void BufferComponent::setPopStart(int cycle) {
  popStart = cycle;
}

void BufferComponent::setInit(int tokens) {
  numInitialTokens = tokens;
}

void BufferComponent::setBufferSize(int tokens) {
  bufferSize = tokens;
}

void BufferComponent::setDataWidth(int sizeInBits) {
  dataWidth = sizeInBits;
}

void BufferComponent::setInputSignal(std::string sigName) {
  inputSignalName = sigName;
}

void BufferComponent::setOutputSignal(std::string sigName) {
  outputSignalName = sigName;
}

void BufferComponent::setTriggerPushId(int id) { pushId = id; }

void BufferComponent::setTriggerPopId(int id) { popId = id; }

void BufferComponent::setScheduleSigName(std::string name) { schedSignalName = name; }
