//
// Created by jachym on 27/3/23.
//

#define BOOST_TEST_MODULE DseTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "helpers/random_generator.h"
#include "algorithms/transformation/singleOutput.h"


BOOST_FIXTURE_TEST_SUITE( dse_test , WITH_SAMPLE)

BOOST_AUTO_TEST_CASE( sample_dse_test )
{
    parameters_list_t params;
    BOOST_REQUIRE(pipeline_sample);

    algorithms::throughput_buffering_tradeoff_dse(pipeline_sample, params);
}

#define MAX_PHASE_COUNT 5

BOOST_AUTO_TEST_CASE( random_remove_reentrancy_test )
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

    VERBOSE_INFO("Running reentrancy");
    for(auto graph : graphs) {
    algorithms::throughput_buffering_tradeoff_dse(graph, params);
    }
}

BOOST_AUTO_TEST_SUITE_END()
