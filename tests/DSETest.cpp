/*
 *
 *  Created on: March 7, 2023
 *      Author: SamKouteili
 */

#define BOOST_TEST_MODULE DSETest
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


BOOST_FIXTURE_TEST_SUITE( DSETest , WITH_VERBOSE)

template <class T>
std::string vec2string(std::vector<T> v){
    return std::accumulate(v.begin(), v.end(), std::string{},
        [](const std::string& a, int b) {
            return a + (a.empty() ? "" : ", ") + std::to_string(b);
        });
}

void printEdgeSet(StorageDistribution sd, models::Dataflow* g, std::string prefix=""){
    for (Edge e : sd.getEdges()){
        VERBOSE_INFO(prefix << " Edge: " << g->getEdgeName(e)
                            << " | Id: " << g->getEdgeId(e)
                            << " | ChnQty: " << sd.getChannelQuantity(e));

    }
}

models::Dataflow* genGraphWFeedbackEdges(models::Dataflow* g){
    /* Generate dataflow_prime; for testing purposes */

    models::Dataflow* dataflow_prime = new models::Dataflow(*g);
    dataflow_prime->reset_computation();
    // add feedback channels in new graph to model bounded channel quantities
    {ForEachEdge(g, c) {
        auto new_edge = dataflow_prime->addEdge(dataflow_prime->getEdgeTarget(c),
                                                dataflow_prime->getEdgeSource(c));
        dataflow_prime->setEdgeInPhases(new_edge,
                                        dataflow_prime->getEdgeOutVector(c));
        dataflow_prime->setEdgeOutPhases(new_edge,
                                        dataflow_prime->getEdgeInVector(c));
        dataflow_prime->setPreload(new_edge, g->getPreload(c));
        dataflow_prime->setEdgeName(new_edge,
                                    dataflow_prime->getEdgeName(c) + "_prime");
    }}

    return dataflow_prime;

}



BOOST_AUTO_TEST_CASE( test_critical_cycle_mapping_to_graph )
{

    parameters_list_t parameters;
    models::Dataflow* g = generateSampleCycle(); // maxThr = 0.0434783

    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g);

    models::Dataflow* cc_g = getCCGraph(g, result);

    VERBOSE_INFO("GENERATING XML OF G");
	std::cout << printers::generateSDF3XML(g) << std::endl;

    VERBOSE_INFO("GENERATING XML OF CC");
	std::cout << printers::generateSDF3XML(cc_g) << std::endl;

    // Iterating through cc to see if data aligns
    int edgeNum = 0;
    {ForEachEdge(cc_g, e_cc){
        
        Vertex src_cc = cc_g->getEdgeSource(e_cc);
        Vertex trg_cc = cc_g->getEdgeTarget(e_cc);

        Edge e_g = g->getEdgeByName(cc_g->getEdgeName(e_cc));
        Vertex src_g = g->getEdgeSource(e_g);
        Vertex trg_g = g->getEdgeSource(e_g);
        VERBOSE_INFO("(CC Edge) " << cc_g->getEdgeName(e_cc) << ": " 
                        << cc_g->getVertexName(src_cc) << "->" << cc_g->getVertexName(trg_cc));
        VERBOSE_INFO("(G Edge)  " << g->getEdgeName(e_g) << ": " 
                        << g->getVertexName(src_g) << "->" << g->getVertexName(trg_g));
        // Checking if graphs are the same
        VERBOSE_ASSERT_EQUALS(cc_g->getPreload(e_cc), g->getPreload(e_g));
        VERBOSE_ASSERT_EQUALS(vec2string(cc_g->getEdgeInVector(e_cc)),
                                 vec2string(g->getEdgeInVector(e_g)));
        VERBOSE_ASSERT_EQUALS(vec2string(cc_g->getEdgeOutVector(e_cc)),
                                 vec2string(g->getEdgeOutVector(e_g)));

        // VERBOSE_ASSERT_EQUALS(cc_g->getVertexName(src_cc), g->getVertexName(src_g));
        // VERBOSE_ASSERT_EQUALS(cc_g->getVertexName(trg_cc), g->getVertexName(trg_g));
        VERBOSE_ASSERT_EQUALS(vec2string(cc_g->getVertexPhaseDuration(src_cc)),
                                    vec2string(g->getVertexPhaseDuration(src_g)));
        VERBOSE_ASSERT_EQUALS(vec2string(cc_g->getVertexPhaseDuration(src_cc)),
                                    vec2string(g->getVertexPhaseDuration(src_g)));
        edgeNum++;
    }}

    VERBOSE_ASSERT_EQUALS(edgeNum, static_cast<int>(result.critical_edges.size()));

    VERBOSE_INFO("test_critical_cycle_mapping_to_graph #*&%" << std::endl << std::endl);

}



