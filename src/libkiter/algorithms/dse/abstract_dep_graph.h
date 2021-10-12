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
  std::string printStatus();
 private:
  std::map<ARRAY_INDEX, std::map<ARRAY_INDEX, bool>> abstractDependencyGraph;
};
#endif /* ABSTRACT_DEP_GRAPH_H_ */
