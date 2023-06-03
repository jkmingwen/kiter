//
// Created by jachym on 19/4/23.
//

#ifndef KITER_MINIMIZINGPREDICTION_H
#define KITER_MINIMIZINGPREDICTION_H

#include <commons/KiterRegistry.h>
#include <vector>

struct Entry {
    uint8_t dat[4];
    uint8_t res[4];
};

struct KeyHasher {
    std::size_t operator()(const std::vector<uint8_t>& key) const {
        std::size_t hash = 0;
        for (std::size_t i = 0; i < key.size(); ++i) {
            hash ^= std::hash<uint8_t>{}(key[i]) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

struct KeyEqual {
    bool operator()(const std::vector<uint8_t>& lhs, const std::vector<uint8_t>& rhs) const {
        return lhs == rhs;
    }
};

class PreloadDict {
public:
    explicit PreloadDict(const std::string& filename);

    std::vector<uint8_t> lookup(const std::vector<uint8_t>& key) const;
    std::unordered_map<std::vector<uint8_t>, std::vector<uint8_t>, KeyHasher, KeyEqual> map_;

private:

    static std::unordered_map<std::vector<uint8_t>, std::vector<uint8_t>, KeyHasher, KeyEqual> buildMap(const std::string& filename);
};
namespace algorithms {
    int get_preload(models::Dataflow*  dataflow, parameters_list_t parameters);
}


#endif //KITER_MINIMIZINGPREDICTION_H