BOOST_AUTO_TEST_CASE ( test_critical_cycle_throughput_equality )
{
    parameters_list_t parameters;

    models::Dataflow* g = generateSampleCycle(); // maxThr = 0.0434783

    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g);

    models::Dataflow* cc_g = getCCGraph(g, result);

    VERBOSE_INFO("Critical cycle graph generated. Now computing CC throughput")

    kperiodic_result_t cc_res = algorithms::compute_Kperiodic_throughput_and_cycles(cc_g);

    // CRITICAL CYCLE THR & GRAPH THR NOT EQUAL!
    VERBOSE_INFO("result.throughput = " << result.throughput << " | cc_res.throughput = " << cc_res.throughput);
    VERBOSE_ASSERT_EQUALS(result.throughput , cc_res.throughput);

    VERBOSE_INFO("test_critical_cycle_throughput_equality #*&%" << std::endl << std::endl);

}

BOOST_AUTO_TEST_CASE( test_setting_throughput_maximal )
{

    parameters_list_t parameters;
    models::Dataflow* g = generateSampleCycle(); 

    // Get max thr of the graph
    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g);
    

    parameters_list_t new_params(parameters);
    new_params["MAX_SET"] = "";

    
    // Get all Storage Distributions with throughput <= new_params["THR"]
    StorageDistributionSet sds = algorithms::compute_Kperiodic_throughput_dse_sd(g, new_params);
    
    TIME_UNIT dse_thr = sds.getThrDist().second;
    VERBOSE_INFO("result.throughput: " << result.throughput 
                    << " | StoreDists.p_max: " << std::to_string(dse_thr));
    // dse.thr should be equal to max throughput
    VERBOSE_ASSERT_EQUALS(result.throughput, dse_thr); 
    
    // All SDs in set should have max throughput
    int i = 0;
    for (std::pair<TOKEN_UNIT, std::vector<StorageDistribution>> sdp : sds.getSet()){
        for (StorageDistribution sd : sdp.second){
            VERBOSE_INFO("\tStorageDistribution" << std::to_string(i)
                            << ": thr=" << std::to_string(sd.getThroughput()) 
                            << ", DistSz=" << std::to_string(sd.getDistributionSize()));
            VERBOSE_ASSERT_EQUALS(result.throughput, sd.getThroughput());
            i++; 
        }
    }


    VERBOSE_INFO("test_setting_throughput_maximal #*&%" << std::endl << std::endl);  

}

BOOST_AUTO_TEST_CASE( test_setting_throughput_maximal_cc )
{

    parameters_list_t parameters;
    models::Dataflow* g = generateSampleCycle(); 


    // Get max thr of the graph
    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g);
    

    parameters_list_t new_params(parameters);
    new_params["MAX_SET"] = "";

    models::Dataflow* cc_g = getCCGraph(g, result);

    
    // Get all Storage Distributions with throughput <= new_params["THR"]
    StorageDistributionSet sds = algorithms::compute_Kperiodic_throughput_dse_sd(cc_g, new_params);

    int k = 0;
    for (std::pair<TOKEN_UNIT, std::vector<StorageDistribution>> sd_p : sds.getSet()){
        for (StorageDistribution sd : sd_p.second){
            std::vector<TOKEN_UNIT> channel_quantities;
            for (Edge e : sd.getEdges()){
                channel_quantities.push_back(sd.getChannelQuantity(e));
            }
            VERBOSE_WARNING("STORE DIST" << k << " : " << vec2string(channel_quantities));
            k++;
        }
    }
    
    TIME_UNIT dse_thr = sds.getThrDist().second;
    VERBOSE_INFO("result.throughput: " << result.throughput 
                    << " | StoreDists.p_max: " << std::to_string(dse_thr));
    // dse.thr should be equal to max throughput
    VERBOSE_ASSERT_EQUALS(result.throughput, dse_thr); 
    
    // All SDs in set should have max throughput
    int i = 0;
    for (std::pair<TOKEN_UNIT, std::vector<StorageDistribution>> sdp : sds.getSet()){
        for (StorageDistribution sd : sdp.second){
            VERBOSE_INFO("\tStorageDistribution" << std::to_string(i)
                            << ": thr=" << std::to_string(sd.getThroughput()) 
                            << ", DistSz=" << std::to_string(sd.getDistributionSize()));
            VERBOSE_ASSERT_EQUALS(result.throughput, sd.getThroughput());
            i++; 
        }
    }


    VERBOSE_INFO("test_setting_throughput_maximal_cc #*&%" << std::endl << std::endl);  

}

