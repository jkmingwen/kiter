//
// Created by toky on 28/4/23.
//

#ifndef KITER_SUBGRAPH_H
#define KITER_SUBGRAPH_H

#include <commons/basic_types.h>
#include <set>

namespace models {
    class Dataflow;
}
namespace algorithms {
    models::Dataflow *build_subgraph(const models::Dataflow *g, const std::set<ARRAY_INDEX> &critical_edges);
}
#endif //KITER_SUBGRAPH_H
