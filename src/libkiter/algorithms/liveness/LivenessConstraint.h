//
// Created by jachym on 17/6/23.
//

#ifndef KITER_LIVENESSCONSTRAINT_H
#define KITER_LIVENESSCONSTRAINT_H

#include <algorithms/dse/Constraint.h>

#include <utility>

namespace algorithms::dse {

class LivenessConstraint : public Constraint {
public:
    using TokenConfigMap = std::map<ARRAY_INDEX, TOKEN_UNIT>;

    LivenessConstraint()= default;

    explicit LivenessConstraint(TokenConfigMap  map) : constraints_(std::move(map)){};

    ~LivenessConstraint() = default;

    TokenConfiguration apply(const TokenConfiguration& config) override;
    void merge(const Constraint& other) override;

private:
    TokenConfigMap constraints_;
};

} // namespace algorithms::dse

#endif //KITER_LIVENESSCONSTRAINT_H
