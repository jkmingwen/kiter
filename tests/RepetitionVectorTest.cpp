/*
 * RepetitionVectorTest.cpp
 *
 *  Created on: Jun 21, 2021
 *      Author: SamKouteili
 */

#define BOOST_TEST_MODULE RepetitionVectorTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "helpers/random_generator.h"
#include <chrono>


BOOST_FIXTURE_TEST_SUITE( repetition_vector_test , WITH_SAMPLE)

long generate_repetition_vector (models::Dataflow* dataflow){
    dataflow->reset_computation();
	auto start = std::chrono::high_resolution_clock::now();
	VERBOSE_ASSERT(computeRepetitionVector(dataflow), "Could not compute Repeptition vector");
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

	 for (auto g : graphs) {
         std::cout << "" << g->getVerticesCount()
	 		<< ", " << g->getEdgesCount()
	 		<< ", " << generate_repetition_vector(g) << std::endl;
	 }
}

BOOST_AUTO_TEST_SUITE_END()




