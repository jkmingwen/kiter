/*
 * VHDLWrapper.h
 *
 *  Created on: 2 December 2024
 *      Author: jkmingwen
 */
#ifndef VHDL_WRAPPER_H_
#define VHDL_WRAPPER_H_

#include "VHDLCommons.h"
#include "VHDLComponent.h"
#include "VHDLCircuit.h"
#include "BufferComponent.h"

class VHDLWrapper : public VHDLComponent {
public:
  VHDLWrapper();
  VHDLWrapper(implType t, int sysPeriod, int sysSlack);
  VHDLWrapper(VHDLCircuit circuit, implType t, int sysPeriod, int sysSlack);

  void portMappingInit();
  void internalSignalsInit();
  void addInternalSignal(std::string sigName, std::string type, int dataWidth = 34);
  std::string genInternalSigs();
  void writeImplementation(std::ofstream &vhdlOutput);
  void initialiseWrapper(const VHDLCircuit &circuit);
  VHDLComponent genComponent(std::string componentType);
  BufferComponent *genIOBufferTT(std::string inSigName,
                                 std::string outSigName,
                                 int pushStart, int popStart);

private:
  int numInputs;
  int numOutputs;
  int numAudioCodecs;
  std::map<std::string, std::vector<std::string>> internalSignals;   // signal type -> signal names
  std::map<std::string, std::string> internalMapping;
  std::vector<std::unique_ptr<VHDLComponent>> components;
  int period;
  int slack;
  VHDLCircuit dspCircuit;
  std::string dspName;
  std::map<int, TIME_UNIT> computeTimes;
};
#endif /* VHDL_WRAPPER_H_ */
