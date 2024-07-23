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
#include "VHDLCommons.h"

namespace models {
  class Dataflow;
}

std::string getNameFromPartialName(models::Dataflow *const dataflow,
                                   const std::string &partialName);

class VHDLComponent {
 public:
  // VHDLComponent();
   VHDLComponent(models::Dataflow *const dataflow, Vertex a,
                 implType t = TT, int freq = 250);

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
  const std::map<std::string, int> &getOutputTypes() const;
  const std::vector<std::string> getHSInputSignals() const;
  const std::vector<std::string> getHSOutputSignals() const;
  const std::vector<std::string> getInputSignals() const;
  const std::vector<std::string> getOutputSignals() const;
  void setDataType(const std::string& newType);
  std::string getDataType() const ;
  bool isConst() const;
  bool isUI() const;
  bool hasMixedType() const;
  void setUniqueName(const std::string& newName);
  void setImplementationName(const std::string& newName);
  void setIOId(int id);
  void addHSInputSignal(const std::string& signalName);
  void addHSOutputSignal(const std::string &signalName);
  void addInputSignal(models::Dataflow *const dataflow, const Edge e);
  void addOutputSignal(models::Dataflow *const dataflow, const Edge e);
  int getSBufferInitTokens() const;
  void setStartTimes(std::vector<TIME_UNIT> times);
  std::vector<TIME_UNIT> getStartTimes() const;
  void addPortMapping(std::string port, std::string signal, std::string type,
                      std::string direction, bool isGeneric = false,
                      int dataWidth = 34);
  implType getImplType() const;
  std::map<std::string, std::string> getPortMapping() const;
  std::string printStatus() const;

  // Code generation methods
  std::string genPortList(std::map<std::string, std::string> portMap,
                          bool terminate = true,
                          std::map<std::string, std::string> replacements =
                              std::map<std::string, std::string>(),
                          std::string relation = ":",
                          std::string delim = ";",
                          std::string term = "") const;
  std::string genDeclaration() const; // generate instantiation of component
  std::string genPortMapping(int id, std::map<std::string, std::string> replacements) const; // generate the port mapping code given a mapping of port names to signal names
  std::string getPortMapName() const;

 private:
  Vertex actor;
  implType implementationType;
  int opFreq;
  std::string uniqueName;
  std::vector<std::string> inputPorts;
  std::vector<std::string> outputPorts;
  std::string componentType;
  std::vector<std::string> inputEdges;
  std::vector<std::string> outputEdges;
  std::vector<std::string> hsInputSignals;
  std::vector<std::string> hsOutputSignals;
  std::vector<std::string> inputSignals;
  std::vector<std::string> outputSignals;
  std::string sharedOutputSignal;
  std::string implementationName; // for use in instantiating FPC-AXI interface
  bool isConstVal; // true if component generates a constant value (const_val.vhd)
  std::string dataType;
  float fpValue;
  int intValue;
  std::string binaryValue; // binary value reflecting FP/Int value of a const_val component
  std::vector<std::string> argOrder; // store order of arguments for noncommutative operators
  std::map<std::string, int> inputTypes;
  std::map<std::string, int> outputTypes;
  bool isMixedType; // true if it has input edges of types int and fp
  bool isUIType;
  int ioId; // if it is of type input/output it will be assigned an ID according
            // to their name (this is so we can assign 0->L, 1-R, etc.)
  std::vector<TIME_UNIT> startTimes;
  std::map<std::string, std::string> genericMappings; // generic port name -> signal
  std::map<std::string, std::string> portMappings;    // port name -> signal
  std::map<std::string, std::string> genericPorts; // generic port -> signal type
  std::map<std::string, std::string> ports; // port -> signal type
  std::string portMapName; // for use when instantiating component in port mapping
  std::string implRefName; // name used to reference implementation // TODO simplify all the different "names"

