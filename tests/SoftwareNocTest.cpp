//
// Created by jachym on 6/4/23.
//
#define BOOST_TEST_MODULE SoftwareNocTest
#include "helpers/test_classes.h"
#include "helpers/random_generator.h"
#include "algorithms/software_noc.h"

BOOST_FIXTURE_TEST_SUITE( software_noc_test, WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE( sample_software_noc_test )
    {
        parameters_list_t params;
        BOOST_REQUIRE(pipeline_sample);
        algorithms::software_noc_bufferless(pipeline_sample, params);
    }

    BOOST_AUTO_TEST_CASE( sample_dynamic_noc_test )
    {
        parameters_list_t params;
        BOOST_REQUIRE(pipeline_sample);
        //TODO un-implemented
//        algorithms::dynamic_noc(pipeline_sample, params);
    }

#define MAX_PHASE_COUNT 1

    BOOST_AUTO_TEST_CASE( random_software_noc_test )
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

        VERBOSE_INFO("TDMA schedule on random");
        for(auto graph : graphs) {
            algorithms::software_noc_bufferless(graph, params);
        }
    }
BOOST_AUTO_TEST_SUITE_END()
