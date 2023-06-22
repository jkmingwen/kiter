//
// Created by toky on 26/4/23.
//

#include "ThroughputBufferingDSE.h"
#include "algorithms/transformation/subgraph.h"
#include "algorithms/transformation/AddFeedbackBuffers.h"
#include "algorithms/throughput/symbolic_execution.h"
#include "ThroughputConstraint.h"
#include <algorithms/dse/kperiodic.h>
#include <algorithms/dse/ModularDSE.h>
#include <printers/stdout.h>

namespace algorithms {
    namespace dse {



        bool ThroughputBufferingStopCondition::operator()(const algorithms::dse::TokenConfiguration& new_config, const algorithms::dse::TokenConfigurationSet& searched_configs) const {
                const algorithms::dse::TokenConfiguration * best = searched_configs.getBestPerformancePoint();

                if (best) {
                    if (surface_only) {
                        return ((new_config.dominates(*best)) &&
                                (best->getPerformance().throughput > target_th));
                    } else {
                        return ((best->getCost() < new_config.getCost()) &&
                                (best->getPerformance().throughput >= target_th));
                    }
                }

                return false;
            }

            algorithms::dse::ModularDSE::NextFuncRes ThroughputBufferingNext::operator()(const algorithms::dse::TokenConfiguration& current) const {
                const models::Dataflow* df =  current.getDataflow();
                const auto& current_configuration = current.getConfiguration();
                std::vector<algorithms::dse::TokenConfiguration> next_configurations;

                if (mode == KDSE_MODE) { // Normal mode, just increase each buffer.
                    VERBOSE_DEBUG("ThroughputBufferingNext Mode: KDSE current:" << current.to_csv_line());
                    for (algorithms::dse::TokenConfiguration::EdgeIdentifier edgeId : current.getPerformance().critical_edges) {
                        Edge c = df->getEdgeById(edgeId);
                        if (df->getEdgeType(c) != EDGE_TYPE::FEEDBACK_EDGE) continue; // Skip non feedback buffers.
                        auto new_configuration = current_configuration; // Make a copy of the current configuration
                        new_configuration[edgeId] = new_configuration[edgeId] + df->getFineGCD(c); // Increment the buffer size for the current edge by 1

                        // Create a new TokenConfiguration with the updated buffer size for the current edge
                        algorithms::dse::TokenConfiguration new_token_configuration(current.getDataflow(), new_configuration);
                        next_configurations.push_back(new_token_configuration);
                    }
                } else if (mode == K2DSE_MODE or mode == K2DSEA_MODE) { // KDSE mode, local search in the cycle to see what increments actually improve this cycle
                    const auto target_th = current.getPerformance().throughput;
                    VERBOSE_DEBUG("ThroughputBufferingNext Mode: K2DSE(A) Target throughput: " <<  target_th << " current:" << current.to_csv_line());

                    // 1. build a minimal graph with the critical edges only.
                    models::Dataflow* cc_g = algorithms::build_subgraph(df, current.getPerformance().critical_edges);
                    std::map<ARRAY_INDEX, TOKEN_UNIT> new_configuration_cc;
                    ForEachEdge(cc_g, e) {
                        if (cc_g->getEdgeType(e) != FEEDBACK_EDGE) continue;
                        auto edgeId = cc_g->getEdgeId(e);
                        new_configuration_cc [edgeId] = current.getConfiguration().at(edgeId);
                    }
                    algorithms::dse::TokenConfiguration tc(cc_g, new_configuration_cc);
                    VERBOSE_DEBUG("Init: " <<  tc.to_csv_line());

                    // 2. run a dse with target throughout to beat.
                    algorithms::dse::ModularDSE dse(cc_g,
                                                    kperiodic_performance_func,
                                                    ThroughputBufferingNext(KDSE_MODE, 0.0),
                                                    ThroughputBufferingStopCondition(target_th, true),
                                                    1);
                    dse.add_initial_job(tc);
                    dse.explore(0, false, false);
                    auto cc_sds = dse.getResults();

                    if (mode == K2DSEA_MODE) {
                        // TODO: cleanup to pareto
                    }

                    VERBOSE_DEBUG("End of local search:");

                    // 3. any solutions that beat the target are considered.
                    for (auto solution : cc_sds) {
                        if (solution.getPerformance().throughput > target_th) {
                            VERBOSE_DEBUG("    - KEEP " << solution.to_csv_line());
                            auto new_configuration = current_configuration; // Make a copy of the current configuration
                            for (auto edgeId_tokenSize : solution.getConfiguration() ) {
                                new_configuration[edgeId_tokenSize.first] = edgeId_tokenSize.second ;
                            }

                            algorithms::dse::TokenConfiguration new_token_configuration(current.getDataflow(), new_configuration);
                            next_configurations.push_back(new_token_configuration);
                        } else {
                            VERBOSE_DEBUG("    - SKIP " << solution.to_csv_line());
                        }
                    }
                }

                VERBOSE_DEBUG("next configs: " << next_configurations.size());
                for (auto solution : next_configurations) {
                    VERBOSE_DEBUG("     " << solution.to_csv_line());
                }

                return {next_configurations, new ThroughputConstraint()};
            }




