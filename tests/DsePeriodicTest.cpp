//
// Created by jachym on 28/3/23.
//

#define BOOST_TEST_MODULE DsePeriodicTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "helpers/random_generator.h"
#include "algorithms/dse/periodic.h"


BOOST_FIXTURE_TEST_SUITE( dse_periodic_test , WITH_SAMPLE)

BOOST_AUTO_TEST_CASE( sample_periodic_test )
{
    parameters_list_t params;
    BOOST_REQUIRE(pipeline_sample);

    VERBOSE_INFO("DSE: periodic - integer solving = false");
    algorithms::compute_csdf_dse_periodic(pipeline_sample, params);

    VERBOSE_INFO("DSE: periodic - integer solving = true");
    params["ILP"] = true;
    algorithms::compute_csdf_dse_periodic(pipeline_sample, params);
}

BOOST_AUTO_TEST_CASE( sample_speriodic_test )
{
    parameters_list_t params;
    BOOST_REQUIRE(pipeline_sample);

    VERBOSE_INFO("DSE: speriodic - integer solving = false");
    algorithms::compute_csdf_dse_speriodic(pipeline_sample, params);

    VERBOSE_INFO("DSE: speriodic - integer solving = true");
    params["ILP"] = true;
    algorithms::compute_csdf_dse_speriodic(pipeline_sample, params);
}

#define MAX_PHASE_COUNT 5

BOOST_AUTO_TEST_CASE( random_periodic_test )
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

    VERBOSE_INFO("Running periodic");
    for(auto graph : graphs) {
        algorithms::compute_csdf_dse_periodic(graph, params);
    }
}

BOOST_AUTO_TEST_CASE( random_speriodic_test )
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

    VERBOSE_INFO("Running speriodic");

    for(auto graph : graphs) {
        algorithms::compute_csdf_dse_speriodic(graph, params);
    }
}

BOOST_AUTO_TEST_SUITE_END()
