//
// Created by toky on 26/4/23.
//

#include "ThroughputBufferingDSE.h"
#include "algorithms/transformation/subgraph.h"
#include "algorithms/transformation/AddFeedbackBuffers.h"
#include <algorithms/dse/dse_utils.h>
#include <algorithms/dse/kperiodic.h>
#include <algorithms/dse/ModularDSE.h>

namespace algorithms {
    namespace dse {


        class ThroughputBufferingStopCondition {
        public:
            ThroughputBufferingStopCondition(TIME_UNIT target_th, bool lteq = true) : lteq(lteq), target_th(target_th) {}

            bool operator()(const algorithms::dse::TokenConfiguration& new_config, const algorithms::dse::TokenConfigurationSet& searched_configs) const {
                const algorithms::dse::TokenConfiguration* best = searched_configs.getBestPerformancePoint();

                if (best) {
                    if (lteq) {
                        return ((best->getCost() < new_config.getCost()) &&
                                (best->getPerformance().throughput >= target_th));
                    } else {

                        return ((best->getCost() < new_config.getCost()) &&
                                (best->getPerformance().throughput > target_th));
                    }
                }

                return false;
            }

        private:
            bool      lteq;
            TIME_UNIT target_th;
        };


        class ThroughputBufferingNext {
        public:
            ThroughputBufferingNext(ThroughputBufferingMode mode, TIME_UNIT max_th) : max_th(max_th), mode(mode)  {}

            std::vector<algorithms::dse::TokenConfiguration>  operator()(const algorithms::dse::TokenConfiguration& current) const {
                const models::Dataflow* df =  current.getDataflow();
                const auto& current_configuration = current.getConfiguration();
                std::vector<algorithms::dse::TokenConfiguration> next_configurations;

                if (mode == DSE_MODE) { // Normal mode, just increase each buffer.
                    for (algorithms::dse::TokenConfiguration::EdgeIdentifier edgeId : current.getPerformance().critical_edges) {
                        Edge c = df->getEdgeById(edgeId);
                        if (df->getEdgeType(c) != EDGE_TYPE::FEEDBACK_EDGE) continue; // Skip non feedback buffers.
                        auto new_configuration = current_configuration; // Make a copy of the current configuration
                        new_configuration[edgeId] = new_configuration[edgeId] + df->getFineGCD(c); // Increment the buffer size for the current edge by 1

                        // Create a new TokenConfiguration with the updated buffer size for the current edge
                        algorithms::dse::TokenConfiguration new_token_configuration(current.getDataflow(), new_configuration);
                        next_configurations.push_back(new_token_configuration);
                    }
                } else if (mode == KDSE_MODE or mode == KDSEA_MODE) { // KDSE mode, local search in the cycle to see what increments actually improve this cycle
                    const auto target_th = current.getPerformance().throughput;
                    VERBOSE_INFO("Target throughput: " <<  target_th);

                    // 1. build a minimal graph with the critical edges only.
                    models::Dataflow* cc_g = algorithms::build_subgraph(df, current.getPerformance().critical_edges);
                    std::map<ARRAY_INDEX, TOKEN_UNIT> new_configuration_cc;
                    ForEachEdge(cc_g, e) {
                        if (cc_g->getEdgeType(e) != FEEDBACK_EDGE) continue;
                        auto edgeId = cc_g->getEdgeId(e);
                        new_configuration_cc [edgeId] = current.getConfiguration().at(edgeId);
                    }
                    algorithms::dse::TokenConfiguration tc(cc_g, new_configuration_cc);

                    // 2. run a dse with target throughout to beat.
                    algorithms::dse::ModularDSE dse(cc_g,
                                                    throughputbuffering_performance_func,
                                                    throughputbuffering_initial_func,
                                                    ThroughputBufferingNext(DSE_MODE, 0.0),
                                                    (mode == KDSE_MODE) ? ThroughputBufferingStopCondition(target_th, false) : // KDSE, look at the surface of next config
                                                                                   ThroughputBufferingStopCondition(max_th, true), // KDSEA, generate a full pareto
                                                    1);
                    dse.add_initial_job(tc);
                    dse.explore(0, false, false);
                    auto cc_sds = dse.getResults();

                    // 3. any solutions that beat the target are considered.
                    for (auto solution : cc_sds) {
                        if (solution.getPerformance().throughput > target_th) {
                            auto new_configuration = current_configuration; // Make a copy of the current configuration
                            for (auto edgeId_tokenSize : solution.getConfiguration() ) {
                                new_configuration[edgeId_tokenSize.first] = edgeId_tokenSize.second ;
                            }

                            algorithms::dse::TokenConfiguration new_token_configuration(current.getDataflow(), new_configuration);
                            next_configurations.push_back(new_token_configuration);
                        }
                    }
                    VERBOSE_INFO("Solutions after deep mode: " << cc_sds.size());
                }

                VERBOSE_INFO("next configs: " << next_configurations.size());
                return next_configurations;
            }

        private:
            TIME_UNIT max_th;
           ThroughputBufferingMode mode;
        };



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

        algorithms::dse::TokenConfiguration::PerformanceResult throughputbuffering_performance_func(models::Dataflow* g, const algorithms::dse::TokenConfiguration& config) {

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
            ThroughputBufferingNext          throughputbuffering_next_distributions(mode, target_throughput);

            algorithms::dse::ModularDSE dse(dataflow_prime,
                                            throughputbuffering_performance_func,
                                            throughputbuffering_initial_func,
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

            ThroughputBufferingMode mode = DSE_MODE;
            if (params.count("mode")) {
                if (params.at("mode") == "KDSE")  mode = KDSE_MODE;
                if (params.at("mode") == "KDSEA")  mode = KDSEA_MODE;
            }

            TokenConfigurationSet result = solve_throughputbuffering   (dataflow, mode, realtime_output, thread_count, timeout, limit, filename, tc);

            delete tc;
        }
    } // end of dse namespace
} // end of algorithms namespace

