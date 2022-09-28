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
    models::NaiveDataflow* g = new models::NaiveDataflow(0);

    VERBOSE_INFO("g->getVerticesCount() == " << g->getVerticesCount());
    BOOST_REQUIRE_EQUAL( g->getVerticesCount(), 0 );

    VERBOSE_INFO("g->getEdgesCount() == " << g->getEdgesCount());
    BOOST_REQUIRE_EQUAL( g->getEdgesCount(), 0 );

    delete g;

}


BOOST_AUTO_TEST_SUITE_END()
