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

    StorageDistributionSet new_compute_Kperiodic_dse_with_init_dist(models::Dataflow *const dataflow,
                                                                    StorageDistribution initDist,
                                                                    TIME_UNIT target_thr=0);
    StorageDistributionSet new_compute_Kperiodic_dse(models::Dataflow *const dataflow);
}

models::Dataflow* getCCGraph(models::Dataflow* g, kperiodic_result_t result, bool print=false);
models::Dataflow* genGraphWFeedbackEdges(models::Dataflow* g);
std::pair<models::Dataflow*, std::map<Edge,Edge> > genGraphWFeedbackEdgesWithPairs(models::Dataflow* g);

StorageDistribution updateStoreDistwCCSD(models::Dataflow* dataflow_prime, StorageDistribution sd_g,
                                         models::Dataflow* cc_g, StorageDistribution sd_cc);


std::pair<TIME_UNIT, std::vector<StorageDistribution>> get_next_storage_distribution_from_cc(StorageDistribution checkDist, models::Dataflow *dataflow_prime,
                                                                                             const std::map<Edge,Edge>& matching, const std::map<Edge, TOKEN_UNIT> & minStepSizes);
StorageDistribution initialiseDist(models::Dataflow* dataflow);
void updateGraphwMatching(models::Dataflow* dataflow, const std::map<Edge,Edge>& matching, StorageDistribution checkDist);
ADD_TRANSFORMATION(KPeriodicThroughputwithDSE,
		transformation_t({"KPeriodicThroughputwithDSE", "Combine buffer space exploration with throughput evaluation of CSDF by K-periodic scheduling method", algorithms::compute_Kperiodic_throughput_dse}));

ADD_TRANSFORMATION(ModKPeriodicThroughputwithDSE,
		transformation_t({"ModPeriodicThroughputwithDSE", "Combine buffer space exploration with throughput evaluation of CSDF by K-periodic scheduling method", algorithms::mod_Kperiodic_throughput_dse}));
#endif /* SRC_LIBKITER_ALGORITHMS_DSE_KPERIODIC_H_ */
