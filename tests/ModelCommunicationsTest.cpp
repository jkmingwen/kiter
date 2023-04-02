//
// Created by jachym on 2/4/23.
//
#define BOOST_TEST_MODULE ModelCommunicationsTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "helpers/random_generator.h"
#include "algorithms/mapping/modelCommunications.h"


BOOST_FIXTURE_TEST_SUITE( model_communications_test , WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE( sample_model_communications_test )
    {
        BOOST_REQUIRE(pipeline_sample);
        BOOST_REQUIRE(cycle_sample);

        get_overlaps(pipeline_sample);
        get_overlaps(cycle_sample);
    }

#define MAX_PHASE_COUNT 1

    BOOST_AUTO_TEST_CASE( random_model_communications_test )
    {
        std::vector<models::Dataflow *> graphs;

        VERBOSE_INFO("Generating Graphs");
        for (int i = 10; i <= 300; i += 50) {

            int buf_num = std::rand() % (i / 2) + i;

            VERBOSE_INFO("generate graph " << i << " with " << buf_num);

            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
            graphs.push_back(g);
        }

        VERBOSE_INFO("Model Communications on random");
        for(auto graph : graphs) {
            get_overlaps(graph);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
