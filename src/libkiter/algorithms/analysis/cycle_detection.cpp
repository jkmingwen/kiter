/*
 * cycle_detection.cpp
 *
 *  Created on: 29 Dec 2021
 *      Author: toky
 */

#include "cycle_detection.h"
#include <commons/verbose.h>
#include <map>
#include <vector>
#include <algorithms/scc.h>

bool algorithms::has_selfloops            (models::Dataflow* const  dataflow) {
	for (auto t : dataflow->vertices()) {
		if (dataflow->getReentrancyFactor(t) > 0) {
			return true;
		}
	}
	return false;
}
bool algorithms::has_cycles            (models::Dataflow* const  dataflow) {
	std::map<int, std::vector<ARRAY_INDEX>> res = algorithms::computeSCCKosaraju(dataflow);
	for (auto item : res) {
		VERBOSE_INFO("Component " << item.first << " has " << item.second.size() << " members");
	}
	for (auto item : res) {
		if (item.second.size() > 1) return true;
	}
	return false;
}
void algorithms::cycle_detection       (models::Dataflow* const  dataflow, parameters_list_t){
	VERBOSE_INFO("has_cycles result: " << algorithms::has_cycles(dataflow));
	std::cout << "has_cycles result: " << algorithms::has_cycles(dataflow) << std::endl;
}
