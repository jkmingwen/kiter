//
// Created by toky on 27/4/23.
//

#ifndef KITER_TDMAWRITER_H
#define KITER_TDMAWRITER_H

#include <string>

namespace models {
    class Dataflow;
}
namespace printers {
        void write_TDMA_file (models::Dataflow* const  dataflow, std::string output_file) ;
}

#endif //KITER_TDMAWRITER_H
