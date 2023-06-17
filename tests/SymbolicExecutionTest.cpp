/*
 *
 *  Created on: Oct 25, 2021
 *      Author: jkmingwen
 */

#define BOOST_TEST_MODULE SymbolicExecutionTest
#include "helpers/test_classes.h"
#include <algorithms/throughput/symbolic_execution.h>
#include <algorithms/schedulings.h>
#include <models/Dataflow.h>


BOOST_FIXTURE_TEST_SUITE(symbolic_execution_test_suite, WITH_SAMPLE)

BOOST_AUTO_TEST_CASE( test_throughput_computation )
{
  parameters_list_t test;
  // test 1: graph with single strongly-connected component
  TIME_UNIT thr_test1 = algorithms::compute_asap_throughput(cycle_sample, test);
  std::cout << "Looking for (Period:23) or (Thr:0.0434782609)" << std::endl;
  BOOST_CHECK_CLOSE( 1 / thr_test1, 23, 0.0001 );

  // test 2: graph with multiple strongly-connected component
  TIME_UNIT thr_test2 = algorithms::compute_asap_throughput(mult_scc_sample, test);
  std::cout << "Looking for (Period:12) or (Thr:0.0833333333)" << std::endl;
  BOOST_CHECK_CLOSE( 1 / thr_test2, 12, 0.0001 );
}