BOOST_AUTO_TEST_CASE( test_setting_target_throughput )
{

    parameters_list_t parameters;
    models::Dataflow* g = generateSampleCycle(); 

    parameters_list_t new_params(parameters);
    new_params["THR"] = "0.04";

    // Get all Storage Distributions with throughput <= new_params["THR"]
    StorageDistributionSet sds = algorithms::compute_Kperiodic_throughput_dse_sd(g, new_params);
    
    TIME_UNIT dse_thr = sds.getThrDist().second;
    VERBOSE_INFO("StoreDists.p_max: " << std::to_string(dse_thr));
    
    bool minDistSzFound = false; // mindistSz that yields trgThr
    int minDistSz;
    int i = 0;
    for (std::pair<TOKEN_UNIT, std::vector<StorageDistribution>> sdp : sds.getSet()){
        for (StorageDistribution sd : sdp.second){
            VERBOSE_INFO("\tStorageDistribution" << std::to_string(i)
                            << ": thr=" << std::to_string(sd.getThroughput()) 
                            << ", DistSz=" << std::to_string(sd.getDistributionSize()));
            
            if (!minDistSzFound){
                if (sd.getThroughput() >= 0.04){
                    minDistSz = sd.getDistributionSize();
                    minDistSzFound = true;
                }
            } else VERBOSE_ASSERT_GreaterEqualThan(minDistSz, sd.getDistributionSize());
                /* if we found a SDsz that achieves trgThr, there should not be other SD
                    with a larger distribution size */
            
            i++; 
        }
    }


    VERBOSE_INFO("test_setting_target_throughput #*&%" << std::endl << std::endl);

}

BOOST_AUTO_TEST_CASE( test_next_after_not_maximal )
{

    parameters_list_t parameters;
    models::Dataflow* g = generateSampleCycle(); 

    parameters_list_t new_params(parameters);
    new_params["THR"] = "0.04";
    StorageDistributionSet sds = algorithms::compute_Kperiodic_throughput_dse_sd(g, new_params);

    parameters_list_t new_paramsMAX(new_params);
    new_paramsMAX["THR"] = std::to_string(0.04+0.000001); //nextafter did not work
    new_paramsMAX["MAX_SET"] = "";
    StorageDistributionSet sdsNxt = algorithms::compute_Kperiodic_throughput_dse_sd(g, new_paramsMAX);


    TIME_UNIT dse_thr = sds.getThrDist().second;
    TIME_UNIT nxt_thr = sdsNxt.getThrDist().second;
    VERBOSE_INFO("StoreDists.p_max: " << std::to_string(dse_thr) 
                    << " | StoreDistsNxt.p_max" << std::to_string(nxt_thr));
    
    /* Printing StorageDistSets */
    int i = 0;
    for (std::pair<TOKEN_UNIT, std::vector<StorageDistribution>> sdp : sds.getSet()){
        for (StorageDistribution sd : sdp.second){
            VERBOSE_INFO("\tStorageDistribution" << std::to_string(i)
                            << ": thr=" << std::to_string(sd.getThroughput()) 
                            << ", DistSz=" << std::to_string(sd.getDistributionSize()));          
            i++; 
        }
    }

    int j = 0;
    for (std::pair<TOKEN_UNIT, std::vector<StorageDistribution>> sdp : sdsNxt.getSet()){
        for (StorageDistribution sd : sdp.second){
            VERBOSE_INFO("\tStorageDistribution(nxt)" << std::to_string(j)
                            << ": thr=" << std::to_string(sd.getThroughput()) 
                            << ", DistSz=" << std::to_string(sd.getDistributionSize()));          
            j++; 
        }
    }

    VERBOSE_ASSERT_GreaterThan(nxt_thr, dse_thr);


    VERBOSE_INFO("test_next_after_not_maximal #*&%" << std::endl << std::endl);

}


