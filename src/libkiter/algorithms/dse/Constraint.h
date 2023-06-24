//
// Created by jachym on 17/6/23.
//

#ifndef KITER_CONSTRAINT_H
#define KITER_CONSTRAINT_H

#include "TokenConfiguration.h"

namespace algorithms::dse {

class Constraint {
public:
    using TokenConfigMap = std::map<ARRAY_INDEX, TOKEN_UNIT>;

    Constraint()= default;

    explicit Constraint(TokenConfigMap  map) : constraints_(std::move(map)){};

    ~Constraint() = default;

    TokenConfiguration apply(const TokenConfiguration& config);
    void update(const Constraint& other);
    void print() { printMap(constraints_); }

private:
    TokenConfigMap constraints_;
    void printMap(const std::map<long, long>& myMap) {
        for (const auto& pair : myMap) {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
    }
};

} // namespace algorithms::dse

#endif //KITER_CONSTRAINT_H
