//
// Created by toky on 24/4/23.
//

#ifndef KITER_TOKEN_CONFIGURATION_H
#define KITER_TOKEN_CONFIGURATION_H

#include <models/Dataflow.h>
#include <commons/commons.h>
#include <vector>

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

            using PerformanceFunc = std::function<PerformanceResult(const TokenConfiguration&)>;

            TokenConfiguration(const models::Dataflow* dataflow, std::map<ARRAY_INDEX, TOKEN_UNIT> configuration)
                    : dataflow{dataflow}, configuration{configuration}, performance{0,{}} {
                refresh_cost(dataflow);
            }

            TokenConfiguration(const models::Dataflow* dataflow, std::vector<TOKEN_UNIT> config_vec)
                    : TokenConfiguration(dataflow, vectorToMap(dataflow, config_vec)) {
            }

            bool hasPerformance() const;
            const PerformanceUnit &getPerformance() const {
                VERBOSE_ASSERT(performance_computed, "Please run compute()");
                return performance;
            };

            CostUnit getCost() const {
                return cost;
            };

            void computePerformance(PerformanceFunc performance_func) {
                if (!performance_computed) {
                    PerformanceResult res = performance_func(*this);
                    performance = res;
                    performance_computed = true;
                } else {
                    FAILED("This should never happen");
                }
            }

            std::string to_csv_line() const;
            bool dominates(const TokenConfiguration& other) const;
            const std::map<ARRAY_INDEX, TOKEN_UNIT>&  getConfiguration() const;

            const models::Dataflow *getDataflow() const;

        private:


            const models::Dataflow* dataflow;
            std::map<ARRAY_INDEX, TOKEN_UNIT> configuration;

            // fitness data
            bool              performance_computed = false;
            PerformanceUnit   performance;
            CostUnit          cost;

            // Performance information
            // double cumulativeTime, executionTime;

        };

        std::ostream& operator<<(std::ostream& out, const algorithms::dse::TokenConfiguration& f) ;
        std::ostream& operator<<(std::ostream& out, const algorithms::dse::TokenConfiguration::PerformanceResult& pr);
    } // algorithms
} // dse


#endif //KITER_TOKEN_CONFIGURATION_H
