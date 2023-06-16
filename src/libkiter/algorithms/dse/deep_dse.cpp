//
// Created by toky on 20/3/23.
//


#include <map>
#include <vector>
#include <string>
#include <commons/verbose.h>
#include <printers/SDF3Wrapper.h> // to write XML files
#include <commons/commons.h>
#include <models/Dataflow.h>
#include "buffer_sizing.h"
#include "kperiodic.h"
#include <chrono> // to take computation timings
#include <boost/filesystem.hpp>
#include <algorithms/dse/deep_dse.h>
#include "StorageDistributionTree.h"

// Check if local DSE completion conditions have been met
bool local_stop_condition(StorageDistributionSet& minStorageDist, StorageDistributionSet& checklist, TIME_UNIT target_thr) {
    return minStorageDist.isSearchComplete(checklist, target_thr);
}
StorageDistributionSet local_dse_with_init_dist(models::Dataflow *const dataflow, StorageDistribution initDist, bool approx) {

    // Compute the minimal steps
    std::map<Edge, TOKEN_UNIT> minStepSizes;
    findMinimumStepSz(dataflow, minStepSizes);

    // Compute the throughput target
    kperiodic_result_t result_max = algorithms::compute_Kperiodic_throughput_and_cycles(dataflow);

    TIME_UNIT thrTarget =  result_max.throughput;


    // Produce the feedback buffers
    std::pair<models::Dataflow*, std::map<Edge,Edge> > dataflow_and_matching = genGraphWFeedbackEdgesWithPairs(dataflow);
    models::Dataflow* dataflow_prime = dataflow_and_matching.first;
    std::map<Edge,Edge>& matching = dataflow_and_matching.second;

    updateGraphwMatching(dataflow_prime, matching, initDist);
    kperiodic_result_t result_min = algorithms::compute_Kperiodic_throughput_and_cycles(dataflow_prime);
    VERBOSE_INFO("    result_min " <<result_min.throughput <<"    result_max " <<result_max.throughput );

    // Prepare the checklist of distribution to explore
    StorageDistributionSet checklist = StorageDistributionSet(initDist);

    // Start the exploration loop
    StorageDistributionSet minStorageDist;


    while (!local_stop_condition(minStorageDist, checklist, thrTarget)) {

        // Pop checkDist
        StorageDistribution checkDist(checklist.getNextDistribution());
        checklist.removeStorageDistribution(checklist.getNextDistribution());
        VERBOSE_DEBUG_DSE("    Pop SD = " << checkDist.getQuantitiesStr());

        // Compute the next distribution to explore
        std::pair<TIME_UNIT, std::vector<StorageDistribution>> new_points = get_next_storage_distribution_from_cc(checkDist, dataflow_prime, matching, minStepSizes);
        checkDist.setThroughput(new_points.first);
        minStorageDist.addStorageDistribution(checkDist);

        if (new_points.first == result_min.throughput) { // This data point doesnt improve the throughput, we dig in
            checklist.addStorageDistributions(new_points.second);
        }

        /*NOTE: check if pareto? minStorageDist should only hold pareto
          also, checklist storagedistributions incorrect? */

        // clean up distributions
        if (approx)
        minStorageDist.minimizeStorageDistributions(checkDist);

    }

    return minStorageDist;

}

