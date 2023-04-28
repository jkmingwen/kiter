//
// Created by toky on 24/4/23.
//

#define BOOST_TEST_MODULE ThroughputBufferingModularDSETest

#include "helpers/test_classes.h"
#include "helpers/random_generator.h"
#include <algorithms/dse/ThroughputBufferingDSE.h>


BOOST_FIXTURE_TEST_SUITE( ThroughputBuffering_modular_dse_test, WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE(solve_ThroughputBuffering_test_with_reentrancy) {

         commons::set_verbose_mode(commons::DEBUG_LEVEL);
        // create the input graph
        models::Dataflow g;
        Vertex v1 = g.addVertex(4,"v1");
        Vertex v2 = g.addVertex(6,"v2");
        Edge e1 = g.addEdge(v1, v2, 1,"e");

        g.setPreload(e1, 2);


        g.setEdgeInPhases(e1, {1, 2});
        g.setEdgeOutPhases(e1, {1});


        g.setVertexDuration(v1,{1,1});
        g.setVertexDuration(v2,{1});

        g.setReentrancyFactor(v1,1);
        g.setReentrancyFactor(v2,1);

        g.setEdgeType(e1,EDGE_TYPE::NORMAL_EDGE);
        auto throughput_res = algorithms::compute_Kperiodic_throughput_and_cycles(&g);
        TIME_UNIT target_throughput = throughput_res.throughput;
        std::cout << "Throughput is " << target_throughput << std::endl;

        algorithms::dse::TokenConfigurationSet res = algorithms::dse::solve_throughputbuffering( &g, false, true) ;

    }

    BOOST_AUTO_TEST_CASE(ThroughputBuffering_dse_test) {

         commons::set_verbose_mode(commons::DEBUG_LEVEL);
        // create the input graph
        models::Dataflow g;
        Vertex v1 = g.addVertex(4,"v1");
        Vertex v2 = g.addVertex(6,"v2");
        Edge e1 = g.addEdge(v1, v2, 1,"e");

        g.setPreload(e1, 2);


        g.setEdgeInPhases(e1, {1, 2});
        g.setEdgeOutPhases(e1, {1});


        g.setVertexDuration(v1,{1,1});
        g.setVertexDuration(v2,{1});

        g.setReentrancyFactor(v1,1);
        g.setReentrancyFactor(v2,1);

        g.setEdgeType(e1,EDGE_TYPE::NORMAL_EDGE);

        parameters_list_t params;
        params["realtime"] = "1";
        algorithms::dse::throughputbuffering_dse(&g, params);
    }


BOOST_AUTO_TEST_SUITE_END()
