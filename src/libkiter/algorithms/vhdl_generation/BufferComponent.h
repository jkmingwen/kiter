/*
 * BufferComponent.h
 *
 *  Created on: 5 December 2024
 *      Author: jkmingwen
 */
#ifndef BUFFER_COMPONENT_H_
#define BUFFER_COMPONENT_H_

#include "VHDLComponent.h"

class BufferComponent : public VHDLComponent {
public:
  BufferComponent(std::string bufferType);

  void portMappingInit();
  void setPushStart(int cycle);
  void setPopStart(int cycle);
  void setInit(int tokens);
  void setBufferSize(int tokens);
  void setDataWidth(int sizeInBits);
  void setInputSignal(std::string sigName);
  void setOutputSignal(std::string sigName);

private:
  std::string inputSignalName;
  std::string outputSignalName;
  int dataWidth = 24;
  int numInitialTokens = 0;
  int bufferSize = 1;
  int pushStart = 0;
  int popStart = 0;
};


#endif /* BUFFER_COMPONENT_H_ */
