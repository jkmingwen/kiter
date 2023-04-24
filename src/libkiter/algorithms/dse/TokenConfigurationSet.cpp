//
// Created by toky on 24/4/23.
//

#include "TokenConfigurationSet.h"

namespace algorithms {
    namespace dse {
        void TokenConfigurationSet::add(const TokenConfiguration& config) {
            if (!isDominated(config)) {
                configurations.push_back(config);
            }
        }


        bool TokenConfigurationSet::isDominated(const TokenConfiguration& config) {
            for (const auto& existing_config : configurations) {
                if (existing_config.dominates(config)) {
                    return true;
                }
            }
            return false;
        }
        bool TokenConfigurationSet::contains(const TokenConfiguration& config) {
            for (const auto& existing_config : configurations) {
                if (existing_config.getConfiguration() == config.getConfiguration()) {
                    return true;
                }
            }
            return false;
        }

        // Print info of all storage distributions of a given distribution size in set
        std::string TokenConfigurationSet::toString() {
            std::string res = "";
            for (auto v  :configurations) {
                res += v.to_csv_line() + "\n";
            }
            return res;
        }

    } // algorithms
} // dse