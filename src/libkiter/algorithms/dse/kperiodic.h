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
#include "buffer_sizing.h"

namespace models {
	class Dataflow;
}

namespace algorithms {
        kperiodic_result_t compute_Kperiodic_throughput_and_cycles(models::Dataflow* const dataflow);
        void mod_Kperiodic_throughput_dse(models::Dataflow *const dataflow, parameters_list_t parameters);
        StorageDistributionSet compute_Kperiodic_throughput_dse_sd(models::Dataflow *const dataflow, parameters_list_t parameters);
        void compute_Kperiodic_throughput_dse(models::Dataflow *const dataflow, parameters_list_t params);

       // my addition
       StorageDistributionSet new_compute_Kperiodic_throughput_dse_sd(models::Dataflow *const dataflow);
}

models::Dataflow* getCCGraph(models::Dataflow* g, kperiodic_result_t result, bool print=false);
models::Dataflow* genGraphWFeedbackEdges(models::Dataflow* g);
std::pair<models::Dataflow*, std::map<Edge,Edge> > genGraphWFeedbackEdgesWithPairs(models::Dataflow* g);

StorageDistribution updateStoreDistwCCSD(models::Dataflow* dataflow_prime, StorageDistribution sd_g,
                                         models::Dataflow* cc_g, StorageDistribution sd_cc);

ADD_TRANSFORMATION(KPeriodicThroughputwithDSE,
		transformation_t({"KPeriodicThroughputwithDSE", "Combine buffer space exploration with throughput evaluation of CSDF by K-periodic scheduling method", algorithms::compute_Kperiodic_throughput_dse}));

ADD_TRANSFORMATION(ModKPeriodicThroughputwithDSE,
		transformation_t({"ModPeriodicThroughputwithDSE", "Combine buffer space exploration with throughput evaluation of CSDF by K-periodic scheduling method", algorithms::mod_Kperiodic_throughput_dse}));
#endif /* SRC_LIBKITER_ALGORITHMS_DSE_KPERIODIC_H_ */
