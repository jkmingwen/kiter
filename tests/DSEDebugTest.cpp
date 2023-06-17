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
#include <algorithms/dse/deep_dse.h>

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
    VERBOSE_INFO("Storage distributions are \n" << sds_new.printDistributions());
    BOOST_REQUIRE_EQUAL(4, sds_new.getSet().size()); // The correct pareto should be 4 points with the zero.
    BOOST_REQUIRE_EQUAL(1, sds_new.getSet().find(0)->second.size());
    BOOST_REQUIRE_EQUAL(1, sds_new.getSet().find(21)->second.size());
    BOOST_REQUIRE_EQUAL(1, sds_new.getSet().find(22)->second.size());
    BOOST_REQUIRE_EQUAL(1, sds_new.getSet().find(23)->second.size());



    VERBOSE_INFO("Start the original implementation of DSE");
    parameters_list_t parameters;
    StorageDistributionSet sds = algorithms::compute_Kperiodic_throughput_dse_sd(g, parameters);
    VERBOSE_INFO("Storage distributions are \n" << sds.printDistributions());
    BOOST_REQUIRE_EQUAL(4, sds.getSet().size()); // The correct pareto should be 4 points with the zero.
    // TODO : Check if 0, 19, 20 ,21 is the correct answer., it has to do with preload.
    BOOST_REQUIRE_EQUAL(1, sds.getSet().find(0)->second.size());
    BOOST_REQUIRE_EQUAL(1, sds.getSet().find(21)->second.size());
    BOOST_REQUIRE_EQUAL(1, sds.getSet().find(22)->second.size());
    BOOST_REQUIRE_EQUAL(1, sds.getSet().find(23)->second.size());


}


    BOOST_AUTO_TEST_CASE( test_getCCGraph ) {

        return;
        VERBOSE_INFO("Start test_getCCGraph and prepare dataflow, dataflow_prime, and matching.");
        models::Dataflow* dataflow = generateSampleCycle();
        std::pair<models::Dataflow*, std::map<Edge,Edge> > dataflow_prime_and_matching = genGraphWFeedbackEdgesWithPairs(dataflow);
        models::Dataflow* dataflow_prime = dataflow_prime_and_matching.first;
        std::map<Edge,Edge>& matching = dataflow_prime_and_matching.second;

        VERBOSE_INFO("Generate a distribution to update the dataflow_prime with");
        auto checkDist = initialiseDist(dataflow);
        updateGraphwMatching(dataflow_prime, matching, checkDist);
        VERBOSE_INFO("First distributions is: " << commons::toString(checkDist));

        // Compute throughput and storage deps
        VERBOSE_INFO("Compute throughput to get a critical cycle, and explore solutions using the CC");
        kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(dataflow_prime);
        models::Dataflow* cc_g = get_critical_cycle_original_edges_from_prime (dataflow, matching , result);
        VERBOSE_INFO(printers::generateSDF3XML(cc_g));
        auto local_dist = initialiseDist(cc_g);

        StorageDistributionSet cc_sds = algorithms::new_compute_Kperiodic_dse(cc_g);
        VERBOSE_INFO("Storage distributions are \n" << cc_sds.printDistributions());

        std::vector<StorageDistribution> new_distributions;
        for (std::pair<TOKEN_UNIT, std::vector<StorageDistribution>> cc_pair : cc_sds.getSet()){
            if (cc_pair.first > local_dist.getDistributionSize()) {
                for (StorageDistribution cc_sd : cc_pair.second) {
                    // The distribution size of the point is bigger than the origial, must be interesting.
                    StorageDistribution new_sd = update_storage_distribution_from_cc(checkDist, cc_sd);
                    new_distributions.push_back(new_sd);
                }
            }
        }

        VERBOSE_INFO("New distributions are \n" << commons::toString(new_distributions));

    }


BOOST_AUTO_TEST_CASE( test_deep_dse ) {
        VERBOSE_INFO("Generate the Sample Cycle");
        models::Dataflow* dataflow = generateSampleCycle();
        VERBOSE_INFO(printers::generateSDF3XML(dataflow));

        auto minStorageDist = algorithms::deep_dse(dataflow, false);

        VERBOSE_INFO("Storage distributions are \n" << minStorageDist.printDistributions());

        BOOST_REQUIRE_EQUAL(4, minStorageDist.getSet().size()); // The correct pareto should be 4 points with the zero.
        BOOST_REQUIRE_EQUAL(1, minStorageDist.getSet().find(0)->second.size());
        BOOST_REQUIRE_EQUAL(1, minStorageDist.getSet().find(21)->second.size());
        BOOST_REQUIRE_EQUAL(1, minStorageDist.getSet().find(22)->second.size());
        BOOST_REQUIRE_EQUAL(1, minStorageDist.getSet().find(23)->second.size());

}
BOOST_AUTO_TEST_SUITE_END()




