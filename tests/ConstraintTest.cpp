#define BOOST_TEST_MODULE ConstraintTest
#include "algorithms/dse/Constraint.h"
#include "algorithms/dse/TokenConfiguration.h"
#include "generators/NormalizedCycleGenerator.h"
#include "helpers/sample.h"
#include "helpers/test_classes.h"

#include <boost/test/unit_test_suite.hpp>
#include <map>
#include <variant>
#include <vector>

using namespace algorithms::dse;

bool areConfigsEqual(const std::map<ARRAY_INDEX, TOKEN_UNIT> &map1,
                     const std::map<ARRAY_INDEX, TOKEN_UNIT> &map2) {
  if (map1.size() != map2.size()) {
    return false;
  }

  for (const auto &pair : map1) {
    auto it = map2.find(pair.first);
    if (it == map2.end()) {
      return false;
    }

    if (pair.second != it->second) {
      return false;
    }
  }

  return true;
}

models::Dataflow *get_dummy_df() {
  size_t node_count = 4;
  std::vector<TOKEN_UNIT> node_weights{2, 2, 2, 2};
  std::vector<TIME_UNIT> node_durations{1, 1, 1, 1};
  std::vector<TOKEN_UNIT> edge_preloads{0, 0, 0, 0};
  models::Dataflow *df = generators::new_normalized_cycle(
      node_count, node_weights, node_durations, edge_preloads);

  {
    ForEachEdge(df, c) { df->setEdgeType(c, EDGE_TYPE::FEEDBACK_EDGE); }
  }

  return df;
}

models::Dataflow *get_small_df() {
  size_t node_count = 2;
  std::vector<TOKEN_UNIT> node_weights{2, 2};
  std::vector<TIME_UNIT> node_durations{1, 1};
  std::vector<TOKEN_UNIT> edge_preloads{0, 0};
  models::Dataflow *df = generators::new_normalized_cycle(
      node_count, node_weights, node_durations, edge_preloads);

  {
    ForEachEdge(df, c) { df->setEdgeType(c, EDGE_TYPE::FEEDBACK_EDGE); }
  }

  return df;
}

TokenConfiguration get_config(const models::Dataflow *dataflow) {
  std::map<ARRAY_INDEX, TOKEN_UNIT> configuration;
  {
    ForEachEdge(dataflow, e) {
      ARRAY_INDEX tid = dataflow->getEdgeId(e);
      if (dataflow->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
        configuration[tid] = 0;
      }
    }
  }
  return {dataflow, configuration};
}

BOOST_FIXTURE_TEST_SUITE(constraint_test, WITH_VERBOSE)

// Empty constraints have no effect on the configuration
BOOST_AUTO_TEST_CASE(empty_constraint_test) {
  std::map<ARRAY_INDEX, TOKEN_UNIT> conf_map;
  conf_map[1] = 0;
  conf_map[2] = 0;
  conf_map[3] = 0;
  conf_map[4] = 0;
  Constraint empty_constraint;
  auto config = get_config(get_dummy_df());

  // Apply constraint
  empty_constraint.apply(config, 0);

  // See if configs match
  BOOST_TEST(areConfigsEqual(config.getConfiguration(), conf_map));
}

BOOST_AUTO_TEST_CASE(single_edge_apply_test) {
  auto config = get_config(get_dummy_df());
  std::map<ARRAY_INDEX, TOKEN_UNIT> constraint_map;
  constraint_map[1] = 4;
  Constraint single_edge_constraint(constraint_map);

  BOOST_TEST(config.getConfiguration().at(1) == 0);

  VERBOSE_DEBUG(
      "Initial config: " << commons::toString(config.getConfiguration()));
  auto new_config = single_edge_constraint.apply(config, 0)[0];
  VERBOSE_DEBUG(
      "Modified config: " << commons::toString(new_config.getConfiguration()));

  BOOST_TEST(new_config.getConfiguration().at(1) == 4);
}

BOOST_AUTO_TEST_CASE(multi_edge_apply_test) {
  auto config = get_config(get_dummy_df());
  std::map<ARRAY_INDEX, TOKEN_UNIT> constraint_map;
  constraint_map[1] = 4;
  constraint_map[2] = 5;
  Constraint single_edge_constraint(constraint_map);

  BOOST_TEST(config.getConfiguration().at(1) == 0);
  BOOST_TEST(config.getConfiguration().at(2) == 0);

  VERBOSE_DEBUG(
      "Initial config: " << commons::toString(config.getConfiguration()));
  auto new_config = single_edge_constraint.apply(config, 0)[0];
  VERBOSE_DEBUG(
      "Modified config: " << commons::toString(new_config.getConfiguration()));

  BOOST_TEST(new_config.getConfiguration().at(1) == 4);
  BOOST_TEST(new_config.getConfiguration().at(2) == 5);
}

BOOST_AUTO_TEST_CASE(multiple_feedback_edges_ctor_test) {
  auto config = get_config(get_small_df());
  std::vector<ARRAY_INDEX> feedback_buffers{1,2};
  TIME_UNIT tgt_throughput = 0;
  TOKEN_UNIT min_tokens = 2;

  Constraint constraint(config, feedback_buffers, tgt_throughput, min_tokens);

  VERBOSE_DEBUG("Constraint: \n" << constraint.toString());

  std::vector<std::vector<ARRAY_INDEX>> options = {{2, 0}, {0, 2}, {1, 1}};
  auto constraint_map = constraint.getMap();

  bool passed = true;

  for(const auto& vec : options) {
    if (auto search = constraint_map.find({vec, 0}); search == constraint_map.end()) {
      VERBOSE_DEBUG("Not found: " << commons::toString(vec) << "\n");
      passed = false;
    }
  }

  BOOST_TEST(passed);
}

BOOST_AUTO_TEST_CASE(multiple_feedback_edges_ctor_preloaded_test) {
  auto df = get_small_df();
  df->setPreload(df->getEdgeById(1), 2);
  df->setPreload(df->getEdgeById(2), 3);
  auto config = get_config(df);

  std::vector<ARRAY_INDEX> feedback_buffers{1,2};
  TIME_UNIT tgt_throughput = 0;
  TOKEN_UNIT min_tokens = 2;

  Constraint constraint(config, feedback_buffers, tgt_throughput, min_tokens);

  VERBOSE_DEBUG("Constraint: \n" << constraint.toString());

  std::vector<std::vector<ARRAY_INDEX>> options = {{4, 3}, {2, 5}, {3, 4}};
  auto constraint_map = constraint.getMap();

  bool passed = true;
  for(const auto& vec : options) {
    if (auto search = constraint_map.find({vec, 0}); search == constraint_map.end()) {
      VERBOSE_DEBUG("Not found: " << commons::toString(vec) << "\n");
      passed = false;
    }
  }

  BOOST_TEST(passed);
}


BOOST_AUTO_TEST_SUITE_END()

