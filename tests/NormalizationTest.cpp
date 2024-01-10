/*
 * NormalizationTest.cpp
 *
 *  Created on: Jun 21, 2021
 *      Author: SamKouteili
 */

#define BOOST_TEST_MODULE NormalizationTest
#include "helpers/test_classes.h"
#include <models/Dataflow.h>
#include "helpers/random_generator.h"
#include <algorithms/normalization.h>
#include <chrono>
#include <printers/stdout.h>


#define MAX_ITER        10
#define MAX_PHASE_COUNT 5

BOOST_FIXTURE_TEST_SUITE( normalization_test_suite , WITH_VERBOSE)

long generate_normalization_vector (models::Dataflow* dataflow){
	printers::printInfos(dataflow);
	VERBOSE_ASSERT(computeRepetitionVector(dataflow),"inconsistent graph");
	auto start = std::chrono::high_resolution_clock::now();
	//TODO : The normalization does not support non-connected graphs!
	//VERBOSE_ASSERT(algorithms::normalize(dataflow), "Could not normalize");
	auto elapse = std::chrono::high_resolution_clock::now() - start;
	return elapse.count()/ 1000000;
}


std::map<ARRAY_INDEX,TOKEN_UNIT> compute_alphas(models::Dataflow *from ) {

    // Compute local lcms
    std::map<ARRAY_INDEX,TOKEN_UNIT> lcms;
    std::map<ARRAY_INDEX,TOKEN_UNIT> alphas;

    {ForEachVertex(from, t) {
        ARRAY_INDEX tid = from->getVertexId(t);
        lcms[tid] = 1;
        {ForInputEdges(from,t,c) {
                lcms[tid] = std::lcm(from->getEdgeOut(c), lcms[tid]);
        }}
        {ForOutputEdges(from,t,c) {
                lcms[tid] = std::lcm(from->getEdgeIn(c), lcms[tid]);
        }}
    }}


    {ForEachEdge(from, c) {
        ARRAY_INDEX cid = from->getEdgeId(c);
        Vertex source = from->getEdgeSource(c);
        Vertex destination = from->getEdgeTarget(c);
        ARRAY_INDEX sourceId = from->getVertexId(source);
        ARRAY_INDEX destinationId = from->getVertexId(destination);
        TOKEN_UNIT sourceProd = from->getEdgeIn(c);
        TOKEN_UNIT destinationCons = from->getEdgeOut(c);
        alphas[cid] = std::lcm(lcms[sourceId] / sourceProd, lcms[destinationId] / destinationCons);
    }}

    return alphas;
}


BOOST_AUTO_TEST_CASE( functional_test )
    {

        models::Dataflow* g      = generateDAC2013();
        models::Dataflow* norm_g = generateNormDAC2013();

        // Normalize

        VERBOSE_ASSERT(computeRepetitionVector(g), "Could not compute repetition vector.");
        VERBOSE_ASSERT(algorithms::normalize(g), "Could not normalize.");

        std::map<ARRAY_INDEX,TOKEN_UNIT> alphas = compute_alphas(g);

        // Check new_normalization
        {ForEachEdge(g,c) {
                std::cout << "alpha:" << alphas[g->getEdgeId(c)]
                          << std::endl;
        }}

        // Check preload
        {ForEachEdge(g,c) {
            Edge norm_c = norm_g->getEdgeById(g->getEdgeId(c));
            std::cout << "mop before:" << g->getPreload(c)
                      << " after:" << g->getNormMop(c)
                      << " expected:" << norm_g->getPreload(norm_c)
                      << std::endl;
            VERBOSE_ASSERT_EQUALS(g->getNormMop(c), norm_g->getPreload(norm_c));
        }}


        // Check weights
        {ForEachEdge(g,c) {
                Edge norm_c = norm_g->getEdgeById(g->getEdgeId(c));
                std::cout << "in before:" << g->getEdgeIn(c)
                          << " after:" << g->getZi(g->getEdgeSource(c))
                          << " expected:" << norm_g->getEdgeIn(norm_c)
                          << std::endl;
                std::cout << "out before:" << g->getEdgeOut(c)
                          << " after:" << g->getZi(g->getEdgeTarget(c))
                          << " expected:" << norm_g->getEdgeOut(norm_c)
                          << std::endl;
                VERBOSE_ASSERT_EQUALS(g->getZi(g->getEdgeSource(c)), norm_g->getEdgeIn(norm_c));
                VERBOSE_ASSERT_EQUALS(g->getZi(g->getEdgeTarget(c)), norm_g->getEdgeOut(norm_c));
            }}


    }



BOOST_AUTO_TEST_CASE( generate_normalization_vector__performance_test )
{

	std::cout << "" << "actor number"
			<< ", " << "buffer number"
			<<", " <<  "repetition vector" << std::endl;

	for (int i = 1; i <= MAX_ITER; i++) {
		int buf_num = std::rand() % std::max(1,(i)/2);
		models::Dataflow*  g = generate_random_graph(i, buf_num,  MAX_PHASE_COUNT, i, i);
		std::cout << "" << i
			<< ", " << buf_num
			<< ", " << generate_normalization_vector(g);
		delete g;
	}
}

BOOST_AUTO_TEST_SUITE_END()




