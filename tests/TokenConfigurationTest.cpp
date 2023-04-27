//
// Created by toky on 26/4/23.
//

#define BOOST_TEST_MODULE TokenConfigurationTest
#include "helpers/test_classes.h"
#include <algorithms/dse/TokenConfiguration.h>

using namespace algorithms::dse;

// Dummy performance function
TokenConfiguration::PerformanceResult dummy_performance_func(models::Dataflow*, const TokenConfiguration& ) {
    return TokenConfiguration::PerformanceResult(1.0,{});
}


BOOST_FIXTURE_TEST_SUITE( test_suite_TokenConfigurationSet , WITH_SAMPLE)

BOOST_AUTO_TEST_CASE(compute_throughput) {
    // ... Create the necessary dataflow object.
        { ForEachEdge(cycle_sample, e) {
                cycle_sample->setEdgeType(e, EDGE_TYPE::FEEDBACK_EDGE);
            }}
    std::vector<TOKEN_UNIT> config_values = {1, 2, 3};
    TokenConfiguration config(cycle_sample, config_values);

    BOOST_CHECK(!config.hasPerformance());
    config.computePerformance(cycle_sample, dummy_performance_func);
    BOOST_CHECK(config.hasPerformance());
    BOOST_CHECK_EQUAL(config.getPerformance().throughput, 1.0);
}

BOOST_AUTO_TEST_CASE(dominates) {
    // ... Create the necessary dataflow object.
        { ForEachEdge(cycle_sample, e) {
                cycle_sample->setEdgeType(e, EDGE_TYPE::FEEDBACK_EDGE);
            }}
    std::vector<TOKEN_UNIT> config_values1 = {1, 2, 3};
    std::vector<TOKEN_UNIT> config_values2 = {2, 3, 4};
    std::vector<TOKEN_UNIT> config_values3 = {1, 1, 1};

    TokenConfiguration config1(cycle_sample, config_values1);
    TokenConfiguration config2(cycle_sample, config_values2);
    TokenConfiguration config3(cycle_sample, config_values3);

    config1.computePerformance(cycle_sample, dummy_performance_func);
    config2.computePerformance(cycle_sample, dummy_performance_func);
    config3.computePerformance(cycle_sample, dummy_performance_func);

    BOOST_CHECK(!config1.dominates(config2));
    BOOST_CHECK(config1.dominates(config3));
    BOOST_CHECK(config2.dominates(config1));
    BOOST_CHECK(config2.dominates(config3));
    BOOST_CHECK(!config3.dominates(config1));
    BOOST_CHECK(!config3.dominates(config2));
}
BOOST_AUTO_TEST_CASE(get_cost) {
    // ... Create the necessary dataflow object.
        { ForEachEdge(cycle_sample, e) {
                cycle_sample->setEdgeType(e, EDGE_TYPE::FEEDBACK_EDGE);
            }}
    std::vector<TOKEN_UNIT> config_values = {1, 2, 3};
    TokenConfiguration config(cycle_sample, config_values);

    BOOST_CHECK_EQUAL(config.getCost(), 6);
}

BOOST_AUTO_TEST_CASE(to_csv_line) {
    // ... Create the necessary dataflow object.
        { ForEachEdge(cycle_sample, e) {
                cycle_sample->setEdgeType(e, EDGE_TYPE::FEEDBACK_EDGE);
            }}
    std::vector<TOKEN_UNIT> config_values = {1, 2, 3};
    TokenConfiguration config(cycle_sample, config_values);

    BOOST_CHECK_EQUAL(config.to_csv_line(), "6,-,\"1,2,3\",-,-,-");
    TokenConfiguration::setBeginTime(std::chrono::duration<double, std::milli>::zero());
    config.computePerformance(cycle_sample, dummy_performance_func);
    BOOST_CHECK_EQUAL(config.to_csv_line().substr(0,14),  "6,1,\"1,2,3\",\"\"");
    std::cout << config.to_csv_line();
}

BOOST_AUTO_TEST_SUITE_END()
