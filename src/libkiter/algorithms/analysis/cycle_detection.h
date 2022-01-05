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

namespace algorithms {

bool has_selfloops         (models::Dataflow* const  dataflow);
bool has_cycles            (models::Dataflow* const  dataflow);
void cycle_detection       (models::Dataflow* const  dataflow, parameters_list_t);
  void find_simple_cycles(models::Dataflow* const  dataflow, parameters_list_t);
  ARRAY_INDEX find_smallest_index(models::Dataflow* const dataflow,
                                  std::map<int, std::vector<ARRAY_INDEX>> scc_map);
  models::Dataflow* generate_scc(models::Dataflow* const dataflow,
                                 std::map<int, std::vector<ARRAY_INDEX>> scc_map,
                                 ARRAY_INDEX start_id);
  models::Dataflow* generate_subgraph(models::Dataflow* const dataflow,
                                      ARRAY_INDEX lower_limit);
  void unblock(ARRAY_INDEX id);
  bool find_simple_cycles_scc(models::Dataflow* const scc,
                              ARRAY_INDEX start_id, ARRAY_INDEX current_id);
  std::string print_status(models::Dataflow* const dataflow);


}


ADD_TRANSFORMATION(CycleDetection,
		transformation_t({ "CycleDetection" , "CycleDetection.", algorithms::cycle_detection}));
ADD_TRANSFORMATION(CycleCount,
                   transformation_t({ "CycleCount" , "Count number of simple cycles in graph.", algorithms::find_simple_cycles}));



#endif /* SRC_LIBKITER_ALGORITHMS_ANALYSIS_CYCLE_DETECTION_H_ */
