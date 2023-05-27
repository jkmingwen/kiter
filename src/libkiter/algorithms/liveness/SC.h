//
// Created by toky on 26/5/23.
//

#ifndef KITER_SC_H
#define KITER_SC_H

#include <commons/KiterRegistry.h>

namespace models {
    class Dataflow;
}

namespace algorithms {
    namespace liveness {
        bool CSDF_SC1(const models::Dataflow *const dataflow) ;
        bool CSDF_SC2(const models::Dataflow *const dataflow) ;

        void LivenessSC(models::Dataflow * dataflow,  parameters_list_t params);
    }
}

ADD_TRANSFORMATION(LivenessSC,
        transformation_t({ "LivenessSC" , "Use the SC1 and SC2 liveness condition to check liveness over a dataflow (Benazouz, DAC13).", algorithms::liveness::LivenessSC}));



#endif //KITER_SC_H
