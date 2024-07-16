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
#include "VHDLCommons.h"

namespace models {
  class Dataflow;
}

class VHDLConnection {
 public:
  // VHDLConnection();
  VHDLConnection(models::Dataflow* const dataflow, Edge e); // actor must be associated to a graph

  Edge getEdge() const;
  const std::string &getName() const {
    return this->connectionName;
  }
  ARRAY_INDEX getId() const;
  std::string getSrcPort() const;
  std::string getDstPort() const;
  std::string getType() const;
  TOKEN_UNIT getBufferSize() const;
  TOKEN_UNIT getInitialTokenCount() const;
  int getTypeWidth() const;
  std::vector<TOKEN_UNIT> getInputVector() const;
  std::vector<TOKEN_UNIT> getOutputVector() const;
  void setName(std::string newName);
  void setId(ARRAY_INDEX newId);
  void setBufferSize(TOKEN_UNIT size);
  void setInitialTokenCount(TOKEN_UNIT count);
  std::map<std::string, std::vector<std::string>> genSignalNames(implType t) const; // signal type -> signal names
  std::string printStatus() const;

 private:
  Edge edge;
  std::string connectionName;
  ARRAY_INDEX id;
  std::string dataType;
  TOKEN_UNIT bufferSize;
  TOKEN_UNIT initialTokenCount;
  int dataTypeWidth;
  std::string srcPort;
  std::string dstPort;
  std::vector<TOKEN_UNIT> inputVector;
  std::vector<TOKEN_UNIT> outputVector;
};
#endif /* VHDL_CONNECTION_H_ */
