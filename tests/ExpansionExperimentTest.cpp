//
// Created by jachym on 31/3/23.
//

#define BOOST_TEST_MODULE ExpansionExperimentTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "helpers/random_generator.h"
#include "algorithms/experiments/expansion_experiment.h"


BOOST_FIXTURE_TEST_SUITE( expansion_experiment_test , WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE( sample_expansion_experiment_test )
    {
        parameters_list_t params;
        BOOST_REQUIRE(pipeline_sample);

        algorithms::generate_expansion(pipeline_sample, params);
    }

#define MAX_PHASE_COUNT 5

    BOOST_AUTO_TEST_CASE( random_expansion_experiment_test )
    {
        std::vector<models::Dataflow *> graphs;
        parameters_list_t params;

        VERBOSE_INFO("Generating Graphs");
        for (int i = 10; i <= 300; i += 50) {

            int buf_num = std::rand() % (i / 2) + i;

            VERBOSE_INFO("generate graph " << i << " with " << buf_num);

            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
            graphs.push_back(g);
        }

        VERBOSE_INFO("Generating expansion on random graphs");
        for(auto graph : graphs) {
            algorithms::generate_expansion(graph, params);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
