//
// Created by jachym on 18/6/23.
//

#ifndef KITER_THROUGHPUTCONSTRAINT_H
#define KITER_THROUGHPUTCONSTRAINT_H

#include "Constraint.h"

#include <utility>

namespace algorithms {
namespace dse {

class ThroughputConstraint : public Constraint {
public:
    using TokenConfigMap = std::map<ARRAY_INDEX, TOKEN_UNIT>;

    ThroughputConstraint() = default;

    explicit ThroughputConstraint(TokenConfigMap  map) : constraints_(std::move(map)){};

    ~ThroughputConstraint() = default;

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

} // namespace dse
} // namespace algorithms

#endif //KITER_THROUGHPUTCONSTRAINT_H
