/*
 * IOConverterTT.h
 *
 *  Created on: 4 December 2024
 *      Author: jkmingwen
 */
#ifndef IO_CONVERTER_TT_H_
#define IO_CONVERTER_TT_H_

#include "VHDLCommons.h"
#include "VHDLComponent.h"

class IOConverterTT : public VHDLComponent {
 public:
  IOConverterTT(int id, std::string direction, std::string name);

  void portMappingInit();

 private:
  int id;
  bool isInput;
  std::string channel;
  std::string circuitName;
};


#endif /* IO_CONVERTER_TT_H_ */
