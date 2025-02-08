/*
 * IOConverterDD.cpp
 *
 *  Created on: 4 December 2024
 *      Author: jkmingwen
 */

#include "VHDLCommons.h"
#include "IOConverterDD.h"
#include <string>

IOConverterDD::IOConverterDD(int instanceId,
                             std::string direction, std::string name) {
  circuitName = name;
  id = instanceId;
  codecId = floor(static_cast<double>(id) / 2);
  if (id % 2) { // right channel on odd IDs
    channel = "r";
  } else {
    channel = "l";
  }
  if (direction == "in") {
    isInput = true;
    implRefName = "i2s_to_fpc";
    portMapName = implRefName + "_" + std::to_string(id);
  } else if (direction == "out") {
    isInput = false;
    implRefName = "fpc_to_i2s";
    portMapName = implRefName + "_" + std::to_string(id);
  }

  portMappingInit();
}

void IOConverterDD::portMappingInit() {
  // TODO fix the ridiculously long names...
  addPortMapping("clk", "sys_clk_sig", "std_logic", "in");
  addPortMapping("rst", "rst_sig", "std_logic", "in");
  if (isInput) {
    std::string sigPrefix = "input_interface_" + std::to_string(codecId) + "_" + channel;
    addPortMapping("op_in_data_0", sigPrefix + "_data_out", "std_logic_vector",
                   "in", 24);
    addPortMapping("op_in_ready_0", portMapName + "_op_in_ready_0", "std_logic",
                   "out");
    addPortMapping("op_in_valid_0", sigPrefix + "_valid", "std_logic", "in");
    addPortMapping("op_out_data_0", portMapName + "_op_out_data_0",
                   "std_logic_vector", "out");
    addPortMapping("op_out_ready_0",
                   circuitName + "_in_ready_" + std::to_string(id) + "_" +
                       portMapName + "_op_out_ready_0",
                   "std_logic", "in");
    addPortMapping("op_out_valid_0", portMapName + "_op_out_valid_0",
                   "std_logic", "out");
  } else {
    std::string sigPrefix = "output_interface_" + std::to_string(codecId) + "_" + channel;
    addPortMapping("op_in_data_0",
                   circuitName + "_out_data_" + std::to_string(id) + "_" +
                       portMapName + "_op_in_data_0",
                   "std_logic_vector", "in");
    addPortMapping("op_in_ready_0", portMapName + "_op_in_ready_0", "std_logic",
                   "out");
    addPortMapping("op_in_valid_0",
                   circuitName + "_out_valid_" + std::to_string(id) + "_" +
                       portMapName + "_op_in_valid_0",
                   "std_logic", "in");
    addPortMapping("op_out_data_0", portMapName + "_op_out_data_0",
                   "std_logic_vector", "out", 24);
    addPortMapping("op_out_ready_0", sigPrefix + "_ready", "std_logic", "in");
    addPortMapping("op_out_valid_0", portMapName + "_op_out_valid_0",
                   "std_logic", "out");
  }
}
