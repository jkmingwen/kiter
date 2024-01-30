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

  Vertex getActor()const;
  std::string getUniqueName()const;
  int getIOId()const;
  const std::vector<std::string>& getInputPorts()const;
  const std::vector<std::string>& getOutputPorts()const;
  const std::vector<std::string>& getInputEdges()const;
  const std::vector<std::string>& getOutputEdges()const;
  std::string getType()const;
  void setFPValue(const float& newVal);
  void setIntValue(const int& newVal);
  float getFPValue() const;
  int getIntValue() const;
  std::string getImplementationName()const;
  const std::vector<std::string>& getArgOrder() const;
  const std::map<std::string, int>& getInputTypes() const;
  const std::map<std::string, int>& getOutputTypes() const;
  void setDataType(const std::string& newType);
  std::string getDataType() const ;
  bool isConst() const;
  bool hasMixedType() const;
  void setUniqueName(const std::string& newName);
  void setImplementationName(const std::string& newName);
  void setIOId(int id);
  std::string printStatus() const ;

 private:
  Vertex actor;
  std::string uniqueName;
  std::vector<std::string> inputPorts;
  std::vector<std::string> outputPorts;
  std::string componentType;
  std::vector<std::string> inputEdges;
  std::vector<std::string> outputEdges;
  std::string implementationName; // for use in instantiating FPC-AXI interface
  bool isConstVal; // true if component generates a constant value (const_val.vhd)
  std::string dataType;
  float fpValue;
  int intValue;
  std::vector<std::string> argOrder; // store order of arguments for noncommutative operators
  std::vector<std::string> uiTypes = {"button", "checkbox", "hslider", "vslider",
                                      "nentry"}; // NOTE temporary workaround to handle UI components
  std::vector<std::string> arithmeticTypes = {"add", "prod", "diff", "div",
                                              "prod", "sqrt", "pow"};
  std::vector<std::string> numOperatorTypes = {"floor", "min", "max", "abs"}; // operators that act on numbers to produce numbers
  std::vector<std::string> routingTypes = {"select2", "select3", "attach", // operators that route input signals to outputs
                                           "vbargraph", "hbargraph"}; // bargraph operators here as their current functionality is simply to send input to output
  std::map<std::string, int> inputTypes;
  std::map<std::string, int> outputTypes;
  bool isMixedType; // true if it has input edges of types int and fp
  int ioId; // if it is of type input/output it will be assigned an ID according to their name (this is so we can assign 0->L, 1-R, etc.)
};
#endif /* VHDL_COMPONENT_H_ */
