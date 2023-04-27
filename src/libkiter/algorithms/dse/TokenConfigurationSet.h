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

            const TokenConfiguration *getBestPerformancePoint() const {
                return this->best_point;
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

            const TokenConfiguration *best_point = nullptr;
            friend class ModularDSE;


             // Iterator for the FullSet
            class iterator {
            public:
                using iterator_category = std::forward_iterator_tag;
                using difference_type = std::ptrdiff_t;
                using value_type = TokenConfiguration;
                using pointer = TokenConfiguration*;
                using reference = TokenConfiguration&;

                iterator() = default;

                iterator(std::map<TokenConfiguration::CostUnit, std::set<TokenConfiguration, TokenConfigurationValuesComparator>>::const_iterator map_it, std::set<TokenConfiguration, TokenConfigurationValuesComparator>::const_iterator set_it)
                        : map_it(map_it), set_it(set_it) {}

                reference operator*() const {
                    return const_cast<reference>(*set_it);
                }

                pointer operator->() {
                    return const_cast<pointer>(std::addressof(*set_it));
                }

                iterator& operator++() {
                    ++set_it;
                    if (set_it == map_it->second.end()) {
                        ++map_it;
                        if (map_it != map_end_it) {
                            set_it = map_it->second.begin();
                        }
                    }
                    return *this;
                }

                iterator operator++(int) {
                    iterator temp = *this;
                    ++(*this);
                    return temp;
                }

                bool operator==(const iterator& other) const {
                    return map_it == other.map_it && (map_it == map_end_it || set_it == other.set_it);
                }

                bool operator!=(const iterator& other) const {
                    return !(*this == other);
                }

            private:
                std::map<TokenConfiguration::CostUnit, std::set<TokenConfiguration, TokenConfigurationValuesComparator>>::const_iterator map_it;
                std::map<TokenConfiguration::CostUnit, std::set<TokenConfiguration, TokenConfigurationValuesComparator>>::const_iterator map_end_it;
                std::set<TokenConfiguration, TokenConfigurationValuesComparator>::const_iterator set_it;
            };

            iterator begin() const {
                return iterator(configurations_by_cost.begin(), configurations_by_cost.empty() ? std::set<TokenConfiguration, TokenConfigurationValuesComparator>::const_iterator() : configurations_by_cost.begin()->second.begin());
            }

            iterator end() const {
                return iterator(configurations_by_cost.end(), std::set<TokenConfiguration, TokenConfigurationValuesComparator>::const_iterator());
            }
        };


    } // algorithms
} // dse

#endif //KITER_TOKENCONFIGURATIONSET_H
