//
// Created by jachym on 2/4/23.
//
#define BOOST_TEST_MODULE MappingsTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "helpers/random_generator.h"
#include "algorithms/mappings.h"


BOOST_FIXTURE_TEST_SUITE( mapping_test , WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE( sample_createNoC_test )
    {
        BOOST_REQUIRE(pipeline_sample);
        BOOST_REQUIRE(cycle_sample);

        parameters_list_t params= parameters_list_t();

        algorithms::mapping::createNoC(pipeline_sample, params);
        algorithms::mapping::createNoC(cycle_sample, params);
    }

    BOOST_AUTO_TEST_CASE( sample_random_mapping_test )
    {
        BOOST_REQUIRE(pipeline_sample);
        BOOST_REQUIRE(cycle_sample);

        parameters_list_t params= parameters_list_t();

        algorithms::mapping::randomMapping(pipeline_sample, params);
        algorithms::mapping::randomMapping(cycle_sample, params);
    }

    BOOST_AUTO_TEST_CASE( sample_modulo_mapping_test )
    {
        BOOST_REQUIRE(pipeline_sample);
        BOOST_REQUIRE(cycle_sample);

        parameters_list_t params= parameters_list_t();

        algorithms::mapping::moduloMapping(pipeline_sample, params);
        algorithms::mapping::moduloMapping(cycle_sample, params);
    }

    BOOST_AUTO_TEST_CASE( sample_bufferless_noCmap_and_route_test )
    {
        BOOST_REQUIRE(pipeline_sample);
        BOOST_REQUIRE(cycle_sample);

        parameters_list_t params = parameters_list_t();

        //TODO: infinite loop
        BOOST_TEST(false);
//        algorithms::mapping::BufferlessNoCMapAndRoute(pipeline_sample, params);
//        algorithms::mapping::BufferlessNoCMapAndRoute(cycle_sample, params);
    }

    BOOST_AUTO_TEST_CASE( sample_xy_routing_test )
    {
        BOOST_REQUIRE(pipeline_sample);
        BOOST_REQUIRE(cycle_sample);

        parameters_list_t params = parameters_list_t();

        algorithms::mapping::xyRouting(pipeline_sample, params);
        algorithms::mapping::xyRouting(cycle_sample, params);
    }

    BOOST_AUTO_TEST_CASE( sample_random_routing_test )
    {
        BOOST_REQUIRE(pipeline_sample);
        BOOST_REQUIRE(cycle_sample);

        parameters_list_t params = parameters_list_t();

        algorithms::mapping::randomRouting(pipeline_sample, params);
        algorithms::mapping::randomRouting(cycle_sample, params);
    }

    BOOST_AUTO_TEST_CASE( sample_generate_tdma_test )
    {
        BOOST_REQUIRE(pipeline_sample);
        BOOST_REQUIRE(cycle_sample);

        parameters_list_t params = parameters_list_t();

        algorithms::mapping::generateTDMA(pipeline_sample, params);
        algorithms::mapping::generateTDMA(cycle_sample, params);
    }

//    BOOST_AUTO_TEST_CASE( sample_model_noc_conflict_free_communication_test )
//    {
//        BOOST_REQUIRE(pipeline_sample);
//        BOOST_REQUIRE(cycle_sample);
//
//        parameters_list_t params = parameters_list_t();
//
//        algorithms::ModelNoCConflictFreeCommunication(pipeline_sample, params);
//        algorithms::ModelNoCConflictFreeCommunication(cycle_sample, params);
//    }

    BOOST_AUTO_TEST_CASE( sample_find_conflict_test )
    {
        BOOST_REQUIRE(pipeline_sample);
        BOOST_REQUIRE(cycle_sample);

        parameters_list_t params = parameters_list_t();

        BOOST_TEST(false);
        //TODO: Function declared but unimplemented
//        algorithms::FindConflicts(pipeline_sample, params);
//        algorithms::FindConflicts(cycle_sample, params);
    }
#define MAX_PHASE_COUNT 1

    BOOST_AUTO_TEST_CASE( random_createNoC_test )
    {
        std::vector<models::Dataflow *> graphs;
        parameters_list_t params = parameters_list_t();

        VERBOSE_INFO("Generating Graphs");
        for (int i = 10; i <= 300; i += 50) {

            int buf_num = std::rand() % (i / 2) + i;

            VERBOSE_INFO("generate graph " << i << " with " << buf_num);

            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
            graphs.push_back(g);
        }

        VERBOSE_INFO("Create NoC on random");
        for(auto graph : graphs) {
            algorithms::mapping::createNoC(graph, params);
        }
    }

    BOOST_AUTO_TEST_CASE( random_random_mapping_test )
    {
        std::vector<models::Dataflow *> graphs;
        parameters_list_t params= parameters_list_t();

        VERBOSE_INFO("Generating Graphs");
        for (int i = 10; i <= 300; i += 50) {

            int buf_num = std::rand() % (i / 2) + i;

            VERBOSE_INFO("generate graph " << i << " with " << buf_num);

            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
            graphs.push_back(g);
        }

        VERBOSE_INFO("Random mapping on random");
        for(auto graph : graphs) {
            algorithms::mapping::randomMapping(graph, params);
        }
    }

    BOOST_AUTO_TEST_CASE( random_modulo_mapping_test )
    {
        std::vector<models::Dataflow *> graphs;
        parameters_list_t params= parameters_list_t();

        VERBOSE_INFO("Generating Graphs");
        for (int i = 10; i <= 300; i += 50) {

            int buf_num = std::rand() % (i / 2) + i;

            VERBOSE_INFO("generate graph " << i << " with " << buf_num);

            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
            graphs.push_back(g);
        }

        VERBOSE_INFO("modulo mapping on random");
        for(auto graph : graphs) {
            algorithms::mapping::moduloMapping(graph, params);
        }
    }

    BOOST_AUTO_TEST_CASE( random_bufferless_noCmap_and_route_test )
    {
        std::vector<models::Dataflow *> graphs;
        parameters_list_t params= parameters_list_t();

        //TODO: infinite loop
        BOOST_TEST(false);

//        VERBOSE_INFO("Generating Graphs");
//        for (int i = 10; i <= 300; i += 50) {
//
//            int buf_num = std::rand() % (i / 2) + i;
//
//            VERBOSE_INFO("generate graph " << i << " with " << buf_num);
//
//            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
//            graphs.push_back(g);
//        }
//
//        VERBOSE_INFO("random bufferless no cmap and route on random");
//        for(auto graph : graphs) {
//            algorithms::mapping::BufferlessNoCMapAndRoute(graph, params);
//        }
    }

    BOOST_AUTO_TEST_CASE( random_xy_routing_test )
    {
        std::vector<models::Dataflow *> graphs;
        parameters_list_t params= parameters_list_t();

        VERBOSE_INFO("Generating Graphs");
        for (int i = 10; i <= 300; i += 50) {

            int buf_num = std::rand() % (i / 2) + i;

            VERBOSE_INFO("generate graph " << i << " with " << buf_num);

            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
            graphs.push_back(g);
        }

        VERBOSE_INFO("xy routing on random");
        for(auto graph : graphs) {
            algorithms::mapping::xyRouting(graph, params);
        }
    }

    BOOST_AUTO_TEST_CASE( random_random_routing_test )
    {
        std::vector<models::Dataflow *> graphs;
        parameters_list_t params= parameters_list_t();

        VERBOSE_INFO("Generating Graphs");
        for (int i = 10; i <= 300; i += 50) {

            int buf_num = std::rand() % (i / 2) + i;

            VERBOSE_INFO("generate graph " << i << " with " << buf_num);

            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
            graphs.push_back(g);
        }

        VERBOSE_INFO("random routing on random");
        for(auto graph : graphs) {
            algorithms::mapping::randomRouting(graph, params);
        }
    }

    BOOST_AUTO_TEST_CASE( random_generate_tdma_test )
    {
        std::vector<models::Dataflow *> graphs;
        parameters_list_t params= parameters_list_t();

        VERBOSE_INFO("Generating Graphs");
        for (int i = 10; i <= 300; i += 50) {

            int buf_num = std::rand() % (i / 2) + i;

            VERBOSE_INFO("generate graph " << i << " with " << buf_num);

            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
            graphs.push_back(g);
        }

        VERBOSE_INFO("generate tdma on random");
        for(auto graph : graphs) {
            algorithms::mapping::generateTDMA(graph, params);
        }
    }

//    BOOST_AUTO_TEST_CASE( random_model_noc_conflict_free_communication_test )
//    {
//        std::vector<models::Dataflow *> graphs;
//        parameters_list_t params= parameters_list_t();
//
//        VERBOSE_INFO("Generating Graphs");
//        for (int i = 10; i <= 300; i += 50) {
//
//            int buf_num = std::rand() % (i / 2) + i;
//
//            VERBOSE_INFO("generate graph " << i << " with " << buf_num);
//
//            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
//            graphs.push_back(g);
//        }
//
//        VERBOSE_INFO("model_noc_conflict_free_communication on random");
//        for(auto graph : graphs) {
//            algorithms::ModelNoCConflictFreeCommunication(graph, params);
//        }
//    }

    BOOST_AUTO_TEST_CASE( random_find_conflict_test )
    {
        BOOST_TEST(false);
        std::vector<models::Dataflow *> graphs;
        parameters_list_t params= parameters_list_t();

        VERBOSE_INFO("Generating Graphs");
        for (int i = 10; i <= 300; i += 50) {

            int buf_num = std::rand() % (i / 2) + i;

            VERBOSE_INFO("generate graph " << i << " with " << buf_num);

            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
            graphs.push_back(g);
        }

//        VERBOSE_INFO("find conflicts on random");
//        for(auto graph : graphs) {
            //TODO: Function declared but unimplemented
//            algorithms::FindConflicts(graph, params);
//        }
    }
BOOST_AUTO_TEST_SUITE_END()
