//
// Created by jachym on 18/6/23.
//

#include "ThroughputConstraint.h"

namespace algorithms {
    namespace dse {

        TokenConfiguration ThroughputConstraint::apply(const algorithms::dse::TokenConfiguration &config) {
            const TokenConfigMap cur_config = config.getConfiguration();
            TokenConfigMap new_config = constraints_;

            for (const auto& entry : cur_config) {
                int key = entry.first;
                if (new_config.find(key) == new_config.end()) {
                    auto value = entry.second;
                    new_config[key] = value;
                }
            }

            return TokenConfiguration(config.getDataflow(), new_config);
        }
    } // namespace dse
} // namespace algorithms
