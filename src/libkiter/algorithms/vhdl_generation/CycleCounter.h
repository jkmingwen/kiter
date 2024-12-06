/*
 * CycleCounter.h
 *
 *  Created on: 5 December 2024
 *      Author: jkmingwen
 */
#ifndef CYCLE_COUNTER_H_
#define CYCLE_COUNTER_H_

#include "VHDLCommons.h"
#include "VHDLComponent.h"

class CycleCounter : public VHDLComponent {
 public:
  CycleCounter(int sysPeriod);

  void portMappingInit();

 private:
  int period;
};


#endif /* CYCLE_COUNTER_H_ */
