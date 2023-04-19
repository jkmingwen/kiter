//
// Created by jachym on 19/4/23.
//

#include "minimizing_prediction.h"
#include <cppflow/raw_ops.h>
#include <cppflow/cppflow.h>
#include <vector>

#ifndef MODEL_PATH
#define MODEL_PATH "tf_model"
#endif

int algorithms::predict_num_tokens(const std::vector<int>& weights) {
    // Load the model
    cppflow::model model(MODEL_PATH);

    // Get model operations
    // for( auto op : model.get_operations()) {
    //   std::cout << op << "\n";
    // }
    //

    std::vector<float> tensor_data(weights.begin(), weights.end());
    auto vec = cppflow::tensor(tensor_data, {1,2});

    // Run
    auto output = model({{"serving_default_dense_input:0",vec}}, {"StatefulPartitionedCall:0"});

    // Show the predicted class
    int res = static_cast<int>(output[0].get_data<float>()[0]);

    return res;
}
