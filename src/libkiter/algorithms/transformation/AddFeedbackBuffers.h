//
// Created by toky on 26/5/23.
//

#ifndef KITER_ADDFEEDBACKBUFFERS_H
#define KITER_ADDFEEDBACKBUFFERS_H


#include <commons/KiterRegistry.h>

namespace algorithms {
    namespace transformation {
        void generateInplaceFeedbackBuffers(models::Dataflow *g);
        void add_feedback_buffers(models::Dataflow *const dataflow, parameters_list_t params);
    }
}

ADD_TRANSFORMATION(AddFeedbackBuffers,
                   transformation_t({
                                            "AddFeedbackBuffers",
                                            "Add feedback buffer for any non-feedback buffers.",
                                            algorithms::transformation::add_feedback_buffers}));


#endif //KITER_ADDFEEDBACKBUFFERS_H
