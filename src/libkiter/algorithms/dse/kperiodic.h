/*
 * kperiodic.h
 *
 *  Created on: Aug 17, 2020
 *      Author: jkmingwen
 */

#ifndef SRC_LIBKITER_ALGORITHMS_DSE_KPERIODIC_H_
#define SRC_LIBKITER_ALGORITHMS_DSE_KPERIODIC_H_

#include <commons/KiterRegistry.h>
#include <algorithms/schedulings.h>
#include <algorithms/throughput/kperiodic.h>

namespace models {
	class Dataflow;
}

namespace algorithms {
        kperiodic_result_t compute_Kperiodic_throughput_and_cycles(models::Dataflow* const dataflow,
                                                                   parameters_list_t params);
}

#endif /* SRC_LIBKITER_ALGORITHMS_DSE_KPERIODIC_H_ */
