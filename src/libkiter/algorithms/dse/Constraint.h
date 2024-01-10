//
// Created by jachym on 17/6/23.
//

#ifndef KITER_CONSTRAINT_H
#define KITER_CONSTRAINT_H

#include "TokenConfiguration.h"
#include <ostream>
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

  Constraint(std::map<ARRAY_INDEX, TOKEN_UNIT> &config_map);

  ~Constraint() = default;

  std::vector<TokenConfiguration> apply(const TokenConfiguration &config);

  void update(const Constraint &other);

  std::string toString() {
    std::string res = "preloads,target_throughput,min_tokens\n";
    for (const auto &it : constraints_) {
      res += commons::toString(it.first.first);
      res += ",";
      res += commons::toString(it.first.second);
      res += ",";
      res += commons::toString(it.second);
      res += "\n";
    }
    return res;
  }

  TokenConfigMap getMap() { return constraints_; }
  bool isEmpty() { return constraints_.empty();}

private:
  TokenConfigMap constraints_;

  // FIXME: this function is incredibly inefficient
  std::vector<std::vector<ARRAY_INDEX>>
  gen_next_preloads(const std::map<ARRAY_INDEX, TOKEN_UNIT> &config,
                    std::vector<ARRAY_INDEX> &feedback_buffers,
                    TOKEN_UNIT min_tokens);
};

} // namespace algorithms::dse

#endif // KITER_CONSTRAINT_H
