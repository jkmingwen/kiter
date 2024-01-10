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
#include "Constraint.h"

namespace algorithms {
    namespace dse {


        struct ExplorationParameters {
            std::string import_filename = "";
            size_t limit = 0;
            size_t timeout_sec = 0;
            size_t thread_count = 1;
            bool bottom_up = false;
            bool realtime_output = false;
            bool return_pareto_only = false;
        };

        class ModularDSE {
        public:
            // TODO: decide if they are declared here or in the TokenConfiguration
            struct NextFuncRes {
                std::vector<TokenConfiguration> configs;
                Constraint constraints;
            };

            using PerformanceFunc = TokenConfiguration::PerformanceFunc;
            using NextConfigurationsFunc = std::function<NextFuncRes(const TokenConfiguration&)>;
            using StopConditionFunc = std::function<bool(const TokenConfiguration&,const TokenConfigurationSet&)>;

            ModularDSE(const models::Dataflow* dataflow,
                       PerformanceFunc performance_func,
                       NextConfigurationsFunc next_func,
                       StopConditionFunc stop_func,
                       bool use_constraints = false)
                    : dataflow(dataflow),
                      performance_func(performance_func),
                      next_func(next_func),
                      stop_func(stop_func),
                      use_constraints(use_constraints) {
                VERBOSE_ASSERT(dataflow->has_repetition_vector(), "OK it is a constant dataflow, but at least give me the repetition vector.");
            }

            ModularDSE& operator<<(const TokenConfiguration& config) {
                if (config.hasPerformance()) {
                    this->results.add(config);
                } else {
                    this->job_pool.add(config);
                }
                return *this;
            }

            void import_results(std::istream& input);
            void import_results(const std::string& filename);
            void import_results(TokenConfigurationSet& database);
            void add_initial_job(const TokenConfiguration& tc) ;
            void add_initial_jobs(const std::vector<TokenConfiguration>& tc) ;
            void stop() ;
            const TokenConfigurationSet & getResults () const {return results;} ;
            size_t results_size () const {return results.size();};
            size_t job_pool_size () const {return job_pool.size();};
            std::string print_space(bool no_timing = false);

            void explore(const ExplorationParameters& exploration_parameters) ;

        private:
            bool should_stop(size_t explored, size_t limit);
            void explore_thread(std::atomic<size_t>& explored,
                                const std::chrono::steady_clock::time_point beginTime, const ExplorationParameters& parameters) ;
        private:
            const models::Dataflow* dataflow;
            PerformanceFunc performance_func;
            NextConfigurationsFunc next_func;
            StopConditionFunc stop_func;

            bool use_constraints;
            Constraint constraints = Constraint();

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
            TokenConfigurationSet in_progress;
            TokenConfigurationSet results;
            bool stop_exploration = false;
        };


    } // algorithms


} // dse



#endif //KITER_MODULARDSE_H
