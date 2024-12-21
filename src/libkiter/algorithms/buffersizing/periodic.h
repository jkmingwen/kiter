/*
 * throughput.h
 *
 *  Created on: 16 avr. 2013
 *      Author: toky
 */

#ifndef THROUGHPUT_H_
#define THROUGHPUT_H_

#include <commons/basic_types.h>
#include <commons/KiterRegistry.h>


namespace algorithms {

    models::BufferSizingResult periodic_memory_sizing_csdf   (models::Dataflow* const  dataflow, TIME_UNIT PERIOD, bool INTEGERSOLVING, bool ILPGENERATIONONLY) ;
    models::BufferSizingResult compute_csdf_1periodic_memory       (models::Dataflow* const  dataflow, parameters_list_t);

} // end of namespace algorithms


ADD_BUFFER_SIZING(PeriodicSizing,
		buffer_sizing_action_t({ "PeriodicSizing" , "Minimal Buffer size estimation by periodic scheduling method.", algorithms::compute_csdf_1periodic_memory}));

#endif /* THROUGHPUT_H_ */
