/*
 * VHDLWrapper.cpp
 *
 *  Created on: 2 December 2024
 *      Author: jkmingwen
 */

#include "VHDLWrapper.h"
#include "VHDLCommons.h"
#include "VHDLI2STransceiver.h"
#include "IOConverterTT.h"
#include "IOConverterDD.h"
#include "CycleCounter.h"
#include "BufferComponent.h"
#include "IOInterface.h"
#include <memory>

VHDLWrapper::VHDLWrapper() {}

VHDLWrapper::VHDLWrapper(implType t, int sysPeriod, int sysSlack) {
  implementationType = t;
  period = sysPeriod;
  slack = sysSlack;
}

VHDLWrapper::VHDLWrapper(VHDLCircuit circuit, implType t, int sysPeriod,
                         int sysSlack) {
  implementationType = t;
  period = sysPeriod;
  slack = sysSlack;
  initialiseWrapper(circuit);
}

VHDLWrapper::VHDLWrapper(VHDLCircuit circuit, VHDLScheduler &s, implType t, int sysPeriod,
                         int sysSlack) {
  implementationType = t;
  period = sysPeriod;
  slack = sysSlack;
  scheduler = s;
  initialiseWrapper(circuit);
}

void VHDLWrapper::portMappingInit() {
  // audio interface wrapper ports and signal mappings
  addPortMapping("sys_clk", "sys_clk_sig", "std_logic", "in");
  addPortMapping("reset", "rst_sig", "std_logic", "in");
  addPortMapping("mclk", "mclk_sig", "std_logic", "in");
  for (int i = 0; i < numAudioCodecs; i++) {
    std::string codecPrefix = "i2s_transceiver_" + std::to_string(i);
    addPortMapping("ac_bclk_" + std::to_string(i), codecPrefix + "_sclk",
                   "std_logic", "out");
    addPortMapping("ac_pbdat_" + std::to_string(i), codecPrefix + "_sd_tx",
                   "std_logic", "out");
    addPortMapping("ac_recdat_" + std::to_string(i), codecPrefix + "_sd_rx",
                   "std_logic", "in");
    addPortMapping("ac_pblrc_" + std::to_string(i), codecPrefix + "_ws",
                   "std_logic", "out");
    addPortMapping("ac_reclrc_" + std::to_string(i), codecPrefix + "_ws",
                   "std_logic", "out");
  }
}

