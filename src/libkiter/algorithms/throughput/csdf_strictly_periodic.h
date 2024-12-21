/*
 * csdf_strictly_periodic.h
 *
 *  Created on: Feb 22, 2021
 *      Author: toky
 */


#include <commons/KiterRegistry.h>
#include <models/Dataflow.h>
#include <map>

namespace models {
	class EventGraph;
}

namespace algorithms {
	models::EventGraph* generate_csdf_strictly_periodic_event_graph(const models::Dataflow * const dataflow, bool max_duration = false);
	TIME_UNIT compute_SPeriodic_throughput    (models::Dataflow*  dataflow, parameters_list_t );
	TIME_UNIT compute_ASPeriodic_throughput    (models::Dataflow*  dataflow, parameters_list_t );

}


ADD_THROUGHPUT(SPeriodicThroughput,"Strictly Periodic schedule (Exact)", algorithms::compute_SPeriodic_throughput);
ADD_THROUGHPUT(ASPeriodicThroughput,"Strictly Periodic schedule (Approx)", algorithms::compute_ASPeriodic_throughput);
