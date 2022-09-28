//
// Created by toky on 23/9/22.
//

#ifndef KITER_NAIVEDATAFLOW_H
#define KITER_NAIVEDATAFLOW_H

#include "AbstractDataflow.h"
namespace models {

    class NaiveDataflow : public AbstractDataflow {
    public:
        NaiveDataflow () : AbstractDataflow() {

        }
        NaiveDataflow (ARRAY_INDEX) : AbstractDataflow() {

        }

        ARRAY_INDEX getVerticesCount () {
          return 0;
        }

        ARRAY_INDEX getEdgesCount () {
            return 0;
        }

    };

} // end of namespace models

void test () {
    models::NaiveDataflow test;
}

#endif //KITER_NAIVEDATAFLOW_H
