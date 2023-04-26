//
// Created by toky on 26/4/23.
//

#ifndef KITER_THROUGHPUTBUFFERINGDSE_H
#define KITER_THROUGHPUTBUFFERINGDSE_H



#include <algorithms/dse/kperiodic.h>
#include <algorithms/dse/ModularDSE.h>

namespace algorithms {
    namespace dse {
        TokenConfiguration::PerformanceResult throughputbuffering_performance_func(const TokenConfiguration& config);
        TokenConfiguration throughputbuffering_initial_func(const models::Dataflow* dataflow);
        std::vector<TokenConfiguration> throughputbuffering_next_func(const TokenConfiguration& );
        bool throughputbuffering_stop_condition(const TokenConfiguration& , const TokenConfigurationSet&);
        void solve_throughputbuffering   (models::Dataflow* const  dataflow, parameters_list_t params);
    }
}


ADD_TRANSFORMATION(throughputbufferingDSE,
                   transformation_t({
                                            "throughputbufferingDSE" ,
                                            "DSE that solve the throughputbuffering problem for a graph with \"feedback\" buffers",
                                            algorithms::dse::solve_throughputbuffering}
                   ));



#endif //KITER_THROUGHPUTBUFFERINGDSE_H
