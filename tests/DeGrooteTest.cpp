//
// Created by jachym on 31/3/23.
//
#define BOOST_TEST_MODULE DeGrooteTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "helpers/random_generator.h"
#include "algorithms/throughput/degroote.h"


BOOST_FIXTURE_TEST_SUITE( degroote_test , WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE( sample_degroote_test )
    {
        parameters_list_t params;
        BOOST_REQUIRE(pipeline_sample);

        models::EventGraph *lcg = algorithms::generate_LCG(pipeline_sample);
        algorithms::compute_deGroote_throughput(pipeline_sample, params);
        algorithms::compute_deGrooteClean_throughput(pipeline_sample, params);
        algorithms::clean_LCG(lcg);

    }

#define MAX_PHASE_COUNT 1

    BOOST_AUTO_TEST_CASE( random_degroote_test )
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

        VERBOSE_INFO("Running DeGroote on random");
        for(auto graph : graphs) {
            algorithms::compute_deGrooteClean_throughput(graph, params);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
