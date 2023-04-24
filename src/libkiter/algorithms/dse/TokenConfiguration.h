//
// Created by toky on 24/4/23.
//

#ifndef KITER_TOKENCONFIGURATION_H
#define KITER_TOKENCONFIGURATION_H

#include <models/Dataflow.h>
#include <commons/commons.h>
#include <vector>

namespace algorithms {
    namespace dse {

        class TokenConfiguration {
        public:

            using PerformanceFunc = std::function<TIME_UNIT(const TokenConfiguration&)>;

            TokenConfiguration(const models::Dataflow* dataflow, std::map<ARRAY_INDEX, TOKEN_UNIT> configuration);
            TIME_UNIT getThroughput() const {
                VERBOSE_ASSERT(computed, "Please run compute()");
                return thr;
            };
            TOKEN_UNIT getCost() const {
                return cost;
            };

            void computeThroughput(PerformanceFunc performance_func) {
                if (!computed) {
                    thr = performance_func(*this);
                    computed = true;
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
            bool      computed = false;
            TIME_UNIT   thr  = 0.0;
            TOKEN_UNIT  cost = 0;

            // Performance information
            // double cumulativeTime, executionTime;

        };

        std::ostream& operator<<(std::ostream& out, const algorithms::dse::TokenConfiguration& f) ;
    } // algorithms
} // dse


#endif //KITER_TOKENCONFIGURATION_H
