/*
 * cycle_detection.cpp
 *
 *  Created on: 29 Dec 2021
 *      Author: toky
 */

#include "cycle_detection.h"
#include <commons/commons.h>
#include <commons/verbose.h>
#include <map>
#include <vector>
#include <algorithms/scc.h>
#include <models/Dataflow.h>
#include <set>

//std::vector<ARRAY_INDEX> dfs_stack;
//std::set<ARRAY_INDEX> blocked_set;
//std::map<ARRAY_INDEX, std::set<ARRAY_INDEX>> blocked_map;
//std::vector<std::vector<ARRAY_INDEX>> simple_cycles;


bool algorithms::has_self_loops            (const models::Dataflow* const  dataflow) {
    return std::any_of(dataflow->vertices().begin(),dataflow->vertices().end(),[&dataflow](Vertex t) { return dataflow->getReentrancyFactor(t) > 0; } );
	for (auto t : dataflow->vertices()) {
		if (dataflow->getReentrancyFactor(t) > 0) {
			return true;
		}
	}
	return false;
}
bool algorithms::has_cycles            (const models::Dataflow* const  dataflow) {
	std::map<int, std::vector<ARRAY_INDEX>> res = algorithms::computeSCCKosaraju(dataflow);
	for (const auto &item : res) {
		VERBOSE_INFO("Component " << item.first << " has " << item.second.size() << " members");
	}
	for (const auto &item : res) {
		if (item.second.size() > 1) return true;
	}
	return false;
}
void algorithms::cycle_detection       (models::Dataflow* const  dataflow, parameters_list_t){
	VERBOSE_INFO("has_cycles result: " << algorithms::has_cycles(dataflow));
	std::cout << "has_cycles result: " << algorithms::has_cycles(dataflow) << std::endl;
}

std::vector<std::vector<ARRAY_INDEX>> algorithms::get_simple_cycles(models::Dataflow* const  dataflow) {

    cycle_detection_cache cache;

  ARRAY_INDEX start_id = 1; // NOTE probably better to check first vertex in dataflow graph rather than assume value of start ID (dataflow->getFirstVertex()?)
  VERBOSE_DEBUG("original graph:" << std::endl);
  VERBOSE_DEBUG(print_status(dataflow) << std::endl);
  while (start_id <= dataflow->getVerticesCount()) {
    models::Dataflow* subgraph = generate_subgraph(dataflow, start_id); // subgraph excludes all vertices less than the specified starting ID
    std::map<int, std::vector<ARRAY_INDEX>> scc_map = algorithms::computeSCCKosaraju(subgraph);
    // find the SCC that contains the vertex with the smallest ID
    ARRAY_INDEX min_id = find_smallest_index(scc_map);
    if (min_id != INT_MAX) {
      models::Dataflow* scc = generate_scc(subgraph, scc_map, min_id);
      VERBOSE_DEBUG("smallest vertex ID amongst SCCs: " << min_id << std::endl);
      VERBOSE_DEBUG(print_status(scc) << std::endl);
        cache.blocked_set.clear();
        cache.blocked_map.clear();
      find_simple_cycles_scc(scc, min_id, min_id, cache);
      start_id = min_id + 1;
    } else {
      break;
    }
  }
  // print detected cycles
  std::cout << "Cycles detected:" << std::endl;
  for (const auto& cycle : cache.simple_cycles) {
    size_t comp_count = 1;
    size_t cycle_size = cycle.size();
    for (auto id : cycle) {
      if (comp_count < cycle_size) {
        std::cout << id << " -> ";
      } else {
        std::cout << id << std::endl;
      }
      comp_count++;
    }
  }
  std::cout << "Total number of cycles: " << cache.simple_cycles.size() << std::endl;
  return cache.simple_cycles;
}

void algorithms::find_simple_cycles(models::Dataflow* const  dataflow, parameters_list_t) {

    std::vector<std::vector<ARRAY_INDEX>> simple_cycles = algorithms::get_simple_cycles(dataflow);

    // print detected cycles
    std::cout << "Cycles detected:" << std::endl;
    for (const auto& cycle : simple_cycles) {
        size_t comp_count = 1;
        size_t cycle_size = cycle.size();
        for (auto id : cycle) {
            if (comp_count < cycle_size) {
                std::cout << id << " -> ";
            } else {
                std::cout << id << std::endl;
            }
            comp_count++;
        }
    }
    std::cout << "Total number of cycles: " << simple_cycles.size() << std::endl;
}

// iterate through SCC map, ignore SCC of size == 1, track minimum ID value
ARRAY_INDEX algorithms::find_smallest_index(const std::map<int, std::vector<ARRAY_INDEX>>& scc_map) {
  ARRAY_INDEX min_id = INT_MAX; // NOTE should be LDBL_MAX but i can't seem to get it to work
  for (auto const& component : scc_map) {
    if (component.second.size() > 1) {
      if (*std::min_element(component.second.begin(), component.second.end()) < min_id) {
        min_id = *std::min_element(component.second.begin(), component.second.end());
      }
    }
  }

  return min_id;
}

