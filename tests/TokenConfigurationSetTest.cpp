//
// Created by toky on 25/4/23.
//
#define BOOST_TEST_MODULE TokenConfigurationSetTest
#include "helpers/test_classes.h"
#include <algorithms/dse/TokenConfigurationSet.h>

BOOST_FIXTURE_TEST_SUITE( test_suite_TokenConfigurationSet , WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE(add_non_dominated_configurations) {
        algorithms::dse::TokenConfigurationSet config_set;
        cycle_sample->setEdgeType(cycle_sample->getFirstEdge(), EDGE_TYPE::FEEDBACK_EDGE);
        algorithms::dse::TokenConfiguration config1(cycle_sample, {1});
        algorithms::dse::TokenConfiguration config2(cycle_sample, {2});
        algorithms::dse::TokenConfiguration config3(cycle_sample, {3});

        config_set.add(config1);
        config_set.add(config2);
        config_set.add(config3);

        BOOST_CHECK_EQUAL(config_set.size(), 3);
    }

    BOOST_AUTO_TEST_CASE(add_dominated_configurations) {
        algorithms::dse::TokenConfigurationSet config_set;

        cycle_sample->setEdgeType(cycle_sample->getFirstEdge(), EDGE_TYPE::FEEDBACK_EDGE);
        algorithms::dse::TokenConfiguration config1(cycle_sample, {1});
        algorithms::dse::TokenConfiguration config2(cycle_sample, {1});
        algorithms::dse::TokenConfiguration config3(cycle_sample, {1});

        config_set.add(config1);
        config_set.add(config2);
        config_set.add(config3);

        BOOST_CHECK_EQUAL(config_set.size(), 1);
    }

    BOOST_AUTO_TEST_CASE(remove_configurations) {
        algorithms::dse::TokenConfigurationSet config_set;

        cycle_sample->setEdgeType(cycle_sample->getFirstEdge(), EDGE_TYPE::FEEDBACK_EDGE);
        algorithms::dse::TokenConfiguration config1(cycle_sample, {1});
        algorithms::dse::TokenConfiguration config2(cycle_sample, {2});
        algorithms::dse::TokenConfiguration config3(cycle_sample, {3});

        config_set.add(config3);
        config_set.add(config1);
        config_set.add(config2);

        BOOST_CHECK_EQUAL(config_set.size(), 3);
        config_set.remove(config3);
        BOOST_CHECK_EQUAL(config_set.size(), 2);
        config_set.remove(config2);
        BOOST_CHECK_EQUAL(config_set.size(), 1);
        config_set.remove(config1);
        BOOST_CHECK_EQUAL(config_set.size(), 0);

    }

BOOST_AUTO_TEST_SUITE_END()
