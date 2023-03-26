//
// Created by jachym on 26/3/23.
//

#define BOOST_TEST_MODULE RemoveReentrancyTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "helpers/random_generator.h"
#include "algorithms/transformation/remove_reentrancy.h"


BOOST_FIXTURE_TEST_SUITE( reentrancy_test , WITH_SAMPLE)

BOOST_AUTO_TEST_CASE( sample_remove_reentrancy_test )
{
    BOOST_REQUIRE(pipeline_sample);
    BOOST_TEST(true);
}

BOOST_AUTO_TEST_SUITE_END()