void VHDLWrapper::internalSignalsInit() {
  addInternalSignal("sys_clk_sig", "std_logic");
  addInternalSignal("rst_sig", "std_logic");
  addInternalSignal("mclk_sig", "std_logic");
  if (implementationType == TT) {
    addInternalSignal("counter_sig", "integer");
    for (auto i = 0; i < numInputs; i++) {
      addInternalSignal(dspName + "_in_data_" +
                            std::to_string(i),
                        "std_logic_vector");
    }
    for (auto i = 0; i < numOutputs; i++) {
      addInternalSignal(dspName + "_out_data_" +
                            std::to_string(i),
                        "std_logic_vector");
    }
    for (int i = 0; i < numAudioCodecs; i++) {
      // I2S transceiver signals
      std::string codecPrefix = "i2s_transceiver_" + std::to_string(i);
      addInternalSignal(codecPrefix + "_l_data_rx", "std_logic_vector", 24);
      addInternalSignal(codecPrefix + "_r_data_rx", "std_logic_vector", 24);
      addInternalSignal(codecPrefix + "_l_data_tx", "std_logic_vector", 24);
      addInternalSignal(codecPrefix + "_r_data_tx", "std_logic_vector", 24);
      addInternalSignal(codecPrefix + "_sclk", "std_logic");
      addInternalSignal(codecPrefix + "_sd_tx", "std_logic");
      addInternalSignal(codecPrefix + "_sd_rx", "std_logic");
      addInternalSignal(codecPrefix + "_ws", "std_logic");
      // input/output interface signals
      if (numInputs) {
        std::string prefix = "fix2fp_" + std::to_string(i);
        addInternalSignal(prefix + "_l_data_in", "std_logic_vector", 24);
        addInternalSignal(prefix + "_r_data_in", "std_logic_vector", 24);
      }
      if (numOutputs) {
        std::string prefix = "fp2fix_" + std::to_string(i);
        addInternalSignal(prefix + "_l_data_out", "std_logic_vector", 24);
        addInternalSignal(prefix + "_r_data_out", "std_logic_vector", 24);
      }
    }
  } else if (implementationType == GS) {
    addInternalSignal("scheduler_sig", "std_logic_vector", scheduler.getScheduleSigWidth());
    for (auto i = 0; i < numInputs; i++) {
      addInternalSignal(dspName + "_in_data_" +
                        std::to_string(i),
                        "std_logic_vector");
    }
    for (auto i = 0; i < numOutputs; i++) {
      addInternalSignal(dspName + "_out_data_" +
                        std::to_string(i),
                        "std_logic_vector");
    }
    for (int i = 0; i < numAudioCodecs; i++) {
      // I2S transceiver signals
      std::string codecPrefix = "i2s_transceiver_" + std::to_string(i);
      addInternalSignal(codecPrefix + "_l_data_rx", "std_logic_vector", 24);
      addInternalSignal(codecPrefix + "_r_data_rx", "std_logic_vector", 24);
      addInternalSignal(codecPrefix + "_l_data_tx", "std_logic_vector", 24);
      addInternalSignal(codecPrefix + "_r_data_tx", "std_logic_vector", 24);
      addInternalSignal(codecPrefix + "_sclk", "std_logic");
      addInternalSignal(codecPrefix + "_sd_tx", "std_logic");
      addInternalSignal(codecPrefix + "_sd_rx", "std_logic");
      addInternalSignal(codecPrefix + "_ws", "std_logic");
      // input/output interface signals
      if (numInputs) {
        std::string prefix = "fix2fp_" + std::to_string(i);
        addInternalSignal(prefix + "_l_data_in", "std_logic_vector", 24);
        addInternalSignal(prefix + "_r_data_in", "std_logic_vector", 24);
      }
      if (numOutputs) {
        std::string prefix = "fp2fix_" + std::to_string(i);
        addInternalSignal(prefix + "_l_data_out", "std_logic_vector", 24);
        addInternalSignal(prefix + "_r_data_out", "std_logic_vector", 24);
      }
    }
  } else if (implementationType == DD) {
    for (auto i = 0; i < numOutputs; i++) {
      std::string outId = std::to_string(i);
      std::string inConvName = "i2s_to_fpc_" + outId;
      std::string outConvName = "fpc_to_i2s_" + outId;
      std::string readySig = dspName + "_in_ready_" + outId + "_" +
                             inConvName + "_op_out_ready_0";
      std::string validSig = dspName + "_out_valid_" + outId + "_" +
                             outConvName + "_op_in_valid_0";
      std::string dataSig = dspName + "_out_data_" + outId + "_" +
                            outConvName + "_op_in_data_0";
      addInternalSignal(readySig, "std_logic");
      addInternalSignal(validSig, "std_logic");
      addInternalSignal(dataSig, "std_logic_vector");
    }
    for (int i = 0; i < numAudioCodecs; i++) {
      // I2S transceiver signals
      std::string codecPrefix = "i2s_transceiver_" + std::to_string(i);
      addInternalSignal(codecPrefix + "_l_data_rx", "std_logic_vector", 24);
      addInternalSignal(codecPrefix + "_r_data_rx", "std_logic_vector", 24);
      addInternalSignal(codecPrefix + "_l_data_tx", "std_logic_vector", 24);
      addInternalSignal(codecPrefix + "_r_data_tx", "std_logic_vector", 24);
      addInternalSignal(codecPrefix + "_sclk", "std_logic");
      addInternalSignal(codecPrefix + "_sd_tx", "std_logic");
      addInternalSignal(codecPrefix + "_sd_rx", "std_logic");
      addInternalSignal(codecPrefix + "_ws", "std_logic");
      // input/output interface signals
      if (numInputs) {
        std::string prefix = "input_interface_" + std::to_string(i);
        std::string lPrefix = "i2s_to_fpc_" + std::to_string(i * 2);
        std::string rPrefix = "i2s_to_fpc_" + std::to_string((i*2)+1);
        addInternalSignal(prefix + "_l_data_out", "std_logic_vector", 24);
        addInternalSignal(prefix + "_r_data_out", "std_logic_vector", 24);
        addInternalSignal(prefix + "_l_valid", "std_logic");
        addInternalSignal(prefix + "_r_valid", "std_logic");
        addInternalSignal(lPrefix + "_op_out_valid_0", "std_logic");
        addInternalSignal(lPrefix + "_op_in_ready_0", "std_logic");
        addInternalSignal(lPrefix + "_op_out_data_0", "std_logic_vector");
        addInternalSignal(rPrefix + "_op_out_valid_0", "std_logic");
        addInternalSignal(rPrefix + "_op_in_ready_0", "std_logic");
        addInternalSignal(rPrefix + "_op_out_data_0", "std_logic_vector");
      }
      if (numOutputs) {
        std::string prefix = "output_interface_" + std::to_string(i);
        std::string lPrefix = "fpc_to_i2s_" + std::to_string(i * 2);
        std::string rPrefix = "fpc_to_i2s_" + std::to_string((i*2)+1);
        addInternalSignal(prefix + "_l_data_out", "std_logic_vector", 24);
        addInternalSignal(prefix + "_r_data_out", "std_logic_vector", 24);
        addInternalSignal(prefix + "_l_ready", "std_logic");
        addInternalSignal(prefix + "_r_ready", "std_logic");
        addInternalSignal(lPrefix + "_op_out_valid_0", "std_logic");
        addInternalSignal(lPrefix + "_op_in_ready_0", "std_logic");
        addInternalSignal(lPrefix + "_op_out_data_0", "std_logic_vector", 24);
        addInternalSignal(rPrefix + "_op_out_valid_0", "std_logic");
        addInternalSignal(rPrefix + "_op_in_ready_0", "std_logic");
        addInternalSignal(rPrefix + "_op_out_data_0", "std_logic_vector", 24);
      }
    }
  }
}

