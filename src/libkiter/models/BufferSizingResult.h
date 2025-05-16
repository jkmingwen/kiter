//
// Created by toky on 21/12/24.
//

#ifndef BUFFERSIZINGRESULT_H
#define BUFFERSIZINGRESULT_H

#include "commons/basic_types.h"

/**
 * This part is about buffer sizing
 */

namespace models {

    class BufferSizingResult {

        bool       _valid;
        TOKEN_UNIT _total_size;
        std::map<ARRAY_INDEX, TOKEN_UNIT> _idv_size;
    public:
        BufferSizingResult(TOKEN_UNIT t) : _valid (true), _total_size(t) {}
        BufferSizingResult() : _valid (false), _total_size(0) {}
        bool is_valid() const {return _valid;}
        TOKEN_UNIT total_size() const {return _total_size;}
        void set_validity(bool validity){_valid = validity;}
        void set_total_size(TOKEN_UNIT token){_total_size = token;}
        void add_edge_size(ARRAY_INDEX edge, TOKEN_UNIT size) {_idv_size.insert({edge, size});}
        TOKEN_UNIT get_edge_size (ARRAY_INDEX edge) {return _idv_size[edge];}
    };

}

typedef std::function< models::BufferSizingResult(models::Dataflow* const, TIME_UNIT) > BufferSizingFun;

#endif //BUFFERSIZINGRESULT_H
