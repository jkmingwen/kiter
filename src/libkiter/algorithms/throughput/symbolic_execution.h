/*
 * symbolic_execution.h
 *
 *  Created on: 21 December 2020
 *      Author: jkmingwen
 */

#ifndef SYMBOLIC_EXECUTION_H_
#define SYMBOLIC_EXECUTION_H_

#include <algorithms/schedulings.h>
#include <commons/KiterRegistry.h>
#include "state.h"

namespace models {
  class Dataflow;
}

namespace algorithms {
  void compute_asap_throughput(models::Dataflow* const dataflow,
                               parameters_list_t);
  TIME_UNIT computeComponentThroughput(models::Dataflow* const dataflow,
                                       std::pair<ARRAY_INDEX, EXEC_COUNT> &minActorInfo,
                                       std::map<Edge, TOKEN_UNIT> &bufferSizes);
  std::pair<TIME_UNIT, scheduling_t> computeComponentThroughputSchedule(models::Dataflow* const dataflow,
                                       std::pair<ARRAY_INDEX, EXEC_COUNT> &minActorInfo, scheduling_t schedule);
  std::vector<models::Dataflow*> generateSCCs(models::Dataflow* const dataflow,
                                              std::map<int, std::vector<ARRAY_INDEX>> sccMap);
  // std::map<ARRAY_INDEX, std::map<ARRAY_INDEX, bool>> computeDeadlockCausalDeps(models::Dataflow* const dataflow,
  //                                                                              State &s,
  //                                                                              std::map<ARRAY_INDEX, Actor> actorMap);
  void computeDeadlockCausalDeps(models::Dataflow* const dataflow,
                                 State &s,
                                 std::map<ARRAY_INDEX, Actor> actorMap);
  std::string printStatus(models::Dataflow* const dataflow, State &s);

}

ADD_TRANSFORMATION(SymbExec,
                   transformation_t({ "SymbExec" , "Execute graph using ASAP scheduling.", algorithms::compute_asap_throughput}));

#endif /* SYMBOLIC_EXECUTION_H_ */
