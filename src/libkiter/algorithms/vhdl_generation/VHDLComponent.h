/*
 * VHDLComponent.h
 *
 *  Created on: 15 June 2021
 *      Author: jkmingwen
 */
#ifndef VHDL_COMPONENT_H_
#define VHDL_COMPONENT_H_

#include <bitset>
#include <models/Dataflow.h>
#include <string>

namespace models {
  class Dataflow;
}

std::string floatToBinary(float f); // for binary representation of constant value components

class VHDLComponent {
 public:
  // VHDLComponent();
  VHDLComponent(models::Dataflow* const dataflow, Vertex a); // actor must be associated to a graph

  Vertex getActor()const;
  std::string getName()const;
  ARRAY_INDEX getId()const;
  const std::vector<std::string>& getInputPorts()const;
  const std::vector<std::string>& getOutputPorts()const;
  const std::vector<std::string>& getInputEdges()const;
  const std::vector<std::string>& getOutputEdges()const;
  std::string getType()const;
  int getLifespan()const;
  std::string getBinaryValue()const;
  std::string getImplementationName()const;
  const std::vector<std::string>& getArgOrder() const;
  const std::map<std::string, int>& getInputTypes() const;
  const std::map<std::string, int>& getOutputTypes() const;
  std::string getDataType() const ;
  bool isConst() const;
  bool hasMixedType() const;
  void setName(std::string newName);
  void setId(ARRAY_INDEX newId);
  void setLifespan(int lifespan);
  void setImplementationName(std::string newName);
  void convConstIntToFloat();
  std::string printStatus() const ;

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
  std::string implementationName; // for use in instantiating FPC-AXI interface
  bool isConstVal; // true if component generates a constant value (const_val.vhd)
  std::string dataType; // TODO add operator type ("fp/int")
  float fpValue;
  int intValue;
  std::string binaryValue; // binary representation of 'value'
  std::vector<std::string> argOrder; // store order of arguments for noncommutative operators
  std::vector<std::string> uiTypes = {"button", "checkbox", "hslider", "nentry",
                                      "vslider"}; // NOTE temporary workaround to handle UI components
  std::vector<std::string> arithmeticTypes = {"add", "prod", "diff", "div",
                                              "prod", "sqrt", "pow"};
  std::vector<std::string> numOperatorTypes = {"floor", "min", "max", "abs"}; // operators that act on numbers to produce numbers
  std::map<std::string, int> inputTypes;
  std::map<std::string, int> outputTypes;
  bool isMixedType; // true if it has input edges of types int and fp
};
#endif /* VHDL_COMPONENT_H_ */
