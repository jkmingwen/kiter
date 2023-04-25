//
// Created by bruno on 24/4/23.
//
#define BOOST_TEST_MODULE NormalizedCycleGeneratorTest
#include "helpers/test_classes.h"
#include "helpers/random_generator.h"
#include "generators/NormalizedCycleGenerator.h"
#include <printers/printers.h>

BOOST_FIXTURE_TEST_SUITE( normalized_cycle_generator_test, WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE( new_normalized_cycle_simple_test )
    {
        // Setup part

        size_t node_count = 4;
        std::vector<TOKEN_UNIT> node_weights {2,2,2,2};
        std::vector<TIME_UNIT>  node_durations {1,1,1,1};
        std::vector<TOKEN_UNIT> edge_preloads {1,0,0,0};
        models::Dataflow* df = generators::new_normalized_cycle(node_count, node_weights, node_durations, edge_preloads);

        // Testing part

        BOOST_REQUIRE_EQUAL(4, df->getVerticesCount());
        BOOST_REQUIRE_EQUAL(4, df->getEdgesCount());

        TOKEN_UNIT sum1 = 0, sum2 = 0;
        {int i = 0; ForEachEdge(df,e){
                BOOST_REQUIRE_EQUAL(df->getEdgeIn(e), node_weights[i]);
                sum1 += df->getPreload(e);
                sum2 += edge_preloads[i++];
        }}
        BOOST_REQUIRE_EQUAL(sum1,sum2);
    }

    BOOST_AUTO_TEST_CASE( new_normalized_cycle_empty_test )
    {
        size_t node_count = 0;
        std::vector<TOKEN_UNIT> node_weights {};
        std::vector<TIME_UNIT> node_durations {};
        std::vector<TOKEN_UNIT> edge_preloads {};
        models::Dataflow* df = generators::new_normalized_cycle(node_count, node_weights, node_durations, edge_preloads);
        BOOST_REQUIRE_EQUAL(0, df->getVerticesCount());
        BOOST_REQUIRE_EQUAL(0, df->getEdgesCount());
    }
    BOOST_AUTO_TEST_CASE( simple_normalized_cycle_generator_test )
    {

        commons::set_verbose_mode(commons::DEBUG_LEVEL);

        size_t node_count = 4;
        std::vector<TOKEN_UNIT> node_weights {2,2,2,2};
        std::vector<TIME_UNIT> node_durations {1,1,1,1};
        std::vector<TOKEN_UNIT> edge_preloads {1,0,0,0};
        models::Dataflow* df1 = generators::new_normalized_cycle(node_count, node_weights, node_durations, edge_preloads);

        parameters_list_t params;
        params["size"] = commons::toString(node_count);
        params["weights"] = commons::join(node_weights, ",");
        params["durations"] = commons::join(node_durations, ",");
        params["preloads"] = commons::join(edge_preloads, ",");

        VERBOSE_INFO("params are " << commons::toString(params));
        models::Dataflow* df2 = generators::generate_normalized_cycle(params);

        // TODO check df1 == df2

        std::string df1Str = printers::generateSDF3XML(df1);
        std::string df2Str = printers::generateSDF3XML(df2);

        BOOST_REQUIRE_EQUAL(df1Str, df2Str);
    }

BOOST_AUTO_TEST_SUITE_END()