models::Dataflow* get_critical_cycle_original_edges_from_prime (const models::Dataflow* g,
                                                                const std::map<Edge,Edge>& matching ,
                                                                const kperiodic_result_t& result){

    models::Dataflow* cc_g = new models::Dataflow();
    std::set<std::string> visited_v;
    std::set<std::string> visited_e;

    VERBOSE_ASSERT(result.critical_edges.size() > 0, "Empty Critical cycle");

    for (Edge prime_edge : result.critical_edges) {

        Edge original_edge = matching.at(prime_edge);
        if (visited_e.count(g->getEdgeName(original_edge))) continue;
        visited_e.insert(g->getEdgeName(original_edge));
        std::string original_source_name = g->getVertexName(g->getEdgeSource(original_edge));
        std::string original_target_name = g->getVertexName(g->getEdgeTarget(original_edge));

        if (!visited_v.count(original_source_name))  {
            Vertex src_cc = cc_g->addVertex(original_source_name);
            cc_g->setPhasesQuantity(src_cc, g->getPhasesQuantity(g->getEdgeSource(original_edge)));
            cc_g->setReentrancyFactor(src_cc, g->getReentrancyFactor(g->getEdgeSource(original_edge)));
            cc_g->setVertexDuration(src_cc, g->getVertexPhaseDuration(g->getEdgeSource(original_edge)));
            visited_v.insert(original_source_name);
        }
        if (!visited_v.count(original_target_name))  {
            Vertex trg_cc = cc_g->addVertex(original_target_name);
            cc_g->setPhasesQuantity(trg_cc, g->getPhasesQuantity(g->getEdgeTarget(original_edge)));
            cc_g->setReentrancyFactor(trg_cc, g->getReentrancyFactor(g->getEdgeTarget(original_edge)));
            cc_g->setVertexDuration(trg_cc, g->getVertexPhaseDuration(g->getEdgeTarget(original_edge)));
            visited_v.insert(original_target_name);
        }

        Vertex src_cc = cc_g->getVertexByName(original_source_name);
        Vertex trg_cc = cc_g->getVertexByName(original_target_name);

        Edge e_cc = cc_g->addEdge(src_cc, trg_cc, g->getEdgeName(original_edge));
        cc_g->setPreload(e_cc, g->getPreload(original_edge));
        cc_g->setEdgeInPhases(e_cc, g->getEdgeInVector(original_edge));
        cc_g->setEdgeOutPhases(e_cc, g->getEdgeOutVector(original_edge));

    }
    return cc_g;

}

std::pair<TIME_UNIT, std::vector<StorageDistribution>> get_next_storage_distribution_efficiently_from_cc(const StorageDistribution& checkDist, models::Dataflow *dataflow_prime,
                                                                                                         const std::map<Edge,Edge>& matching, const std::map<Edge, TOKEN_UNIT> & minStepSizes, bool approx) {


    VERBOSE_DEBUG_DSE("Update the dataflow_prime with");
    updateGraphwMatching(dataflow_prime, matching, checkDist);

    // Compute throughput and storage deps
    VERBOSE_DEBUG_DSE("Compute throughput to get a critical cycle, and explore solutions using the CC");
    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(dataflow_prime);

    if (result.critical_edges.size() == 0) {
        // Reach the end.
        return std::pair<TIME_UNIT, std::vector<StorageDistribution>> (result.throughput, {});
    }

    VERBOSE_ASSERT(result.critical_edges.size() > 0, "compute_Kperiodic_throughput_and_cycles  returns empty cycle");
    models::Dataflow* cc_g = get_critical_cycle_original_edges_from_prime (dataflow_prime, matching , result);
    VERBOSE_ASSERT (cc_g->getVerticesCount() > 0, "Empty Critical cycle");
    auto local_dist = initialiseDist(cc_g);
    for (Edge c: local_dist.getEdges()) { // FIXME: This is a workaround
        std::string cc_edge_name = cc_g->getEdgeName(c);
        Edge prime_edge = dataflow_prime->getEdgeByName(cc_edge_name);
        local_dist.setChannelQuantity(c, checkDist.getChannelQuantity(matching.at(prime_edge)));
    }

    VERBOSE_INFO("Local search result from " << local_dist );

    // TODO: There is no stopping condition here, this could be improved, but the stopping condition is not trivial!
    StorageDistributionSet cc_sds = local_dse_with_init_dist(cc_g, local_dist, approx);


    //StorageDistributionTree tree;
    std::vector<StorageDistribution> new_distributions;
    for (std::pair<TOKEN_UNIT, std::vector<StorageDistribution>> cc_pair : cc_sds.getSet()){
        if (cc_pair.first > local_dist.getDistributionSize()) {
            for (StorageDistribution cc_sd : cc_pair.second) {
                // The distribution size of the point is bigger than the origial, must be interesting.
                StorageDistribution new_sd = update_storage_distribution_from_cc(checkDist, cc_sd);
                //tree.addDistribution(cc_sd);
                VERBOSE_INFO("     Check " << commons::toString(cc_sd) << " : " << commons::toString(new_sd));
                int reachable = 0;
                for (StorageDistribution sd : new_distributions) { // for every storage distribution avaible
                                                                   // if it cannot reach new_sd, then new_sd interesting
                    bool reached = true;
                    for (auto &e : sd.getEdges()) {
                        if (sd.getChannelQuantity(e) > new_sd.getChannelQuantity(e)) {
                            reached = false;
                        }
                    }

                    if (reached) {
                        reachable += 1;
                    }

                    VERBOSE_INFO("           Compare " << commons::toString(new_sd) << " : " << commons::toString(sd) << " reached is " << reached);
                }

                VERBOSE_INFO("     reachable count " << reachable);

                if (reachable == 0) new_distributions.push_back(new_sd);
            }
            // TODO: If you break here, you can miss important configuration
            // TODO: Careful about it in the proof.
        }


    }

    //VERBOSE_INFO("StorageDistributionTree \n" << tree.print());
    VERBOSE_INFO("New distributions are \n" << commons::toString(new_distributions));

    //std::pair<TIME_UNIT, std::vector<StorageDistribution>> new_points = get_next_storage_distribution_from_cc(checkDist, dataflow_prime, matching, minStepSizes);


    return std::pair<TIME_UNIT, std::vector<StorageDistribution>> (result.throughput, new_distributions);
}

