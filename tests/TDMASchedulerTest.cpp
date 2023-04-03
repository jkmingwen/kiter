//
// Created by jachym on 3/4/23.
//

#define BOOST_TEST_MODULE TDMASchedulerTest
#include "helpers/test_classes.h"
#include "helpers/random_generator.h"
#include "algorithms/scheduling/TDMAScheduler.h"

BOOST_FIXTURE_TEST_SUITE( schedulings_test, WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE( sample_tdmascheduler_test )
    {
        parameters_list_t params;
        BOOST_REQUIRE(pipeline_sample);
        params["slotfile"] = "slot_4x4";
        params["pktfile"] = "modem.txt";
        params["mapfile"] = "modem_task";

        algorithms::scheduling::TDMASchedule(pipeline_sample, params);
    }

#define MAX_PHASE_COUNT 1

    BOOST_AUTO_TEST_CASE( random_tdmascheduler_test )
    {
        std::vector<models::Dataflow *> graphs;
        parameters_list_t params;
        params["slotfile"] = "slot_4x4";
        params["pktfile"] = "modem.txt";
        params["mapfile"] = "modem_task";

        VERBOSE_INFO("Generating Graphs");
        for (int i = 10; i <= 300; i += 50) {

            int buf_num = std::rand() % (i / 2) + i;

            VERBOSE_INFO("generate graph " << i << " with " << buf_num);

            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
            graphs.push_back(g);
        }

        VERBOSE_INFO("TDMA schedule on random");
        for(auto graph : graphs) {
            algorithms::scheduling::TDMASchedule(graph, params);
        }
    }
BOOST_AUTO_TEST_SUITE_END()
