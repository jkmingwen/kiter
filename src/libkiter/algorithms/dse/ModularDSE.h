//
// Created by toky on 24/4/23.
//

#ifndef KITER_MODULARDSE_H
#define KITER_MODULARDSE_H

#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>

#include "TokenConfiguration.h"
#include "TokenConfigurationSet.h"

namespace algorithms {
    namespace dse {

        class ModularDSE {
        public:
            // TODO: decide if they are declared here or in the TokenConfiguration
            using InitialConfigurationFunc = std::function<TokenConfiguration(const models::Dataflow*)>;
            using PerformanceFunc = TokenConfiguration::PerformanceFunc;
            using NextConfigurationFunc = std::function<std::vector<TokenConfiguration>(const TokenConfiguration&)>;
            using StopConditionFunc = std::function<bool(const TokenConfiguration&,const TokenConfigurationSet&)>;

            ModularDSE(const models::Dataflow* dataflow,
                       PerformanceFunc performance_func,
                       InitialConfigurationFunc initial_func,
                       NextConfigurationFunc next_func,
                       StopConditionFunc stop_func,
                       unsigned int num_threads = std::thread::hardware_concurrency())
                    : dataflow(dataflow),
                      performance_func(performance_func),
                      initial_func(initial_func),
                      next_func(next_func),
                      stop_func(stop_func),
                      num_threads(num_threads) {}

            void explore() ;
            void add_initial_job(const TokenConfiguration& tc) ;
            void stop() ;
            std::string print_space();
        private:
            const models::Dataflow* dataflow;
            PerformanceFunc performance_func;
            InitialConfigurationFunc initial_func;
            NextConfigurationFunc next_func;
            StopConditionFunc stop_func;
            unsigned int num_threads;

            std::mutex mtx;
            std::condition_variable cv;

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

            TokenConfigurationSet job_pool;
            // std::priority_queue<TokenConfiguration, std::vector<TokenConfiguration>, TokenConfigurationCostComparator> job_pool;

            bool stop_exploration = false;

            TokenConfigurationSet results;
        };


    } // algorithms


} // dse



#endif //KITER_MODULARDSE_H
