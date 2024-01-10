//
// Created by jachym on 6/4/23.
//
#define BOOST_TEST_MODULE LTEGeneratorTest
#include "helpers/test_classes.h"
#include "helpers/random_generator.h"
#include "generators/LTEGenerator.h"

BOOST_FIXTURE_TEST_SUITE( lte_generator_test, WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE( simple_lte_generator_test )
    {
        parameters_list_t params;
        generators::generate_lte_sdf(params);
    }

BOOST_AUTO_TEST_SUITE_END()
