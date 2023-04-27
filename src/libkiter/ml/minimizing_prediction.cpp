//
// Created by jachym on 19/4/23.
//

#include "minimizing_prediction.h"
#include <commons/verbose.h>
#include <vector>

#ifdef CPPFLOW_FOUND
#include <cppflow/raw_ops.h>
#include <cppflow/cppflow.h>


int ml::predict_num_tokens(const std::vector<int>& weights, parameters_list_t parameters) {
    // Load the model
    if(!parameters.count("MODEL_DIR")) {
        VERBOSE_ERROR("predict_num_tokens: Model path not found!");
        return -1;
    }

    long input_size = static_cast<long>(weights.size());
    std::string model_dir = parameters["MODEL_DIR"];
    cppflow::model model(model_dir);
    std::vector<float> tensor_data(weights.begin(), weights.end());
    auto vec = cppflow::tensor(tensor_data, {1,input_size});

    // Run
    auto output = model({{"serving_default_dense_input:0",vec}}, {"StatefulPartitionedCall:0"});

    // Show the predicted class
    int res = static_cast<int>(output[0].get_data<float>()[0]);

    return res;
}
#else
int ml::predict_num_tokens(const std::vector<int>& weights, parameters_list_t parameters) {
    VERBOSE_ERROR("CPPFLOW must be installed at compiled-time for this to work.");
}
#endif
