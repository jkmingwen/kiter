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

// identify storage dependencies in abstract causal dependency graph identifying cycles
std::set<Edge> abstractDepGraph::computeStorageDependencies(models::Dataflow* const dataflow) {
  std::map<ARRAY_INDEX, bool> visited;
  std::map<ARRAY_INDEX, ARRAY_INDEX> adjActors;
  std::map<Edge, bool> storageDeps;
  std::set<Edge> storageDepSet;

  {ForEachVertex(dataflow, v) {
      visited[dataflow->getVertexId(v)] = false;
    }}
  {ForEachEdge(dataflow, e) {
      storageDeps[e] = false;
    }}

  {ForEachVertex(dataflow, v) {
      adjActors[dataflow->getVertexId(v)] = dataflow->getVertexId(v);
      computeCycles(dataflow, dataflow->getVertexId(v), visited, adjActors, storageDeps);
    }}

  for (auto const& e : storageDeps) {
    if (e.second) {
      storageDepSet.insert(e.first);
    }
  }

  return storageDepSet;
}

// Compute cycles within the abstract dependency graph
void abstractDepGraph::computeCycles(models::Dataflow* const dataflow,
                                     ARRAY_INDEX startActor,
                                     std::map<ARRAY_INDEX, bool> &visited,
                                     std::map<ARRAY_INDEX, ARRAY_INDEX> &adjActors,
                                     std::map<Edge, bool> &storageDeps) {
  visited[startActor] = true;

  {ForEachVertex(dataflow, v) {
      ARRAY_INDEX dstActor = dataflow->getVertexId(v);
      if (this->abstractDependencyGraph[startActor][dstActor]) { // causal dep exists
        if (visited[dstActor]) {
          ARRAY_INDEX v1 = startActor;
          ARRAY_INDEX v2 = dstActor;
          do {
            {ForEachEdge(dataflow, e) {
                ARRAY_INDEX src = dataflow->getVertexId(dataflow->getEdgeSource(e));
                ARRAY_INDEX dst = dataflow->getVertexId(dataflow->getEdgeTarget(e));
                if (src == v1 && dst == v2) { // edge connects the two actors
                  storageDeps[e] = true;
                }
              }}
            // walk back along connected edges
            v2 = v1;
            v1 = adjActors[v2];
          } while (v2 != dstActor);
        } else {
          adjActors[dstActor] = startActor;
          computeCycles(dataflow, dstActor, visited, adjActors, storageDeps);
        }
      }
    }}

  {ForEachVertex(dataflow, v) {
      ARRAY_INDEX vId = dataflow->getVertexId(v);
      this->abstractDependencyGraph[vId][startActor] = false;
      this->abstractDependencyGraph[startActor][vId] = false;
    }}
  visited[startActor] = false;
}

std::string abstractDepGraph::printStatus() {
  std::stringstream outputStream;

  outputStream << "Printing status of abstract dep graph:" << std::endl;
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
