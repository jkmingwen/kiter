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
            bool isDominated(const TokenConfiguration& config);
            bool contains(const TokenConfiguration& config);

            std::string toString() ;

        private:
            std::vector<TokenConfiguration> configurations;

        };


    } // algorithms
} // dse

#endif //KITER_TOKENCONFIGURATIONSET_H
