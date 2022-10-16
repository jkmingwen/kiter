/*
 * DataflowTest.cpp
 *
 *  Created on: Nov 6, 2019
 *      Author: toky
 */

#define BOOST_TEST_MODULE NaiveDataflowTest
#include "helpers/test_classes.h"

#include <models/NaiveDataflow.h>

BOOST_FIXTURE_TEST_SUITE( test_suite_create_naive_dataflow , WITH_VERBOSE)


    BOOST_AUTO_TEST_CASE( test_empty_naive_dataflow )
    {
        models::NaiveDataflow* g = new models::NaiveDataflow();

        VERBOSE_INFO("g->getVerticesCount() == " << g->getVerticesCount());
        BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 0 );

        VERBOSE_INFO("g->getEdgesCount() == " << g->getEdgesCount());
        BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 0 );

        delete g;

    }


    BOOST_AUTO_TEST_CASE( test_add_remove_dataflow )
    {
        models::NaiveDataflow* g = new models::NaiveDataflow();

        auto v1 = g->addVertex();
        g->removeVertex(v1);

        BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 0 );
        BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 0 );

        delete g;
    }

    BOOST_AUTO_TEST_CASE( test_add_remove_named_dataflow )
    {
        models::NaiveDataflow* g = new models::NaiveDataflow();

        auto v1 = g->addVertex("nop");
        g->removeVertex(v1);

        BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 0 );
        BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 0 );

        delete g;
    }

    BOOST_AUTO_TEST_CASE( test_add_remove_ided_dataflow )
    {
        models::NaiveDataflow* g = new models::NaiveDataflow();

        auto v1 = g->addVertex(10);
        g->removeVertex(v1);

        BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 0 );
        BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 0 );

        delete g;
    }

    BOOST_AUTO_TEST_CASE( test_small_naive_dataflow )
    {
        models::NaiveDataflow* g = new models::NaiveDataflow();

        const models::NaiveVertexRef v1 = g->addVertex(0, "A");
        VERBOSE_DEBUG("g->addVertex() happended");

        BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 1 );
        BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 0 );
        BOOST_REQUIRE_EQUAL( g->getVertexByName("A"), v1 );

        auto v2 = g->addVertex(1, "B");

        BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 2 );
        BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 0 );
        BOOST_REQUIRE_EQUAL( g->getVertexByName("B"), v2 );
        BOOST_REQUIRE_NE( g->getVertexByName("A"), v2 );

        auto v3 = g->addVertex(2, "C");

        BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 3 );
        BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 0 );

        BOOST_REQUIRE_EQUAL( g->getVertexByName("A"), g->getVertexById(0) );
        BOOST_REQUIRE_EQUAL( g->getVertexByName("B"), g->getVertexById(1) );
        BOOST_REQUIRE_EQUAL( g->getVertexByName("C"), g->getVertexById(2) );

        BOOST_REQUIRE_NE( g->getVertexByName("A"), v3 );
        BOOST_REQUIRE_NE( g->getVertexByName("B"), v3 );
        BOOST_REQUIRE_EQUAL( g->getVertexByName("C"), v3 );

        auto e1 = g->addEdge(g->getVertexByName("A"),g->getVertexByName("B"), 0, "e12");

        BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 3 );
        BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 1 );

        auto e2 = g->addEdge(g->getVertexByName("B"),g->getVertexByName("C"), 1, "e23");

        BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 3 );
        BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 2 );

        g->removeEdge(g->getEdgeByName("e23"));

        BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 3 );
        BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 1 );

        g->removeVertex(g->getVertexByName("A"));

        BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 2 );
        BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 0 );

        // Crash please
        BOOST_REQUIRE_THROW(g->getVertexByName("A"), std::out_of_range);


        delete g;

    }

    BOOST_AUTO_TEST_CASE( test_anonymous_dataflow )
    {
    models::NaiveDataflow* g = new models::NaiveDataflow();

        auto v1 = g->getVertexId(g->addVertex());
        auto v2 = g->getVertexId(g->addVertex());
        auto v3 = g->getVertexId(g->addVertex());
        auto e1 = g->getEdgeId(g->addEdge(g->getVertexById(v1),g->getVertexById(v2)));
        auto e2 = g->getEdgeId(g->addEdge(g->getVertexById(v2),g->getVertexById(v3)));

        g->removeEdge(g->getEdgeById(e1));
        g->removeVertex(g->getVertexById(v1));

        BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 2 );
        BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 1 );

        g->removeVertex(g->getVertexById(v3));

        BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 1 );
        BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 0 );

        delete g;
    }
BOOST_AUTO_TEST_SUITE_END()
