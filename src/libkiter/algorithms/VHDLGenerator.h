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

namespace models {
  class Dataflow;
}

namespace algorithms {
  enum ActorType { input, output, add, prod, div };
  void generateVHDL(models::Dataflow* const dataflow,
                                 parameters_list_t);
  void generateOperators(VHDLCircuit &circuit, std::string componentDirectory,
                         bool isBufferless);
  void generateConstOperator(std::string compDir, std::string referenceDir);
  void generateFPCOperator(VHDLComponent comp, std::string compDir,
                           std::string referenceDir);
  void generateOperator(VHDLComponent comp, std::string componentDirectory,
                        std::string referenceDir);
  void generateCircuit(VHDLCircuit &circuit, std::string outputDirectory,
                       bool isBufferless);
  std::string generateComponent(VHDLComponent comp);
  std::string generateBufferComponent(std::string circuitName);
  void generateAXIInterfaceComponents(std::string componentDir,
                                      std::string referenceDir,
                                      bool isBufferless);
  std::vector<std::string> generateSendSigNames(std::string srcPort,
                                                VHDLCircuit circuit);
  std::vector<std::string> generateReceiveSigNames(std::string dstPort,
                                                   VHDLCircuit circuit);
  std::string generatePortMapping(VHDLCircuit circuit, bool isBufferless);
}

ADD_TRANSFORMATION(GenerateVHDL,
                   transformation_t({ "GenerateVHDL" , "Generate VHDL from SDF.", algorithms::generateVHDL}));

#endif /* VHDL_GENERATOR_H_ */
