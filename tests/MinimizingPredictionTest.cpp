//
// Created by jachym on 20/4/23.
//

#define BOOST_TEST_MODULE MinimizingPredictionTest

#include "helpers/test_classes.h"
#include "algorithms/ml/minimizing_prediction.h"
#include "helpers/sample.h"

BOOST_FIXTURE_TEST_SUITE(minimizing_prediction_test, WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE(minimizing_prediciton_not_found_test)
    {
        parameters_list_t params;
        params["DICT_PATH"] = "/home/jachym/Projects/Kiter/notes/data/kiter_data.txt";

        PreloadDict preload_dict(params["DICT_PATH"]);

        std::vector<uint8_t> weights = {60, 223, 42, 12};
        std::vector<uint8_t> result = preload_dict.lookup(weights);
        std::vector<uint8_t> expected = {0,0,0,0};

        BOOST_ASSERT(result == expected);
    }

    BOOST_AUTO_TEST_CASE(sample_minimizing_prediciton_test)
    {
        //TODO: testing only --- absolute path badness
        parameters_list_t params;
        std::string DICT_PATH;

        // Need to change the dictionary path
        if (DICT_PATH.empty()) {
            BOOST_ASSERT(false);
        }

        PreloadDict preload_dict(DICT_PATH);
        // Perform lookups
        std::vector<uint8_t> weights1 = {19, 9, 28,21};
        std::vector<uint8_t> weights2 = {26, 11, 17, 3};
        std::vector<uint8_t> weights3 = {7, 13, 21, 11};

        std::vector<uint8_t> expected1 = {70,66,66,66};
        std::vector<uint8_t> expected2 = {54,54,54,54};
        std::vector<uint8_t> expected3 = {45,47,47,45};

        std::vector<uint8_t> result1 = preload_dict.lookup(weights1);
        std::vector<uint8_t> result2 = preload_dict.lookup(weights2);
        std::vector<uint8_t> result3 = preload_dict.lookup(weights3);

        BOOST_ASSERT(result1 == expected1);
        BOOST_ASSERT(result2 == expected2);
        BOOST_ASSERT(result3 == expected3);
    }
BOOST_AUTO_TEST_SUITE_END()