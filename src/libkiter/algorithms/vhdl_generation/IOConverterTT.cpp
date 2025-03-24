/*
 * IOConverterTT.cpp
 *
 *  Created on: 4 December 2024
 *      Author: jkmingwen
 */

#include "VHDLCommons.h"
#include "IOConverterTT.h"
#include <string>

IOConverterTT::IOConverterTT(int instanceId,
                             std::string direction, std::string name) {
  circuitName = name;
  id = instanceId;
  int codecId = floor(static_cast<double>(id) / 2);
  if (id % 2) { // right channel on odd IDs
    channel = "r";
  } else {
    channel = "l";
  }
  if (direction == "in") {
    isInput = true;
    implRefName = "fix2fp_and_scaledown";
    portMapName = "fix2fp_" + std::to_string(codecId) + "_" + channel; // TODO this needs to not include the channel for port map name tracking to work properly
  } else if (direction == "out") {
    isInput = false;
    implRefName = "fp2fix_and_scaleup";
    portMapName = "fp2fix_" + std::to_string(codecId) + "_" + channel;
  }

  portMappingInit();
}

void IOConverterTT::portMappingInit() {
  addGenericMapping("i2s_bit_width", "24", "natural", 24, "24");
  addGenericMapping("fp_bit_width", "34", "natural", 34, "34");
  addGenericMapping("bit_depth", "2", "natural", 2, "2");
  addPortMapping("clk", "sys_clk_sig", "std_logic", "in");
  addPortMapping("rst", "rst_sig", "std_logic", "in");
  if (isInput) {
    addGenericMapping("scale_factor", "\"0100110100000000000000000000000001\"",
                      "std_logic_vector", 34,
                      "\"0100110100000000000000000000000001\"");
    addPortMapping("i2s_in", portMapName + "_data_in", "std_logic_vector",
                   "in", 24);
    addPortMapping("fp_out", circuitName + "_in_data_" + std::to_string(id),
                   "std_logic_vector", "out", 34);
  } else {
    addGenericMapping("scale_factor", "\"0101001010111111111111111111111110\"",
                      "std_logic_vector", 34,
                      "\"0101001010111111111111111111111110\"");
    addPortMapping("fp_in", circuitName + "_out_data_" + std::to_string(id),
                   "std_logic_vector", "in", 34);
    addPortMapping("i2s_out", portMapName + "_data_out", "std_logic_vector",
                   "out", 24);
  }
}
