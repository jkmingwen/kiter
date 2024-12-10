/*
 * VHDLScheduler.cpp
 *
 *  Created on: 14 November 2024
 *      Author: jkmingwen
 */

#include "VHDLScheduler.h"

VHDLScheduler::VHDLScheduler() {
  portMapName = "sched";
  implRefName = "scheduler";

}

int VHDLScheduler::addExecution(int execTime) {
  int schedIndex = scheduleWidth;
  if (trigOnTimes.count(execTime)) {
    trigOnTimes.at(execTime).push_back(schedIndex);
  } else {
    trigOnTimes[execTime].push_back(schedIndex);
  }
  scheduleWidth++;

  return schedIndex;
}

void VHDLScheduler::portMappingInit() {
  // trigger port inititalised and updated in addExecution
  addPortMapping("clk", "sys_clk_sig", "std_logic", "in");
  addPortMapping("rst", "rst_sig", "std_logic", "in");
  addPortMapping("cycle_count", "counter_sig", "integer", "in");
  addPortMapping("trigger", "scheduler_sig", "std_logic_vector", "out",
                 scheduleWidth);
}

int VHDLScheduler::getScheduleSigWidth() {
  return scheduleWidth;
}

std::string VHDLScheduler::genProcess() {
  std::ostringstream codeOut;
  std::string procName = "execute_schedule";
  std::string countSig = "cycle_count";
  std::string pad = "    ";
  codeOut << procName << " : process(" << countSig << ")\n"
          << "begin\n"
          << pad << "trigger <= (others => '0');\n"
          << pad << "case ("<< countSig << ") is" << std::endl;
  for (auto const &[time, ids] : trigOnTimes) {
    codeOut << pad << "when " << time << " =>\n";
    for (auto const &id : ids) {
      codeOut << pad + pad << "trigger(" << id << ") <= '1';\n";
    }
  }
  codeOut << pad << "when others =>\n"
          << pad + pad << "null;\n"
          << "end case;\n"
          << "end process " << procName << ";"
          << std::endl;

  return codeOut.str();
}

void VHDLScheduler::writeImplementation(std::ofstream &vhdlOutput) {
  portMappingInit();

  vhdlOutput << "library ieee;\n"
             << "use ieee.std_logic_1164.all;\n"
             << "use ieee.numeric_std.all;\n"
             << std::endl;

  vhdlOutput << genEntityDecl() << std::endl;

  vhdlOutput << "architecture Behavioral of " << implRefName << " is\n"
             << "begin\n"
             << std::endl;
  vhdlOutput << genProcess() << std::endl;
  vhdlOutput << "end Behavioral;" << std::endl;
}

