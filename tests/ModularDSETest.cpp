//
// Created by toky on 24/4/23.
//

#define BOOST_TEST_MODULE ModularDSETest

#include "helpers/test_classes.h"
#include "helpers/random_generator.h"
#include "generators/NormalizedCycleGenerator.h"
#include "algorithms/dse/Constraint.h"
#include <algorithms/dse/kperiodic.h>
#include <algorithms/dse/ModularDSE.h>
#include <future>


using namespace algorithms::dse;

std::string remove_job_pool_from_export (std::string export_log) {
    // Stringstream to help with line-by-line reading
    std::istringstream stream(export_log);

    // Result string without lines containing dashes
    std::string result;
    std::string line;

    // Process each line in the string
    while (std::getline(stream, line)) {
        // Check if the line contains a dash
        if (line.find('-') == std::string::npos) {  // npos means not found
            // If there is no dash in the line, add it to the result
            // Ensure to maintain line breaks except for the last line
            if (!result.empty()) {
                result += "\n";
            }
            result += line;
        }
    }
    return result;
}

TokenConfiguration::PerformanceResult dummy_pc(models::Dataflow*, const TokenConfiguration& config) {
    return TokenConfiguration::PerformanceResult(config.getCost(), {});
}

std::vector<TokenConfiguration> dummy_if(const models::Dataflow* dataflow) {
    std::map<ARRAY_INDEX , TOKEN_UNIT> configuration;
    { ForEachEdge(dataflow,e) {
            ARRAY_INDEX tid = dataflow->getEdgeId(e);
            if (dataflow->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                configuration[tid] = 0;
            }
        }}
    return {algorithms::dse::TokenConfiguration(dataflow, configuration)};
}

ModularDSE::NextFuncRes dummy_nf(const TokenConfiguration& current ) {
    const models::Dataflow* df =  current.getDataflow();
    std::vector<algorithms::dse::TokenConfiguration> next_configurations;
    const auto& current_configuration = current.getConfiguration();
    for (auto edgeId_items : current.getConfiguration()) {
        Edge c = df->getEdgeById(edgeId_items.first);
        if (df->getEdgeType(c) != EDGE_TYPE::FEEDBACK_EDGE) continue; // Skip non feedback buffers.
        auto new_configuration = current_configuration;
        new_configuration[edgeId_items.first] = new_configuration[edgeId_items.first] + 1;
        algorithms::dse::TokenConfiguration new_token_configuration(current.getDataflow(), new_configuration);
        next_configurations.push_back(new_token_configuration);
    }

    return {next_configurations, Constraint()};
}

bool dummy_sc(const TokenConfiguration& , const TokenConfigurationSet& ) {
    return false;
}

static const std::string csv_file_content =
        "storage distribution size,throughput,channel quantities,feedback quantities,critical channels,critical feedback,duration,cumulative duration\n"
        "0,0,\"\",\"0,0,0,0\",\"\",\"\",-,-\n"
        "1,1,\"\",\"0,0,0,1\",\"\",\"\",-,-\n"
        "1,1,\"\",\"0,0,1,0\",\"\",\"\",-,-\n"
        "1,1,\"\",\"0,1,0,0\",\"\",\"\",-,-\n"
        "1,1,\"\",\"1,0,0,0\",\"\",\"\",-,-\n"
        "2,2,\"\",\"0,0,0,2\",\"\",\"\",-,-\n"
        "2,2,\"\",\"0,0,1,1\",\"\",\"\",-,-\n"
        "2,2,\"\",\"0,0,2,0\",\"\",\"\",-,-\n"
        "2,2,\"\",\"0,1,0,1\",\"\",\"\",-,-\n"
        "2,2,\"\",\"0,1,1,0\",\"\",\"\",-,-\n"
        "2,-,\"\",\"0,2,0,0\",-,-,-,-\n"
        "2,-,\"\",\"1,0,0,1\",-,-,-,-\n"
        "2,-,\"\",\"1,0,1,0\",-,-,-,-\n"
        "2,-,\"\",\"1,1,0,0\",-,-,-,-\n"
        "2,-,\"\",\"2,0,0,0\",-,-,-,-\n"
        "3,-,\"\",\"0,0,0,3\",-,-,-,-\n"
        "3,-,\"\",\"0,0,1,2\",-,-,-,-\n"
        "3,-,\"\",\"0,0,2,1\",-,-,-,-\n"
        "3,-,\"\",\"0,0,3,0\",-,-,-,-\n"
        "3,-,\"\",\"0,1,0,2\",-,-,-,-\n"
        "3,-,\"\",\"0,1,1,1\",-,-,-,-\n"
        "3,-,\"\",\"0,1,2,0\",-,-,-,-\n"
        "3,-,\"\",\"0,2,0,1\",-,-,-,-\n"
        "3,-,\"\",\"0,2,1,0\",-,-,-,-\n"
        "3,-,\"\",\"1,0,0,2\",-,-,-,-\n"
        "3,-,\"\",\"1,0,1,1\",-,-,-,-\n"
        "3,-,\"\",\"1,0,2,0\",-,-,-,-\n"
        "3,-,\"\",\"1,1,0,1\",-,-,-,-\n"
        "3,-,\"\",\"1,1,1,0\",-,-,-,-\n";


