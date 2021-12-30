/*
 * cycle_detection.h
 *
 *  Created on: 29 Dec 2021
 *      Author: toky
 */

#ifndef SRC_LIBKITER_ALGORITHMS_ANALYSIS_CYCLE_DETECTION_H_
#define SRC_LIBKITER_ALGORITHMS_ANALYSIS_CYCLE_DETECTION_H_


#include <commons/KiterRegistry.h>

namespace models {
	class Dataflow;
}

namespace algorithms {

bool has_selfloops         (models::Dataflow* const  dataflow);
bool has_cycles            (models::Dataflow* const  dataflow);
void cycle_detection       (models::Dataflow* const  dataflow, parameters_list_t);


}


ADD_TRANSFORMATION(CycleDetection,
		transformation_t({ "CycleDetection" , "CycleDetection.", algorithms::cycle_detection}));




#endif /* SRC_LIBKITER_ALGORITHMS_ANALYSIS_CYCLE_DETECTION_H_ */
