/*
 * deGroote.h
 *
 *  Created on: 5 juil. 2013
 *      Author: toky
 */

#ifndef DEGROOTE_H_
#define DEGROOTE_H_

#include <commons/KiterRegistry.h>


namespace models {
	class Dataflow;
	class EventGraph;
}

namespace algorithms {


models::EventGraph*     generate_LCG   		                  (models::Dataflow* const  dataflow);
void                    clean_LCG   		                  (models::EventGraph*      lcg);
    TIME_UNIT				compute_deGroote_throughput   		  (models::Dataflow* const  dataflow, parameters_list_t);
    TIME_UNIT				compute_deGrooteClean_throughput	  (models::Dataflow* const  dataflow, parameters_list_t);

}

ADD_THROUGHPUT(deGrooteThroughput,"Throughput analysis from deGroote2012 paper except event graph reduction.", algorithms::compute_deGroote_throughput);

ADD_THROUGHPUT(deGrooteCleanThroughput,"Throughput analysis from deGroote2012 paper.", algorithms::compute_deGrooteClean_throughput);

#endif /* DEGROOTE_H_ */
