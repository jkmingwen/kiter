//
// Created by jachym on 17/6/23.
//

#include "LivenessConstraint.h"

namespace algorithms::dse {

    TokenConfiguration LivenessConstraint::apply(const algorithms::dse::TokenConfiguration &config) {
        const TokenConfigMap& cur_config = config.getConfiguration();
        TokenConfigMap new_config = constraints_;

        for (const auto& entry : cur_config) {
            ARRAY_INDEX key = entry.first;
            if (new_config.find(key) == new_config.end()) {
                auto value = entry.second;
                new_config[key] = value;
            }
        }

        return {config.getDataflow(), new_config};
    }

    void LivenessConstraint::merge(const Constraint& other) {
        try {
            const auto& other_typed = dynamic_cast<const LivenessConstraint&>(other);
            for (const auto& entry : other_typed.constraints_) {
                long key = entry.first;
                long value = entry.second;

                auto it = constraints_.find(key);
                if (it != constraints_.end()) {
                    if (value > it->second) {
                        it->second = value;
                    }
                } else {
                    constraints_[key] = value;
                }
            }
        } catch (const std::bad_cast&) {
            throw std::runtime_error("LivenessConstraint: merge got unexpected argument\n");
        }
    }
} // namespace algorithms::dse
