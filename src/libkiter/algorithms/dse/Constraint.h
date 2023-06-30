//
// Created by jachym on 17/6/23.
//

#ifndef KITER_CONSTRAINT_H
#define KITER_CONSTRAINT_H

#include "TokenConfiguration.h"
#include <utility>
#include <vector>

namespace algorithms::dse {

class Constraint {
public:
  using ConstraintKey = std::pair<std::vector<ARRAY_INDEX>, TIME_UNIT>;
  using ConstraintValue = TOKEN_UNIT;
  using TokenConfigMap = std::map<ConstraintKey, ConstraintValue>;

  Constraint() = default;

  explicit Constraint(TokenConfigMap map) : constraints_(std::move(map)){};

  ~Constraint() = default;

  TokenConfiguration apply(const TokenConfiguration &config);
  void update(const Constraint &other);
  // FIXME: printing of the constraints
  // std::string toString() { return commons::toString(constraints_); }

private:
  TokenConfigMap constraints_;
};

} // namespace algorithms::dse

#endif // KITER_CONSTRAINT_H
