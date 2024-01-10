//
// Created by jachym on 31/3/23.
//
#define BOOST_TEST_MODULE ExpansionGraphTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "algorithms/experiments/ExpansionGraph.h"


BOOST_FIXTURE_TEST_SUITE( expansion_graph_test , WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE( sample_expansion_graph_test )
    {
        periodicity_vector_t pv;
        BOOST_REQUIRE(pipeline_sample);

        //Initialize the periodicity vector
        Vertex v1 = pipeline_sample->getVertexById(1);
        Vertex v2 = pipeline_sample->getVertexById(2);
        Vertex v3 = pipeline_sample->getVertexById(3);
        pv[v1] = 1;
        pv[v2] = 2;
        pv[v3] = 3;


        //Construct the graph
        ExpansionGraph(pipeline_sample, pv);
    }

BOOST_AUTO_TEST_SUITE_END()