// Configure audio wrapper implementation according to circuit it's wrapping
void VHDLWrapper::initialiseWrapper(const VHDLCircuit &circuit) {
  implRefName = "audio_interface_wrapper";
  dspCircuit = circuit;
  numInputs = dspCircuit.getOperatorCount("INPUT");
  numOutputs = dspCircuit.getOperatorCount("OUTPUT");
  dspName = dspCircuit.getName();
  numAudioCodecs = std::max((numInputs / 2 + (numInputs % 2 != 0)),
                            (numOutputs / 2 + (numOutputs % 2 != 0)));
  computeTimes = circuit.getComputeTimes();

  // add components
  if (implementationType == TT) {
    // I2S transceiver
    for (int i = 0; i < numAudioCodecs; i++) {
      components.push_back(std::unique_ptr<VHDLComponent>(new VHDLI2STransceiver(i)));
    }

    // Input/Output converters
    for (int inId = 0; inId < numInputs; inId++) {
      components.push_back(std::unique_ptr<VHDLComponent>(
          new IOConverterTT(inId, "in", dspName)));
      // input buffer
      std::string chId = std::to_string(inId);
      std::string channel = (inId % 2) ? "r" : "l";
      std::string inSigName = "i2s_transceiver_" + chId + "_" + channel + "_data_rx";
      std::string outSigName = "fix2fp_" + chId + "_" + channel + "_data_in";
      int pushStart, popStart;
      if (channel == "l") {
        pushStart = (period / 2) + slack;
        popStart = (period / 2) + slack + 1;
      } else {
        pushStart = slack;
        popStart = slack + 1;
      }
      BufferComponent *inBuffer = genIOBuffer(implementationType, inSigName, outSigName,
                                              pushStart, popStart);
      components.push_back(std::unique_ptr<VHDLComponent>(inBuffer));
    }
    for (int outId = 0; outId < numOutputs; outId++) {
      components.push_back(std::unique_ptr<VHDLComponent>(
          new IOConverterTT(outId, "out", dspName)));
      // output buffer
      std::string chId = std::to_string(outId);
      std::string channel = (outId % 2) ? "r" : "l";
      std::string inSigName = "fp2fix_" + chId + "_" + channel + "_data_out";
      std::string outSigName = "i2s_transceiver_" + chId + "_" + channel + "_data_tx";
      int pushStart, popStart;
      TIME_UNIT computeTime = computeTimes.at(outId);
      if (channel == "l") {
        pushStart = (period / 2) + slack + computeTime;
        popStart = (period / 2) + slack + computeTime + 1;
      } else {
        pushStart = slack + computeTime;
        popStart = slack + computeTime + 1;
      }
      BufferComponent *outBuffer = genIOBuffer(implementationType, inSigName, outSigName,
                                               pushStart, popStart);
      components.push_back(std::unique_ptr<VHDLComponent>(outBuffer));
    }
    // Cycle counter
    components.push_back(
        std::unique_ptr<VHDLComponent>(new CycleCounter(period)));

  } else if (implementationType == GS) {
    // I2S transceiver
    for (int i = 0; i < numAudioCodecs; i++) {
      components.push_back(std::unique_ptr<VHDLComponent>(new VHDLI2STransceiver(i)));
    }

    // Input/Output converters
    for (int inId = 0; inId < numInputs; inId++) {
      components.push_back(std::unique_ptr<VHDLComponent>(
          new IOConverterTT(inId, "in", dspName)));
      // input buffer
      std::string chId = std::to_string(inId);
      std::string channel = (inId % 2) ? "r" : "l";
      std::string inSigName = "i2s_transceiver_" + chId + "_" + channel + "_data_rx";
      std::string outSigName = "fix2fp_" + chId + "_" + channel + "_data_in";
      int pushStart, popStart;
      if (channel == "l") {
        pushStart = (period / 2) + slack;
        popStart = (period / 2) + slack + 1;
      } else {
        pushStart = slack;
        popStart = slack + 1;
      }
      BufferComponent *inBuffer = genIOBuffer(implementationType, inSigName, outSigName,
                                              pushStart, popStart);
      components.push_back(std::unique_ptr<VHDLComponent>(inBuffer));
    }
    for (int outId = 0; outId < numOutputs; outId++) {
      components.push_back(std::unique_ptr<VHDLComponent>(
          new IOConverterTT(outId, "out", dspName)));
      // output buffer
      std::string chId = std::to_string(outId);
      std::string channel = (outId % 2) ? "r" : "l";
      std::string inSigName = "fp2fix_" + chId + "_" + channel + "_data_out";
      std::string outSigName = "i2s_transceiver_" + chId + "_" + channel + "_data_tx";
      int pushStart, popStart;
      TIME_UNIT computeTime = computeTimes.at(outId);
      if (channel == "l") {
        pushStart = (period / 2) + slack + computeTime;
        popStart = (period / 2) + slack + computeTime + 1;
      } else {
        pushStart = slack + computeTime;
        popStart = slack + computeTime + 1;
      }
      BufferComponent *outBuffer = genIOBuffer(implementationType, inSigName, outSigName,
                                               pushStart, popStart);
      components.push_back(std::unique_ptr<VHDLComponent>(outBuffer));
    }
    scheduler.portMappingInit();
    dspCircuit.setScheduleWidth(scheduler.getScheduleSigWidth());
  } else if (implementationType == DD) {
    for (int i = 0; i < numAudioCodecs; i++) {
      components.push_back(
          std::unique_ptr<VHDLComponent>(new VHDLI2STransceiver(i)));
      components.push_back(std::unique_ptr<VHDLComponent>(new IOInterface(i, "in")));
      components.push_back(std::unique_ptr<VHDLComponent>(new IOInterface(i, "out")));
    }
    for (int inId = 0; inId < numInputs; inId++) {
      components.push_back(std::unique_ptr<VHDLComponent>(
          new IOConverterDD(inId, "in", dspName)));
    }
    for (int outId = 0; outId < numOutputs; outId++) {
      components.push_back(std::unique_ptr<VHDLComponent>(
          new IOConverterDD(outId, "out", dspName)));
    }
  }
  dspCircuit.portMappingInit();
  portMappingInit();
  internalSignalsInit();
}