static const std::string csv_file_content_with_timings =
        "storage distribution size,throughput,channel quantities,feedback quantities,critical channels,critical feedback,duration,cumulative duration\n"
        "0,0,\"\",\"0,0,0,0\",\"\",\"\",0.001835,0.545401\n"
        "1,1,\"\",\"0,0,0,1\",\"\",\"\",0.000393,0.644924\n"
        "1,1,\"\",\"0,0,1,0\",\"\",\"\",0.000294,0.705254\n"
        "1,1,\"\",\"0,1,0,0\",\"\",\"\",0.00028,0.779911\n"
        "1,1,\"\",\"1,0,0,0\",\"\",\"\",0.000378,0.858076\n"
        "2,2,\"\",\"0,0,0,2\",\"\",\"\",0.00026,0.938773\n"
        "2,2,\"\",\"0,0,1,1\",\"\",\"\",0.000293,0.98937\n"
        "2,2,\"\",\"0,0,2,0\",\"\",\"\",0.000266,1.05367\n"
        "2,2,\"\",\"0,1,0,1\",\"\",\"\",0.000237,1.1284\n"
        "2,2,\"\",\"0,1,1,0\",\"\",\"\",0.000226,1.20212\n"
        "2,-,\"\",\"0,2,0,0\",-,-,-,-\n"
        "2,-,\"\",\"1,0,0,1\",-,-,-,-\n"
        "2,-,\"\",\"1,0,1,0\",-,-,-,-\n"
        "2,-,\"\",\"1,1,0,0\",-,-,-,-\n"
        "2,-,\"\",\"2,0,0,0\",-,-,-,-\n"
        "3,-,\"\",\"0,0,0,3\",-,-,-,-\n"
        "3,-,\"\",\"0,0,1,2\",-,-,-,-\n"
        "3,-,\"\",\"0,0,2,1\",-,-,-,-\n"
        "3,-,\"\",\"0,0,3,0\",-,-,-,-\n"
        "3,-,\"\",\"0,1,0,2\",-,-,-,-\n"
        "3,-,\"\",\"0,1,1,1\",-,-,-,-\n"
        "3,-,\"\",\"0,1,2,0\",-,-,-,-\n"
        "3,-,\"\",\"0,2,0,1\",-,-,-,-\n"
        "3,-,\"\",\"0,2,1,0\",-,-,-,-\n"
        "3,-,\"\",\"1,0,0,2\",-,-,-,-\n"
        "3,-,\"\",\"1,0,1,1\",-,-,-,-\n"
        "3,-,\"\",\"1,0,2,0\",-,-,-,-\n"
        "3,-,\"\",\"1,1,0,1\",-,-,-,-\n"
        "3,-,\"\",\"1,1,1,0\",-,-,-,-\n";

