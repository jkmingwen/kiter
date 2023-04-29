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



TokenConfiguration::PerformanceResult dummy_pc(models::Dataflow*, const TokenConfiguration& config) {
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

std::vector<TokenConfiguration> dummy_nf(const TokenConfiguration& current ) {
    const models::Dataflow* df =  current.getDataflow();
    std::vector<algorithms::dse::TokenConfiguration> next_configurations;
    const auto& current_configuration = current.getConfiguration();
    for (auto edgeId_items : current.getConfiguration()) {
        Edge c = df->getEdgeById(edgeId_items.first);
        if (df->getEdgeType(c) != EDGE_TYPE::FEEDBACK_EDGE) continue; // Skip non feedback buffers.
        auto new_configuration = current_configuration;
        new_configuration[edgeId_items.first] = new_configuration[edgeId_items.first] + 1;
        algorithms::dse::TokenConfiguration new_token_configuration(current.getDataflow(), new_configuration);
        next_configurations.push_back(new_token_configuration);
    }

    return next_configurations;
}

bool dummy_sc(const TokenConfiguration& , const TokenConfigurationSet& ) {
    return false;
}

static const std::string csv_file_content =
        "storage distribution size,throughput,channel quantities,feedback quantities,critical channels,critical feedback,execution time,cumulative time\n"
        "0,0,\"\",\"0,0,0,0\",\"\",\"\",-,-\n"
        "1,1,\"\",\"0,0,0,1\",\"\",\"\",-,-\n"
        "1,1,\"\",\"0,0,1,0\",\"\",\"\",-,-\n"
        "1,1,\"\",\"0,1,0,0\",\"\",\"\",-,-\n"
        "1,1,\"\",\"1,0,0,0\",\"\",\"\",-,-\n"
        "2,2,\"\",\"0,0,0,2\",\"\",\"\",-,-\n"
        "2,2,\"\",\"0,0,1,1\",\"\",\"\",-,-\n"
        "2,2,\"\",\"0,0,2,0\",\"\",\"\",-,-\n"
        "2,2,\"\",\"0,1,0,1\",\"\",\"\",-,-\n"
        "2,2,\"\",\"0,1,1,0\",\"\",\"\",-,-\n"
        "2,-,\"\",\"0,2,0,0\",-,-,-,-\n"
        "2,-,\"\",\"1,0,0,1\",-,-,-,-\n"
        "2,-,\"\",\"1,0,1,0\",-,-,-,-\n"
        "2,-,\"\",\"1,1,0,0\",-,-,-,-\n"
        "2,-,\"\",\"2,0,0,0\",-,-,-,-\n"
        "3,-,\"\",\"0,0,0,3\",-,-,-,-\n"
        "3,-,\"\",\"0,0,1,2\",-,-,-,-\n"
        "3,-,\"\",\"0,0,2,1\",-,-,-,-\n"
        "3,-,\"\",\"0,0,3,0\",-,-,-,-\n"
        "3,-,\"\",\"0,1,0,2\",-,-,-,-\n"
        "3,-,\"\",\"0,1,1,1\",-,-,-,-\n"
        "3,-,\"\",\"0,1,2,0\",-,-,-,-\n"
        "3,-,\"\",\"0,2,0,1\",-,-,-,-\n"
        "3,-,\"\",\"0,2,1,0\",-,-,-,-\n"
        "3,-,\"\",\"1,0,0,2\",-,-,-,-\n"
        "3,-,\"\",\"1,0,1,1\",-,-,-,-\n"
        "3,-,\"\",\"1,0,2,0\",-,-,-,-\n"
        "3,-,\"\",\"1,1,0,1\",-,-,-,-\n"
        "3,-,\"\",\"1,1,1,0\",-,-,-,-\n";


static const std::string csv_file_content_with_timings =
        "storage distribution size,throughput,channel quantities,feedback quantities,critical channels,critical feedback,execution time,cumulative time\n"
        "0,0,\"\",\"0,0,0,0\",\"\",\"\",0.001835,0.545401\n"
        "1,1,\"\",\"0,0,0,1\",\"\",\"\",0.000393,0.644924\n"
        "1,1,\"\",\"0,0,1,0\",\"\",\"\",0.000294,0.705254\n"
        "1,1,\"\",\"0,1,0,0\",\"\",\"\",0.00028,0.779911\n"
        "1,1,\"\",\"1,0,0,0\",\"\",\"\",0.000378,0.858076\n"
        "2,2,\"\",\"0,0,0,2\",\"\",\"\",0.00026,0.938773\n"
        "2,2,\"\",\"0,0,1,1\",\"\",\"\",0.000293,0.98937\n"
        "2,2,\"\",\"0,0,2,0\",\"\",\"\",0.000266,1.05367\n"
        "2,2,\"\",\"0,1,0,1\",\"\",\"\",0.000237,1.1284\n"
        "2,2,\"\",\"0,1,1,0\",\"\",\"\",0.000226,1.20212\n"
        "2,-,\"\",\"0,2,0,0\",-,-,-,-\n"
        "2,-,\"\",\"1,0,0,1\",-,-,-,-\n"
        "2,-,\"\",\"1,0,1,0\",-,-,-,-\n"
        "2,-,\"\",\"1,1,0,0\",-,-,-,-\n"
        "2,-,\"\",\"2,0,0,0\",-,-,-,-\n"
        "3,-,\"\",\"0,0,0,3\",-,-,-,-\n"
        "3,-,\"\",\"0,0,1,2\",-,-,-,-\n"
        "3,-,\"\",\"0,0,2,1\",-,-,-,-\n"
        "3,-,\"\",\"0,0,3,0\",-,-,-,-\n"
        "3,-,\"\",\"0,1,0,2\",-,-,-,-\n"
        "3,-,\"\",\"0,1,1,1\",-,-,-,-\n"
        "3,-,\"\",\"0,1,2,0\",-,-,-,-\n"
        "3,-,\"\",\"0,2,0,1\",-,-,-,-\n"
        "3,-,\"\",\"0,2,1,0\",-,-,-,-\n"
        "3,-,\"\",\"1,0,0,2\",-,-,-,-\n"
        "3,-,\"\",\"1,0,1,1\",-,-,-,-\n"
        "3,-,\"\",\"1,0,2,0\",-,-,-,-\n"
        "3,-,\"\",\"1,1,0,1\",-,-,-,-\n"
        "3,-,\"\",\"1,1,1,0\",-,-,-,-\n";

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


        algorithms::dse::ModularDSE dse(df,
                                        dummy_pc,
                                        dummy_if,
                                        dummy_nf,
                                        dummy_sc, 1);

        algorithms::dse::TokenConfiguration tc = dummy_if(df);
        dse.add_initial_job(tc);
        dse.explore(10);

        // Test the results
        BOOST_REQUIRE_EQUAL(dse.results_size(), 10);
        BOOST_REQUIRE_EQUAL(dse.job_pool_size(), 19);
        std::cout << dse.print_space(false) << std::endl;
        std::string output = dse.print_space(true);
        BOOST_REQUIRE_EQUAL(output, csv_file_content);

    }

    BOOST_AUTO_TEST_CASE(modular_dse_import_test) {

        size_t node_count = 4;
        std::vector<TOKEN_UNIT> node_weights  {2,2,2,2};
        std::vector<TIME_UNIT> node_durations {1,1,1,1};
        std::vector<TOKEN_UNIT> edge_preloads  {0,0,0,0};
        models::Dataflow* df = generators::new_normalized_cycle(node_count, node_weights, node_durations, edge_preloads);

        {ForEachEdge(df,c) {
                df->setEdgeType(c, EDGE_TYPE::FEEDBACK_EDGE);
            }}


        algorithms::dse::ModularDSE dse(df,
                                        dummy_pc,
                                        dummy_if,
                                        dummy_nf,
                                        dummy_sc, 1);

        algorithms::dse::TokenConfiguration tc = dummy_if(df);

        std::istringstream input(csv_file_content_with_timings);
        dse.import_results(input);
        dse.explore(1);

        // Test the results
        BOOST_REQUIRE_EQUAL(dse.results_size(), 11);
        BOOST_REQUIRE_EQUAL(dse.job_pool_size(), 20);

        std::cout << dse.print_space();
    }


BOOST_AUTO_TEST_SUITE_END()
