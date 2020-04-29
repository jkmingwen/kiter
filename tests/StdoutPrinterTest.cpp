/*
 *
 *  Created on: Nov 6, 2019
 *      Author: toky
 */

#include <printers/stdout.h>
#include <algorithms/schedulings.h>
#include <algorithms/repetition_vector.h>

#define BOOST_TEST_MODULE TemplateTest
#include "helpers/test_classes.h"


BOOST_FIXTURE_TEST_SUITE( stdout_printer_test_suite, WITH_SAMPLE)

BOOST_AUTO_TEST_CASE( DOT_graph_printers )
{
	std::string dot_string1 = printers::GenerateGraphDOT       (sample  , true);
	std::string dot_string2 = printers::GenerateGraphDOT       (sample  , false);
	std::string dot_string3 = printers::GenerateGraphDOT       (sample  );
}

BOOST_AUTO_TEST_CASE( DOT_scheduling_printers )
{
	VERBOSE_ASSERT(computeRepetitionVector(sample), "computeRepetitionVector failed");
	models::Scheduling res1 = algorithms::scheduling::CSDF_KPeriodicScheduling_LP    (sample, algorithms::scheduling::generate1PeriodicVector(sample));
	std::string dot_string1 = printers::PeriodicScheduling2DOT    (sample, res1,  100, false, 10, 10);

}

BOOST_AUTO_TEST_SUITE_END()