// generate SCC that contains start_id
models::Dataflow* algorithms::generate_scc(const models::Dataflow* const dataflow,
                                           const std::map<int, std::vector<ARRAY_INDEX>>& scc_map,
                                           ARRAY_INDEX start_id) {
  auto* scc = new models::Dataflow(*dataflow);
  for (auto const& component : scc_map) {
    if (std::find(component.second.begin(),
                  component.second.end(),
                  start_id) == component.second.end()) {
      continue;
    } else {
      {ForEachVertex(dataflow, v) {
          ARRAY_INDEX actor_id = dataflow->getVertexId(v);
          if (std::find(component.second.begin(),
                        component.second.end(),
                        actor_id) == component.second.end()) {
            scc->removeVertex(scc->getVertexById(actor_id));
          }
        }}
      return scc; // return first SCC with start_id
    }
  }
  return scc;
}
// make subgraph excluding all vertices less than a specified ID
models::Dataflow* algorithms::generate_subgraph(const models::Dataflow* const dataflow,
                                                ARRAY_INDEX lower_limit) {
  auto* subgraph = new models::Dataflow(*dataflow);
  subgraph->reset_computation();
  // remove all actors with IDs less than specified lower_limit
  {ForEachVertex(dataflow, v) {
      ARRAY_INDEX actor_id = dataflow->getVertexId(v);
      if (actor_id < lower_limit) {
        subgraph->removeVertex(subgraph->getVertexById(actor_id));
      }
    }}
  return subgraph;
}

// rescursively remove specified vertex ID from blocked_set while taking into account blocked_map
void algorithms::unblock(ARRAY_INDEX id, cycle_detection_cache& cache ) {
  VERBOSE_DEBUG("removing " << id << " from blocked set" << std::endl);
    cache.blocked_set.erase(cache.blocked_set.find(id));

  VERBOSE_DEBUG("\tblocked set: ");
  for (auto block : cache.blocked_set) {
    VERBOSE_DEBUG(block << " ");
  }
  VERBOSE_DEBUG(std::endl);

  if (cache.blocked_map.find(id) != cache.blocked_map.end()) {
    for (auto actor_id : cache.blocked_map[id]) {
      if (cache.blocked_set.find(actor_id) != cache.blocked_set.end()) {
        unblock(actor_id,cache );
      }
    }
    VERBOSE_DEBUG("removing blocked mapping for all " << id << std::endl);
      cache.blocked_map.erase(cache.blocked_map.find(id));
  }
}

// find simple cycles starting at start_vertex from current_vertex
bool algorithms::find_simple_cycles_scc(const models::Dataflow* const scc,
                                        ARRAY_INDEX start_id, ARRAY_INDEX current_id, cycle_detection_cache& cache) {
  bool found_cycle = false;
    cache.dfs_stack.push_back(current_id);
    cache.blocked_set.insert(current_id);

  {ForOutputEdges(scc, scc->getVertexById(current_id), e) {
      ARRAY_INDEX neighbour_id = scc->getVertexId(scc->getEdgeTarget(e));
      VERBOSE_DEBUG("current_id: " << current_id << std::endl);
      VERBOSE_DEBUG("neighbour_id: " << neighbour_id << std::endl);
      if (neighbour_id == start_id) { // cycle found
        VERBOSE_DEBUG("\tcurrent id == neighbour id; cycle found!" << std::endl);
          cache.dfs_stack.push_back(start_id);
        std::vector<ARRAY_INDEX> cycle(cache.dfs_stack);
        VERBOSE_DEBUG("\tFound cycle: ");
        for (auto i : cycle) {
          VERBOSE_DEBUG(i << " ");
        }
        VERBOSE_DEBUG(std::endl);
          cache.simple_cycles.push_back(cycle);
        // std::cout << "Cycles detected so far: " << simple_cycles.size() << std::endl;
          cache.dfs_stack.pop_back();
        found_cycle = true;
      } else if (cache.blocked_set.find(neighbour_id) == cache.blocked_set.end()) { // neighbouring vertex not in blocked_set
        bool got_cycle = find_simple_cycles_scc(scc, start_id, neighbour_id, cache);
        found_cycle = found_cycle || got_cycle;
      }
    }}
  if (found_cycle) {
    unblock(current_id,cache);
  } else {
    {ForOutputEdges(scc, scc->getVertexById(current_id), e) {
        ARRAY_INDEX block_dep_id = scc->getVertexId(scc->getEdgeTarget(e));
        VERBOSE_DEBUG("Adding " << block_dep_id << "->" << current_id << " to block map" << std::endl);
            cache.blocked_map[block_dep_id].insert(current_id);
      }}
  }
    cache.dfs_stack.pop_back();
  return found_cycle;
}

// prints current status of dataflow graph
std::string algorithms::print_status(const models::Dataflow* const dataflow) {
  std::stringstream output_stream;

  output_stream << "\nActors:" << std::endl;
  {ForEachVertex(dataflow, v) {
      output_stream << dataflow->getVertexName(v) << std::endl;
    }}
  output_stream << "\nConnections:" << std::endl;
  {ForEachEdge(dataflow, e) {
      output_stream << "\tChannel " << dataflow->getEdgeName(e) << " ("
                   << dataflow->getVertexName(dataflow->getEdgeSource(e))
                   << "->"
                   << dataflow->getVertexName(dataflow->getEdgeTarget(e))
                   << "): " << std::endl;
    }}

  return output_stream.str();
}
