/*
 * VHDLComponent.h
 *
 *  Created on: 15 June 2021
 *      Author: jkmingwen
 */
#ifndef VHDL_COMPONENT_H_
#define VHDL_COMPONENT_H_

#include <models/Dataflow.h>
#include <string>

namespace models {
  class Dataflow;
}

class VHDLComponent {
 public:
  // VHDLComponent();
  VHDLComponent(models::Dataflow* const dataflow, Vertex a); // actor must be associated to a graph

  std::string getName();
  ARRAY_INDEX getId();
  std::vector<std::string> getInputPorts();
  std::vector<std::string> getOutputPorts();
  std::string getType();
  void setName(std::string newName);
  void setId(ARRAY_INDEX newId);
  std::string printStatus();
 private:
  std::string componentName;
  ARRAY_INDEX id;
  std::vector<std::string> inputPorts;
  std::vector<std::string> outputPorts;
  std::string componentType;
};
#endif /* VHDL_COMPONENT_H_ */
