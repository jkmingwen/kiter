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
    // trigOnTimes.at(execTime).push_back(schedIndex);
    schedIndex = trigOnTimes.at(execTime).front();
  } else {
    trigOnTimes[execTime].push_back(schedIndex);
    scheduleWidth++;
  }

  return schedIndex;
}

void VHDLScheduler::portMappingInit() {
  // trigger port inititalised and updated in addExecution
  addPortMapping("clk", "sys_clk_sig", "std_logic", "in");
  addPortMapping("rst", "rst_sig", "std_logic", "in");
  addPortMapping("trigger", "scheduler_sig", "std_logic_vector", "out",
                 scheduleWidth);
}

int VHDLScheduler::getScheduleSigWidth() {
  return scheduleWidth;
}

std::string VHDLScheduler::genProcess() {
  std::ostringstream codeOut;
  std::string procName = "execute_schedule";
  std::string countSig = "clock_count";
  std::string pad = "    ";
  codeOut << procName << " : process(clk)\n"
          << "begin\n"
          << "if rising_edge(clk) then\n"
          << pad << "if " << countSig << " < " << period - 1 << " then\n"
          << pad + pad << countSig << " <= " << countSig << " + 1;"
          << std::endl;
  codeOut << pad << "else\n"
          << pad + pad << countSig << " <= 0;\n"
          << pad << "end if;\n"
          << "end if;\n"
          << "if rst = '0' then\n"
          << pad << countSig << " <= 0;\n"
          << "end if;\n"
          << "end process " << procName << ";"
          << std::endl;

  return codeOut.str();
}

std::string VHDLScheduler::writeSchedule() {
  std::ostringstream codeOut;
  std::string pad = "    ";

  codeOut << "constant trigger_rom : rom_type := (" << std::endl;

  for (auto const &[time, romId] : trigOnTimes) {
    codeOut << pad << time << " => ";
    std::string triggerSig(scheduleWidth, '0');
    for (auto const &id : romId) {
      triggerSig.at(scheduleWidth-1 - id) = '1';
    }
    codeOut << "\"" << triggerSig << "\",\n";
  }
  codeOut << pad << "others => (others => '0')" << std::endl;
  codeOut << ");" << std::endl;

  return codeOut.str();
}

void VHDLScheduler::writeImplementation(std::ofstream &vhdlOutput) {
  std::string pad = "    ";
  std::string countSig = "clock_count";
  portMappingInit();

  vhdlOutput << "library ieee;\n"
             << "use ieee.std_logic_1164.all;\n"
             << "use ieee.numeric_std.all;\n"
             << std::endl;

  vhdlOutput << genEntityDecl() << std::endl;

  vhdlOutput << "architecture Behavioral of " << implRefName
             << " is\n"
             // internal signal for clock count
             << pad << "signal clock_count : integer range 0 to "
             << period - 1
             << " := 0;\n"
             // type definition for ROM array
             << "type rom_type is array (0 to " << period - 1
             << ") of std_logic_vector(" << scheduleWidth-1 << " downto 0);\n"
             << writeSchedule()
             << "begin\n"
             << pad << "trigger <= trigger_rom(" << countSig << ");\n"
             << std::endl;
  vhdlOutput << genProcess() << std::endl;
  vhdlOutput << "end Behavioral;" << std::endl;
}

void VHDLScheduler::setPeriod(int schedulerPeriod) {
  this->period = schedulerPeriod;
}
