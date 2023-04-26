//
// Created by toky on 26/4/23.
//

#ifndef KITER_DSE_UTILS_H
#define KITER_DSE_UTILS_H

#include <commons/KiterRegistry.h>

namespace algorithms {

    void generateInplaceFeedbackBuffers(models::Dataflow* g);
    void add_feedback_buffers(models::Dataflow *const dataflow, parameters_list_t params);

}


models::Dataflow* genGraphWFeedbackEdges(models::Dataflow* g);

ADD_TRANSFORMATION(AddFeedbackBuffers,
                   transformation_t({
                                            "AddFeedbackBuffers",
                                            "Add feedback buffer for any non-feedback buffers.",
                                            algorithms::add_feedback_buffers}));


#endif //KITER_DSE_UTILS_H
