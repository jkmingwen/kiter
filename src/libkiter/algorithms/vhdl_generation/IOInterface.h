/*
 * IOInterfaceDD.h
 *
 *  Created on: 6 December 2024
 *      Author: jkmingwen
 */
#ifndef IO_INTERFACE_H_
#define IO_INTERFACE_H_

#include "VHDLCommons.h"
#include "VHDLComponent.h"

class IOInterface : public VHDLComponent {
 public:
  IOInterface(int id, std::string direction);

  void portMappingInit();

 private:
  int id;
  bool isInput;
  int codecId;
};


#endif /* IO_INTERFACE_H_ */
