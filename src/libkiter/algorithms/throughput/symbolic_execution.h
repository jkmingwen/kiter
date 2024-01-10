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
  void compute_asap_throughput_wrapper(models::Dataflow* const dataflow,
                                       parameters_list_t param_list);
  TIME_UNIT compute_asap_throughput(models::Dataflow* const dataflow,
                                    parameters_list_t param_list);
  void compute_asap_throughput_and_cycles_debug(models::Dataflow* const dataflow,
                                                parameters_list_t param_list);
  kperiodic_result_t compute_asap_throughput_and_cycles(models::Dataflow* const dataflow,
                                                        parameters_list_t param_list,
                                                        StorageDistribution &storDist);
  TIME_UNIT computeComponentThroughput(models::Dataflow* const dataflow,
                                       std::pair<ARRAY_INDEX, EXEC_COUNT> &minActorInfo);
  kperiodic_result_t computeComponentThroughputCycles(models::Dataflow* const dataflow,
                                                      std::pair<ARRAY_INDEX, EXEC_COUNT> &minActorInfo,
                                                      StorageDistribution &storDist,
                                                      bool useCorrectedStorDepDetection);
  std::pair<TIME_UNIT, scheduling_t> computeComponentThroughputSchedule(models::Dataflow* const dataflow,
                                       std::pair<ARRAY_INDEX, EXEC_COUNT> &minActorInfo, scheduling_t schedule);
  std::vector<models::Dataflow*> generateSCCs(models::Dataflow* const dataflow,
                                              std::map<int, std::vector<ARRAY_INDEX>> sccMap);
  std::set<Edge> computePeriodicStorageDeps(models::Dataflow* const dataflow,
                                            State &currState,
                                            State &prevState,
                                            ActorMap_t &actorMap,
                                            ARRAY_INDEX minRepActorId,
                                            EXEC_COUNT minRepFactor,
                                            TOKEN_UNIT minRepActorExecCount,
                                            bool useCorrectedStorDepDetection);
  std::set<Edge> computeDeadlockStorageDeps(models::Dataflow* const dataflow,
                                            State &s,
                                            ActorMap_t &actorMap,
                                            bool useCorrectedStorDepDetection);
  std::string printStatus(models::Dataflow* const dataflow, State &s);

}

ADD_TRANSFORMATION(SymbExec,
                   transformation_t({ "SymbExec" , "Execute graph using ASAP scheduling.", algorithms::compute_asap_throughput_wrapper}));
ADD_TRANSFORMATION(SymbExecDebug,
                   transformation_t({ "SymbExecDebug" , "Test SymbExec with preset storage distribution.", algorithms::compute_asap_throughput_and_cycles_debug}));

#endif /* SYMBOLIC_EXECUTION_H_ */
