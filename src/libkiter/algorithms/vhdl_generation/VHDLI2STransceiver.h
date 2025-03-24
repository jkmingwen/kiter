/*
 * VHDLI2STransceiver.h
 *
 *  Created on: 4 December 2024
 *      Author: jkmingwen
 */
#ifndef VHDL_I2S_TRANSCEIVER_H_
#define VHDL_I2S_TRANSCEIVER_H_

#include "VHDLComponent.h"

class VHDLI2STransceiver : public VHDLComponent {
 public:
  VHDLI2STransceiver(int id);

  void portMappingInit();
  void genSignals();
  void addSignal(std::string sigName, std::string type, int dataWidth = 34);
  std::map<std::string, std::vector<std::string>> getSignals();

 private:
  int id;
  std::map<std::string, std::vector<std::string>>
    signals; // signal type -> signal names
};


#endif /* VHDL_I2S_TRANSCEIVER_H_ */
