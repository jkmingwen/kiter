//
// Created by jachym on 20/4/23.
//

#define BOOST_TEST_MODULE MinimizingPredictionTest

#include "helpers/test_classes.h"
#include "helpers/random_generator.h"
#include "algorithms/ml/minimizing_prediction.h"

BOOST_FIXTURE_TEST_SUITE(minimizing_prediction_test, WITH_VERBOSE)

    BOOST_AUTO_TEST_CASE(no_model_minimizing_prediciton_test)
    {
        parameters_list_t params;
        std::vector<int> weights = {1,3,2,6};

        int num_tokens = algorithms::predict_num_tokens(weights, params);
        BOOST_ASSERT(num_tokens == -1);
    }

    BOOST_AUTO_TEST_CASE(sample_minimizing_prediciton_test)
    {
        parameters_list_t params;
//        params["MODEL_DIR"] = "";
        std::vector<int> weights = {1,3,2,6};

        int num_tokens = algorithms::predict_num_tokens(weights, params);
        VERBOSE_DEBUG("Number of tokens = " << num_tokens);
        BOOST_ASSERT(num_tokens != -1);
    }
BOOST_AUTO_TEST_SUITE_END()