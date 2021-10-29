/*
 * dse.h
 *
 * Created on: Oct 27, 2021
 *     Author: jkmingwen
 */

#ifndef SRC_LIBKITER_ALGORITHMS_DSE_H_
#define SRC_LIBKITER_ALGORITHMS_DSE_H_

#include <commons/KiterRegistry.h>
#include <algorithms/schedulings.h>
#include <algorithms/throughput/kperiodic.h>
#include <algorithms/throughput/symbolic_execution.h>

namespace models {
	class Dataflow;
}

namespace algorithms {
  void throughput_buffering_tradeoff_dse(models::Dataflow* const dataflow,
                                         parameters_list_t parameters);
}

ADD_TRANSFORMATION(ThroughputBufferingDSE,
                   transformation_t({"ThroughputBufferingDSE", "Combine buffer space exploration with throughput evaluation of SDF/CSDF", algorithms::throughput_buffering_tradeoff_dse}));
#endif /* SRC_LIBKITER_ALGORITHMS_DSE_H_ */
