//
// Created by jachym on 17/6/23.
//

#ifndef KITER_CONSTRAINT_H
#define KITER_CONSTRAINT_H

#include "TokenConfiguration.h"

namespace algorithms::dse {

class Constraint {
public:
    virtual TokenConfiguration apply(const TokenConfiguration& config) = 0;
    virtual void update(const Constraint& other) = 0;
    virtual void print() = 0;
};

} // namespace algorithms::dse

#endif //KITER_CONSTRAINT_H
