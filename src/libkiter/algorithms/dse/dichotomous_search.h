/*
 * dichotomous_search.h
 *
 *  Created on: Apr 9, 2020
 *      Author: toky
 */

#ifndef SRC_LIBKITER_ALGORITHMS_DSE_DICHOTOMOUS_SEARCH_H_
#define SRC_LIBKITER_ALGORITHMS_DSE_DICHOTOMOUS_SEARCH_H_

#include <commons/KiterRegistry.h>
#include <commons/basic_types.h>
#include <functional>
#include<models/BufferSizingResult.h>

namespace models {
	class Dataflow;
}

namespace algorithms {
	StorageDistributionSet compute_csdf_dse_periodic        (models::Dataflow* const  dataflow, parameters_list_t params) ;
	StorageDistributionSet compute_csdf_dse_speriodic       (models::Dataflow* const  dataflow, parameters_list_t params) ;
	StorageDistributionSet compute_csdf_dse_from_function   (models::Dataflow* const  dataflow, const BufferSizingFun& sizing_fun, std::ostream& dseLog) ;
}

ADD_THROUGHPUT_BUFFERING_DSE(PeriodicDSE, "Bodin2013 Periodic DSE", algorithms::compute_csdf_dse_periodic);
ADD_THROUGHPUT_BUFFERING_DSE(SPeriodicDSE,"Experimental Strictly Periodic DSE", algorithms::compute_csdf_dse_speriodic);
#endif /* SRC_LIBKITER_ALGORITHMS_DSE_DICHOTOMOUS_SEARCH_H_ */
