/*
 * test_helper.h
 *
 *  Created on: Nov 19, 2019
 *      Author: toky
 */

#ifndef TESTS_TEST_HELPER_H_
#define TESTS_TEST_HELPER_H_
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/test/included/unit_test.hpp>
#include <commons/verbose.h>

#include "sample.h"
class WITHOUT_VERBOSE {
	public:
	WITHOUT_VERBOSE() {
    	commons::set_verbose_mode(commons::INFO_LEVEL);
    	BOOST_TEST_MESSAGE( "WITHOUT_VERBOSE Setup Done" );
    }
	~WITHOUT_VERBOSE()                    {
    	BOOST_TEST_MESSAGE( "WITHOUT_VERBOSE Teardown Done" );
    }
};

class WITH_VERBOSE {
	public:
	WITH_VERBOSE() {
    	commons::set_verbose_mode(commons::DEBUG_LEVEL);
    	BOOST_TEST_MESSAGE( "WITH_VERBOSE Setup Done" );
    }
	~WITH_VERBOSE()                    {
    	BOOST_TEST_MESSAGE( "WITH_VERBOSE Teardown Done" );
    }
};

class WITH_SAMPLE {
	public :

	models::Dataflow * pipeline_sample ;
	models::Dataflow * cycle_sample ;
	models::Dataflow * mult_scc_sample ;
	WITH_SAMPLE () {
		pipeline_sample = generateSamplePipeline () ;
		cycle_sample = generateSampleCycle () ;
                mult_scc_sample = generateSampleMultSCC();
    	BOOST_TEST_MESSAGE( "WITH_SAMPLE Setup Done" );
	}
	~WITH_SAMPLE () {
		delete pipeline_sample;
		delete cycle_sample;
                delete mult_scc_sample;
    	BOOST_TEST_MESSAGE( "WITH_SAMPLE Teardown Done" );
	}
};



#endif /* TESTS_TEST_HELPER_H_ */
