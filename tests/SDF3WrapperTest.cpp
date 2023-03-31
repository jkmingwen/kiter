//
// Created by jachym on 31/3/23.
//

#define BOOST_TEST_MODULE SDF3WrapperTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "algorithms/throughput/SDF3Wrapper.h"


BOOST_FIXTURE_TEST_SUITE( sdf3_wrapper_test , WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE( sample_sdf3_wrapper_test )
    {
        BOOST_REQUIRE(pipeline_sample);
        BOOST_REQUIRE(cycle_sample);

        algorithms::runSDF3Throughput(pipeline_sample);
        algorithms::runSDF3Throughput(cycle_sample);
    }

BOOST_AUTO_TEST_SUITE_END()