StorageDistribution update_storage_distribution_from_cc(const StorageDistribution& sd_g, const  StorageDistribution& sd_cc){
    /* Update storage distribution with new critical cycle channel quantities */

    StorageDistribution new_sd(sd_g); // making copy to update

    for (Edge e_cc : sd_cc.getEdges()){
        std::string cc_edge_name = sd_cc.getDataflow()->getEdgeName(e_cc);
        Edge original_edge = sd_g.getDataflow()->getEdgeByName(cc_edge_name);
        new_sd.setChannelQuantity(original_edge, sd_cc.getChannelQuantity(e_cc));
    }

    return new_sd;
}


StorageDistributionSet algorithms::deep_dse(models::Dataflow *const dataflow, bool approx) {
        std::cout << "storage distribution size,throughput,channel quantities,computation duration,cumulative duration" << std::endl;
        auto beginTime = std::chrono::steady_clock::now();
        auto initDist = initialiseDist(dataflow);

        // Compute the minimal steps
        std::map<Edge, TOKEN_UNIT> minStepSizes;
        findMinimumStepSz(dataflow, minStepSizes);

        // Compute the throughput target
        kperiodic_result_t result_max = algorithms::compute_Kperiodic_throughput_and_cycles(dataflow);

        TIME_UNIT thrTarget = result_max.throughput;
        VERBOSE_DSE("Max Throughput: " << result_max.throughput << " | Target Throughput: " << thrTarget);


        // Produce the feedback buffers
        std::pair<models::Dataflow*, std::map<Edge,Edge> > dataflow_and_matching = genGraphWFeedbackEdgesWithPairs(dataflow);
        models::Dataflow* dataflow_prime = dataflow_and_matching.first;
        std::map<Edge,Edge>& matching = dataflow_and_matching.second;

        // Prepare the checklist of distribution to explore
        StorageDistributionSet checklist = StorageDistributionSet(initDist);

        // Start the exploration loop
        StorageDistributionSet minStorageDist;

        // Add a storage distribution full of zeros to avoid comparison problems
        StorageDistribution zeroDist (dataflow);
        {ForEachEdge(dataflow, c) {
                zeroDist.setChannelQuantity(c, 0);
            }}
        minStorageDist.addStorageDistribution(zeroDist);


        while (!minStorageDist.isSearchComplete(checklist, thrTarget)) {

            // Pop checkDist
            StorageDistribution checkDist(checklist.getNextDistribution());
            checklist.removeStorageDistribution(checklist.getNextDistribution());
            VERBOSE_DSE("Pop SD = " << checkDist.getQuantitiesStr());

            // Compute the next distribution to explore
            auto startTime = std::chrono::steady_clock::now();
            std::pair<TIME_UNIT, std::vector<StorageDistribution>> new_points = get_next_storage_distribution_efficiently_from_cc(checkDist, dataflow_prime, matching, minStepSizes, approx);
            auto endTime = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> execTime = endTime - startTime; // duration in ms
            checklist.addStorageDistributions(new_points.second);
            checkDist.setThroughput(new_points.first);
            checkDist.setExecutionTime(execTime.count());
            std::chrono::duration<double, std::milli> cumulTime = endTime - beginTime; // duration in ms
            checkDist.setCumulativeTime(cumulTime.count());
            std::cout << checkDist.get_csv_line() << std::endl;
            minStorageDist.addStorageDistribution(checkDist);

            // clean up distributions
            minStorageDist.minimizeStorageDistributions(checkDist);


        }


        return minStorageDist;

}