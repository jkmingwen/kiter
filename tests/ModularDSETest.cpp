//
// Created by toky on 24/4/23.
//

#define BOOST_TEST_MODULE ModularDSETest

#include "helpers/test_classes.h"
#include "helpers/random_generator.h"
#include "generators/NormalizedCycleGenerator.h"
#include <algorithms/dse/kperiodic.h>
#include <algorithms/dse/ModularDSE.h>
#include <future>


using namespace algorithms::dse;



TokenConfiguration::PerformanceResult dummy_pc(const TokenConfiguration& config) {
    return TokenConfiguration::PerformanceResult(config.getCost(), {});
}

TokenConfiguration dummy_if(const models::Dataflow* dataflow) {
    std::map<ARRAY_INDEX , TOKEN_UNIT> configuration;
    { ForEachEdge(dataflow,e) {
            ARRAY_INDEX tid = dataflow->getEdgeId(e);
            if (dataflow->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                configuration[tid] = 0;
            }
        }}
    return algorithms::dse::TokenConfiguration(dataflow, configuration);
}

std::vector<TokenConfiguration> dummy_nf(const TokenConfiguration& ) {
    return {};
}

bool dummy_sc(const TokenConfiguration& , const TokenConfigurationSet& ) {
    return false;
}


BOOST_FIXTURE_TEST_SUITE( modular_dse_test, WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE(token_configuration_test) {

        size_t node_count = 4;
        std::vector<TOKEN_UNIT> node_weights  {2,2,2,2};
        std::vector<TIME_UNIT> node_durations {1,1,1,1};
        std::vector<TOKEN_UNIT> edge_preloads  {0,0,0,0};
        models::Dataflow* df = generators::new_normalized_cycle(node_count, node_weights, node_durations, edge_preloads);

        {ForEachEdge(df,c) {
                df->setEdgeType(c, EDGE_TYPE::FEEDBACK_EDGE);

        }}

        algorithms::dse::TokenConfiguration tc1 = dummy_if(df);
        algorithms::dse::TokenConfiguration tc2 = dummy_if(df);

        BOOST_REQUIRE(tc1.dominates(tc2));
        BOOST_REQUIRE(tc2.dominates(tc1));
    }
    BOOST_AUTO_TEST_CASE(modular_dse_test) {


        size_t node_count = 4;
        std::vector<TOKEN_UNIT> node_weights  {2,2,2,2};
        std::vector<TIME_UNIT> node_durations {1,1,1,1};
        std::vector<TOKEN_UNIT> edge_preloads  {0,0,0,0};
        models::Dataflow* df = generators::new_normalized_cycle(node_count, node_weights, node_durations, edge_preloads);

        {ForEachEdge(df,c) {
                df->setEdgeType(c, EDGE_TYPE::FEEDBACK_EDGE);
        }}

        commons::set_verbose_mode(commons::DEBUG_LEVEL);

        algorithms::dse::ModularDSE dse(df,
                                        dummy_pc,
                                        dummy_if,
                                        dummy_nf,
                                        dummy_sc, 1);

        algorithms::dse::TokenConfiguration tc = dummy_if(df);
        dse.add_initial_job(tc);

        // Run the DSE exploration for a short period of time (e.g., 100 milliseconds)
        std::future<void> exploration_future = std::async(std::launch::async, [&dse] { dse.explore(); });
        //std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        //dse.stop();
        exploration_future.wait();

        // Test the results
        // Replace with the correct test assertions for your implementation
        BOOST_CHECK(true); // Replace with a proper test assertion based on your specific requirements
    }


BOOST_AUTO_TEST_SUITE_END()
