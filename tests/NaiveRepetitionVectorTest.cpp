/*
 * NaiveRepetitionVectorTest.cpp
 *
 *  Created on: oct 5, 2022
 *      Author: bbodin
 */

#define BOOST_TEST_MODULE NaiveRepetitionVectorTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "helpers/random_generator.h"
#include <chrono>
#include <models/NaiveDataflow.h>
#include <models/repetition_vector.h>

BOOST_FIXTURE_TEST_SUITE( naive_repetition_vector_test , WITH_SAMPLE)

long generate_repetition_vector (models::NaiveDataflow* dataflow){
	auto start = std::chrono::high_resolution_clock::now();
	VERBOSE_ASSERT(generic_repetition_vector(dataflow), "Could not compute Repeptition vector");
	auto elapse = std::chrono::high_resolution_clock::now() - start;

	delete dataflow;
	return elapse.count()/ 1000000;
}

BOOST_AUTO_TEST_CASE( sample_repetition_vector_test )
{
	BOOST_REQUIRE(pipeline_sample);
	Vertex a = pipeline_sample->getVertexByName("a");
	Vertex b = pipeline_sample->getVertexByName("b");
	Vertex c = pipeline_sample->getVertexByName("c");
	BOOST_REQUIRE(computeRepetitionVector(pipeline_sample));
	BOOST_REQUIRE_EQUAL(pipeline_sample->getNi(a), 3);
	BOOST_REQUIRE_EQUAL(pipeline_sample->getNi(b), 2);
	BOOST_REQUIRE_EQUAL(pipeline_sample->getNi(c), 1);

}


#define MAX_ITER 30
#define MAX_PHASE_COUNT 5

BOOST_AUTO_TEST_CASE( generate_repetition_vector_performance_test )
{


    RandomGeneratorConfiguration config;

    RandomGenerator generator (config);

    commons::set_verbose_mode(commons::INFO_LEVEL);

    VERBOSE_INFO("Start performance test.");


    std::vector<models::Dataflow *> graphs;

    for (int i = 10; i <= 300; i += 50) {

        int buf_num = std::rand() % (i / 2) + i;

        VERBOSE_INFO("generate graph " << i << " with " << buf_num);

        models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
        graphs.push_back(g);
    }


	 std::cout << "" << "actor number"
	 		<< ", " << "buffer number"
	 		<<", " <<  "repetition vector" << std::endl;

	 for (auto original : graphs) {

         VERBOSE_INFO("Convert");
         models::NaiveDataflow *g = new models::NaiveDataflow (*original) ;

         VERBOSE_INFO("Compute");
         auto res = generic_repetition_vector(g);

         VERBOSE_INFO("print");
         std::cout << "" << g->getVerticesCount()
	 		<< ", " << g->getEdgesCount()
	 		<< ", " << res << std::endl;
	 }
}

BOOST_AUTO_TEST_SUITE_END()




