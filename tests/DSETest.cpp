/*
 *
 *  Created on: Nov 6, 2019
 *      Author: toky
 */

#define BOOST_TEST_MODULE DSETest
#include <set>
#include <map>
#include <vector>
#include <string>
#include <numeric>
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


BOOST_FIXTURE_TEST_SUITE( DSETest , WITH_VERBOSE)

std::string vec2string(std::vector<TIME_UNIT> v){
    return std::accumulate(v.begin(), v.end(), std::string{},
        [](const std::string& a, int b) {
            return a + (a.empty() ? "" : ", ") + std::to_string(b);
        });
}

std::string vec2string(std::vector<TOKEN_UNIT> v){
    return std::accumulate(v.begin(), v.end(), std::string{},
        [](const std::string& a, int b) {
            return a + (a.empty() ? "" : ", ") + std::to_string(b);
        });
}

std::pair<models::Dataflow*, std::map<ARRAY_INDEX, ARRAY_INDEX>> get_CC_graph_pair(models::Dataflow* g, parameters_list_t parameters){
    
    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g, parameters);

    models::Dataflow* cc_g = new models::Dataflow(); /***@@@*****/

    std::map<ARRAY_INDEX, ARRAY_INDEX> visited_vertices; // old index -> new index

    VERBOSE_INFO("Critical edges found:");
    for (std::set<Edge>::iterator it = (result.critical_edges).begin();
         it != (result.critical_edges).end(); it++){
                
        ARRAY_INDEX srcID = g->getVertexId(g->getEdgeSource(*it));
        ARRAY_INDEX trgID = g->getVertexId(g->getEdgeTarget(*it));

        // need to check if vertex visited because cycle will add same ID twice
        bool src_visited = false, trg_visited = false;
        for (std::pair<ARRAY_INDEX, ARRAY_INDEX> v : visited_vertices){
            if (srcID == v.first) src_visited = true;
            if (trgID == v.first) trg_visited = true;
        }
        
        Vertex src;
        Vertex trg; 
        if (!src_visited){
            src = cc_g->addVertex(g->getVertexName(g->getEdgeSource(*it))); // Adds ID as name
            visited_vertices[srcID] = cc_g->getVertexId(src);
        } else {
            src = cc_g->getVertexById(visited_vertices[srcID]);
        }
        if (!trg_visited){
            trg = cc_g->addVertex(g->getVertexName(g->getEdgeTarget(*it)));
            visited_vertices[trgID] = cc_g->getVertexId(trg);
        } else {
            trg = cc_g->getVertexById(visited_vertices[trgID]);
        }
        Edge e = cc_g->addEdge(src, trg);
        cc_g->setPreload(e, g->getPreload(*it)); 
        cc_g->setEdgeInPhases(e, g->getEdgeInVector(*it));
        cc_g->setEdgeOutPhases(e, g->getEdgeOutVector(*it));

        cc_g->setVertexDuration(src, g->getVertexPhaseDuration(g->getEdgeSource(*it)));
        cc_g->setVertexDuration(trg, g->getVertexPhaseDuration(g->getEdgeTarget(*it)));
        // Edge e = cc_g->addEdge(src, trg, g->getEdgeId(*it), g->getEdgeName(*it));
        
        VERBOSE_INFO("EdgeID: " << cc_g->getEdgeId(e) << " | Channel Qnt: " << cc_g->getPreload(e));        
        VERBOSE_INFO("\tVertex(src)ID: " << cc_g->getVertexId(src) << " -> Vertex(trg)ID: " << cc_g->getVertexId(trg)); 
        VERBOSE_INFO("\tVertex(src)Dur: " << vec2string(cc_g->getVertexPhaseDuration(src)));    
        VERBOSE_INFO("\tVertex(trg)Dur: " << vec2string(cc_g->getVertexPhaseDuration(trg))); 
        VERBOSE_INFO("\tEdge(In)Phases: " << vec2string(cc_g->getEdgeInVector(e)));    
        VERBOSE_INFO("\tEdge(Out)Phases: " << vec2string(cc_g->getEdgeOutVector(e)));  

    }

    std::pair<models::Dataflow*, std::map<ARRAY_INDEX, ARRAY_INDEX>> res;
    res.first = cc_g;
    res.second = visited_vertices;

    return res;

}

