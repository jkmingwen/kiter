/*
 *  merge_operators.h
 *
 *  Created on: Sept 5, 2023
 *      Author: jkmingwen
 */

#ifndef SRC_LIBKITER_ALGORITHMS_TRANSFORMATION_MERGE_OPERATORS_H_
#define SRC_LIBKITER_ALGORITHMS_TRANSFORMATION_MERGE_OPERATORS_H_



#include <string>
#include <vector>
#include <commons/commons.h>
#include <models/Dataflow.h>
#include <commons/KiterRegistry.h>
#include "../dse/abstract_dep_graph.h"


namespace models {
  class Dataflow;
  class Scheduling;
}

namespace algorithms {
  namespace transformation {
    void merge_operators(models::Dataflow* const dataflow, parameters_list_t  parameters);
  }
  /* bool checkForNumericInputs(models::Dataflow* const dataflow, Vertex v); */
  /* bool checkForStaticDelay(models::Dataflow* const dataflow, Vertex v, */
  /*                          Edge &inputSig, Edge &delayArg, int &delayAmt); */
  /* std::string evalBinop(std::string op, std::string arg1, std::string arg2); */
  /* std::string evalCast(std::string op, std::string arg); */
  /* std::string evalOther(std::string op, std::vector<std::string> args); */
  /* std::string evalTrig(std::string op, std::string arg); */
  /* void bypassDelay(models::Dataflow* const dataflow, Vertex v, */
  /*                  Edge inputSig, Edge delayArg, int delayAmt); */
  /* void bypassProj(models::Dataflow* const dataflow, Vertex v); */
  /* void routeMultiOutDelay(models::Dataflow* const dataflow, Vertex v); */
  /* void applyResult(models::Dataflow* const dataflow, Vertex v, std::string result); */
  /* int getChannelNumber(std::string channelName); */
  void generateMergedGraph(models::Dataflow* dataflow,
                           std::vector<Vertex> &vertices,
                           int &isOffset, int &osOffset);
  void addReentrancy(models::Dataflow* const dataflow, Vertex v,
                     std::string actorName, std::vector<TOKEN_UNIT> execRates);
  std::string replaceActorName(std::string originalName, const std::string& toReplace,
                               const std::string& replacement);

  // Merge strategies
  std::vector<std::vector<ARRAY_INDEX>> greedyMerge(models::Dataflow* const dataflow,
                                                    int operatorFreq);
  std::vector<std::vector<ARRAY_INDEX>> smartMerge(models::Dataflow* const dataflow,
                                                   int operatorFreq);
  void findCausalDependency(models::Dataflow* const dataflow, Vertex v,
                            abstractDepGraph &g, std::map<ARRAY_INDEX, bool> &visited);

}
ADD_TRANSFORMATION(MergeOperators,
                   transformation_t({ "MergeOperators" , "Identify operators (via a given merging strategy) and merge into a single occurance.", algorithms::transformation::merge_operators}));


#endif /* SRC_LIBKITER_ALGORITHMS_TRANSFORMATION_MERGE_OPERATORS_H_ */
