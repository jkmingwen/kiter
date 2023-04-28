//
// Created by toky on 24/4/23.
//

#ifndef KITER_TOKEN_CONFIGURATION_H
#define KITER_TOKEN_CONFIGURATION_H

#include <models/Dataflow.h>
#include <commons/commons.h>
#include <vector>
#include <chrono>

namespace algorithms {
    namespace dse {



        class TokenConfiguration {

        public:

            using EdgeIdentifier = ARRAY_INDEX;


            struct PerformanceResult {
                PerformanceResult(TIME_UNIT d, const std::set<EdgeIdentifier>& ce)
                        : throughput(d), critical_edges(ce) {
                }

                TIME_UNIT throughput;
                std::set<EdgeIdentifier> critical_edges;

                bool operator<(const PerformanceResult& other) const {
                    return throughput < other.throughput;
                }
            };


            using PerformanceUnit = PerformanceResult;
            using CostUnit = TOKEN_UNIT;



        private :


                void refresh_cost(const models::Dataflow* dataflow);
                static std::map<EdgeIdentifier, TOKEN_UNIT> vectorToMap(const models::Dataflow* dataflow, const std::vector<TOKEN_UNIT>& config_vec);



        public:

            using PerformanceFunc = std::function<PerformanceResult(models::Dataflow*, const TokenConfiguration&)>;

            TokenConfiguration(const models::Dataflow *dataflow,
                               const std::map<ARRAY_INDEX, TOKEN_UNIT> &configuration,
                               const PerformanceResult &result,
                               TIME_UNIT execTime,
                               TIME_UNIT cumulTime);

            TokenConfiguration(const models::Dataflow* dataflow, const std::map<ARRAY_INDEX, TOKEN_UNIT> &configuration)
                    : dataflow{dataflow}, configuration{configuration}, performance{0,{}} {
                refresh_cost(dataflow);
            }

            TokenConfiguration(const models::Dataflow* dataflow, const std::vector<TOKEN_UNIT> &config_vec)
                    : TokenConfiguration(dataflow, vectorToMap(dataflow, config_vec)) {
            }

        public:


            bool hasPerformance()  const {
                return this->performance_computed;
            }

            const models::Dataflow *getDataflow() const {
                return dataflow;
            }

            const PerformanceUnit &getPerformance() const {
                VERBOSE_ASSERT(performance_computed, "Please run compute()");
                return performance;
            };

            CostUnit getCost() const {
                return cost;
            };

            TIME_UNIT getCumulExecutionTimeMs() const {
                return cumulativeTime;
            }


            const std::map<ARRAY_INDEX, TOKEN_UNIT>&  getConfiguration() const {
                return this->configuration;
            }


            void computePerformance(PerformanceFunc performance_func, const std::chrono::steady_clock::time_point beginTime = std::chrono::steady_clock::now(), models::Dataflow* sandbox = nullptr) {
                if (!performance_computed) {

                    // Create the sandbox if it doesn't exist. A sandbox is a dataflow that the perf. func. can modify.
                    if (!sandbox) sandbox = new models::Dataflow(*this->dataflow);

                    // Run the performance function and store its duration.
                    auto startTime = std::chrono::steady_clock::now();
                    PerformanceResult res = performance_func(sandbox, *this);
                    auto endTime = std::chrono::steady_clock::now();

                    // Update the TokenConfiguration with its performance results.
                    std::chrono::duration<double, std::milli> execTime = endTime - startTime; // duration in ms
                    std::chrono::duration<double, std::milli> cumulTime = endTime - beginTime;
                    executionTime = execTime.count();
                    cumulativeTime = cumulTime.count();
                    performance = res;
                    performance_computed = true;
                } else {
                    FAILED("This should never happen");
                }
            }
            static const std::string csv_header();
            static size_t csv_header_size();
            static TokenConfiguration from_csv_line(const models::Dataflow* dataflow, const std::string &line);
            std::string to_csv_line(bool no_timing = false) const;

            bool dominates(const TokenConfiguration& other) const;


        private:


            const models::Dataflow* dataflow;
            std::map<ARRAY_INDEX, TOKEN_UNIT> configuration;

            // fitness data
            bool              performance_computed = false;
            PerformanceUnit   performance;
            CostUnit          cost;

            // Performance information
            TIME_UNIT cumulativeTime, executionTime;
        };

        std::ostream& operator<<(std::ostream& out, const algorithms::dse::TokenConfiguration& f) ;
        std::ostream& operator<<(std::ostream& out, const algorithms::dse::TokenConfiguration::PerformanceResult& pr);
    } // algorithms
} // dse


#endif //KITER_TOKEN_CONFIGURATION_H
