/*
 *  abstract_dep_graph.h
 *
 *  Created on: 8 October 2021
 *      Author: jkmingwen
 */
#ifndef ABSTRACT_DEP_GRAPH_H_
#define ABSTRACT_DEP_GRAPH_H_

#include <models/Dataflow.h>

namespace models {
  class Dataflow;
}

class abstractDepGraph {
 public:
  abstractDepGraph();
  abstractDepGraph(models::Dataflow* const dataflow);
  void addCausalDep(ARRAY_INDEX v1, ARRAY_INDEX v2);
  std::set<Edge> computeStorageDependencies(models::Dataflow* const dataflow);
  void computeCycles(models::Dataflow* const dataflow, ARRAY_INDEX startActor,
                     std::map<ARRAY_INDEX, bool> &visited,
                     std::map<ARRAY_INDEX, ARRAY_INDEX> &adjActors,
                     std::map<Edge, bool> &storageDeps);
  std::set<Edge> computeStorageDependenciesSCC(models::Dataflow* const dataflow);
  void findStorageDepsFromSCC(models::Dataflow* const dataflow,
                              std::map<int, std::vector<ARRAY_INDEX>> &sccMap,
                              std::map<Edge, bool> &storageDeps);
  void computeDFSStack(ARRAY_INDEX startId,
                       std::map<ARRAY_INDEX, bool> &visitedActors,
                       std::stack<ARRAY_INDEX> &dfsOrder);
  void dfsTranspose(ARRAY_INDEX startId,
                    std::map<ARRAY_INDEX, bool> &visitedActors,
                    std::vector<ARRAY_INDEX> &sccActors);
  void computeExecTime(models::Dataflow* const dataflow,
                       ARRAY_INDEX vId,
                       std::map<ARRAY_INDEX, int> &execTimes);
  std::string printStatus();
 private:
  std::map<ARRAY_INDEX, std::map<ARRAY_INDEX, bool>> abstractDependencyGraph;
};
#endif /* ABSTRACT_DEP_GRAPH_H_ */
