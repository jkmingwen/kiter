//
// Created by toky on 24/4/23.
//

#include "TokenConfigurationSet.h"

namespace algorithms {
    namespace dse {
        void TokenConfigurationSet::add(const TokenConfiguration& new_config) {
            auto insert_res = this->configurations_by_cost[new_config.getCost()].insert(new_config);
            if (new_config.hasPerformance() and insert_res.second) {
                last_added = &(*insert_res.first);
                if ((best_point == nullptr) or (best_point->getPerformance() < new_config.getPerformance())) {
                    best_point = last_added;
                }
           }
        }

        void TokenConfigurationSet::remove(const TokenConfiguration& config) {
            size_t erased = this->configurations_by_cost[config.getCost()].erase(config);
            if (erased != 1) {
                VERBOSE_ERROR("failed to remove the TokenConfiguration " << config);
                VERBOSE_ERROR("Content of the Set was:");
                for (auto xconfig : *this) {
                    VERBOSE_ERROR("     - " << xconfig);
                }
            }
            VERBOSE_ASSERT_EQUALS(1, erased);

            // remove empty set
            if (this->configurations_by_cost[config.getCost()].empty()) {
                this->configurations_by_cost.erase(config.getCost());
            }
        }

        bool TokenConfigurationSet::isDominated(const TokenConfiguration& config) {

            for (auto cost_items : this->configurations_by_cost) {
                if (cost_items.first >= config.getCost()) {
                    for (const auto& existing_config : cost_items.second) {
                        if (existing_config.dominates(config)) {
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        bool TokenConfigurationSet::contains(const TokenConfiguration& config) {
            if (this->hasConfigWithCost(config.getCost())) {
                if (this->configurations_by_cost[config.getCost()].count(config) > 0) {
                    return true;
                }
            }
            return false;
        }

        // Print info of all storage distributions of a given distribution size in set
        std::string TokenConfigurationSet::toString() {

            std::string res = TokenConfiguration::csv_header() + "\n";

            for (auto cost_items : this->configurations_by_cost) {
                for (auto v  : cost_items.second) {
                    res += v.to_csv_line() + "\n";
                }
            }

            return res;
        }

        bool TokenConfigurationSet::hasConfigWithCost(TokenConfiguration::CostUnit cost) {
            return (this->configurations_by_cost.find(cost) != this->configurations_by_cost.end());
        }

        size_t TokenConfigurationSet::cleanByDominance(const TokenConfiguration& newConfig) {

            size_t removed = 0;
            TokenConfigurationSet to_be_removed;
            for (auto config : *this) {
                VERBOSE_DEBUG("Compare config " << config);
                if (newConfig.getCost() > config.getCost() &&
                    newConfig.getPerformance().throughput <= config.getPerformance().throughput) {
                    VERBOSE_DEBUG("Case 1 check");
                    to_be_removed.add(newConfig);
                    //this->remove(newConfig);
                    removed++;
                } else if (newConfig.getCost() >= config.getCost() &&
                           newConfig.getPerformance().throughput < config.getPerformance().throughput) {
                    VERBOSE_DEBUG("Case 2 check");
                    to_be_removed.add(newConfig);
                    //this->remove(newConfig);
                    removed++;
                } else if (newConfig.getCost() == config.getCost() &&
                           newConfig.getPerformance().throughput > config.getPerformance().throughput) {
                    VERBOSE_DEBUG("Case 3 check");
                    to_be_removed.add(config);
                    //this->remove(config);
                    removed++;
                }
            }

            // Remove them all at once.
            // TODO: this is slower and takes up space, but help to avoid fixing the iterator to support deletion.
            for (auto item: to_be_removed) {
                VERBOSE_DEBUG("Actually remove the item " << item);
                this->remove(item);
            }

            return removed;
        }


    } // algorithms
} // dse