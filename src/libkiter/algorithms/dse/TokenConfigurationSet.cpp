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
            VERBOSE_ASSERT_EQUALS(1, erased);

            // remove empty set
            if (this->configurations_by_cost.begin()->second.empty()) {
                this->configurations_by_cost.erase(this->configurations_by_cost.begin());
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


        bool TokenConfigurationSet::isNonMinimal(const TokenConfiguration& newConfig)  {

            for (auto config : *this) {
                if ((newConfig.getCost() > config.getCost() &&
                     (newConfig.getPerformance().throughput <= config.getPerformance().throughput )))
                    return true;
                if (newConfig.getCost() >= config.getCost() &&
                        newConfig.getPerformance().throughput < config.getPerformance().throughput)
                    return true;
            }


        }

        size_t TokenConfigurationSet::removeDominatedBy(const TokenConfiguration& newConfig) {

            size_t removed = 0;

            for (auto config : *this) {
                    if (newConfig.getCost() == config.getCost() && // remove existing equal storage dist (size) with lower thr
                                                                   newConfig.getPerformance().throughput  > config.getPerformance().throughput ) {

                        this->remove(config);
                        removed ++;
                    }
            }
            return removed;
        }


    } // algorithms
} // dse