  std::map<std::string, std::vector<std::string>> opInputPorts = {
      {"fp_add", {"X", "Y"}},
      {"fp_prod", {"X", "Y"}},
      {"fp_div", {"X", "Y"}},
      {"fp_sqrt", {"X"}},
      {"fp_diff", {"X", "Y"}},
      {"fp_pow", {"X", "Y"}},
      {"int_add", {"X", "Y"}},
      {"int_prod", {"X", "Y"}},
      {"int_diff", {"X", "Y"}},
      {"float2int", {"X"}},
      {"int2float", {"X"}},
      {"sbuffer", {"in_data"}},
      {"int_max", {"X", "Y"}},
      {"int_min", {"X", "Y"}},
      {"fp_max", {"X", "Y"}},
      {"fp_min", {"X", "Y"}},
      {"fp_abs", {"X"}},
      {"int_abs", {"X"}},
      {"select2", {"X", "Y", "Z"}},
      {"attach", {"X", "Y"}} // ,
      // {"vbargraph", {"X"}},
      // {"fp_floor", {"op_in_data_0"}}
  };

  std::map<std::string, std::vector<std::string>> opOutputPorts = {
      {"fp_add", {"R"}},    {"fp_prod", {"R"}},   {"fp_div", {"R"}},
      {"fp_sqrt", {"R"}},   {"fp_diff", {"R"}},   {"fp_pow", {"R"}},
      {"int_add", {"R"}},   {"int_prod", {"R"}},  {"int_diff", {"R"}},
      {"float2int", {"R"}}, {"int2float", {"R"}}, {"sbuffer", {"out_data"}},
      {"int_max", {"R"}},   {"int_min", {"R"}},   {"fp_max", {"R"}},
      {"fp_min", {"R"}},    {"fp_abs", {"R"}},    {"int_abs", {"R"}},
      {"select2", {"R"}},   {"attach", {"R"}} // ,
                                              // {"vbargraph", {"X"}},
                                              // {"fp_floor", {"op_in_data_0"}}
  };

  std::map<std::string, std::string> implementationNames = {
      {"fp_add", "fp_add_flopoco"},
      {"fp_prod", "fp_prod_flopoco"},
      {"fp_div", "fp_div_flopoco"},
      {"fp_sqrt", "fp_sqrt_flopoco"},
      {"fp_diff", "fp_diff_flopoco"},
      {"fp_pow", "fp_pow_flopoco"},
      {"int_add", "int_add_flopoco"},
      {"int_diff", "int_diff_flopoco"},
      {"int_prod", "int_prod_flopoco"},
      {"float2int", "float2int_flopoco"},
      {"int2float", "int2float_flopoco"},
      {"fp_floor", "fp_floor"},
      {"int_max", "int_max"},
      {"int_min", "int_min"},
      {"fp_max", "fp_max"},
      {"fp_min", "fp_min"},
      {"fp_abs", "fp_abs"},
      {"select2", "select2"},
      {"attach", "attach"},
      {"delay", "delay"}, // NOTE delay doesn't have a fixed lifespan, which is
      // why it's not in the operatorLifespan map
      {"int_abs", "int_abs"},
      {"vbargraph", "vbargraph"},
      {"hbargraph", "hbargraph"},
      {"hslider", "hslider"},
      {"vslider", "vslider"},
      {"nentry", "nentry"},
      {"checkbox", "checkbox"},
      {"button", "button"},
      {"sbuffer", "sbuffer"},
      {"Proj", "hs_splitter"}};

  std::vector<std::string> uiTypes = {"button", "checkbox", "hslider", "vslider",
                                      "nentry", "vbargraph", "hbargraph"}; // NOTE temporary workaround to handle UI components
  std::vector<std::string> arithmeticTypes = {"add", "prod", "diff", "div",
                                              "prod", "sqrt", "pow"};
  std::vector<std::string> numOperatorTypes = {"floor", "min", "max", "abs"}; // operators that act on numbers to produce numbers
  std::vector<std::string> routingTypes = {"select2", "select3", "attach"}; // operators that route input signals to outputs
};
#endif /* VHDL_COMPONENT_H_ */
