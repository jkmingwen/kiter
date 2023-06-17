//
// Created by toky on 11/5/23.
//

#ifndef KITER_ADDINOUTLOOP_H
#define KITER_ADDINOUTLOOP_H

#include <string>
#include <vector>
#include <commons/commons.h>
#include <commons/KiterRegistry.h>


namespace models {
    class Dataflow;
    class Scheduling;
}

namespace algorithms {
    namespace transformation {
        void add_input_output_loop    (models::Dataflow* const dataflow,
                                       const std::vector<ARRAY_INDEX>& inputs,
                                       const std::vector<ARRAY_INDEX>&  output  );
        void add_input_output_loop    (models::Dataflow* const dataflow, parameters_list_t  parameters  );
    }
}

ADD_TRANSFORMATION(AddIOLoop,
        transformation_t({ "AddIOLoop" , "Take any output tasks and connect it to any input task. Weights only works for HSDFG.", algorithms::transformation::add_input_output_loop}));


#endif //KITER_ADDINOUTLOOP_H