BOOST_AUTO_TEST_CASE( test_next_after_maximal )
{ // Confirming we still get output if trgThr > maxThr
// NEED TO MIN!

    parameters_list_t parameters;
    models::Dataflow* g = generateSampleCycle(); 

    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g);

    parameters_list_t new_params(parameters);
    new_params["THR"] = std::to_string(std::min(result.throughput, result.throughput+0.000001));
    new_params["MAX_SET"] = "";

    
    // Get all Storage Distributions with throughput <= new_params["THR"]
    StorageDistributionSet sds = algorithms::compute_Kperiodic_throughput_dse_sd(g, new_params);
    
    TIME_UNIT dse_thr = sds.getThrDist().second;
    VERBOSE_INFO("result.throughput: " << result.throughput 
                    << " | StoreDists.p_max: " << std::to_string(dse_thr));
    // res.thr should be higher as THR upper bound was set at 0.0000001
    VERBOSE_ASSERT_EQUALS(result.throughput, dse_thr); 
    
    int i = 0;
    for (std::pair<TOKEN_UNIT, std::vector<StorageDistribution>> sdp : sds.getSet()){
        for (StorageDistribution sd : sdp.second){
            VERBOSE_INFO("\tStorageDistribution" << std::to_string(i)
                            << ": thr=" << std::to_string(sd.getThroughput()) 
                            << ", DistSz=" << std::to_string(sd.getDistributionSize()));
            VERBOSE_ASSERT_EQUALS(result.throughput, sd.getThroughput());
            i++; 
        }
    }


    VERBOSE_INFO("test_next_after_maximal #*&%" << std::endl << std::endl);

}


BOOST_AUTO_TEST_CASE( append_graph_sd_with_new_cc_sd ){

    parameters_list_t parameters;
    models::Dataflow* g = generateSampleCycle(); 
    models::Dataflow* dataflow_prime = genGraphWFeedbackEdges(g);
    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g);

    parameters_list_t new_params(parameters);
    new_params["MAX_SET"] = "t";
    models::Dataflow* cc_g = getCCGraph(dataflow_prime, result);

    StorageDistributionSet sds_g = algorithms::compute_Kperiodic_throughput_dse_sd(g, parameters);
    StorageDistributionSet sds_cc = algorithms::compute_Kperiodic_throughput_dse_sd(cc_g, new_params);

    StorageDistribution sd_g = sds_g.getNextDistribution(); // get
    StorageDistribution sd_cc = sds_cc.getNextDistribution();
    
    printEdgeSet(sd_g, g, "(G) ");
    printEdgeSet(sd_cc, cc_g, "(CC)");

    StorageDistribution sd = updateStoreDistwCCSD(dataflow_prime, sd_g, cc_g, sd_cc);

    printEdgeSet(sd, g, "(SD)"); // Weird that this works because g should not have prime


    VERBOSE_INFO("append_graph_sd_with_new_cc_sd #*&%" << std::endl << std::endl);

}

BOOST_AUTO_TEST_CASE( full_dse_test ){

    models::Dataflow* g = generateSampleCycle(); 
    parameters_list_t parameters;
    parameters["LOGDIR"] = "/home/sam/yalenus/Dataflow/2kiter/kiter/tests/data";
    algorithms::compute_Kperiodic_throughput_dse(g, parameters);
    VERBOSE_WARNING("FIRST DSE DONE");
    models::Dataflow* g_ = generateSampleCycle();
    parameters["LOGDIR"] = "/home/sam/yalenus/Dataflow/2kiter/kiter/tests/data2";
    algorithms::mod_Kperiodic_throughput_dse(g_, parameters);
}

BOOST_AUTO_TEST_SUITE_END()




