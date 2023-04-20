//
// Created by jachym on 19/4/23.
//

#ifndef KITER_MINIMIZINGPREDICTION_H
#define KITER_MINIMIZINGPREDICTION_H

#include <commons/KiterRegistry.h>
#include <vector>

namespace algorithms {
    int predict_num_tokens(const std::vector<int>& weights, parameters_list_t parameters);
}


#endif //KITER_MINIMIZINGPREDICTION_H
