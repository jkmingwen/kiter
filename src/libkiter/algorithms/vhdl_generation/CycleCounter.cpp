/*
 * CycleCounter.cpp
 *
 *  Created on: 5 December 2024
 *      Author: jkmingwen
 */

#include "VHDLCommons.h"
#include "CycleCounter.h"
#include <string>

CycleCounter::CycleCounter(int sysPeriod) {
  period = sysPeriod;
  portMapName = "counter";
  implRefName = "cycle_counter";
  portMappingInit();
}

void CycleCounter::portMappingInit() {
  addGenericMapping("period", std::to_string(period), "integer");
  addPortMapping("clk", "sys_clk_sig", "std_logic", "in");
  addPortMapping("rst", "rst_sig", "std_logic", "in");
  addPortMapping("count", "counter_sig", "integer", "out");
}
