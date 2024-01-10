/*
 * CycleDetectionTest.cpp
 *
 *  Created on: Sep 29 2022
 *      Author: toky
 */


#define BOOST_TEST_MODULE CycleDetectionTest
#include "helpers/test_classes.h"
#include <printers/stdout.h>
#include <models/Dataflow.h>
#include <algorithms/analysis/cycle_detection.h>

BOOST_FIXTURE_TEST_SUITE( CycleDetectionTest , WITH_VERBOSE)

BOOST_AUTO_TEST_CASE( has_self_loops_test )
{
    models::Dataflow* g = new models::Dataflow(0);
    auto v1 = g->addVertex();
    auto v2 = g->addVertex();
    auto e1 = g->addEdge(v1,v2);
    BOOST_REQUIRE_EQUAL(algorithms::has_self_loops(g), false); // default 0
    g->setReentrancyFactor(v1, 1);
    BOOST_REQUIRE_EQUAL(algorithms::has_self_loops(g), true); // set to 1
    g->setReentrancyFactor(v1, 0);
    BOOST_REQUIRE_EQUAL(algorithms::has_self_loops(g), false); // set back to 0
}

BOOST_AUTO_TEST_CASE( has_cycles_test )
{

    models::Dataflow* g = new models::Dataflow(0);
    auto v1 = g->addVertex();
    auto v2 = g->addVertex();
    auto v3 = g->addVertex();
    auto v4 = g->addVertex();
    auto v5 = g->addVertex();
    auto e1 = g->addEdge(v1,v2);
    auto e2 = g->addEdge(v2,v3);
    auto e3 = g->addEdge(v3,v4);

    BOOST_REQUIRE_EQUAL(algorithms::has_cycles(g), false);

    auto e4 = g->addEdge(v2,v4);

    BOOST_REQUIRE_EQUAL(algorithms::has_cycles(g), false);

    auto e5 = g->addEdge(v4,v1);
    auto e6 = g->addEdge(v4,v5);
    auto e7 = g->addEdge(v5,v1);

    BOOST_REQUIRE_EQUAL(algorithms::has_cycles(g), true);

}

BOOST_AUTO_TEST_CASE( get_simple_cycles_test )
{
    models::Dataflow* g = new models::Dataflow(0);
    auto v1 = g->addVertex();
    auto v2 = g->addVertex();
    auto v3 = g->addVertex();
    auto v4 = g->addVertex();
    auto v5 = g->addVertex();
    auto e1 = g->addEdge(v1,v2);
    auto e2 = g->addEdge(v2,v3);
    auto e3 = g->addEdge(v3,v4);

    BOOST_REQUIRE_EQUAL(algorithms::get_simple_cycles(g).size(), 0);

    auto e4 = g->addEdge(v2,v4);

    BOOST_REQUIRE_EQUAL(algorithms::get_simple_cycles(g).size(), 0);

    auto e5 = g->addEdge(v4,v1);
    auto e6 = g->addEdge(v4,v5);
    auto e7 = g->addEdge(v5,v1);

    BOOST_REQUIRE_EQUAL(algorithms::get_simple_cycles(g).size(), 4);

    VERBOSE_INFO("Cycles are " << commons::toString(algorithms::get_simple_cycles(g)));

}

BOOST_AUTO_TEST_SUITE_END()






