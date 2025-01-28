/*
 * VHDLTransceiver.cpp
 *
 *  Created on: 4 December 2024
 *      Author: jkmingwen
 */

#include "VHDLI2STransceiver.h"
#include <string>

VHDLI2STransceiver::VHDLI2STransceiver(int instanceId) {
  id = instanceId;
  implRefName = "i2s_transceiver";
  portMapName = implRefName;
  portMappingInit();
  genSignals();
}

void VHDLI2STransceiver::portMappingInit() {
  std::string instanceName = portMapName + "_" + std::to_string(id);
  addPortMapping("mclk", "mclk_sig", "std_logic", "in");
  addPortMapping("reset_n", "rst_sig", "std_logic", "in");
  addPortMapping("sclk", instanceName + "_sclk", "std_logic", "out");
  addPortMapping("ws", instanceName + "_ws", "std_logic", "out");
  addPortMapping("sd_rx", "ac_recdat_" + std::to_string(id), "std_logic", "in");
  addPortMapping("sd_tx", instanceName + "_sd_tx", "std_logic", "out");
  addPortMapping("l_data_rx", instanceName + "_l_data_rx", "std_logic_vector",
                 "out", 24);
  addPortMapping("r_data_rx", instanceName + "_r_data_rx", "std_logic_vector",
                 "out", 24);
  addPortMapping("l_data_tx", instanceName + "_l_data_tx", "std_logic_vector",
                 "in", 24);
  addPortMapping("r_data_tx", instanceName + "_r_data_tx", "std_logic_vector",
                 "in", 24);
}

void VHDLI2STransceiver::addSignal(std::string sigName, std::string type,
                                   int dataWidth) {
  if (type == "std_logic_vector") {
    type += "(" + std::to_string(dataWidth - 1) + " downto 0)";
  }
  signals[type].push_back(sigName);
}

void VHDLI2STransceiver::genSignals() {
  addSignal(portMapName + "_l_data_rx", "std_logic_vector", 24);
  addSignal(portMapName + "_r_data_rx", "std_logic_vector", 24);
  addSignal(portMapName + "_l_data_tx", "std_logic_vector", 24);
  addSignal(portMapName + "_r_data_tx", "std_logic_vector", 24);
  addSignal(portMapName + "_sclk", "std_logic");
  addSignal(portMapName + "_sd_tx", "std_logic");
  addSignal(portMapName + "_sd_rx", "std_logic");
  addSignal(portMapName + "_ws", "std_logic");
}

std::map<std::string, std::vector<std::string>>
VHDLI2STransceiver::getSignals() {
  return signals;
}