std::string VHDLWrapper::genInternalSigs() {
  std::stringstream codeOut;
  for (auto &[sigType, names] : internalSignals) {
    for (auto &name : names) {
      codeOut << "signal " << name << " : " << sigType << ";" << std::endl;
    }
  }

  return codeOut.str();
}

void VHDLWrapper::addInternalSignal(std::string sigName, std::string type,
                                    int dataWidth) {
  if (type == "std_logic_vector") {
    type += "(" + std::to_string(dataWidth - 1) + " downto 0)";
  }
  internalSignals[type].push_back(sigName);
}

void VHDLWrapper::writeImplementation(std::ofstream &vhdlOutput) {
  vhdlOutput << "library ieee;\n"
             << "use ieee.std_logic_1164.all;" << std::endl;

  vhdlOutput << genEntityDecl() << std::endl;
  vhdlOutput << "architecture structure of " << implRefName << " is" << std::endl;
  std::map<std::string, int>
      opCounts; // track counts of operators for instantiation in port mapping
  std::map<std::string, std::string> replacementSigs;
  std::map<std::string, int>
      trackDeclarations; // track counts of operators for instantiation in port mapping
  for (const auto &comp : components) {
    std::string name = comp->getPortMapName();
    if (!trackDeclarations.count(name)) {
      trackDeclarations[name] = 1;
      vhdlOutput << comp->genDeclaration() << std::endl;
    }
  }
  vhdlOutput << dspCircuit.genDeclaration() << std::endl;

  if (implementationType == GS) {
    vhdlOutput << scheduler.genDeclaration() << std::endl;
  }

  vhdlOutput << genInternalSigs() << std::endl;

  vhdlOutput << "begin" << std::endl;

  // internal mappings according to number of audio codecs
  vhdlOutput << "mclk_sig <= mclk;\n"
             << "rst_sig <= reset;\n"
             << "sys_clk_sig <= sys_clk;" << std::endl;
  for (int i = 0; i < numAudioCodecs; i++) {
    std::string sigPrefix = "i2s_transceiver_" + std::to_string(i);
    vhdlOutput << "ac_bclk_" << i << " <= " << sigPrefix + "_sclk;\n"
               << "ac_pbdat_" << i << " <= " << sigPrefix + "_sd_tx;\n"
               << sigPrefix + "_sd_rx" << " <= " << "ac_recdat_" << i << ";\n"
               << "ac_pblrc_" << i << " <= " << sigPrefix + "_ws;\n"
               << "ac_reclrc_" << i << " <= " << sigPrefix + "_ws;"
               << std::endl;
  }

  for (const auto &comp : components) {
    std::string opName = comp->getPortMapName();
    if (opCounts.count(opName)) {
      opCounts[opName]++;
    } else {
      opCounts[opName] = 0;
    }
    std::string portMap = comp->genPortMapping(opCounts[opName], replacementSigs);
    vhdlOutput << portMap << std::endl;
  }
  vhdlOutput << dspCircuit.genPortMapping(0, replacementSigs);
  if (implementationType == GS) {
    vhdlOutput << scheduler.genPortMapping(0, replacementSigs) << std::endl;
  }
  vhdlOutput << "end structure;" << std::endl;
}

