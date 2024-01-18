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

BOOST_FIXTURE_TEST_SUITE(vhdl_merge_operators_test, WITHOUT_VERBOSE)

// The sequential arithmetic graph is the simplest case as its mergeable actors
// have no overlapping executions.
BOOST_AUTO_TEST_CASE(test_simple_merge) {
  models::Dataflow *original_graph = generateSeqArithmeticThreeSimplified();
  // with mergeable actors with no overlapping executions, we expect the
  // results of smart and greedy merge to be the same
  std::vector<std::vector<ARRAY_INDEX>> expectedMergeIds {{6, 9}, // add
                                                          {5, 7, 10}}; // prod

  // greedy merge
  std::vector<std::vector<ARRAY_INDEX>> greedyMergeIds;
  greedyMergeIds = algorithms::greedyMerge(original_graph, 125);
  VERBOSE_INFO("Greedy merge list size: " << greedyMergeIds.size());
  for (auto &i : greedyMergeIds) {
    VERBOSE_INFO("Merge group:");
    for (auto &j : i) {
      VERBOSE_INFO(std::to_string(j) << "  ");
    }
  }
  BOOST_TEST_REQUIRE(greedyMergeIds == expectedMergeIds);

  // smart merge
  std::vector<std::vector<ARRAY_INDEX>> smartMergeIds =
      algorithms::smartMerge(original_graph, 125);
  BOOST_TEST_REQUIRE(smartMergeIds == expectedMergeIds);
}

// Test FIR4 for a merge involving actors that execute in parallel
BOOST_AUTO_TEST_CASE(test_overlapping_merge) {
  models::Dataflow* original_graph = generateFIR4Simplified();
  std::vector<std::vector<ARRAY_INDEX>> expectedGreedyMergeIds{
      {4, 5},     // add
      {1, 2, 3}}; // prod
  std::vector<std::vector<ARRAY_INDEX>> expectedSmartMergeIds {{4, 5}}; // add

  // greedy merge
  std::vector<std::vector<ARRAY_INDEX>> greedyMergeIds =
      algorithms::greedyMerge(original_graph, 125);
  BOOST_TEST_REQUIRE(greedyMergeIds == expectedGreedyMergeIds);

  // smart merge
  std::vector<std::vector<ARRAY_INDEX>> smartMergeIds =
    algorithms::smartMerge(original_graph, 125);
  BOOST_TEST_REQUIRE(smartMergeIds == expectedSmartMergeIds);
}

BOOST_AUTO_TEST_CASE(test_merge_problematics) {
  // rms and echo are two benchmark applications that I've had issues
  // merging in the past

  std::vector<std::vector<ARRAY_INDEX>> expectedEchoGreedyMergeIds{
      {6, 10, 12}, // int
      {5, 15, 16}, // prod
      {7, 11}};    // add

  std::vector<std::vector<ARRAY_INDEX>> expectedEchoSmartMergeIds{
      {6, 10, 12}, // int
      {5, 16},     // prod
      {7, 11}};    // add

  models::Dataflow *echo_original = generateEchoSimplified();

  // greedy merge
  std::vector<std::vector<ARRAY_INDEX>> echoGreedyMergeIds =
      algorithms::greedyMerge(echo_original, 125);
  BOOST_TEST_REQUIRE(echoGreedyMergeIds == expectedEchoGreedyMergeIds);

  // smart merge
  std::vector<std::vector<ARRAY_INDEX>> echoSmartMergeIds =
    algorithms::smartMerge(echo_original, 125);
  BOOST_TEST_REQUIRE(echoSmartMergeIds == expectedEchoSmartMergeIds);

  std::vector<std::vector<ARRAY_INDEX>> expectedRMS2GreedyMergeIds{
      {9, 17},         // int
      {7, 15},         // pow
      {8, 16, 13, 20}, // prod
      {14, 21},        // sqrt
      {3, 5},          // float
      {10, 18},        // add
      {11, 19}};       // diff

  std::vector<std::vector<ARRAY_INDEX>> expectedRMS2SmartMergeIds{
      {8, 13}}; // prod

  models::Dataflow *rms2_original = generateRMS2Simplified();
  // greedy merge
  std::vector<std::vector<ARRAY_INDEX>> rms2GreedyMergeIds =
      algorithms::greedyMerge(rms2_original, 125);
  BOOST_TEST_REQUIRE(rms2GreedyMergeIds == expectedRMS2GreedyMergeIds);

  // smart merge
  std::vector<std::vector<ARRAY_INDEX>> rms2SmartMergeIds =
    algorithms::smartMerge(rms2_original, 125);
  BOOST_TEST_REQUIRE(rms2SmartMergeIds == expectedRMS2SmartMergeIds);
}

BOOST_AUTO_TEST_SUITE_END()
