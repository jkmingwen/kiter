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
  codecId = floor(static_cast<double>(id) / 2);
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
  int lConvId = id * 2;
  int rConvId = (id * 2) + 1;
  addPortMapping("clk", "sys_clk_sig", "std_logic", "in");
  addPortMapping("rst", "rst_sig", "std_logic", "in");
  addPortMapping("ws", codecName + "_ws", "std_logic", "in");
  // generate connections (mappings) to L/R input/output conversion components
  if (isInput) {
    std::string inConvName = "i2s_to_fpc";
    std::string lInConvName = inConvName + "_" + std::to_string(lConvId);
    std::string rInConvName = inConvName + "_" + std::to_string(rConvId);
    addPortMapping("l_data_in", codecName + "_l_data_rx", "std_logic_vector",
                   "in", 24);
    addPortMapping("l_data_out", compName + "_l_data_out",
                   "std_logic_vector", "out", 24);
    addPortMapping("l_ready",
                   lInConvName + "_op_in_ready_0",
                   "std_logic", "in");
    addPortMapping("l_valid", compName + "_l_valid", "std_logic", "out");
    addPortMapping("r_data_in", codecName + "_r_data_rx", "std_logic_vector",
                   "in", 24);
    addPortMapping("r_data_out", compName + "_r_data_out",
                   "std_logic_vector", "out", 24);
    addPortMapping("r_ready",
                   rInConvName + "_op_in_ready_0",
                   "std_logic", "in");
    addPortMapping("r_valid", compName + "_r_valid", "std_logic", "out");
  } else {
    std::string outConvName = "fpc_to_i2s";
    std::string lOutConvName = outConvName + "_" + std::to_string(lConvId);
    std::string rOutConvName = outConvName + "_" + std::to_string(rConvId);
    addPortMapping("l_data_in",
                   lOutConvName + "_op_out_data_0",
                   "std_logic_vector", "in", 24);
    addPortMapping("l_data_out", codecName + "_l_data_tx", "std_logic_vector",
                   "out", 24);
    addPortMapping("l_ready", compName + "_l_ready", "std_logic", "out");
    addPortMapping("l_valid",
                   lOutConvName + "_op_out_valid_0",
                   "std_logic", "in");
    addPortMapping("r_data_in",
                   rOutConvName + "_op_out_data_0",
                   "std_logic_vector", "in", 24);
    addPortMapping("r_data_out", codecName + "_r_data_tx",
                   "std_logic_vector", "out", 24);
    addPortMapping("r_ready", compName + "_r_ready", "std_logic", "out");
    addPortMapping("r_valid",
                   rOutConvName + "_op_out_valid_0",
                   "std_logic", "in");
  }
}