BOOST_AUTO_TEST_CASE( test_storage_dependency_computation )
{
  // test 1: strongly connected graph (cycle_sample)
  // test 2: graph with multiple strongly connected components (mult_scc_sample)
  // test for storage dep detection using storage distributions leading to
  // deadlocked and non-deadlocked graph configurations
  parameters_list_t test;

  // test 1: deadlocked storage distribution
  std::set<Edge> test1_deadlock_expected;
  std::map<Edge, BufferInfos> test1_channel_quantities;
  TOKEN_UNIT test1_dist_sz = 0;
  // initialise storage distribution that will lead to deadlock
  test1_channel_quantities[cycle_sample->getEdgeById(1)].buffer_size = 3;
  test1_channel_quantities[cycle_sample->getEdgeById(2)].buffer_size = 7;
  test1_channel_quantities[cycle_sample->getEdgeById(3)].buffer_size = 5;
  {ForEachEdge(cycle_sample, e) {
      test1_channel_quantities[e].preload = cycle_sample->getPreload(e);
      test1_dist_sz += test1_channel_quantities[e].buffer_size;
    }}
  StorageDistribution test1_deadlock(cycle_sample,
                                     0,
                                     test1_channel_quantities);
  test1_deadlock_expected.insert(cycle_sample->getEdgeById(1));

  // test 1: periodic storage distribution
  std::set<Edge> test1_periodic_expected;
  test1_dist_sz = 0; // reset for next test
  // inititalise storage distribution with known throughput
  test1_channel_quantities[cycle_sample->getEdgeById(1)].buffer_size = 8;
  test1_channel_quantities[cycle_sample->getEdgeById(2)].buffer_size = 9;
  test1_channel_quantities[cycle_sample->getEdgeById(3)].buffer_size = 5;
  {ForEachEdge(cycle_sample, e) {
      test1_channel_quantities[e].preload = cycle_sample->getPreload(e);
      test1_dist_sz += test1_channel_quantities[e].buffer_size;
    }}
  StorageDistribution test1_periodic(cycle_sample,
                                     0,
                                     test1_channel_quantities);
  test1_periodic_expected.insert(cycle_sample->getEdgeById(1));
  test1_periodic_expected.insert(cycle_sample->getEdgeById(2));

  // test 1 expected results:
  // - deadlocked graph: period = 0,  storage dependency channels = {1}
  // - periodic graph:   period = 25, storage dependency channels = {1, 2}
  kperiodic_result_t dse_test1_deadlock = algorithms::compute_asap_throughput_and_cycles(cycle_sample, test, test1_deadlock);
  std::cout << "Comparing storage dependecies (Channel 1)" << std::endl;
  BOOST_CHECK(dse_test1_deadlock.critical_edges == test1_deadlock_expected);
  std::cout << "Looking for (Period:0) or (Thr:0)" << std::endl;
  BOOST_CHECK_CLOSE(dse_test1_deadlock.throughput, 0, 0.0001);

  kperiodic_result_t dse_test1_periodic = algorithms::compute_asap_throughput_and_cycles(cycle_sample, test, test1_periodic);
  std::cout << "Comparing storage dependencies (Channels 1 and 2)" << std::endl;
  BOOST_CHECK(dse_test1_periodic.critical_edges == test1_periodic_expected);
  std::cout << "Looking for (Period:25) or (Thr:0.04)" << std::endl;
  BOOST_CHECK_CLOSE(1 / dse_test1_periodic.throughput, 25, 0.0001);

  // test 2: deadlocked storage distribution
  std::set<Edge> test2_deadlock_expected;
  std::map<Edge, BufferInfos>  test2_channel_quantities;
  TOKEN_UNIT test2_dist_sz = 0;
  // initialise storage distribution that will lead to deadlock
  test2_channel_quantities[mult_scc_sample->getEdgeById(1)].buffer_size = 7;
  test2_channel_quantities[mult_scc_sample->getEdgeById(2)].buffer_size = 4;
  test2_channel_quantities[mult_scc_sample->getEdgeById(3)].buffer_size = 25;
  test2_channel_quantities[mult_scc_sample->getEdgeById(4)].buffer_size = 36;
  {ForEachEdge(mult_scc_sample, e) {
      test2_channel_quantities[e].preload = mult_scc_sample->getPreload(e);
      test2_dist_sz += test2_channel_quantities[e].buffer_size;
    }}
  StorageDistribution test2_deadlock(mult_scc_sample,
                                     0,
                                     test2_channel_quantities);
  test2_deadlock_expected.insert(mult_scc_sample->getEdgeById(1));
  // test 2: periodic storage distribution
  std::set<Edge> test2_periodic_expected;
  test2_dist_sz = 0; // reset for next test
  // inititalise storage distribution with known throughput
  test2_channel_quantities[mult_scc_sample->getEdgeById(1)].buffer_size = 9;
  test2_channel_quantities[mult_scc_sample->getEdgeById(2)].buffer_size = 4;
  test2_channel_quantities[mult_scc_sample->getEdgeById(3)].buffer_size = 25;
  test2_channel_quantities[mult_scc_sample->getEdgeById(4)].buffer_size = 36;
  {ForEachEdge(mult_scc_sample, e) {
      test2_channel_quantities[e].preload = mult_scc_sample->getPreload(e);
      test2_dist_sz += test2_channel_quantities[e].buffer_size;
    }}
  StorageDistribution test2_periodic(mult_scc_sample,
                                     0,
                                     test2_channel_quantities);
  test2_periodic_expected.insert(mult_scc_sample->getEdgeById(1));
  test2_periodic_expected.insert(mult_scc_sample->getEdgeById(2));

  // test 2 expected results:
  // - deadlocked graph: period = 0,  storage dependency channels = {1}
  // - periodic graph:   period = 13, storage dependency channels = {1, 2}
  kperiodic_result_t dse_test2_deadlock = algorithms::compute_asap_throughput_and_cycles(mult_scc_sample, test, test2_deadlock);
  std::cout << "Comparing storage dependecies (Channel 1)" << std::endl;
  BOOST_CHECK(dse_test2_deadlock.critical_edges == test2_deadlock_expected);
  std::cout << "Looking for (Period:0) or (Thr:0)" << std::endl;
  BOOST_CHECK_CLOSE(dse_test2_deadlock.throughput, 0, 0.0001);

  kperiodic_result_t dse_test2_periodic = algorithms::compute_asap_throughput_and_cycles(mult_scc_sample, test, test2_periodic);
  std::cout << "Comparing storage dependencies (Channels 1 and 2)" << std::endl;
  BOOST_CHECK(dse_test2_periodic.critical_edges == test2_periodic_expected);
  std::cout << "Looking for (Period:13) or (Thr:0.0769231)" << std::endl;
  BOOST_CHECK_CLOSE(1 / dse_test2_periodic.throughput, 13, 0.0001);
}

BOOST_AUTO_TEST_SUITE_END()
