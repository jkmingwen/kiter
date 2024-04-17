//
// Created by jachym on 3/4/23.
//
#define BOOST_TEST_MODULE SchedulingsTest
#include "helpers/test_classes.h"
#include "helpers/random_generator.h"
#include "algorithms/schedulings.h"
#include <models/Dataflow.h>
#include <commons/KiterRegistry.h>

BOOST_FIXTURE_TEST_SUITE( schedulings_test, WITH_SAMPLE)

    void sampleTest(const std::function<void(models::Dataflow*, parameters_list_t)> fun, models::Dataflow* const graph) {
        parameters_list_t params = parameters_list_t();
        fun(graph, params);
    }

#define MAX_PHASE_COUNT 1

    void randomTest(std::function<void(models::Dataflow*, parameters_list_t)> fun) {
        parameters_list_t params = parameters_list_t();
        std::vector<models::Dataflow *> graphs;

        VERBOSE_INFO("Generating Graphs");
        for (int i = 10; i <= 300; i += 50) {

            int buf_num = std::rand() % (i / 2) + i;

            VERBOSE_INFO("generate graph " << i << " with " << buf_num);

            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
            graphs.push_back(g);
        }

        for(auto graph : graphs) {
            fun(graph, params);
        }
    }

    void randomTest(std::function<void(models::Dataflow*)> fun) {
        std::vector<models::Dataflow *> graphs;

        VERBOSE_INFO("Generating Graphs");
        for (int i = 10; i <= 300; i += 50) {

            int buf_num = std::rand() % (i / 2) + i;

            VERBOSE_INFO("generate graph " << i << " with " << buf_num);

            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
            graphs.push_back(g);
        }

        for(auto graph : graphs) {
            fun(graph);
        }
    }

    BOOST_AUTO_TEST_CASE(KPeriodic_taskNoCbufferless, * boost::unit_test::disabled()){
        BOOST_REQUIRE(pipeline_sample);
        //TODO: function unimplemented
        BOOST_TEST(false);
//        VERBOSE_INFO("Sample test");
//        sampleTest(algorithms::scheduling::KPeriodic_taskNoCbufferless, pipeline_sample);
//        VERBOSE_INFO("Random test");
//        randomTest(algorithms::scheduling::KPeriodic_taskNoCbufferless);
    }


    BOOST_AUTO_TEST_CASE( BufferlessNoCScheduling) {
        BOOST_REQUIRE(pipeline_sample);
        VERBOSE_INFO("Sample test");
        sampleTest(algorithms::BufferlessNoCScheduling, pipeline_sample);
        VERBOSE_INFO("Random test");
        randomTest(algorithms::BufferlessNoCScheduling);
    }

    BOOST_AUTO_TEST_CASE( OnePeriodicScheduling_LP) {
        BOOST_REQUIRE(pipeline_sample);
        VERBOSE_INFO("Sample test");
        sampleTest(algorithms::scheduling::CSDF_1PeriodicScheduling_LP, pipeline_sample);
        VERBOSE_INFO("Random test");
        randomTest(algorithms::scheduling::CSDF_1PeriodicScheduling_LP);
    }

    BOOST_AUTO_TEST_CASE(N_PeriodicScheduling_LP) {
        BOOST_REQUIRE(pipeline_sample);
        VERBOSE_INFO("Sample test");
        sampleTest(algorithms::scheduling::CSDF_NPeriodicScheduling_LP, pipeline_sample);
        VERBOSE_INFO("Random test");
        randomTest(algorithms::scheduling::CSDF_NPeriodicScheduling_LP);
    }

    BOOST_AUTO_TEST_CASE(OnePeriodicThroughput) {
        BOOST_REQUIRE(pipeline_sample);
        VERBOSE_INFO("Sample test");
        sampleTest(algorithms::scheduling::CSDF_1PeriodicThroughput, pipeline_sample);
        VERBOSE_INFO("Random test");
        randomTest(algorithms::scheduling::CSDF_1PeriodicThroughput);
    }

    BOOST_AUTO_TEST_CASE(NPeriodicThroughput) {
        BOOST_REQUIRE(pipeline_sample);
        VERBOSE_INFO("Sample test");
        sampleTest(algorithms::scheduling::CSDF_NPeriodicThroughput, pipeline_sample);
        VERBOSE_INFO("Random test");
        randomTest(algorithms::scheduling::CSDF_NPeriodicThroughput);
    }

    BOOST_AUTO_TEST_CASE(OnePeriodicScheduling) {
        BOOST_REQUIRE(pipeline_sample);
        VERBOSE_INFO("Sample test");
        sampleTest(algorithms::scheduling::OnePeriodicScheduling, pipeline_sample);
        VERBOSE_INFO("Random test");
        randomTest(algorithms::scheduling::OnePeriodicScheduling);
    }

    BOOST_AUTO_TEST_CASE(CSDF_Real1PeriodicScheduling_LP) {
        BOOST_REQUIRE(pipeline_sample);
        VERBOSE_INFO("Sample test");
        sampleTest(algorithms::scheduling::CSDF_Real1PeriodicScheduling_LP, pipeline_sample);
        VERBOSE_INFO("Random test");
        randomTest(algorithms::scheduling::CSDF_Real1PeriodicScheduling_LP);
    }

    BOOST_AUTO_TEST_CASE(SPeriodicScheduling) {
        BOOST_REQUIRE(pipeline_sample);
        VERBOSE_INFO("Sample test");
        sampleTest(algorithms::scheduling::SPeriodicScheduling, pipeline_sample);
        VERBOSE_INFO("Random test");
        randomTest(algorithms::scheduling::SPeriodicScheduling);
    }

    BOOST_AUTO_TEST_CASE(ASAPScheduling) {
        BOOST_REQUIRE(pipeline_sample);
        parameters_list_t params = parameters_list_t();

        VERBOSE_INFO("Sample test");
        algorithms::scheduling::ASAPScheduling(pipeline_sample, params);

        VERBOSE_INFO("Random test");
        std::vector<models::Dataflow *> graphs;

        VERBOSE_INFO("Generating Graphs");
        for (int i = 10; i <= 300; i += 50) {

            int buf_num = std::rand() % (i / 2) + i;

            VERBOSE_INFO("generate graph " << i << " with " << buf_num);

            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
            graphs.push_back(g);
        }

        for(auto graph : graphs) {
            algorithms::scheduling::ASAPScheduling(graph, params);
        }
    }


    BOOST_AUTO_TEST_CASE(So4Scheduling, * boost::unit_test::disabled()) {
        BOOST_REQUIRE(pipeline_sample);
        parameters_list_t params = parameters_list_t();

        VERBOSE_INFO("Sample test");
        //TODO: not sure why this segfaults
        BOOST_TEST(false);
//        algorithms::scheduling::So4Scheduling(pipeline_sample, params);
    }

    BOOST_AUTO_TEST_CASE(CSDF_KPeriodicScheduling) {
        BOOST_REQUIRE(pipeline_sample);
        algorithms::scheduling::CSDF_KPeriodicScheduling(pipeline_sample);
        randomTest(algorithms::scheduling::CSDF_KPeriodicScheduling);
    }

    BOOST_AUTO_TEST_CASE(generate1PeriodicVector) {
        BOOST_REQUIRE(pipeline_sample);
        algorithms::scheduling::generate1PeriodicVector(pipeline_sample);
        randomTest(algorithms::scheduling::generate1PeriodicVector);
    }

    BOOST_AUTO_TEST_CASE(generateKPeriodicVector) {
        BOOST_REQUIRE(pipeline_sample);
        algorithms::scheduling::generateKPeriodicVector(pipeline_sample, 1);
    }

    BOOST_AUTO_TEST_CASE(generateNPeriodicVector) {
        BOOST_REQUIRE(pipeline_sample);
        algorithms::scheduling::generateNPeriodicVector(pipeline_sample);
        randomTest(algorithms::scheduling::generateNPeriodicVector);
    }

    BOOST_AUTO_TEST_CASE(bufferless_scheduling) {
        BOOST_REQUIRE(pipeline_sample);
        Vertex a = pipeline_sample->getVertexById(1);
        Vertex b = pipeline_sample->getVertexById(2);
        Vertex c = pipeline_sample->getVertexById(3);
        periodicity_vector_t kvector;
        kvector[a] = 0;
        kvector[b] = 0;
        kvector[c] = 0;
        algorithms::scheduling::bufferless_scheduling(pipeline_sample, kvector);
    }

    BOOST_AUTO_TEST_CASE(CSDF_1PeriodicScheduling) {
        BOOST_REQUIRE(pipeline_sample);
        algorithms::scheduling::CSDF_1PeriodicScheduling(pipeline_sample);
        //randomTest(algorithms::scheduling::CSDF_1PeriodicScheduling);
    }

    BOOST_AUTO_TEST_CASE(CSDF_RealPeriodicScheduling_LP) {
        BOOST_REQUIRE(pipeline_sample);
        algorithms::scheduling::CSDF_RealPeriodicScheduling_LP(pipeline_sample);
        randomTest(algorithms::scheduling::CSDF_RealPeriodicScheduling_LP);
    }

    BOOST_AUTO_TEST_CASE(CSDF_SPeriodicScheduling) {
        BOOST_REQUIRE(pipeline_sample);
        algorithms::scheduling::CSDF_SPeriodicScheduling(pipeline_sample);
        //randomTest(algorithms::scheduling::CSDF_SPeriodicScheduling);
    }

    BOOST_AUTO_TEST_CASE(ModelASAPScheduling, * boost::unit_test::disabled()) {
        BOOST_REQUIRE(pipeline_sample);
        //TOOD unimplemented
//        algorithms::scheduling::ASAPScheduling(pipeline_sample);
    }

    BOOST_AUTO_TEST_CASE(ModelSo4Scheduling, * boost::unit_test::disabled()) {
        BOOST_REQUIRE(pipeline_sample);
        //TOOD unimplemented
//        algorithms::scheduling::So4Scheduling(pipeline_sample);
    }

BOOST_AUTO_TEST_SUITE_END()