BOOST_AUTO_TEST_CASE( test_critical_cycle_loading_dataflow )
{

    parameters_list_t parameters;


    models::Dataflow* g = generateSampleCycle(); // maxThr = 0.0434783

	std::cout << printers::generateSDF3XML(g) << std::endl;

    auto cc_g = get_CC_graph_pair(g, parameters);
	std::cout << printers::generateSDF3XML(cc_g.first) << std::endl;


    // VERBOSE_INFO("Critical cycle graph generated. Now computing CC throughput")
    
    // kperiodic_result_t cc_res = algorithms::compute_Kperiodic_throughput_and_cycles(cc_g.first, parameters);

    // VERBOSE_ASSERT_EQUALS(cc_g.second.size(), cc_res.critical_edges.size())

    // // for (std::pair<const ARRAY_INDEX, ARRAY_INDEX> id_pair : cc_g.second){
    // //     Edge e = cc_g.first->getEdgeByIdid_pair.first;
    // //     if (cc_res.critical_edges.find(id) == cc_res.critical_edges.end()){

    // //     }
    // // }

    // // CRITICAL CYCLE THR & GRAPH THR NOT EQUAL!
    // kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g, parameters);
    // VERBOSE_INFO("result.throughput = " << result.throughput << " | cc_res.throughput = " << cc_res.throughput);
    // VERBOSE_ASSERT_EQUALS(result.throughput , cc_res.throughput);

    // VERBOSE_INFO("test_critical_cycle_loading_dataflow #*&%" << std::endl << std::endl);

}

// BOOST_AUTO_TEST_CASE ( test_update_graph_with_new_cc )
// {
//     parameters_list_t parameters;

//     models::Dataflow* g = generateSampleCycle(); // maxThr = 0.0434783

//     // not sure how to check this? most was copy pasted anyways
//     StorageDistributionSet sds = algorithms::compute_Kperiodic_throughput_dse_sd(g, parameters);
//     kperiodic_result_t cc_res = algorithms::compute_Kperiodic_throughput_and_cycles(g, parameters);


//     StorageDistribution checkDist(checklist.getNextDistribution());

//     for (std::pair<TOKEN_UNIT, std::vector<StorageDistribution>> cc_pair : sds.getSet()){
//       /* Maybe check here if token size is same as previous one? better yet check if each
//       individual channel has actually grown */
//       for (StorageDistribution cc_sd : cc_pair.second){
//         StorageDistribution newDist(checkDist);
//         // update cc edge quantities and add to distribution
//         for (Edge it : cc_sd.getEdges()) {
//             if (dataflow_prime->getEdgeId(it) > dataflow->getEdgesCount()) {
//           VERBOSE_DSE("\tFound storage dependency in channel "
//                       << dataflow_prime->getEdgeName(it) << std::endl);
//           // make new modelled storage distribution according to storage dependencies
//           newDist.setChannelQuantity(it, cc_sd.getChannelQuantity(it));
//           VERBOSE_DSE("\t\tIncreasing channel size of "
//                       << dataflow_prime->getEdgeName(it) << " to "
//                       << newDist.getChannelQuantity(it) << std::endl);
//           VERBOSE_DSE("\tUpdating checklist with new storage distribution..."
//                       << std::endl);
//         }

//     VERBOSE_INFO("test_update_graph_with_new_cc #*&%" << std::endl << std::endl);

// }

// BOOST_AUTO_TEST_CASE( test_setting_target_throughput )
// {

//     parameters_list_t parameters;
//     parameters["THR"] = "0.00000001";

//     // how to get graph from XML?
//     models::Dataflow* g = generateSampleCycle(); /***@@@*****/

