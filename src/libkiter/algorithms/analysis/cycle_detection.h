/*
 * cycle_detection.h
 *
 *  Created on: 29 Dec 2021
 *      Author: toky
 */

#ifndef SRC_LIBKITER_ALGORITHMS_ANALYSIS_CYCLE_DETECTION_H_
#define SRC_LIBKITER_ALGORITHMS_ANALYSIS_CYCLE_DETECTION_H_


#include <commons/KiterRegistry.h>
#include <algorithms/schedulings.h>

namespace models {
	class Dataflow;
}

struct cycle_detection_cache {
    std::vector<ARRAY_INDEX> dfs_stack {};
    std::set<ARRAY_INDEX> blocked_set{};
    std::map<ARRAY_INDEX, std::set<ARRAY_INDEX>> blocked_map{};
    std::vector<std::vector<ARRAY_INDEX>> simple_cycles{};
};

namespace algorithms {

bool has_self_loops         (const models::Dataflow* const  dataflow);
bool has_cycles            (const models::Dataflow* const  dataflow);
void cycle_detection       (models::Dataflow* const  dataflow, parameters_list_t);
  void find_simple_cycles(models::Dataflow* const  dataflow, parameters_list_t);

  std::vector<std::vector<ARRAY_INDEX>> get_simple_cycles(models::Dataflow* const  dataflow);

  ARRAY_INDEX find_smallest_index(const std::map<int, std::vector<ARRAY_INDEX>>& scc_map);
  models::Dataflow* generate_scc(const models::Dataflow* const dataflow,
                                 const std::map<int, std::vector<ARRAY_INDEX>>& scc_map,
                                 ARRAY_INDEX start_id);
  models::Dataflow* generate_subgraph(const models::Dataflow* const dataflow,
                                      ARRAY_INDEX lower_limit);
  void unblock(ARRAY_INDEX id, cycle_detection_cache& cache);
  bool find_simple_cycles_scc(const models::Dataflow* const scc,
                              ARRAY_INDEX start_id, ARRAY_INDEX current_id, cycle_detection_cache& cache);
  std::string print_status(const models::Dataflow* const dataflow);


}


ADD_TRANSFORMATION(CycleDetection,
                   transformation_t({ "CycleDetection" , "Returns 1 if there is a cycle in the graph and 0 if not..", algorithms::cycle_detection}));
ADD_TRANSFORMATION(CycleCount,
                   transformation_t({ "CycleCount" , "Count number of simple cycles in graph.", algorithms::find_simple_cycles}));



#endif /* SRC_LIBKITER_ALGORITHMS_ANALYSIS_CYCLE_DETECTION_H_ */
