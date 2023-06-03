//
// Created by jachym on 19/4/23.
//

#include "minimizing_prediction.h"
#include <commons/verbose.h>
#include <algorithms/normalization.h>
#include <models/Dataflow.h>

PreloadDict::PreloadDict(const std::string& filename) {
    map_ = buildMap(filename);
}

std::vector<uint8_t> PreloadDict::lookup(const std::vector<uint8_t>& key) const {
    std::vector<uint8_t> result = {0,0,0,0};
    auto it = map_.find(key);

    if (it != map_.end()) {
        return it->second;
    }

    return result;
}

std::unordered_map<std::vector<uint8_t>, std::vector<uint8_t>, KeyHasher, KeyEqual>
PreloadDict::buildMap(const std::string& filename) {
    std::unordered_map<std::vector<uint8_t>, std::vector<uint8_t>, KeyHasher, KeyEqual> map;


    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Failed to open the binary file: " << filename << std::endl;
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    int numEntries = fileSize / 8;

    auto* entries = new Entry[numEntries];

    file.read(reinterpret_cast<char*>(entries), fileSize);
    file.close();

    for(int i = 0; i < numEntries; ++i) {
        std::vector<uint8_t> key(entries[i].dat, entries[i].dat + 4);
        std::vector<uint8_t> val(entries[i].res, entries[i].res + 4);
        map[key] = val;
    }
    delete[] entries;
    return map;
}

// Machine learning version of the algorithm
//int algorithms::get_preload(const std::vector<int>& weights, parameters_list_t parameters) {
//    // Load the model
//    if(!parameters.count("MODEL_DIR")) {
//        VERBOSE_ERROR("get_preload: Model path not found!")
//        return -1;
//    }
//
//    long input_size = static_cast<long>(weights.size());
//    std::string model_dir = parameters["MODEL_DIR"];
//    cppflow::model model(model_dir);
//    std::vector<float> tensor_data(weights.begin(), weights.end());
//    auto vec = cppflow::tensor(tensor_data, {1,input_size});
//
//    // Run
//    auto output = model({{"serving_default_dense_input:0",vec}}, {"StatefulPartitionedCall:0"});
//
//    // Show the predicted class
//    int res = static_cast<int>(output[0].get_data<float>()[0]);
//
//    return res;
//}
