//
// Created by jachym on 17/6/23.
//

#ifndef KITER_CONSTRAINT_H
#define KITER_CONSTRAINT_H

#include "TokenConfiguration.h"

namespace algorithms {
namespace dse {

class Constraint {
public:
    virtual TokenConfiguration apply(const TokenConfiguration& config) = 0;
};

} // namespace dse
} // namespace algorithms

#endif //KITER_CONSTRAINT_H
