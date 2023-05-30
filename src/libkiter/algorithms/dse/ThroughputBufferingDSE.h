//
// Created by toky on 26/4/23.
//

#ifndef KITER_THROUGHPUT_BUFFERING_DSE_H
#define KITER_THROUGHPUT_BUFFERING_DSE_H



#include <algorithms/dse/kperiodic.h>
#include <algorithms/dse/ModularDSE.h>

namespace algorithms {
    namespace dse {
        enum ThroughputBufferingMode {DSE_MODE, KDSE_MODE, KDSEA_MODE};
        TokenConfiguration::PerformanceResult throughputbuffering_performance_func(models::Dataflow* dataflow, const TokenConfiguration& config);
        std::vector<TokenConfiguration> throughputbuffering_initial_func(const models::Dataflow* dataflow);


        TokenConfigurationSet solve_throughputbuffering   (models::Dataflow* const  dataflow,
                                                           ThroughputBufferingMode mode,
                                                bool realtime_output = false,
                                                size_t thread_count = 1,
                                                size_t timeout = 0,
                                                size_t limit = 0,
                                                std::string  filename = "",
                                                algorithms::dse::TokenConfiguration* tc = nullptr) ;

        void throughputbuffering_dse   (models::Dataflow* const  dataflow, parameters_list_t params);
    }
}


ADD_TRANSFORMATION(throughputbufferingDSE,
                   transformation_t({
                                            "throughputbufferingDSE" ,
                                            "DSE that solve the throughputbuffering problem for a graph with \"feedback\" buffers",
                                            algorithms::dse::throughputbuffering_dse}
                   ));



#endif //KITER_THROUGHPUT_BUFFERING_DSE_H
