//
// Created by jachym on 17/6/23.
//

#ifndef KITER_LIVENESSCONSTRAINT_H
#define KITER_LIVENESSCONSTRAINT_H

#include <algorithms/dse/Constraint.h>

namespace algorithms {
namespace dse {

class LivenessConstraint : public Constraint {
public:
    using TokenConfigMap = std::map<ARRAY_INDEX, TOKEN_UNIT>;

    LivenessConstraint()= default;

    explicit LivenessConstraint(const TokenConfigMap& map) : constraints_(map){};

    ~LivenessConstraint() {};

    TokenConfiguration apply(const TokenConfiguration& config) override;

private:
    TokenConfigMap constraints_;
};

} // namespace dse
} // namespace algorithms

#endif //KITER_LIVENESSCONSTRAINT_H