//     StorageDistributionSet sds = algorithms::compute_Kperiodic_throughput_dse_sd(g, parameters);

//     kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g, parameters);
    
//     // WHY CAN"T I CALL getThrDist????
//     // auto dse_thr = sds.getThrDist().second;
//     // VERBOSE_INFO("result.throughput: " << result.throughput << " | StoreDists.p_max: " << std::to_string(dse_thr));
//     // VERBOSE_ASSERT_GreaterThan(result.throughput , dse_thr);

//     VERBOSE_INFO("test_setting_target_throughput #*&%" << std::endl << std::endl);


// }

// BOOST_AUTO_TEST_CASE( test_max_throughput_stor_dist_set )
// {

//     parameters_list_t parameters;

//     // how to get graph from XML?
//     models::Dataflow* g = generateSampleCycle(); /***@@@*****/

//     StorageDistributionSet sds = algorithms::compute_Kperiodic_throughput_dse_sd(g, parameters);
    
//     parameters_list_t new_params(parameters);
//     new_params["MAX_SET"] = "t";
//     StorageDistributionSet sds_max = algorithms::compute_Kperiodic_throughput_dse_sd(g, new_params);
   
//     StorageDistribution sd = sds.getNextDistribution();
//     StorageDistribution sd_max = sds_max.getNextDistribution();

//     VERBOSE_INFO(" sd_max.getThroughput(): " << sd_max.getThroughput() << " | sd.getThroughput: " << sd.getThroughput()); 
//     VERBOSE_ASSERT_GreaterThan(sd_max.getThroughput(), sd.getThroughput());    

// }


// BOOST_AUTO_TEST_CASE( test_next_after_throughput_increases )
// {

//     parameters_list_t parameters;

//     models::Dataflow* g = generateSampleCycle(); /***@@@*****/

//     StorageDistributionSet sds = algorithms::compute_Kperiodic_throughput_dse_sd(g, parameters);

//     StorageDistribution sd = sds.getNextDistribution();


//     parameters_list_t new_params(parameters);
//     // STEP 1: Check next after; write unit test with jaime's function
//     // STEP 2: Check if max_set is returning the maximal set
//     new_params["THR"] = std::to_string(std::nextafter(sd.getThroughput(), sd.getThroughput()+1));
//     new_params["MAX_SET"] = "t";
//     StorageDistributionSet sds_max = algorithms::compute_Kperiodic_throughput_dse_sd(g, new_params);

//     StorageDistribution sd_max = sds_max.getNextDistribution();

//     VERBOSE_INFO("sd_max.getThroughput(): " << sd_max.getThroughput() << " | sd.getThroughput(): " << sd.getThroughput()); 
//     VERBOSE_ASSERT_GreaterThan(sd_max.getThroughput(), sd.getThroughput());



// }

// BOOST_AUTO_TEST_CASE( test_next_after_throughput_equal_at_max )
// {

//     parameters_list_t parameters;

//     // how to get graph from XML?
//     models::Dataflow* g = generateSampleCycle(); /***@@@*****/

//     kperiodic_result_t res = algorithms::compute_Kperiodic_throughput_and_cycles(g, parameters);


//     parameters_list_t new_params(parameters);
//     // STEP 1: Check next after; write unit test with jaime's function
//     // STEP 2: Check if max_set is returning the maximal set
//     new_params["THR"] = std::to_string(std::nextafter(res.throughput, res.throughput+1));
//     new_params["MAX_SET"] = "t";
//     StorageDistributionSet sds_max = algorithms::compute_Kperiodic_throughput_dse_sd(g, new_params);

//     StorageDistribution sd_max = sds_max.getNextDistribution();

//     VERBOSE_INFO("sd_max.getThroughput(): " << sd_max.getThroughput() << " | res.throughput: " << res.throughput); 
//     VERBOSE_ASSERT_EQUALS(sd_max.getThroughput(), res.throughput);

// }

BOOST_AUTO_TEST_SUITE_END()




