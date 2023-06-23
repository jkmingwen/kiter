//
// Created by jachym on 17/6/23.
//

#include "LivenessConstraint.h"

namespace algorithms::dse {

    //TODO: copy the old config -- check with constraints, increase if higher
    TokenConfiguration LivenessConstraint::apply(const algorithms::dse::TokenConfiguration &config) {
        const TokenConfigMap& cur_config = config.getConfiguration();
        TokenConfigMap new_config = cur_config;

        for (const auto& entry : constraints_) {
            long key = entry.first;
            long value = entry.second;

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

    void LivenessConstraint::update(const Constraint& other) {
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
            throw std::runtime_error("LivenessConstraint: update got unexpected argument\n");
        }
    }
} // namespace algorithms::dse
