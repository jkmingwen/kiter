//
// Created by toky on 24/4/23.
//

#ifndef KITER_LIVENESSMODULARDSE_H
#define KITER_LIVENESSMODULARDSE_H


#include <algorithms/dse/kperiodic.h>
#include <algorithms/dse/ModularDSE.h>

namespace algorithms {
    namespace dse {
        TokenConfiguration::PerformanceResult liveness_performance_func(models::Dataflow* dataflow, const TokenConfiguration& config);
        TokenConfiguration liveness_initial_func(const models::Dataflow* dataflow);
        std::vector<TokenConfiguration> liveness_next_func(const TokenConfiguration& );
        bool liveness_stop_condition(const TokenConfiguration& , const TokenConfigurationSet&);
        void solve_liveness   (models::Dataflow* const  dataflow, parameters_list_t params);
    }
}


ADD_TRANSFORMATION(LivenessDSE,
                   transformation_t({
                       "LivenessDSE" ,
                       "DSE that solve the liveness problem for a graph with \"feedback\" buffers",
                       algorithms::dse::solve_liveness}
                       ));


#endif //KITER_LIVENESSMODULARDSE_H