        TOKEN_UNIT computeMinimalFeedbackSize(const models::Dataflow *dataflow, const Edge c) {

                    // initialise channel size to maximum int size
                    TOKEN_UNIT feedback_size = INT_MAX; // NOTE (should use ULONG_MAX but it's a really large value)
                    TOKEN_UNIT ratePeriod = (TOKEN_UNIT) std::lcm(dataflow->getEdgeInPhasesCount(c),
                                                                  dataflow->getEdgeOutPhasesCount(c));

                    TOKEN_UNIT tokensInitial = dataflow->getPreload(c);
                    for (TOKEN_UNIT i = 0; i < ratePeriod; i++) {
                        // might want to change variables to p, c, and t for legibility
                        TOKEN_UNIT tokensProduced = dataflow->getEdgeInVector(c)[i % dataflow->getEdgeInPhasesCount(c)];
                        TOKEN_UNIT tokensConsumed = dataflow->getEdgeOutVector(c)[i % dataflow->getEdgeOutPhasesCount(c)];

                        TOKEN_UNIT lowerBound;

                        if (std::gcd(tokensProduced, tokensConsumed)) {
                            lowerBound = tokensProduced + tokensConsumed -
                                         std::gcd(tokensProduced, tokensConsumed) +
                                         tokensInitial % std::gcd(tokensProduced, tokensConsumed);
                        } else {
                            lowerBound = tokensProduced + tokensConsumed -
                                         std::gcd(tokensProduced, tokensConsumed);
                        }
                        lowerBound = (lowerBound > tokensInitial ? lowerBound : tokensInitial);

                        // take the lowest bound amongst phases of prod/cons
                        if (lowerBound < feedback_size) {
                            feedback_size = lowerBound;
                        }
                    }
            return feedback_size - commons::floor(dataflow->getPreload(c), dataflow->getFineGCD(c));
        }
        algorithms::dse::TokenConfiguration::PerformanceResult asap_performance_func(models::Dataflow* g, const algorithms::dse::TokenConfiguration& config) {

            {ForEachEdge(g,e) {
                    if (g->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                        TOKEN_UNIT v = config.getConfiguration().at(g->getEdgeId(e));
                        g->setPreload(e,  v );
                    }
                }}

            //VERBOSE_INFO("compute_Kperiodic_throughput_and_cycles");
            std::pair<ARRAY_INDEX, EXEC_COUNT> actorInfo;
            bool useCorrectedStorDepDetection = false;

            std::map<Edge, BufferInfos> channelQuants;
            TOKEN_UNIT distSz = 0;
            {ForEachEdge(g, e) {
                    channelQuants[e].buffer_size = g->getPreload(e); // assume initial tokesn specified by dataflow graph
                    channelQuants[e].preload = g->getPreload(e);
                    distSz += g->getPreload(e);
                }}
            StorageDistribution testSD(g,
                                       0,
                                       channelQuants);

            kperiodic_result_t result_max = computeComponentThroughputCycles(g, actorInfo, testSD, useCorrectedStorDepDetection);
            //VERBOSE_INFO("Done");
            algorithms::dse::TokenConfiguration::PerformanceResult res (0, {});
            res.throughput = result_max.throughput;
            for (Edge e : result_max.critical_edges) res.critical_edges.insert(g->getEdgeId(e));
            return res;
        }

        algorithms::dse::TokenConfiguration::PerformanceResult kperiodic_performance_func(models::Dataflow* g, const algorithms::dse::TokenConfiguration& config) {

            {ForEachEdge(g,e) {
                    if (g->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                        TOKEN_UNIT v = config.getConfiguration().at(g->getEdgeId(e));
                        g->setPreload(e,  v );
                    }
                }}

            //VERBOSE_INFO("compute_Kperiodic_throughput_and_cycles");
            kperiodic_result_t result_max = algorithms::compute_Kperiodic_throughput_and_cycles(g);
            //VERBOSE_INFO("Done");
            algorithms::dse::TokenConfiguration::PerformanceResult res (0, {});
            res.throughput = result_max.throughput;
            for (Edge e : result_max.critical_edges) res.critical_edges.insert(g->getEdgeId(e));
            return res;
        }

