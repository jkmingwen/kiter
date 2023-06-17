//
// Created by jachym on 19/4/23.
//

#include "minimizing_prediction.h"
#include <commons/verbose.h>
#include <algorithms/normalization.h>
#include <models/Dataflow.h>

PreloadDict::PreloadDict(const std::string &filename) {
    map_ = buildMap(filename);
}

std::vector<uint8_t> PreloadDict::lookup(const std::vector<uint8_t> &key) const {
    std::vector<uint8_t> result = {0};
    auto it = map_.find(key);

    if (it != map_.end()) {
        return it->second;
    }

    return result;
}

std::unordered_map<std::vector<uint8_t>, std::vector<uint8_t>, KeyHasher, KeyEqual>
PreloadDict::buildMap(const std::string &filename) {
    std::unordered_map<std::vector<uint8_t>, std::vector<uint8_t>, KeyHasher, KeyEqual> map;


    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Failed to open the binary file: " << filename << std::endl;
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    int numEntries = fileSize / 8;

    auto *entries = new Entry[numEntries];

    file.read(reinterpret_cast<char *>(entries), fileSize);
    file.close();

    for (int i = 0; i < numEntries; ++i) {
        std::vector<uint8_t> key(entries[i].dat, entries[i].dat + 4);
        std::vector<uint8_t> val(entries[i].res, entries[i].res + 4);
        map[key] = val;
    }
    delete[] entries;
    return map;
}

std::vector<int> ml::get_preload(models::Dataflow *dataflow, parameters_list_t parameters) {
    // Load the model
    if (!parameters.count("DICT_PATH")) {
        VERBOSE_ERROR("get_preload: Model path not found!");
        std::vector<int> res{-1};
        return res;
    }
    VERBOSE_ASSERT(algorithms::normalize(dataflow), "get_preload: Normalization failed");

    std::vector<uint8_t> weights;

    dataflow->getVertices();
    ForEachVertex(dataflow, v) {
        weights.push_back(static_cast<uint8_t>(dataflow->getZi(v)));
    }

    //TODO: this should just reference a singleton
    PreloadDict preload_dict(parameters["DICT_PATH"]);
    std::vector<uint8_t> dict_result = preload_dict.lookup(weights);

    std::vector<int> res(dict_result.begin(), dict_result.end());
    return res;
}
