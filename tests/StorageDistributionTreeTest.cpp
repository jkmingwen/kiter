/*
 *
 *  Created on: March 7, 2023
 *      Author: Bruno
 */

#define BOOST_TEST_MODULE StorageDistributionTreeTest
#include <boost/test/included/unit_test.hpp>
#include <algorithms/dse/StorageDistributionTree.h>

#include <set>
#include <map>
#include <vector>
#include <numeric>
#include <commons/commons.h>
#include <models/Dataflow.h>
#include "helpers/test_classes.h"
#include <algorithms/dse/buffer_sizing.h>

StorageDistribution get_distribution (models::Dataflow* g, int A, int B, int C) {
    std::map<Edge,BufferInfos> channel_quantities;
    channel_quantities[g->getEdgeByName("channel_1")]= {g->getPreload(g->getEdgeByName("channel_1")), A};
    channel_quantities[g->getEdgeByName("channel_2")]= {g->getPreload(g->getEdgeByName("channel_2")), B};
    channel_quantities[g->getEdgeByName("channel_3")]= {g->getPreload(g->getEdgeByName("channel_3")), C};
    return StorageDistribution  (g, 0.0,  channel_quantities);
}

BOOST_FIXTURE_TEST_SUITE( StorageDistributionTreeTest , WITH_VERBOSE)


BOOST_AUTO_TEST_CASE( test_StorageDistributionTree_addDistribution )
{

    models::Dataflow* g = generateSampleCycle();

    StorageDistributionTree tree;
    tree.addDistribution(get_distribution(g,0,0,0));
    tree.addDistribution(get_distribution(g,2,2,2));
    tree.addDistribution(get_distribution(g,1,1,1));
    tree.addDistribution(get_distribution(g,0,1,0));
    tree.addDistribution(get_distribution(g,5,2,2));
    tree.addDistribution(get_distribution(g,1,1,6));

    std::cout << tree.print() << std::endl;

    // Check if the number of distributions is correct
    std::vector<StorageDistribution> distributions = tree.getDistributions();

    BOOST_CHECK_EQUAL(distributions.size(), 6);

}


BOOST_AUTO_TEST_SUITE_END()




