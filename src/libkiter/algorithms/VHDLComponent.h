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

  Vertex getActor();
  std::string getName();
  ARRAY_INDEX getId();
  std::vector<std::string> getInputPorts();
  std::vector<std::string> getOutputPorts();
  std::vector<std::string> getInputEdges();
  std::vector<std::string> getOutputEdges();
  std::string getType();
  int getLifespan();
  void setName(std::string newName);
  void setId(ARRAY_INDEX newId);
  void setLifespan(int lifespan);
  std::string printStatus();

 private:
  Vertex actor;
  std::string componentName;
  ARRAY_INDEX id;
  std::vector<std::string> inputPorts;
  std::vector<std::string> outputPorts;
  std::string componentType;
  std::vector<std::string> inputEdges;
  std::vector<std::string> outputEdges;
  int lifespan;
};
#endif /* VHDL_COMPONENT_H_ */
