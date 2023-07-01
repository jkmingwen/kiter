//
// Created by jachym on 24/6/23.
//

#include "Constraint.h"
#include "TokenConfiguration.h"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <vector>

namespace algorithms::dse {

void get_combinations(const std::vector<std::pair<int, int>> &ranges,
                      std::vector<ARRAY_INDEX> &indices, size_t level,
                      std::vector<std::vector<ARRAY_INDEX>> &results) {
  if (level == indices.size()) {
    results.push_back(indices);
  } else {
    for (int i = ranges[level].first; i <= ranges[level].second; i++) {
      indices[level] = i;
      get_combinations(ranges, indices, level + 1, results);
    }
  }
}

std::vector<std::vector<ARRAY_INDEX>>
Constraint::gen_next_preloads(const TokenConfiguration &config,
                              std::vector<ARRAY_INDEX> &feedback_buffers,
                              TOKEN_UNIT min_tokens) {
  // If we have one feedback buffer, we return immediately
  if (feedback_buffers.size() == 1) {
    return {{min_tokens}};
  }

  size_t num_feedback_buffers = feedback_buffers.size();
  const std::map<ARRAY_INDEX, TOKEN_UNIT> &cur_config =
      config.getConfiguration();
  std::vector<ARRAY_INDEX> original_values(num_feedback_buffers, 0);
  std::vector<ARRAY_INDEX> indices(num_feedback_buffers,
                                   0); // Initialize indices vector with 0s
  std::vector<std::pair<int, int>> ranges(num_feedback_buffers);
  std::vector<std::vector<ARRAY_INDEX>> preload_combinations;

  for (size_t i = 0; i < feedback_buffers.size(); ++i) {
    auto it = cur_config.find(feedback_buffers[i]);
    // The feedback buffer should always have a value in the config
    assert(it != cur_config.end());

    TOKEN_UNIT cur_tokens = it->second;

    original_values[i] = cur_tokens;
    ranges[i] = {cur_tokens, min_tokens};
  }

  get_combinations(ranges, indices, 0, preload_combinations);

  return preload_combinations;
}

Constraint::Constraint(const TokenConfiguration &config,
                       std::vector<ARRAY_INDEX> feedback_buffers,
                       TIME_UNIT target_throughput, TOKEN_UNIT min_tokens) {

  auto preload_combinations =
      Constraint::gen_next_preloads(config, feedback_buffers, min_tokens);

  for (auto preload_combination : preload_combinations) {
    ConstraintKey key = {preload_combination, target_throughput};
    constraints_[key] = min_tokens;
  }
}

// FIXME: include throughput in the conditions
std::vector<TokenConfiguration>
Constraint::apply(const algorithms::dse::TokenConfiguration &config,
                  [[maybe_unused]] TIME_UNIT throughput) {
  const std::map<ARRAY_INDEX, TOKEN_UNIT> &cur_config =
      config.getConfiguration();
  auto new_config = cur_config;

  for (const auto &entry : constraints_) {
    ARRAY_INDEX key = entry.first.first[0];
    ConstraintValue value = entry.second;

    auto it = new_config.find(key);
    if (it != new_config.end()) {
      if (value > it->second) {
        it->second = value;
      }
    } else {
      new_config[key] = value;
    }
  }

  return {{config.getDataflow(), new_config}};
}

// TODO: add assertion that we never see the same numbers twice
void Constraint::update(const Constraint &other) {
  for (const auto &entry : other.constraints_) {
    ConstraintKey key = entry.first;
    ConstraintValue value = entry.second;

    auto it = constraints_.find(key);
    if (it != constraints_.end()) {
      if (value > it->second) {
        it->second = value;
      }
    } else {
      constraints_[key] = value;
    }
  }
}

} // namespace algorithms::dse
