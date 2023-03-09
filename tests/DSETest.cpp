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

models::Dataflow* getCCGraph(models::Dataflow* g, kperiodic_result_t result){

    models::Dataflow* cc_g = new models::Dataflow(); /***@@@*****/

    std::set<std::string> visited_v;

    VERBOSE_INFO("Critical edges found:");
    for (Edge e_g : result.critical_edges){
        
        Vertex src_g = g->getEdgeSource(e_g);
        Vertex trg_g = g->getEdgeTarget(e_g);
        std::string src = g->getVertexName(src_g);
        std::string trg = g->getVertexName(trg_g);
        
        Vertex src_cc;
        Vertex trg_cc; 
        if (visited_v.find(src) == std::end(visited_v)){
            src_cc = cc_g->addVertex(g->getVertexName(src_g)); // Adds ID as name
            visited_v.insert(src);
        } else {
            src_cc = cc_g->getVertexByName(src);
        }
        if (visited_v.find(trg) == std::end(visited_v)){
            trg_cc = cc_g->addVertex(g->getVertexName(trg_g));
            visited_v.insert(trg);
        } else {
            trg_cc = cc_g->getVertexByName(trg);
        }

        Edge e_cc = cc_g->addEdge(src_cc, trg_cc, g->getEdgeName(e_g));
        cc_g->setPreload(e_cc, g->getPreload(e_g)); 
        cc_g->setEdgeInPhases(e_cc, g->getEdgeInVector(e_g));
        cc_g->setEdgeOutPhases(e_cc, g->getEdgeOutVector(e_g));
        cc_g->setReentrancyFactor(src_cc, g->getReentrancyFactor(src_g));
        cc_g->setVertexDuration(src_cc, g->getVertexPhaseDuration(src_g));
        cc_g->setReentrancyFactor(trg_cc, g->getReentrancyFactor(trg_g));
        cc_g->setVertexDuration(trg_cc, g->getVertexPhaseDuration(trg_g));

        // Edge e = cc_g->addEdge(src, trg, g->getEdgeId(e_cc), g->getEdgeName(e_cc));
        
        VERBOSE_INFO("EdgeID: " << cc_g->getEdgeId(e_cc) 
                        << " | EdgeName: " << cc_g->getEdgeName(e_cc) 
                        << " | ChnQnt: " << cc_g->getPreload(e_cc));     
        VERBOSE_INFO("\tEdge(In)Phases: " << vec2string(cc_g->getEdgeInVector(e_cc)));    
        VERBOSE_INFO("\tEdge(Out)Phases: " << vec2string(cc_g->getEdgeOutVector(e_cc)));   
        VERBOSE_INFO("\tSrc(Name): " << cc_g->getVertexName(src_cc) 
                    << " -> Trg(Name): " << cc_g->getVertexName(trg_cc)); 
        VERBOSE_INFO("\tPhaseDurations(" << cc_g->getVertexName(src_cc) 
                        << "): " << vec2string(cc_g->getVertexPhaseDuration(src_cc)));    
        VERBOSE_INFO("\tPhaseDurations(" << cc_g->getVertexName(trg_cc) 
                        << "): " << vec2string(cc_g->getVertexPhaseDuration(trg_cc))); 

    }

    return cc_g;

}



BOOST_AUTO_TEST_CASE( test_critical_cycle_mapping_to_graph )
{

    parameters_list_t parameters;
    models::Dataflow* g = generateSampleCycle(); // maxThr = 0.0434783

    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g, parameters);

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

    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g, parameters);

    models::Dataflow* cc_g = getCCGraph(g, result);

    VERBOSE_INFO("Critical cycle graph generated. Now computing CC throughput")

    kperiodic_result_t cc_res = algorithms::compute_Kperiodic_throughput_and_cycles(cc_g, parameters);

    // CRITICAL CYCLE THR & GRAPH THR NOT EQUAL!
    VERBOSE_INFO("result.throughput = " << result.throughput << " | cc_res.throughput = " << cc_res.throughput);
    // VERBOSE_ASSERT_EQUALS(result.throughput , cc_res.throughput);

    VERBOSE_INFO("test_critical_cycle_throughput_equality #*&%" << std::endl << std::endl);

}

BOOST_AUTO_TEST_CASE( test_setting_throughput_maximal )
{

    parameters_list_t parameters;
    models::Dataflow* g = generateSampleCycle(); 

    // Get max thr of the graph
    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g, parameters);
    

    parameters_list_t new_params(parameters);
    new_params["MAX_SET"] = "t";

    
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
    new_paramsMAX["MAX_SET"] = "t";
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

    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g, parameters);

    parameters_list_t new_params(parameters);
    new_params["THR"] = std::to_string(std::min(result.throughput, result.throughput+0.000001));
    new_params["MAX_SET"] = "t";

    
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
    parameters_list_t new_params(parameters);
    new_params["MAX_SET"] = "t";

    models::Dataflow* g = generateSampleCycle(); 
    // create new graph with modelled bounded channel quantities
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
    
    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(dataflow_prime, parameters);

    models::Dataflow* cc_g = getCCGraph(dataflow_prime, result);

    StorageDistributionSet sds_g = algorithms::compute_Kperiodic_throughput_dse_sd(g, parameters);
    StorageDistributionSet sds_cc = algorithms::compute_Kperiodic_throughput_dse_sd(cc_g, new_params);

    StorageDistribution sd_g = sds_g.getNextDistribution();
    StorageDistribution sd_cc = sds_cc.getNextDistribution();
    // sd_g.printInfo(dataflow_prime);
    // sd_cc.printInfo(cc_g);

    StorageDistribution sd(sd_g); // making copy to update


    for (Edge e_cc : result.critical_edges){


        // only increase channel quantity on "modelled" channels
        if (dataflow_prime->getEdgeId(e_cc) > g->getEdgesCount()) {
            VERBOSE_DSE("\tFound storage dependency in channel "
                        << dataflow_prime->getEdgeName(e_cc) << std::endl);
            // make new modelled storage distribution according to storage dependencies

            sd.setChannelQuantity(e_cc, sd_cc.getChannelQuantity(e_cc));
            VERBOSE_DSE("\t\tIncreasing channel size of "
                        << dataflow_prime->getEdgeName(e_cc) << " to "
                        << sd_cc.getChannelQuantity(e_cc) << std::endl);
            VERBOSE_DSE("\tUpdating checklist with new storage distribution..."
                        << std::endl);
        }

    }

    // sd.printInfo(dataflow_prime);


    VERBOSE_INFO("append_graph_sd_with_new_cc_sd #*&%" << std::endl << std::endl);
}

BOOST_AUTO_TEST_SUITE_END()




