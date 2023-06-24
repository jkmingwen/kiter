//
// Created by jachym on 24/6/23.
//

#include "Constraint.h"

namespace algorithms::dse {

TokenConfiguration Constraint::apply(const algorithms::dse::TokenConfiguration &config) {
    const TokenConfigMap& cur_config = config.getConfiguration();
    TokenConfigMap new_config = cur_config;

    for (const auto& entry : constraints_) {
        ARRAY_INDEX key = entry.first;
        TOKEN_UNIT value = entry.second;

        auto it = new_config.find(key);
        if (it != new_config.end()) {
            if (value > it->second) {
                it->second = value;
            }
        } else {
            new_config[key] = value;
        }
    }

   return {config.getDataflow(), new_config};
}

void Constraint::update(const Constraint& other) {
    for (const auto& entry : other.constraints_) {
        ARRAY_INDEX key = entry.first;
        TOKEN_UNIT value = entry.second;

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
