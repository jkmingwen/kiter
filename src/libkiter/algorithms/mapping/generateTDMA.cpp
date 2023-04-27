/*
 * generateTDMA.cpp
 *
 *  Created on: Jul 2021
 *      Author: katwinkl3
 */

#include <commons/KiterRegistry.h>
#include <printers/TDMAWriter.h>
#include <algorithms/mappings.h>
#include <utility>

void algorithms::mapping::generateTDMA (models::Dataflow* const  dataflow, parameters_list_t params_list) {
    std::string output_file = commons::get_parameter(std::move(params_list), "output", "");
    printers::write_TDMA_file (dataflow, output_file);
}

