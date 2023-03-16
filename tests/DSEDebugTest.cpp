/*
 *
 *  Created on: March 7, 2023
 *      Author: SamKouteili
 */

#define BOOST_TEST_MODULE DSEDebug
#include <set>
#include <map>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <commons/commons.h>
#include <models/Dataflow.h>
#include <models/EventGraph.h>
#include <algorithms/normalization.h>
#include <models/repetition_vector.h>
#include "helpers/test_classes.h"
#include <algorithms/dse/kperiodic.h>
#include <algorithms/dse/buffer_sizing.h>
#include <algorithms/dse/buffer_sizing.cpp>
#include <printers/SDF3Wrapper.h>

void printEdgeSet(StorageDistribution sd, models::Dataflow* g, std::string prefix=""){
    for (Edge e : sd.getEdges()){
        VERBOSE_INFO(prefix << " Edge: " << g->getEdgeName(e)
                            << " | Id: " << g->getEdgeId(e)
                            << " | ChnQty: " << sd.getChannelQuantity(e));

    }
}

BOOST_FIXTURE_TEST_SUITE( DSEDebug , WITH_VERBOSE)

BOOST_AUTO_TEST_CASE( test_setting_throughput_maximal )
{



    VERBOSE_INFO("generateSampleCycle");
    models::Dataflow* g = generateSampleCycle(); 

    // Get max thr of the graph
    VERBOSE_INFO("compute_Kperiodic_throughput_and_cycles");
    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g);
    


    VERBOSE_INFO("Start the DSE");
    StorageDistributionSet sds_new = algorithms::new_compute_Kperiodic_throughput_dse_sd(g);
    VERBOSE_INFO("Storage distributions are \n" << sds_new.printDistributions(g));


    VERBOSE_INFO("Start the DSE");
    parameters_list_t parameters;
    StorageDistributionSet sds = algorithms::compute_Kperiodic_throughput_dse_sd(g, parameters);
    VERBOSE_INFO("Storage distributions are \n" << sds.printDistributions(g));

    TIME_UNIT dse_thr = sds.getThrDist().second;
    VERBOSE_INFO("result.throughput: " << result.throughput 
                    << " | StoreDists.p_max: " << std::to_string(dse_thr));

    VERBOSE_INFO("test_setting_throughput_maximal #*&%" << std::endl << std::endl);  

}

BOOST_AUTO_TEST_SUITE_END()




