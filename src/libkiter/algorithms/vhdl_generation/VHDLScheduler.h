/*
 * VHDLScheduler.cpp
 *
 *  Created on: 14 November 2024
 *      Author: jkmingwen
 */
#ifndef VHDL_SCHEDULER_H_
#define VHDL_SCHEDULER_H_

#include "VHDLCommons.h"
#include "algorithms/vhdl_generation/VHDLComponent.h"

class VHDLScheduler : public VHDLComponent {
public:
  VHDLScheduler();

  int addExecution(int execTime);
  int getScheduleSigWidth();
  std::string genProcess();
  std::string writeSchedule();
  void writeImplementation(std::ofstream &vhdlOutput);
  void portMappingInit();
  void setPeriod(int schedulerPeriod);

private:
  std::map<int, std::vector<int>> trigOnTimes; // execution times -> actor schedule IDs
  std::map<int, std::string> schedule; // schedule index -> VHDL component name
  std::map<std::string, std::string> genericMappings; // generic port name -> signal
  std::map<std::string, std::string> portMappings;   // port name -> signal
  std::map<std::string, std::string> genericPorts;   // generic port -> signal type
  std::map<std::string, std::string> ports;          // port -> signal type
  int scheduleWidth = 0;
  int period = 0;
};
#endif /* VHDL_SCHEDULER_H_ */
