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

// identify storage dependencies in abstract causal dependency graph by identifying SCCs
std::set<Edge> abstractDepGraph::computeStorageDependenciesSCC(models::Dataflow* const dataflow) {
  std::map<ARRAY_INDEX, bool> visited;
  std::stack<ARRAY_INDEX> dfsOrder;
  std::vector<ARRAY_INDEX> sccActors;
  std::map<int, std::vector<ARRAY_INDEX>> sccMap; // track edge IDs split by SCC
  std::map<Edge, bool> storageDeps;
  std::set<Edge> storageDepSet;
  int sccCount = 1;

  {ForEachVertex(dataflow, v) {
      visited[dataflow->getVertexId(v)] = false;
    }}
  {ForEachEdge(dataflow, e) {
      storageDeps[e] = false;
    }}
  for (auto const& it : visited) {
    if (!visited[it.first]) {
      // populate visited actors by computing the DFS stack
      computeDFSStack(it.first, visited, dfsOrder);
    }
  }

  // reset visited actor list
  for (auto const& it : visited) {
    visited[it.first] = false;
  }
  // perform transposed version of dfs according to order stack
  while (!dfsOrder.empty()) {
    ARRAY_INDEX firstId = dfsOrder.top();
    dfsOrder.pop();
    if (!visited[firstId]) { // only perform DFS on actors in stack that haven't been visited
      dfsTranspose(firstId, visited, sccActors);
      // add to SCC map and reset for next component
      sccMap[sccCount] = sccActors;
      sccCount++;
      sccActors.clear();
    }
  }

  VERBOSE_DEBUG("Number of SCCs in abstract dependency graph: " << sccMap.size() << std::endl);
  if (VERBOSE_IS_DEBUG())
  for (auto const& actorList : sccMap) {
    VERBOSE_DEBUG("\tSCC ID: " << actorList.first << std::endl);
    for (auto const& actorId : actorList.second) {
      VERBOSE_DEBUG("\t\t" << actorId << std::endl);
    }
  }

  findStorageDepsFromSCC(dataflow, sccMap, storageDeps);
  for (auto const& e : storageDeps) {
    if (e.second) {
      storageDepSet.insert(e.first);
    }
  }

  return storageDepSet;
}

void abstractDepGraph::findStorageDepsFromSCC(models::Dataflow* const dataflow,
                                              std::map<int, std::vector<ARRAY_INDEX>> &sccMap,
                                              std::map<Edge, bool> &storageDeps) {
  {ForEachEdge(dataflow, e) {
      ARRAY_INDEX srcId = dataflow->getVertexId(dataflow->getEdgeSource(e));
      ARRAY_INDEX dstId = dataflow->getVertexId(dataflow->getEdgeTarget(e));
      if (this->abstractDependencyGraph[srcId][dstId]) {
        for (auto const& component : sccMap) {
          // check if both actors are part of the same SCC in abstract dep graph
          bool srcActorInSCC = std::find(component.second.begin(),
                                         component.second.end(), srcId) != component.second.end();
          bool dstActorInSCC = std::find(component.second.begin(),
                                         component.second.end(), dstId) != component.second.end();
          if (srcActorInSCC && dstActorInSCC) {
            storageDeps[e] = true;
          }
        }
      }
    }}
}

void abstractDepGraph::computeDFSStack(ARRAY_INDEX startId,
                                       std::map<ARRAY_INDEX, bool> &visited,
                                       std::stack<ARRAY_INDEX> &dfsOrder) {
  if (visited[startId]) {
    return;
  }
  visited[startId] = true;
  // iterate through outgoing edges (absDepGraph[startId][adjId]) and add to stack
  for (auto const& adjId : this->abstractDependencyGraph[startId]) {
    if (this->abstractDependencyGraph[startId][adjId.first]) { // edge exists from startId->adjId in absDepGraph
      computeDFSStack(adjId.first, visited, dfsOrder);
    }
  }
  dfsOrder.push(startId);
}

void abstractDepGraph::dfsTranspose(ARRAY_INDEX startId,
                                    std::map<ARRAY_INDEX, bool> &visited,
                                    std::vector<ARRAY_INDEX> &sccActors) {
  if (visited[startId]) {
    return;
  }
  visited[startId] = true;
  sccActors.push_back(startId);
  // iterate through incoming edges (absDepGraph[adjId][startId]) and add to stack
  for (auto const& adjId : this->abstractDependencyGraph) {
    if (this->abstractDependencyGraph[adjId.first][startId]) {
      dfsTranspose(adjId.first, visited, sccActors);
    }
  }
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

// Computes the execution times of each execution in the abstract dependency graph
/* Performs a breadth-first search, incrementing the time by adding the given actor's
   execution time to its parent's. Note that this function has been designed to be
   called on the root nodes in the abstract dependency graph. */
void abstractDepGraph::computeExecTime(models::Dataflow* const dataflow,
                                       ARRAY_INDEX vId,
                                       std::map<ARRAY_INDEX, int> &execTimes) {
  std::map<ARRAY_INDEX, bool> visited;
  std::list<ARRAY_INDEX> visitQueue;
  {ForEachVertex(dataflow, v) { // initialise visit queue to avoid visiting same actors twice
      visited[dataflow->getVertexId(v)] = false;
    }}
  visited[vId] = true;
  visitQueue.push_back(vId);
  execTimes[vId] = 0;

  while (!visitQueue.empty()) {
    vId = visitQueue.front();
    visitQueue.pop_front();

    for (auto &adj : this->abstractDependencyGraph[vId]) {
      if (adj.second) { // this vertex is adjacent to vId
        if (!visited[adj.first]) {
          visited[adj.first] = true;
          visitQueue.push_back(adj.first);
          VERBOSE_ASSERT(dataflow->getVertexPhaseDuration(dataflow->getVertexById(adj.first)).size() == 1, "Currently only supports computing execution timings of single phase actors.")
            int opLifespan = dataflow->getVertexPhaseDuration(dataflow->getVertexById(adj.first)).front(); // TODO set exec time to that of operator
          if (execTimes[adj.first] <= execTimes[vId] + opLifespan) { // always set to any higher existing computed exec time
            execTimes[adj.first] = execTimes[vId] + opLifespan;
          }
        }
      }
    }
  }

  return;
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
