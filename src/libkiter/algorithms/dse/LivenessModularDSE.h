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


        TokenConfigurationSet solve_liveness   (models::Dataflow* const  dataflow,
                                                bool realtime_output = false,
                                                size_t thread_count = 1,
                                                size_t timeout = 0,
                                                size_t limit = 0,
                                                std::string  filename = "",
                                                algorithms::dse::TokenConfiguration* tc = nullptr) ;

        void liveness_dse   (models::Dataflow* const  dataflow, parameters_list_t params);
    }
}


ADD_TRANSFORMATION(LivenessDSE,
                   transformation_t({
                       "LivenessDSE" ,
                       "DSE that solve the liveness problem for a graph with \"feedback\" buffers",
                       algorithms::dse::liveness_dse}
                       ));


#endif //KITER_LIVENESSMODULARDSE_H
