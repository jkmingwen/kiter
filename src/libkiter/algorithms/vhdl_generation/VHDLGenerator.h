/*
 * VHDLGenerator.h
 *
 *  Created on: 14 June 2021
 *      Author: jkmingwen
 */

#ifndef VHDL_GENERATOR_H_
#define VHDL_GENERATOR_H_

#include <algorithms/schedulings.h>
#include <commons/KiterRegistry.h>
#include "VHDLCircuit.h"
#include "VHDLConnection.h"
#include "VHDLComponent.h"

VHDLCircuit generateCircuitObject(models::Dataflow *const dataflow);
std::string binaryValue(VHDLComponent const comp);
void copyFileAndReplaceWords(
    std::string refFile, std::string dstFile,
    const std::map<std::string, std::string> &replacementMap);
int getOperatorLifespan(const std::string &opType, int operatorFreq);

namespace models {
  class Dataflow;
}

namespace algorithms {
  void generateVHDL(models::Dataflow* const dataflow,
                                 parameters_list_t);
  void generateOperators(VHDLCircuit &circuit);
  void generateConstOperator(std::map<int, int> outputCounts);
  void generateSplitterOperators(std::map<int, int> outputCounts);
  void generateRoutingOperators(VHDLComponent comp);
  void generateFloorOperator(VHDLComponent comp);
  void generateInputOutputSelectorOperator(VHDLComponent comp,
                                           std::map<int, int> inputCounts,
                                           std::map<int, int> outputCounts);
  void generateFPCOperator(std::string compImplementationName);
  void generateUIOperator(VHDLComponent comp);
  void generateOperator(VHDLComponent comp);


    // GenerateCircuit helper functions
    void generateVHDLHeader(std::ofstream &vhdlOutput);
    void generateVHDLEntity(VHDLCircuit &circuit, int numInputPorts, int numOutputPorts, std::ofstream &vhdlOutput);
    void generateVHDLArchitecture(VHDLCircuit &circuit, std::map<std::string, int> &operatorMap,
                                  bool noOperators, std::ofstream &vhdlOutput);


  void generateCircuit(VHDLCircuit &circuit);
  void generateAudioInterfaceWrapper(const VHDLCircuit &circuit);
  std::string generateAudioInterfaceWrapperPorts(int id);
  std::string generateAudioInterfaceWrapperMapping(int id);
  std::string generateI2SToFPCComponent(int inputBitWidth, int outputBitWidth);
  std::string generateFPCToI2SComponent(int inputBitWidth, int outputBitWidth);
  std::string generateInputInterfaceComponent(int bitWidth);
  std::string generateOutputInterfaceComponent(int bitWidth);
  std::string generateI2STransceiverMapping(int id, int bitWidth);
  std::string generateInputInterfaceMapping(int id, int bitWidth);
  std::string generateOutputInterfaceMapping(int id, int bitWidth);
  std::string generateI2SToFPCMapping(int id, std::string entityName);
  std::string generateFPCToI2SMapping(int id, std::string entityName);
  std::string generateI2STransceiverSignalNames(int id, int bitWidth);
  std::string generateInputInterfaceSignalNames(int id, int bitWidth);
  std::string generateOutputInterfaceSignalNames(int id, int bitWidth);
  std::string generateI2SToFPCSignalNames(int id);
  std::string generateFPCToI2SSignalNames(int id);
  std::string generateComponent(VHDLComponent comp);
  std::string generateInputOutputSelectorComponent(VHDLComponent comp,
                                                   std::map<int, int> inputCounts,
                                                   std::map<int, int> outputCounts);
  std::string generateDelayComponent(VHDLComponent comp);
  std::string generateBufferComponent(std::string circuitName);
  std::string generateConstComponents(std::map<int, int> outputCounts);
  std::string generateUIComponents(VHDLComponent comp);
  std::string generateSplitterComponents(std::map<int, int> outputCounts);
  void generateHSInterfaceComponents();
  void generateAudioInterfaceComponents();
  std::vector<std::string> generateSendSigNames(const std::string &srcPort,
                                                const VHDLCircuit &circuit);
  std::vector<std::string> generateReceiveSigNames(const std::string &dstPort,
                                                   const VHDLCircuit &circuit);
  std::string generatePortMapping(const VHDLCircuit &circuit,
                                  bool noOperators);
  void printCircuitInfo(models::Dataflow* const dataflow,
                        parameters_list_t param_list);
}

ADD_TRANSFORMATION(GenerateVHDL,
                   transformation_t({ "GenerateVHDL" , "Generate VHDL from SDF.", algorithms::generateVHDL}));
ADD_TRANSFORMATION(PrintCircuitInfo,
                   transformation_t({ "PrintCircuitInfo" , "Print information about VHDL components that would be generated by GenerateVHDL.", algorithms::printCircuitInfo}));


#endif /* VHDL_GENERATOR_H_ */
