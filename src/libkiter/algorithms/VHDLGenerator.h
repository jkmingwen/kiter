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
  void generateComponents(VHDLCircuit circuit, std::string componentDirectory);
  void generateComponent(VHDLComponent comp, std::string componentDirectory);
  /* std::string printStatus(models::Dataflow* const dataflow); */
}

ADD_TRANSFORMATION(GenerateVHDL,
                   transformation_t({ "GenerateVHDL" , "Generate VHDL from SDF.", algorithms::generateVHDL}));

#endif /* VHDL_GENERATOR_H_ */
