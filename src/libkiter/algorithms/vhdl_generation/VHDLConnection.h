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

  // Code generation methods
  void addPortMapping(std::string port, std::string signal, std::string type,
                      std::string direction, bool isGeneric = false,
                      int dataWidth = 34);
  std::string genPortList(std::map<std::string, std::string> portMap,
                          bool terminate = true,
                          std::map<std::string, std::string> replacements =
                          std::map<std::string, std::string>(),
                          std::string relation = ":",
                          std::string delim = ";",
                          std::string term = "") const;
  std::string genDeclaration() const; // generate instantiation of component
  std::string genPortMapping(int id, std::map<std::string, std::string> replacements) const; // generate the port mapping code given a mapping of port names to signal names

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
  std::string bufferInstanceName;
  std::string bufferImplRefName;

  // for intermediate component instantiation
  std::map<std::string, std::string> genericMappings; // generic port name -> signal
  std::map<std::string, std::string> portMappings;    // port name -> signal
  std::map<std::string, std::string> genericPorts; // generic port -> signal type
  std::map<std::string, std::string> ports; // port -> signal type
};
#endif /* VHDL_CONNECTION_H_ */
