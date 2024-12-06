/*
 * IOInterface.cpp
 *
 *  Created on: 6 December 2024
 *      Author: jkmingwen
 */

#include "VHDLCommons.h"
#include "IOInterface.h"
#include <string>

IOInterface::IOInterface(int instanceId, std::string direction) {
  id = instanceId;
  if (direction == "in") {
    isInput = true;
    implRefName = "input_interface";
    portMapName = implRefName;
  } else if (direction == "out") {
    isInput = false;
    implRefName = "output_interface";
    portMapName = implRefName;
  }

  portMappingInit();
}

void IOInterface::portMappingInit() {
  // TODO fix the ridiculously long names...
  std::string compName = portMapName + "_" + std::to_string(id);
  std::string codecName = "i2s_transceiver_" + std::to_string(id);
  addPortMapping("clk", "sys_clk_sig", "std_logic", "in");
  addPortMapping("rst", "rst_sig", "std_logic", "in");
  addPortMapping("ws", codecName + "_ws", "std_logic", "in");
  if (isInput) {
    std::string inConvName = "i2s_to_fpc";
    addPortMapping("l_data_in", codecName + "_l_data_rx", "std_logic_vector",
                   "in", 24);
    addPortMapping("l_data_out", compName + "_l_data_out",
                   "std_logic_vector", "out", 24);
    addPortMapping("l_ready",
                   inConvName + "_" + std::to_string(id) + "_op_in_ready_0",
                   "std_logic", "in");
    addPortMapping("l_valid", compName + "_l_valid", "std_logic", "out");
    addPortMapping("r_data_in", codecName + "_r_data_rx", "std_logic_vector",
                   "in", 24);
    addPortMapping("r_data_out", compName + "_r_data_out",
                   "std_logic_vector", "out", 24);
    addPortMapping("r_ready",
                   inConvName + "_" + std::to_string(id + 1) + "_op_in_ready_0",
                   "std_logic", "in");
    addPortMapping("r_valid", compName + "_r_valid", "std_logic", "out");
  } else {
    std::string outConvName = "fpc_to_i2s";
    addPortMapping("l_data_in",
                   outConvName + "_" + std::to_string(id) + "_op_out_data_0",
                   "std_logic_vector", "in", 24);
    addPortMapping("l_data_out", codecName + "_l_data_tx", "std_logic_vector",
                   "out", 24);
    addPortMapping("l_ready", compName + "_l_ready", "std_logic", "out");
    addPortMapping("l_valid",
                   outConvName + "_" + std::to_string(id) + "_op_out_valid_0",
                   "std_logic", "in");
    addPortMapping("r_data_in",
                   outConvName + "_" + std::to_string(id + 1) + "_op_out_data_0",
                   "std_logic_vector", "in", 24);
    addPortMapping("r_data_out", codecName + "_r_data_tx",
                   "std_logic_vector", "out", 24);
    addPortMapping("r_ready", compName + "_r_ready", "std_logic", "out");
    addPortMapping("r_valid",
                   outConvName + "_" + std::to_string(id + 1) +
                       "_op_out_valid_0",
                   "std_logic", "in");
  }
}
