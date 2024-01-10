//
// Created by toky on 24/4/23.
//

#define BOOST_TEST_MODULE SC1SC2Test

#include "helpers/test_classes.h"
#include "helpers/random_generator.h"
#include "algorithms/normalization.h"
#include  <algorithms/liveness/SC.h>


BOOST_FIXTURE_TEST_SUITE( liveness_sc1_sc2_test, WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE(sc1) {

         commons::set_verbose_mode(commons::DEBUG_LEVEL);

        models::Dataflow* g      = generateDAC2013();
        computeRepetitionVector(g);
        algorithms::normalize(g);

        algorithms::liveness::CSDF_SC1(g) ;

    }


    BOOST_AUTO_TEST_CASE(sc2) {

        commons::set_verbose_mode(commons::DEBUG_LEVEL);

        models::Dataflow* g      = generateDAC2013();
        computeRepetitionVector(g);
        algorithms::normalize(g);

        algorithms::liveness::CSDF_SC2(g) ;

    }


BOOST_AUTO_TEST_SUITE_END()
