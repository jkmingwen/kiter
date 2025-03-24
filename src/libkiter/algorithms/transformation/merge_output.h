/*
 * merge_outputs.h
 *
 *  Created on: 28 Oct 2024
 *      Author: jkmingwen
 */

#ifndef SRC_LIBKITER_ALGORITHMS_TRANSFORMATION_MERGE_OUTPUTS_H_
#define SRC_LIBKITER_ALGORITHMS_TRANSFORMATION_MERGE_OUTPUTS_H_

#include <string>
#include <vector>
#include <commons/commons.h>
#include <commons/KiterRegistry.h>



namespace models {
	class Dataflow;
	class Scheduling;
}



namespace algorithms {
namespace transformation {
void merge_output    (models::Dataflow* const dataflow, parameters_list_t  parameters  ) ;
}
}


ADD_TRANSFORMATION(MergeOutput,
		transformation_t({ "MergeOutput" , "Merge outputs from a task to a intermediate task acting as a broadcaster.", algorithms::transformation::merge_output}));

#endif /* SRC_LIBKITER_ALGORITHMS_TRANSFORMATION_MERGE_OUTPUTS_H_ */
