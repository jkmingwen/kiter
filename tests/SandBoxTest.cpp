/*
 * SandBoxTest.cpp
 *
 *  Created on: Oct 4, 2022
 *      Author: toky
 */


#define BOOST_TEST_MODULE SandBoxTest
#include "helpers/test_classes.h"
#include <chrono>
#include <models/NaiveDataflow.h>

#include <commons/iterators.h>


BOOST_FIXTURE_TEST_SUITE( sand_box_test_suite, WITH_VERBOSE)



    BOOST_AUTO_TEST_CASE( generator_test )
{
        models::NaiveDataflow g;
        g.addVertex();
        g.addVertex();
        g.addVertex();
        models::NaiveVertexRef ref1 = {0};
        models::NaiveVertexRef ref2 = {1};
        models::NaiveVertexRef ref3 = {2};

        range_t<incremental_iterator<models::NaiveVertexRef>> vertices = {incremental_iterator<models::NaiveVertexRef>(ref1, 3), incremental_iterator<models::NaiveVertexRef>()};

    for (auto i : vertices) {
        std::cout << "pouet " << i.vector_index << std::endl;
    }


}

BOOST_AUTO_TEST_SUITE_END()




