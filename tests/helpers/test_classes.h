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
    }
	~WITHOUT_VERBOSE()                    {
    }
};

class WITH_VERBOSE {
	public:
	WITH_VERBOSE() {
    	commons::set_verbose_mode(commons::DEBUG_LEVEL);
    }
	~WITH_VERBOSE()                    {
    }
};

class WITH_SAMPLE {
	public :

	models::Dataflow * pipeline_sample ;
	models::Dataflow * cycle_sample ;
	WITH_SAMPLE () {
		pipeline_sample = generateSamplePipeline () ;
		cycle_sample = generateSampleCycle () ;
	}
	~WITH_SAMPLE () {
		delete pipeline_sample;
		delete cycle_sample;
	}
};



#endif /* TESTS_TEST_HELPER_H_ */
