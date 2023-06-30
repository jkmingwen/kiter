//
// Created by jachym on 24/6/23.
//

#include "Constraint.h"
#include "TokenConfiguration.h"
#include <vector>

namespace algorithms::dse {

  std::vector<TokenConfiguration> Constraint::apply(const algorithms::dse::TokenConfiguration &config, TIME_UNIT throughput) {
    const std::map<ARRAY_INDEX, TOKEN_UNIT>& cur_config = config.getConfiguration();
    auto new_config = cur_config;

    for (const auto& entry : constraints_) {
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

void Constraint::update(const Constraint& other) {
    for (const auto& entry : other.constraints_) {
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
