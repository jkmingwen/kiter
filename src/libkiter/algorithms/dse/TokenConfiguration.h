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

            using PerformanceUnit = TIME_UNIT;
            using CostUnit = TOKEN_UNIT;


        public:

            using PerformanceFunc = std::function<PerformanceUnit(const TokenConfiguration&)>;

            TokenConfiguration(const models::Dataflow* dataflow, std::vector<TOKEN_UNIT> configuration);
            TokenConfiguration(const models::Dataflow* dataflow, std::map<ARRAY_INDEX, TOKEN_UNIT> configuration);

            bool hasPerformance() const;
            PerformanceUnit getThroughput() const {
                VERBOSE_ASSERT(computed, "Please run compute()");
                return thr;
            };

            CostUnit getCost() const {
                return cost;
            };

            void computeThroughput(PerformanceFunc performance_func) {
                if (!computed) {
                    thr = performance_func(*this);
                    computed = true;
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
            bool              computed = false;
            PerformanceUnit   thr  = 0.0;
            CostUnit          cost = 0;

            // Performance information
            // double cumulativeTime, executionTime;

        };

        std::ostream& operator<<(std::ostream& out, const algorithms::dse::TokenConfiguration& f) ;
    } // algorithms
} // dse


#endif //KITER_TOKEN_CONFIGURATION_H
