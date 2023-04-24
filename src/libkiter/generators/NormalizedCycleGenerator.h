//
// Created by toky on 24/4/23.
//

#ifndef KITER_NORMALIZEDCYCLEGENERATOR_H
#define KITER_NORMALIZEDCYCLEGENERATOR_H

#include <commons/commons.h>
#include <commons/KiterRegistry.h>


namespace models {
    class Dataflow;
}

namespace generators {
    models::Dataflow*  new_normalized_cycle(size_t node_count ,
                                            const std::vector<TOKEN_UNIT>& node_weights,
                                            const std::vector<TIME_UNIT>& node_duration,
                                            TOKEN_UNIT preload);

    models::Dataflow*  generate_normalized_cycle(parameters_list_t   param_list);

} // generators


ADD_GENERATOR(GenerateNormalizedCycle,
        generator_t(
        { "GenerateNormalizedCycle" , "generate a normalized SDF cycle.", generators::generate_normalized_cycle})
);


#endif //KITER_NORMALIZEDCYCLEGENERATOR_H
