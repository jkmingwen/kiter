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
    void update(const Constraint& other) override;
    void print() override { printMap(constraints_); }

private:
    TokenConfigMap constraints_;
    void printMap(const std::map<long, long>& myMap) {
        for (const auto& pair : myMap) {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
    }
};

} // namespace algorithms::dse

#endif //KITER_LIVENESSCONSTRAINT_H
