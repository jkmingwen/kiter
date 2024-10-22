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
  void merge_operators(models::Dataflow *const dataflow,
                       parameters_list_t parameters);
  void broadcast_os(models::Dataflow *const dataflow,
                    parameters_list_t parameters);
  void pipeline_buffers(models::Dataflow *const dataflow,
                        parameters_list_t parameters);
  }
  void generateMergedGraph(models::Dataflow* dataflow,
                           std::vector<Vertex> &vertices,
                           int &isOffset, int &osOffset);
  std::string replaceActorName(std::string originalName, const std::string& toReplace,
                               const std::string& replacement, std::vector<TOKEN_UNIT> replacementMask = {0});

  // Merge strategies
  std::vector<std::vector<ARRAY_INDEX>> greedyMerge(models::Dataflow* const dataflow,
                                                    int operatorFreq);
  std::vector<std::vector<ARRAY_INDEX>> smartMerge(models::Dataflow* const dataflow,
                                                   int operatorFreq);
  void findCausalDependency(models::Dataflow *const dataflow, Vertex v,
                            abstractDepGraph &g,
                            std::map<ARRAY_INDEX, bool> &visited);

  void sequentialiseVertices(models::Dataflow *const dataflow, Vertex v1,
                             Vertex v2);
  void pipelineBuffers(models::Dataflow *const dataflow, Vertex src);

}
ADD_TRANSFORMATION(
    MergeOperators,
    transformation_t({"MergeOperators",
                      "Identify operators (via a given merging strategy) and "
                      "merge into a single occurance.",
                      algorithms::transformation::merge_operators}));
ADD_TRANSFORMATION(
    BroadcastOS,
    transformation_t({"BroadcastOS",
                      "Replace output selectors with broadcasts with buffers "
                      "on its output edges (used in conjuction with SDFs that "
                      "have been merged via MergeOperators algorithm).",
                      algorithms::transformation::broadcast_os}));

ADD_TRANSFORMATION(PipelineBuffers,
                   transformation_t({
                       "PipelineBuffers",
                       "Sequentialise buffers that store data from the same "
                       "actor to maximise data re-use.",
                       algorithms::transformation::pipeline_buffers}));

#endif /* SRC_LIBKITER_ALGORITHMS_TRANSFORMATION_MERGE_OPERATORS_H_ */
