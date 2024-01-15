/*
 * VHDLMergeOperatorsTest.cpp
 *
 *  Created on: Jan 11, 2024
 *      Author: jkmingwen
 */

#define BOOST_TEST_MODULE VHDLMergeTest
#include <printers/stdout.h>
#include "helpers/dadop_test_graphs.h"
#include <models/Dataflow.h>
#include <printers/SDF3Wrapper.h>
#include <algorithms/transformation/merge_operators.h>
#include <boost/test/unit_test_suite.hpp>

#include "helpers/test_classes.h"

BOOST_FIXTURE_TEST_SUITE(test_vhdl_merge_operators_test, WITHOUT_VERBOSE)

// The sequential arithmetic graph simplest case as its mergeable actors
// have no overlapping executions.
BOOST_AUTO_TEST_CASE(test_simple_merge) {
  models::Dataflow* original_graph = generateSeqArithmeticThreeSimplified();
  parameters_list_t parameters;
  parameters["MERGE_STRATEGY"] = "greedy";
  algorithms::transformation::merge_operators(original_graph, parameters);
  parameters["MERGE_STRATEGY"] = "smart";
  algorithms::transformation::merge_operators(original_graph, parameters);
  // models::Dataflow* expected_graph
}

// We test FIR4 for a merge involving actors that execute in parallel
BOOST_AUTO_TEST_CASE(test_overlapping_merge) {
  models::Dataflow* original_graph = generateFIR4Simplified();
  models::Dataflow* expected_graph_smart = generateFIR4SmartMerge();
  models::Dataflow* expected_graph_greedy = generateFIR4GreedyMerge();

  parameters_list_t parameters;
  parameters["MERGE_STRATEGY"] = "greedy";
  algorithms::transformation::merge_operators(original_graph, parameters);
  parameters["MERGE_STRATEGY"] = "smart";
  algorithms::transformation::merge_operators(original_graph, parameters);
}

BOOST_AUTO_TEST_CASE(test_merge_problematics) {
  // rms and echo are two benchmark applications that I've had issues
  // merging in the past (using greedy merge)
  parameters_list_t parameters;
  parameters["MERGE_STRATEGY"] = "greedy";
  models::Dataflow* original_graph = generateEcho();
  models::Dataflow* rms2_simplified = generateRMS2Simplified();
  algorithms::transformation::merge_operators(original_graph, parameters);
  algorithms::transformation::merge_operators(rms2_simplified, parameters);
}



BOOST_AUTO_TEST_SUITE_END()
