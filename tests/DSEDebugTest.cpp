/*
 *
 *  Created on: March 7, 2023
 *      Author: SamKouteili
 */

#define BOOST_TEST_MODULE DSEDebug
#include <string>
#include <numeric>
#include <commons/commons.h>
#include <models/Dataflow.h>
#include "helpers/test_classes.h"
#include <algorithms/dse/kperiodic.h>
#include <algorithms/dse/buffer_sizing.h>
#include <algorithms/dse/buffer_sizing.cpp>
#include <printers/SDF3Wrapper.h>


BOOST_FIXTURE_TEST_SUITE( DSEDebug , WITHOUT_VERBOSE) // WITH_VERBOSE or WITHOUT_VERBOSE

BOOST_AUTO_TEST_CASE( test_setting_throughput_maximal )
{

    VERBOSE_INFO("Generate the Sample Cycle");
    models::Dataflow* g = generateSampleCycle(); 
    VERBOSE_INFO(printers::generateSDF3XML(g));

    // Get max thr of the graph
    VERBOSE_INFO("Compute throughput and critical cycle.");
    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(g);

    VERBOSE_INFO("The computed throughput is " << result.throughput);
    BOOST_REQUIRE(std::abs(0.043478260869565216 - result.throughput) < 1e-05 ); // This unit test assumes 0.043478

    VERBOSE_INFO("Start the new implementation of DSE");
    StorageDistributionSet sds_new = algorithms::new_compute_Kperiodic_dse(g);
    VERBOSE_INFO("Storage distributions are \n" << sds_new.printDistributions(g));
    BOOST_REQUIRE_EQUAL(4, sds_new.getSet().size()); // The correct pareto should be 4 points with the zero.
    BOOST_REQUIRE_EQUAL(1, sds_new.getSet().find(0)->second.size());
    BOOST_REQUIRE_EQUAL(1, sds_new.getSet().find(21)->second.size());
    BOOST_REQUIRE_EQUAL(1, sds_new.getSet().find(22)->second.size());
    BOOST_REQUIRE_EQUAL(1, sds_new.getSet().find(23)->second.size());



    VERBOSE_INFO("Start the original implementation of DSE");
    parameters_list_t parameters;
    StorageDistributionSet sds = algorithms::compute_Kperiodic_throughput_dse_sd(g, parameters);
    VERBOSE_INFO("Storage distributions are \n" << sds.printDistributions(g));
    BOOST_REQUIRE_EQUAL(4, sds.getSet().size()); // The correct pareto should be 4 points with the zero.
    BOOST_REQUIRE_EQUAL(1, sds.getSet().find(0)->second.size());
    BOOST_REQUIRE_EQUAL(1, sds.getSet().find(21)->second.size());
    BOOST_REQUIRE_EQUAL(1, sds.getSet().find(22)->second.size());
    BOOST_REQUIRE_EQUAL(1, sds.getSet().find(23)->second.size());


}

    std::pair<TIME_UNIT, std::vector<StorageDistribution>> get_next_storage_distribution_efficiently_from_cc(StorageDistribution checkDist, models::Dataflow *dataflow_prime,
                                                                                                 std::map<Edge,Edge>& matching, std::map<Edge, TOKEN_UNIT> & minStepSizes) {

        std::vector<StorageDistribution> new_distributions;

        // Update graph with storage distribution just removed from checklist
        updateGraphwMatching(dataflow_prime, matching, checkDist);

        // Compute throughput and storage deps
        VERBOSE_DEBUG_DSE("Compute throughput and storage deps");
        kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(dataflow_prime);

        // Create new storage distributions for every storage dependency found; add new storage distributions to checklist
        VERBOSE_DEBUG_DSE(" Create new storage distributions");
        for (Edge c : result.critical_edges) {
            VERBOSE_DEBUG_DSE(" - Critical Edge " << dataflow_prime->getEdgeName(c));
            if (dataflow_prime->getEdgeType(c) == FEEDBACK_EDGE) {
                VERBOSE_DEBUG_DSE("   is interesting ");
                Edge original_edge = matching[c];
                StorageDistribution newDist(checkDist);
                newDist.setChannelQuantity(original_edge, (newDist.getChannelQuantity(original_edge) + minStepSizes[original_edge]));
                new_distributions.push_back(newDist);
            }
        }

        return std::pair<TIME_UNIT, std::vector<StorageDistribution>> (result.throughput, new_distributions);
    }

BOOST_AUTO_TEST_CASE( test_experiment ) {

        VERBOSE_INFO("Generate the Sample Cycle");
        models::Dataflow* dataflow = generateSampleCycle();
        VERBOSE_INFO(printers::generateSDF3XML(dataflow));


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
            std::pair<TIME_UNIT, std::vector<StorageDistribution>> new_points = get_next_storage_distribution_efficiently_from_cc(checkDist, dataflow_prime, matching, minStepSizes);
            checklist.addStorageDistributions(new_points.second);
            checkDist.setThroughput(new_points.first);
            minStorageDist.addStorageDistribution(checkDist);
            /*NOTE: check if pareto? minStorageDist should only hold pareto
              also, checklist storagedistributions incorrect? */

            // clean up distributions
            minStorageDist.minimizeStorageDistributions(checkDist);


        }

        // The minimum storage distribution for a throughput of 0 is (0, 0,..., 0)
        if (minStorageDist.getNextDistribution().getThroughput() == 0) {
            StorageDistribution zeroDist(minStorageDist.getNextDistribution());
            {ForEachEdge(dataflow_prime, c) { zeroDist.setChannelQuantity(c, 0); }}
            minStorageDist.removeStorageDistribution(minStorageDist.getNextDistribution());
            minStorageDist.addStorageDistribution(zeroDist);
        }

        VERBOSE_INFO("Storage distributions are \n" << minStorageDist.printDistributions(dataflow));

        BOOST_REQUIRE_EQUAL(4, minStorageDist.getSet().size()); // The correct pareto should be 4 points with the zero.
        BOOST_REQUIRE_EQUAL(1, minStorageDist.getSet().find(0)->second.size());
        BOOST_REQUIRE_EQUAL(1, minStorageDist.getSet().find(21)->second.size());
        BOOST_REQUIRE_EQUAL(1, minStorageDist.getSet().find(22)->second.size());
        BOOST_REQUIRE_EQUAL(1, minStorageDist.getSet().find(23)->second.size());

}
BOOST_AUTO_TEST_SUITE_END()




