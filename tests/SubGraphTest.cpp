
#define BOOST_TEST_MODULE DSEDebug
#include "helpers/test_classes.h"
#include <algorithms/transformation/subgraph.h>
#include <models/Dataflow.h>

BOOST_FIXTURE_TEST_SUITE( test_build_subgraph , WITHOUT_VERBOSE) // WITH_VERBOSE or WITHOUT_VERBOSE

    BOOST_AUTO_TEST_CASE(test_empty_critical_cycle) {
        models::Dataflow g;
        std::set<ARRAY_INDEX> critical_edges;
        models::Dataflow *small = algorithms::build_subgraph(&g, critical_edges);
        BOOST_REQUIRE_EQUAL(small->getEdgesCount(), 0);
        BOOST_REQUIRE_EQUAL(small->getVerticesCount(), 0);
    }


    BOOST_AUTO_TEST_CASE(test_subgraph_construction) {
        // create the input graph
        models::Dataflow g;
        Vertex v1 = g.addVertex(4,"v1");
        Vertex v2 = g.addVertex(6,"v2");
        Edge e = g.addEdge(v1, v2, 1,"e");
        g.setPreload(e, 10);
        g.setEdgeInPhases(e, {1, 2});
        g.setEdgeOutPhases(e, {1});
        g.setVertexDuration(v1,{1,1});
        g.setVertexDuration(v2,{1});
        g.setReentrancyFactor(v1,0);
        g.setReentrancyFactor(v2,0);

        // select critical edges
        std::set<ARRAY_INDEX> critical_edges = {g.getEdgeId(e)};

        // build subgraph
        models::Dataflow* cc_g = algorithms::build_subgraph(&g, critical_edges);

        // check subgraph properties
        BOOST_CHECK(cc_g->getVerticesCount() == 2);
        BOOST_CHECK(cc_g->getEdgesCount() == 1);
        BOOST_CHECK(cc_g->getVertexName(cc_g->getVertexById(4)) == "v1");
        BOOST_CHECK(cc_g->getVertexName(cc_g->getVertexById(6)) == "v2");
        BOOST_CHECK(cc_g->getEdgeName(cc_g->getEdgeById(1)) == "e");
        BOOST_CHECK(cc_g->getPreload(cc_g->getEdgeById(1)) == 10);
        BOOST_CHECK(cc_g->getEdgeInVector(cc_g->getEdgeById(1)) == std::vector<TOKEN_UNIT>({1, 2}));
        BOOST_CHECK(cc_g->getEdgeOutVector(cc_g->getEdgeById(1)) == std::vector<TOKEN_UNIT>({1}));

        delete cc_g;
    }

BOOST_AUTO_TEST_SUITE_END()
