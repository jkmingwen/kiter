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
  void setStartTimes(std::vector<TIME_UNIT> times,
                     std::vector<TIME_UNIT> popTime, TIME_UNIT slack = 0);
  std::vector<TIME_UNIT> getStartTimes() const;
  void addPortMapping(std::string port, std::string signal, std::string type,
                      std::string direction, int dataWidth = 34);
  void addHSPortMapping(std::string portPrefix, std::string signal,
                        int id, std::string direction);
  void addGenericMapping(std::string port, std::string signal,
                         std::string type, int dataWidth =34);
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
  std::string genDeclaration() const; // generate instantiation of component (when used in another component)
  std::string genEntityDecl() const; // generate entity declaration (for component itself)
  std::string genPortMapping(int id, std::map<std::string, std::string> replacements) const; // generate the port mapping code given a mapping of port names to signal names
  std::string getPortMapName() const;
  void genImplementation(std::string refDir, std::string dstDir) const;

  // temporary function to log shift register numbers
  std::string writePIPOCSV() const;
  std::map<std::string, int> getPIPONumbers();

 private:
  Vertex actor;
  implType implementationType;
  int opFreq;
  int opLifespan;
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
  std::map<std::string, std::string> ports;        // port -> signal type
  std::string portMapName; // for use when instantiating component in port mapping
  std::string implRefName; // name used to reference implementation // TODO
                           // simplify all the different "names"
  std::map<std::string, std::string>
      implReplacementMap; // key words that need to replaced to properly define
                          // the implementation of the given component
  std::map<std::string, int> pipoNumbers;

};
#endif /* VHDL_COMPONENT_H_ */
