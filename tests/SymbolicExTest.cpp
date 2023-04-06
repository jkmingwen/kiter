//
// Created by jachym on 6/4/23.
//

#define BOOST_TEST_MODULE SymbolicExTest
#include "helpers/test_classes.h"
#include "helpers/random_generator.h"
#include "algorithms/symbolic_execution.h"

BOOST_FIXTURE_TEST_SUITE(symbolic_ex_test, WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE(sample_symbolic_execution_packet_test)
    {
        parameters_list_t params;
        BOOST_REQUIRE(pipeline_sample);
        BOOST_TEST(false);
        //TODO: infinite loop
//        algorithms::symbolic_execution_with_packets(pipeline_sample, params);
    }

    BOOST_AUTO_TEST_CASE(sample_symbolic_execution_test)
    {
        parameters_list_t params;
        BOOST_REQUIRE(pipeline_sample);
        algorithms::symbolic_execution(pipeline_sample, params);
    }

    BOOST_AUTO_TEST_CASE(sample_packet_list_test)
    {
        parameters_list_t params;
        BOOST_REQUIRE(pipeline_sample);
        // TODO: defined, unimplemented
        //    algorithms::packet_list(pipeline_sample, params);
    }

BOOST_AUTO_TEST_SUITE_END()
