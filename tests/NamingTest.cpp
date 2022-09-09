/*
 * NamingTest.cpp
 *
 *  Created on: Feb 8, 2021
 *      Author: toky
 */


#define BOOST_TEST_MODULE NamingTest
#include "helpers/test_classes.h"
#include <models/Dataflow.h>
#include <chrono>

BOOST_FIXTURE_TEST_SUITE( naming_test_suite, WITH_VERBOSE)

BOOST_AUTO_TEST_CASE( renaming_test )
{
    models::Dataflow* g = new models::Dataflow(0);
    auto v1 = g->addVertex(1);
    auto v2 = g->addVertex(2);
    auto v3 = g->addVertex(3);
    auto v4 = g->addVertex(4);
    auto v5 = g->addVertex(5);
    auto e1 = g->addEdge(v1,v2);
    auto e2 = g->addEdge(v2,v3);
    auto e3 = g->addEdge(v3,v4);
    auto e4 = g->addEdge(v2,v4);
    auto e5 = g->addEdge(v4,v1);
    auto e6 = g->addEdge(v4,v5);

    VERBOSE_INFO("g->getVerticesCount() == " << g->getVerticesCount());
    BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 5 );

    VERBOSE_INFO("g->getEdgesCount() == " << g->getEdgesCount());
    BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 6 );
    g->setVertexName(v1, "v1");
    g->setVertexName(v2, "v2");
    g->setVertexName(v3, "v3");
    g->setVertexName(v4, "v4");


    BOOST_REQUIRE_EQUAL( g->getVertexName(v1), "v1" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v2), "v2" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v3), "v3" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v4), "v4" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v5), "" );


    BOOST_REQUIRE_THROW( g->getVertexByName("v1bis"), std::out_of_range);
    BOOST_REQUIRE_EQUAL( v1, g->getVertexByName("v1") );
    BOOST_REQUIRE_EQUAL( v2, g->getVertexByName("v2") );
    BOOST_REQUIRE_EQUAL( v3, g->getVertexByName("v3") );
    BOOST_REQUIRE_EQUAL( v4, g->getVertexByName("v4") );

    // This one should not work
    BOOST_REQUIRE_EQUAL( v5, g->getVertexByName("") );

    g->setVertexName(v1, "v1");

    BOOST_REQUIRE_EQUAL( g->getVertexName(v1), "v1" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v2), "v2" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v3), "v3" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v4), "v4" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v5), "" );


    BOOST_REQUIRE_THROW( g->getVertexByName("v1bis"), std::out_of_range);
    BOOST_REQUIRE_EQUAL( v1, g->getVertexByName("v1") );
    BOOST_REQUIRE_EQUAL( v2, g->getVertexByName("v2") );
    BOOST_REQUIRE_EQUAL( v3, g->getVertexByName("v3") );
    BOOST_REQUIRE_EQUAL( v4, g->getVertexByName("v4") );

    // This one should not work
    BOOST_REQUIRE_EQUAL( v5, g->getVertexByName("") );

    g->setVertexName(v1, "v1bis");


    BOOST_REQUIRE_EQUAL( g->getVertexName(v1), "v1bis" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v2), "v2" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v3), "v3" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v4), "v4" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v5), "" );


    BOOST_REQUIRE_THROW( g->getVertexByName("v1"), std::out_of_range);
    BOOST_REQUIRE_EQUAL( v1, g->getVertexByName("v1bis") );
    BOOST_REQUIRE_EQUAL( v2, g->getVertexByName("v2") );
    BOOST_REQUIRE_EQUAL( v3, g->getVertexByName("v3") );
    BOOST_REQUIRE_EQUAL( v4, g->getVertexByName("v4") );


    // This one should not work
    BOOST_REQUIRE_EQUAL( v5, g->getVertexByName("") );


    g->setVertexName(v1, "v1");

    BOOST_REQUIRE_EQUAL( g->getVertexName(v1), "v1" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v2), "v2" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v3), "v3" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v4), "v4" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v5), "" );

    BOOST_REQUIRE_EQUAL( v1, g->getVertexByName("v1") );
    BOOST_REQUIRE_EQUAL( v2, g->getVertexByName("v2") );
    BOOST_REQUIRE_EQUAL( v3, g->getVertexByName("v3") );
    BOOST_REQUIRE_EQUAL( v4, g->getVertexByName("v4") );


    // This one should not work
    BOOST_REQUIRE_EQUAL( v5, g->getVertexByName("") );

    g->removeVertex(v4);

    // Need to update the Vertex references after removing a vertex
    v1 = g->getVertexById(1);
    v2 = g->getVertexById(2);
    v3 = g->getVertexById(3);
    BOOST_REQUIRE_THROW( g->getVertexById(4), std::out_of_range);
    v5 = g->getVertexById(5);

    BOOST_REQUIRE_EQUAL( g->getVertexName(v1), "v1" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v2), "v2" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v3), "v3" );
    BOOST_REQUIRE_EQUAL( g->getVertexName(v5), "" );

    BOOST_REQUIRE_EQUAL( v1, g->getVertexByName("v1") );
    BOOST_REQUIRE_EQUAL( v2, g->getVertexByName("v2") );
    BOOST_REQUIRE_EQUAL( v3, g->getVertexByName("v3") );
    BOOST_REQUIRE_THROW( g->getVertexByName("v4"), std::out_of_range);


    // This one should not work
    BOOST_REQUIRE_EQUAL( v5, g->getVertexByName("") );




    g->setVertexName(v5, "v4");
    BOOST_REQUIRE_EQUAL( v5, g->getVertexByName("v4") );



    delete g;
}

BOOST_AUTO_TEST_SUITE_END()




