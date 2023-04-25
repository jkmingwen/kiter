//
// Created by toky on 24/4/23.
//

#ifndef KITER_TOKENCONFIGURATIONSET_H
#define KITER_TOKENCONFIGURATIONSET_H

#include "TokenConfiguration.h"

namespace algorithms {
    namespace dse {

        class TokenConfigurationSet {
        public:
            void add(const TokenConfiguration& config);
            void remove(const TokenConfiguration& config);
            bool isDominated(const TokenConfiguration& config);
            bool contains(const TokenConfiguration& config);

            std::string toString() ;

            bool hasConfigWithCost(TokenConfiguration::CostUnit i);

            bool empty() {
                return this->configurations_by_cost.empty();
            }
            const TokenConfiguration& top () const {
                return *(this->configurations_by_cost.begin()->second.begin());
            }

            void pop () {
                // remove the first config
                this->configurations_by_cost.begin()->second.erase(this->configurations_by_cost.begin()->second.begin());

                // remove empty set
                if (this->configurations_by_cost.begin()->second.empty()) {
                    this->configurations_by_cost.erase(this->configurations_by_cost.begin());
                }
            }

            void push(const TokenConfiguration& config) {
                this->add(config);
            }
            // TODO: make this a cached value.
            size_t size() {
                size_t sum = 0;
                for (auto cost_items : this->configurations_by_cost) {
                    sum += cost_items.second.size();
                }
                return sum;
            }
        private:

            struct TokenConfigurationCostComparator {
                bool operator()(const TokenConfiguration& lhs, const TokenConfiguration& rhs) const {
                    return lhs.getCost() > rhs.getCost();
                }
            };

            struct TokenConfigurationValuesComparator {
                bool operator()(const TokenConfiguration& lhs, const TokenConfiguration& rhs) const {
                    auto lhs_config = lhs.getConfiguration();
                    auto rhs_config = rhs.getConfiguration();

                    for (auto lhs_it = lhs_config.begin(), rhs_it = rhs_config.begin(); lhs_it != lhs_config.end() && rhs_it != rhs_config.end(); ++lhs_it, ++rhs_it) {
                        if (lhs_it->first != rhs_it->first) {
                            return lhs_it->first < rhs_it->first;
                        }
                        if (lhs_it->second != rhs_it->second) {
                            return lhs_it->second < rhs_it->second;
                        }
                    }

                    return false;
                }
            };

            std::map<TokenConfiguration::CostUnit, std::set<TokenConfiguration, TokenConfigurationValuesComparator> > configurations_by_cost;

        };


    } // algorithms
} // dse

#endif //KITER_TOKENCONFIGURATIONSET_H