BOOST_FIXTURE_TEST_SUITE( modular_dse_test, WITH_SAMPLE)

    BOOST_AUTO_TEST_CASE(token_configuration_test) {

        size_t node_count = 4;
        std::vector<TOKEN_UNIT> node_weights  {2,2,2,2};
        std::vector<TIME_UNIT> node_durations {1,1,1,1};
        std::vector<TOKEN_UNIT> edge_preloads  {0,0,0,0};
        models::Dataflow* df = generators::new_normalized_cycle(node_count, node_weights, node_durations, edge_preloads);

        {ForEachEdge(df,c) {
                df->setEdgeType(c, EDGE_TYPE::FEEDBACK_EDGE);

        }}

        algorithms::dse::TokenConfiguration tc1 = dummy_if(df).front();
        algorithms::dse::TokenConfiguration tc2 = dummy_if(df).front();

        BOOST_REQUIRE(tc1.dominates(tc2));
        BOOST_REQUIRE(tc2.dominates(tc1));
    }
    BOOST_AUTO_TEST_CASE(modular_dse_test) {

        size_t node_count = 4;
        std::vector<TOKEN_UNIT> node_weights  {2,2,2,2};
        std::vector<TIME_UNIT> node_durations {1,1,1,1};
        std::vector<TOKEN_UNIT> edge_preloads  {0,0,0,0};
        models::Dataflow* df = generators::new_normalized_cycle(node_count, node_weights, node_durations, edge_preloads);

        {ForEachEdge(df,c) {
                df->setEdgeType(c, EDGE_TYPE::FEEDBACK_EDGE);
        }}
        computeRepetitionVector(df);

        algorithms::dse::ModularDSE dse(df,
                                        dummy_pc,
                                        dummy_nf,
                                        dummy_sc, 1);

        std::vector<algorithms::dse::TokenConfiguration> tc = dummy_if(df);
        dse.add_initial_jobs(tc);

        ExplorationParameters params = {
                .limit = 10,
                .bottom_up = false,
                .realtime_output = false,
                .return_pareto_only = false
        };

        dse.explore(params);

        // Test the results
        BOOST_REQUIRE_EQUAL(dse.results_size(), 10);
        BOOST_REQUIRE_EQUAL(dse.job_pool_size(), 19);
        std::cout << dse.print_space(false) << std::endl;
        std::string output = dse.print_space(true);
        BOOST_REQUIRE_EQUAL(output, csv_file_content);

    }

    BOOST_AUTO_TEST_CASE(modular_dse_import_test) {

        size_t node_count = 4;
        std::vector<TOKEN_UNIT> node_weights  {2,2,2,2};
        std::vector<TIME_UNIT> node_durations {1,1,1,1};
        std::vector<TOKEN_UNIT> edge_preloads  {0,0,0,0};
        models::Dataflow* df = generators::new_normalized_cycle(node_count, node_weights, node_durations, edge_preloads);

        {ForEachEdge(df,c) {
                df->setEdgeType(c, EDGE_TYPE::FEEDBACK_EDGE);
            }}

        computeRepetitionVector(df);

        algorithms::dse::ModularDSE dse(df,
                                        dummy_pc,
                                        dummy_nf,
                                        dummy_sc,
                                        1);


        std::istringstream input(csv_file_content_with_timings);
        dse.import_results(input);
        ExplorationParameters params = {
                .limit = 1,
                .bottom_up = false,
                .realtime_output = false,
                .return_pareto_only = false
        };

        dse.explore(params);

        // Test the results
        BOOST_REQUIRE_EQUAL(dse.results_size(), 11);
        BOOST_REQUIRE_EQUAL(dse.job_pool_size(), 20);

        std::cout << dse.print_space();
    }


    BOOST_AUTO_TEST_CASE(modular_dse_export_import_test) {

        // 1. Prepare the graph
        size_t node_count = 4;
        std::vector<TOKEN_UNIT> node_weights  {2,2,2,2};
        std::vector<TIME_UNIT> node_durations {1,1,1,1};
        std::vector<TOKEN_UNIT> edge_preloads  {0,0,0,0};
        models::Dataflow* df = generators::new_normalized_cycle(node_count, node_weights, node_durations, edge_preloads);

        {ForEachEdge(df,c) {
                df->setEdgeType(c, EDGE_TYPE::FEEDBACK_EDGE);
        }}

        computeRepetitionVector(df);

        // 2. Start an exploration

        algorithms::dse::ModularDSE dse(df,
                                        dummy_pc,
                                        dummy_nf,
                                        dummy_sc,
                                        1);

        auto tcs = dummy_if(df);

        BOOST_REQUIRE_EQUAL(dse.results_size(), 0);
        BOOST_REQUIRE_EQUAL(dse.job_pool_size(), 0);

        dse.add_initial_jobs(tcs);

        BOOST_REQUIRE_EQUAL(dse.results_size(), 0);
        BOOST_REQUIRE_EQUAL(dse.job_pool_size(), tcs.size());

        ExplorationParameters params = {
                .limit = 10,
                .bottom_up = false,
                .realtime_output = false,
                .return_pareto_only = false
        };

        dse.explore(params);


        auto export_log = dse.print_space();
        std::cout << export_log;

        // Test the results
        BOOST_REQUIRE_EQUAL(dse.results_size(), 10);
        BOOST_REQUIRE_EQUAL(dse.job_pool_size(), 19);


        algorithms::dse::ModularDSE dse_bis(df,
                                        dummy_pc,
                                        dummy_nf,
                                        dummy_sc,
                                        1);

        ExplorationParameters params_bis = {
                .limit = 1,
                .bottom_up = false,
                .realtime_output = false,
                .return_pareto_only = false
        };
        std::istringstream iss(export_log);
        dse_bis.import_results(iss);

        BOOST_REQUIRE_EQUAL(dse_bis.results_size(), 10);
        BOOST_REQUIRE_EQUAL(dse_bis.job_pool_size(), 19);
        dse_bis.explore(params_bis);

        // Test the results
        BOOST_REQUIRE_EQUAL(dse_bis.results_size(), 11);
        BOOST_REQUIRE_EQUAL(dse_bis.job_pool_size(), 20);

    }

    BOOST_AUTO_TEST_CASE(modular_dse_export_import_without_job_pool_test) {

        // 1. Prepare the graph
        size_t node_count = 4;
        std::vector<TOKEN_UNIT> node_weights  {2,2,2,2};
        std::vector<TIME_UNIT> node_durations {1,1,1,1};
        std::vector<TOKEN_UNIT> edge_preloads  {0,0,0,0};
        models::Dataflow* df = generators::new_normalized_cycle(node_count, node_weights, node_durations, edge_preloads);

        {ForEachEdge(df,c) {
                df->setEdgeType(c, EDGE_TYPE::FEEDBACK_EDGE);
            }}

        computeRepetitionVector(df);

        // 2. Start an exploration

        algorithms::dse::ModularDSE dse(df,
                                        dummy_pc,
                                        dummy_nf,
                                        dummy_sc,
                                        1);

        auto tcs = dummy_if(df);

        BOOST_REQUIRE_EQUAL(dse.results_size(), 0);
        BOOST_REQUIRE_EQUAL(dse.job_pool_size(), 0);

        dse.add_initial_jobs(tcs);

        BOOST_REQUIRE_EQUAL(dse.results_size(), 0);
        BOOST_REQUIRE_EQUAL(dse.job_pool_size(), tcs.size());

        ExplorationParameters params = {
                .limit = 10,
                .bottom_up = false,
                .realtime_output = false,
                .return_pareto_only = false
        };

        dse.explore(params);


        auto export_log = dse.print_space();
        std::cout << export_log;
        auto filtered_log = remove_job_pool_from_export(export_log);
        std::cout << filtered_log;

        // Test the results
        BOOST_REQUIRE_EQUAL(dse.results_size(), 10);
        BOOST_REQUIRE_EQUAL(dse.job_pool_size(), 19);


        algorithms::dse::ModularDSE dse_bis(df,
                                            dummy_pc,
                                            dummy_nf,
                                            dummy_sc,
                                            1);

        ExplorationParameters params_bis = {
                .limit = 1,
                .bottom_up = false,
                .realtime_output = false,
                .return_pareto_only = false
        };
        std::istringstream iss(filtered_log);
        dse_bis.import_results(iss);

        BOOST_REQUIRE_EQUAL(dse_bis.results_size(), 10);
        BOOST_REQUIRE_EQUAL(dse_bis.job_pool_size(), 0);
        dse_bis.explore(params_bis);

        // Test the results
        BOOST_REQUIRE_EQUAL(dse_bis.results_size(), 10);
        BOOST_REQUIRE_EQUAL(dse_bis.job_pool_size(), 0);
        dse_bis.add_initial_job( dummy_if(df).front());

        // Test the results
        BOOST_REQUIRE_EQUAL(dse_bis.results_size(), 10);
        BOOST_REQUIRE_EQUAL(dse_bis.job_pool_size(), 1);
        BOOST_REQUIRE_EQUAL(dse_bis.in_progress_size(), 0);
        dse_bis.explore(params_bis);
        std::cout << dse_bis.print_unfinished();

        BOOST_REQUIRE_EQUAL(dse_bis.results_size(), 10);
        BOOST_REQUIRE_EQUAL(dse_bis.job_pool_size(), 0);
        BOOST_REQUIRE_EQUAL(dse_bis.in_progress_size(), 0);




    }


BOOST_AUTO_TEST_SUITE_END()