        std::vector<algorithms::dse::TokenConfiguration> throughputbuffering_initial_func(const models::Dataflow* dataflow) {

            std::map<ARRAY_INDEX , TOKEN_UNIT> configuration; // Replace Edge with the correct type for your implementation


            // FIXME: This matching section is ugly, but it's only once I'll fix it later .. (maybe)
            { ForEachEdge(dataflow,e1) {

                    if (dataflow->getEdgeType(e1) == EDGE_TYPE::NORMAL_EDGE) {
                        Edge e2 = dataflow->getEdgeByName(dataflow->getEdgeName(e1) + "_prime"); // Must be found ... if not better to crash !

                        VERBOSE_ASSERT(dataflow->getEdgeType(e2) == EDGE_TYPE::FEEDBACK_EDGE, "The prime edge must be a feedback edge.");

                        VERBOSE_ASSERT((dataflow->getEdgeSource(e1) == dataflow->getEdgeTarget(e2)) and
                                       (dataflow->getEdgeSource(e2) == dataflow->getEdgeTarget(e1) ), "The prime edge must be return way.");

                        ARRAY_INDEX tid = dataflow->getEdgeId(e2);
                        configuration[tid] = computeMinimalFeedbackSize(dataflow, e1);
                    }



            }}


            { ForEachEdge(dataflow,e) {
                    ARRAY_INDEX tid = dataflow->getEdgeId(e);
                    if (dataflow->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                        VERBOSE_ASSERT(configuration.count(tid) > 0, "All feedback edge must be set.");
                    }
            }}

            return {algorithms::dse::TokenConfiguration(dataflow, configuration)};
        }


        TokenConfigurationSet solve_throughputbuffering   (models::Dataflow* const  dataflow,
                                                           ThroughputBufferingMode mode,
                                                           bool realtime_output,
                                                           size_t thread_count,
                                                           size_t timeout,
                                                           size_t limit,
                                                           std::string  filename,
                                                           algorithms::dse::TokenConfiguration* tc
        ) {


            // Ensure dataflow has no feedbackbuffers.
            // Create feedbackbuffers with correct initial configuration (taking into account useful preload).

            // Compute the target throughput
            TIME_UNIT target_throughput = algorithms::compute_Kperiodic_throughput_and_cycles(dataflow).throughput;
            models::Dataflow* dataflow_prime = new models::Dataflow(*dataflow);

            algorithms::transformation::generateInplaceFeedbackBuffers(dataflow_prime);
            dataflow_prime->precomputeFineGCD();
            ThroughputBufferingStopCondition throughputbuffering_stop_condition(target_throughput);
            ThroughputBufferingNext          throughputbuffering_next_distributions((mode == ASAP_DSE_MODE)? KDSE_MODE : mode , target_throughput);

            algorithms::dse::ModularDSE dse(dataflow_prime,
                                            (mode == ASAP_DSE_MODE)? asap_performance_func : kperiodic_performance_func ,
                                            throughputbuffering_next_distributions,
                                            throughputbuffering_stop_condition,
                                            thread_count);

            if(filename != "") {
                VERBOSE_INFO("Load previous search points from " << filename);
                dse.import_results(filename);
            } else if (tc) {
                VERBOSE_INFO("Initial state of the search is forced to " << *tc);
                dse.add_initial_job(*tc);
            } else {
                dse.add_initial_jobs(throughputbuffering_initial_func(dataflow_prime));
            }


            // Run the DSE exploration for a short period of time (e.g., 100 milliseconds)
            std::future<void> exploration_future = std::async(std::launch::async, [&dse,limit,realtime_output] { dse.explore(limit, false, realtime_output); });

            if (timeout > 0) {
                std::this_thread::sleep_for(std::chrono::seconds (timeout));
                dse.stop();
            }

            exploration_future.wait();

            return dse.getResults();

        }


        void throughputbuffering_dse   (models::Dataflow* const  dataflow, parameters_list_t params) {
            size_t realtime_output = (params.count("realtime") > 0) ? commons::fromString<bool>(params.at("realtime")) : false;
            size_t thread_count    = (params.count("thread") > 0) ? commons::fromString<size_t>(params.at("thread")) : 1;
            size_t timeout         = (params.count("timeout") > 0) ? commons::fromString<size_t>(params.at("timeout")) : 0;
            size_t limit           = (params.count("limit") > 0) ? commons::fromString<size_t>(params.at("limit")) : 0;
            std::string  filename  = (params.count("import") > 0) ? params.at("import") : "";
            algorithms::dse::TokenConfiguration* tc = (params.count("init") > 0) ? new algorithms::dse::TokenConfiguration(dataflow, commons::split<TOKEN_UNIT>(params.at("init"), ',')) : nullptr;

            VERBOSE_ASSERT(params.count("init")  + params.count("import")  != 2, "Please pass your init inside the import file");

            ThroughputBufferingMode mode = KDSE_MODE; // Default mode
            if (params.count("mode")) {
                if (params.at("mode") == "ASAP")  mode = ASAP_DSE_MODE;
                if (params.at("mode") == "KDSE")  mode = KDSE_MODE;
                if (params.at("mode") == "K2DSE")  mode = K2DSE_MODE;
                if (params.at("mode") == "K2DSEA")  mode = K2DSEA_MODE;
            }

            TokenConfigurationSet result = solve_throughputbuffering   (dataflow, mode, realtime_output, thread_count, timeout, limit, filename, tc);

            delete tc;
        }
    } // end of dse namespace
} // end of algorithms namespace

