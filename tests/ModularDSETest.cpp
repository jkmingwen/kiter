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

// TODO: The copy of the graph is atrocious
// TODO: the kperiodic_results should be stored somewhere
TIME_UNIT dummy_performance_func(const algorithms::dse::TokenConfiguration& config) {
    VERBOSE_INFO("Copy graph");
    models::Dataflow g = *config.getDataflow();
    VERBOSE_INFO("Prepare graph with config " << commons::toString(config));
    {ForEachEdge(&g,e) {
            if (g.getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                TOKEN_UNIT v = config.getConfiguration().at(g.getEdgeId(e));
                g.setPreload(e,  v );
            }
    }}

    VERBOSE_INFO("compute_Kperiodic_throughput_and_cycles");
    kperiodic_result_t result_max = algorithms::compute_Kperiodic_throughput_and_cycles(&g);
    VERBOSE_INFO("Done");
    return result_max.throughput;
}

// TODO: for the liveness problem it might make sense, but for buffer sizing it is so dumb.
algorithms::dse::TokenConfiguration dummy_initial_func(const models::Dataflow* dataflow) {
    std::map<ARRAY_INDEX , TOKEN_UNIT> configuration; // Replace Edge with the correct type for your implementation
    { ForEachEdge(dataflow,e) {
        ARRAY_INDEX tid = dataflow->getEdgeId(e);
        if (dataflow->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
            configuration[tid] = 0;
        }
    }}
    return algorithms::dse::TokenConfiguration(dataflow, configuration);
}

// TODO : the kperiodic results is missing here
// TODO : also the step size is not there
std::vector<algorithms::dse::TokenConfiguration> dummy_next_func(const algorithms::dse::TokenConfiguration& current) {

    // stopping condition
    if (current.getThroughput() > 0) {
        VERBOSE_INFO("Stopping condition reached");
        return  {};
    }

    std::vector<algorithms::dse::TokenConfiguration> next_configurations;
    const auto& current_configuration = current.getConfiguration();

    for (const auto& edge_buffer_pair : current_configuration) {
        auto new_configuration = current_configuration; // Make a copy of the current configuration
        new_configuration[edge_buffer_pair.first] = edge_buffer_pair.second + 1; // Increment the buffer size for the current edge by 1

        // Create a new TokenConfiguration with the updated buffer size for the current edge
        algorithms::dse::TokenConfiguration new_token_configuration(current.getDataflow(), new_configuration);
        next_configurations.push_back(new_token_configuration);
    }

    return next_configurations;
}

bool dummy_stop_condition(const algorithms::dse::TokenConfiguration& new_config, const algorithms::dse::TokenConfigurationSet& searched_configs) {

    const algorithms::dse::TokenConfiguration* best = searched_configs.getBestPerformancePoint();
    if (best) return ((best->getCost() < new_config.getCost()) && (best->getThroughput() > 0));
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

        algorithms::dse::TokenConfiguration tc1 = dummy_initial_func(df);
        algorithms::dse::TokenConfiguration tc2 = dummy_initial_func(df);

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
                                        dummy_performance_func,
                                        dummy_initial_func,
                                        dummy_next_func,
                                        dummy_stop_condition, 1);

        algorithms::dse::TokenConfiguration tc = dummy_initial_func(df);
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
