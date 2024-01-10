//
// Created by jachym on 24/6/23.
//

#include "Constraint.h"
#include "TokenConfiguration.h"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

namespace algorithms::dse {

void get_combinations(const std::vector<std::pair<int, int>> &ranges,
                      std::vector<ARRAY_INDEX> &indices, size_t level,
                      std::vector<std::vector<ARRAY_INDEX>> &results,
                      TOKEN_UNIT min_tokens) {
  if (level == indices.size()) {
    auto result = std::reduce(indices.begin(), indices.end());
    if (result == min_tokens) {
      results.push_back(indices);
    }
  } else {
    for (int i = ranges[level].first; i <= ranges[level].second; i++) {
      indices[level] = i;
      get_combinations(ranges, indices, level + 1, results, min_tokens);
    }
  }
}

std::vector<std::vector<ARRAY_INDEX>>
Constraint::gen_next_preloads(const std::map<ARRAY_INDEX, TOKEN_UNIT> &config,
                              std::vector<ARRAY_INDEX> &feedback_buffers,
                              TOKEN_UNIT min_tokens) {
  // If we have one feedback buffer, we return immediately
  if (feedback_buffers.size() == 1) {
    return {{min_tokens}};
  }

  size_t num_feedback_buffers = feedback_buffers.size();
  std::vector<ARRAY_INDEX> indices(num_feedback_buffers, 0);
  std::vector<std::pair<int, int>> ranges(num_feedback_buffers);
  std::vector<std::vector<ARRAY_INDEX>> preload_combinations;
  TOKEN_UNIT cur_total = 0;

  for (size_t i = 0; i < feedback_buffers.size(); ++i) {
    auto it = config.find(feedback_buffers[i]);
    // The feedback buffer should always have a value in the config
    assert(it != config.end());

    TOKEN_UNIT cur_tokens = it->second;

    ranges[i] = {cur_tokens, min_tokens};
    cur_total += cur_tokens;
  }

  get_combinations(ranges, indices, 0, preload_combinations, min_tokens);

  return preload_combinations;
}

Constraint::Constraint(std::map<ARRAY_INDEX, TOKEN_UNIT> &config_map) {
  for (const auto &it : config_map) {
    ConstraintKey key = {{it.first}, 0};
    ConstraintValue val = it.second;
    constraints_[key] = val;
  }
}

std::vector<TokenConfiguration>
Constraint::apply(const TokenConfiguration &config) {
  if(isEmpty()) return {config};

  const std::map<ARRAY_INDEX, TOKEN_UNIT> &cur_config =
      config.getConfiguration();

  TIME_UNIT cur_throughput = (config.hasPerformance()) ? config.getPerformance().throughput : 0;
  std::vector<TokenConfiguration> new_configs;

  for (const auto &entry : constraints_) {
    ConstraintKey key = entry.first;
    ConstraintValue value = entry.second;

    // Check the throughput
    if (key.second > cur_throughput)
      continue;

    auto new_preloads =
        gen_next_preloads(config.getConfiguration(), key.first, value);

    for (const std::vector<TOKEN_UNIT> &preload : new_preloads) {
      auto new_config = cur_config;
      int i = 0;

      for (ARRAY_INDEX id : key.first) {
        new_config[id] = preload[i];
        ++i;
      }

      new_configs.push_back({config.getDataflow(), new_config});
    }
  }

  return new_configs;
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