BufferComponent *VHDLWrapper::genIOBuffer(implType t, std::string inSigName,
                                          std::string outSigName,
                                          int pushStart, int popStart) {
  BufferComponent *buffer = new BufferComponent("sbuffer", t);
  if (implementationType == TT) {
    buffer->setInputSignal(inSigName);
    buffer->setOutputSignal(outSigName);
    buffer->setBufferSize(1);
    buffer->setInit(0);
    buffer->setDataWidth(24);
    buffer->setPushStart(pushStart);
    buffer->setPopStart(popStart);
    buffer->portMappingInit();
  } else if (implementationType == GS) {
    buffer->setInputSignal(inSigName);
    buffer->setOutputSignal(outSigName);
    buffer->setBufferSize(1);
    buffer->setInit(0);
    int pushId = scheduler.addExecution(pushStart);
    int popId = scheduler.addExecution(popStart);
    buffer->setDataWidth(24);
    buffer->setTriggerPushId(pushId);
    buffer->setTriggerPopId(popId);
    buffer->setScheduleSigName("scheduler_sig");
    buffer->portMappingInit();
  }

  return buffer;
}

void VHDLWrapper::writeSchedulerImplementation(std::string dir) {
  std::ofstream scheduleImpl;
  scheduleImpl.open(dir + "scheduler.vhdl");
  scheduler.writeImplementation(scheduleImpl);
  scheduleImpl.close();
}

void VHDLWrapper::writeCircuitImplementation(std::string dir) {
  std::ofstream vhdlOutput;
  std::string graphName = dspCircuit.getName() + "_circuit"; // TODO decide on naming convention

  vhdlOutput.open(dir + graphName + ".vhdl"); // instantiate VHDL file
  dspCircuit.writeImplementation(vhdlOutput);
}
