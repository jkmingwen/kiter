/*
 * VHDLConnection.h
 *
 *  Created on: 15 June 2021
 *      Author: jkmingwen
 */
#ifndef VHDL_CONNECTION_H_
#define VHDL_CONNECTION_H_

#include <models/Dataflow.h>
#include <string>

namespace models {
  class Dataflow;
}

class VHDLConnection {
 public:
  // VHDLConnection();
  VHDLConnection(models::Dataflow* const dataflow, Edge e); // actor must be associated to a graph

  std::string getName();
  ARRAY_INDEX getId();
  std::string getSrcPort();
  std::string getDstPort();
  std::string getType();
  TOKEN_UNIT getBufferSize();
  int getTypeWidth();
  void setName(std::string newName);
  void setId(ARRAY_INDEX newId);
  std::string printStatus();
  
 private:
  std::string connectionName;
  ARRAY_INDEX id;
  std::string dataType;
  TOKEN_UNIT bufferSize;
  int dataTypeWidth;
  std::string srcPort;
  std::string dstPort;
};
#endif /* VHDL_CONNECTION_H_ */
