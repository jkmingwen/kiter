/*
 *  abstract_dep_graph.cpp
 *
 *  Created on: 8 October 2021
 *      Author: jkmingwen
 */
#include "abstract_dep_graph.h"
#include <models/Dataflow.h>

// abstractDepGraph::abstractDepGraph() {}

abstractDepGraph::abstractDepGraph(models::Dataflow* const dataflow) {
  std::map<ARRAY_INDEX, bool> initialMapping;
  // initialise abstract dependency graph
  {ForEachVertex(dataflow, v) {
      ARRAY_INDEX vId = dataflow->getVertexId(v);
      initialMapping[vId] = false;
    }}
  {ForEachVertex(dataflow, v) {
      ARRAY_INDEX vId = dataflow->getVertexId(v);
      abstractDependencyGraph[vId] = initialMapping;
    }}
}

// add an edge representing a causal dependency between v1 and v2
void abstractDepGraph::addCausalDep(ARRAY_INDEX v1, ARRAY_INDEX v2) {
  // TODO add assert if v1/v2 out of range of abstractDepGraph
  this->abstractDependencyGraph[v1][v2] = true;
}

std::string abstractDepGraph::printStatus() {
  std::stringstream outputStream;

  outputStream << "\nPrinting status of abstract dep graph:" << std::endl;
  for (auto const& v1 : this->abstractDependencyGraph) {
    for (auto const& v2 : v1.second) {
      if (v2.second) {
        outputStream << "\tCausal dependency between " << v1.first << " and "
                     << v2.first << std::endl;
      }
    }
  }

  return outputStream.str();
}
