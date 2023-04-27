//
// Created by toky on 27/4/23.
//

#ifndef KITER_SO4NOCWRITER_H
#define KITER_SO4NOCWRITER_H

#include <tuple>
#include <map>
#include <string>
#include <set>
#include "commons/basic_types.h"

namespace models {
    class Dataflow;
}

namespace printers {
    std::tuple<std::map<ARRAY_INDEX, long>, long> generateConditions(const models::Dataflow* const dataflow, const std::string& filename, std::set<ARRAY_INDEX> );
}
#endif //KITER_SO4NOCWRITER_H
