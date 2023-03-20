//
// Created by toky on 20/3/23.
//

#ifndef KITER_DEEP_DSE_H
#define KITER_DEEP_DSE_H

#include <algorithms/dse/buffer_sizing.h>

namespace algorithms {

    StorageDistributionSet deep_dse(models::Dataflow *const dataflow) ;

    inline void deep_dse_user(models::Dataflow *const dataflow, parameters_list_t params) {
        deep_dse(dataflow);
    }
}

models::Dataflow* get_critical_cycle_original_edges_from_prime (const models::Dataflow* g,
                                                                const std::map<Edge,Edge>& matching ,
                                                                const kperiodic_result_t &result);
std::pair<TIME_UNIT, std::vector<StorageDistribution>> get_next_storage_distribution_efficiently_from_cc(const StorageDistribution &checkDist, models::Dataflow *dataflow_prime,
                                                                                                         const std::map<Edge,Edge>& matching, const std::map<Edge, TOKEN_UNIT> & minStepSizes) ;
StorageDistribution update_storage_distribution_from_cc(const StorageDistribution &sd_g, const StorageDistribution& sd_cc);

ADD_TRANSFORMATION(DeepKPeriodicThroughputwithDSE,
                   transformation_t({"DeepKPeriodicThroughputwithDSE", "Experiment", algorithms::deep_dse_user}));

#endif //KITER_DEEP_DSE_H
