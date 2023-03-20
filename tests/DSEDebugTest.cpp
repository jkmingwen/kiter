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
    return;
    TIME_UNIT dse_thr = sds.getThrDist().second;
    VERBOSE_INFO("result.throughput: " << result.throughput 
                    << " | StoreDists.p_max: " << std::to_string(dse_thr));

    VERBOSE_INFO("test_setting_throughput_maximal #*&%" << std::endl << std::endl);  

}

BOOST_AUTO_TEST_SUITE_END()




