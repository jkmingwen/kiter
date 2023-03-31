//
// Created by jachym on 31/3/23.
//

#define BOOST_TEST_MODULE BackpressureTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "helpers/random_generator.h"
#include "algorithms/buffersizing/backpressure.h"
#include <algorithms/throughput/kperiodic.h>
#include <algorithms/buffersizing/periodic.h>


// This tests suite might be failing because of inappropriate graphs
// The functions require a non-zero period ->
    // the samples or the generated graphs might not qualify
// TODO: generate graphs with known periods
BOOST_FIXTURE_TEST_SUITE( backpressure_test , WITH_SAMPLE)

    TIME_UNIT get_period(models::Dataflow* const dataflow) {
        std::map<Vertex,EXEC_COUNT> kvector = algorithms::scheduling::generate1PeriodicVector(dataflow);

        kperiodic_result_t result = algorithms::KSchedule(dataflow,&kvector);

        TIME_UNIT MIN_PERIOD = 1 / result.throughput;
        return MIN_PERIOD;
    }

    BOOST_AUTO_TEST_CASE( sample_backpressure_test )
    {
        BOOST_REQUIRE(pipeline_sample);
        BOOST_REQUIRE(cycle_sample);
        BOOST_REQUIRE(mult_scc_sample);
        parameters_list_t params;


        VERBOSE_INFO("Backpressure: pipeline sample")
        params["PERIOD"] = get_period(pipeline_sample);
        algorithms::compute_backpressure_memory_sizing(pipeline_sample, params);

        VERBOSE_INFO("Backpressure: cycle sample")
        params["PERIOD"] = get_period(cycle_sample);
        algorithms::compute_backpressure_memory_sizing(cycle_sample, params);

        VERBOSE_INFO("Backpressure: mult scc sample")
        params["PERIOD"] = get_period(mult_scc_sample);
        algorithms::compute_backpressure_memory_sizing(mult_scc_sample, params);
    }

#define MAX_PHASE_COUNT 5

    BOOST_AUTO_TEST_CASE( random_backpressure_test )
    {
        std::vector<models::Dataflow *> graphs;
        parameters_list_t params;

        VERBOSE_INFO("Generating Graphs");
        for (int i = 10; i <= 300; i += 50) {

            int buf_num = std::rand() % (i / 2) + i;

            VERBOSE_INFO("generate graph " << i << " with " << buf_num);

            models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
            graphs.push_back(g);
        }

        VERBOSE_INFO("Running backpressure");
        for(auto graph : graphs) {
            params["PERIOD"] = get_period(graph);
            algorithms::compute_backpressure_memory_sizing(graph, params);
        }
    }
BOOST_AUTO_TEST_SUITE_END()
