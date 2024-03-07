/*
 * randomMapping.cpp
 *
 *  Created on: Aug 21, 2019
 *      Author: toky
 */

#include <models/Dataflow.h>
#include <algorithms/mappings.h>

void algorithms::mapping::randomMapping (models::Dataflow* const  dataflow, parameters_list_t params) {

	VERBOSE_INFO("randomMapping");

    VERBOSE_ASSERT(dataflow->getNoC().size() > 0, "The dataflow must have a NoC associated to it to be mapped.");
    dataflow->reset_computation();
	for (auto t : dataflow->vertices()) {
		if (params.find(dataflow->getVertexName(t)) != params.end()) {
			dataflow->setMapping(t, commons::fromString<node_id_t> (params[dataflow->getVertexName(t)]));
		} else {
            auto noc_size = dataflow->getNoC().size() ;
            auto rand_val = std::rand() ;
			node_id_t random_id = noc_size + rand_val % noc_size;
			dataflow->setMapping(t, random_id);
		}
	}

}

