//
// Created by toky on 3/7/23.
//

#ifndef KITER_NORMALIZE_H
#define KITER_NORMALIZE_H

#include <string>
#include <vector>
#include <commons/commons.h>
#include <commons/KiterRegistry.h>

namespace models {
    class Dataflow;
}

namespace algorithms {
    namespace transformation {
        void apply_normalization    (models::Dataflow* const dataflow, parameters_list_t  parameters  );
    }
}

ADD_TRANSFORMATION(RemoveReentrancy,
        transformation_t({ "Normalize" , "Transform the graph such that it is normalized.", algorithms::transformation::apply_normalization}));

#endif //KITER_NORMALIZE_H
