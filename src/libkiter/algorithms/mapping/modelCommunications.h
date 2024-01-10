//
// Created by toky on 29/9/22.
//

#ifndef KITER_MODELCOMMUNICATIONS_H
#define KITER_MODELCOMMUNICATIONS_H
#include <vector>
#include <commons/basic_types.h>

namespace models {
    class Dataflow;
}
std::vector<std::vector<ARRAY_INDEX>> get_overlaps (models::Dataflow* const  dataflow) ;

#endif //KITER_MODELCOMMUNICATIONS_H
