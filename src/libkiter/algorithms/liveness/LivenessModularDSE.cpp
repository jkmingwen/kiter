//
// Created by toky on 24/4/23.
//

#include <algorithms/dse/kperiodic.h>
#include <algorithms/dse/ModularDSE.h>
#include <algorithms/liveness/LivenessModularDSE.h>
#include <algorithms/transformation/subgraph.h>

namespace algorithms {
    namespace dse {


        class LivenessNextFunc {
        private :


           static  std::vector<algorithms::dse::TokenConfiguration> liveness_next_func_by_dichotomy(const algorithms::dse::TokenConfiguration& starting_point) {

                VERBOSE_DEBUG("[start] liveness_next_func_by_dichotomy:" << starting_point.to_csv_line());

                const models::Dataflow* df =  starting_point.getDataflow();


                // 1. build a minimal graph with the critical edges only.
                VERBOSE_DEBUG("[start] liveness_next_func_by_dichotomy: Build cc");
                models::Dataflow* cc_g = algorithms::build_subgraph(df, starting_point.getPerformance().critical_edges);
                VERBOSE_DEBUG("[start] liveness_next_func_by_dichotomy: cc built");

                // 1bis. find critical feedback_edge
                TOKEN_UNIT sumOfWeight = 0;
                std::vector<algorithms::dse::TokenConfiguration::EdgeIdentifier> critical_feedbacks;
                {ForEachEdge(cc_g, c) {
                        sumOfWeight += cc_g->getEdgeIn(c) + cc_g->getEdgeOut(c);
                        auto edgeId = cc_g->getEdgeId(c);
                        if (cc_g->getEdgeType(c) != EDGE_TYPE::FEEDBACK_EDGE) continue; // Skip non feedback buffers.
                        critical_feedbacks.push_back(edgeId);
                        VERBOSE_DEBUG("[start] Found a FEEDBACK_EDGE");
                    }}

                VERBOSE_ASSERT((critical_feedbacks.size() > 0), "Unsupported case.");
                VERBOSE_ASSERT((critical_feedbacks.size() == 1), "Unsupported case.");


                // 2. Check the critical edge
                auto criticalEdgeId = *critical_feedbacks.begin();
                Edge criticalEdge = cc_g->getEdgeById(criticalEdgeId);
                VERBOSE_ASSERT(cc_g->getEdgeType(criticalEdge) == EDGE_TYPE::FEEDBACK_EDGE, "Unsupported case.");
                VERBOSE_DEBUG("[start] liveness_next_func_by_dichotomy: edge check");


                // Create a new TokenConfiguration with the updated buffer size for the current edge
                // And compute throughput
                std::map<ARRAY_INDEX , TOKEN_UNIT> current_configuration; // Replace Edge with the correct type for your implementation
                { ForEachEdge(cc_g,e) {
                        ARRAY_INDEX tid = cc_g->getEdgeId(e);
                        if (cc_g->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                            current_configuration[tid] = starting_point.getConfiguration().at(criticalEdgeId);
                        } else {
                            // current_configuration[tid] = 0;
                        }
                    }}
                //std::map<ARRAY_INDEX, TOKEN_UNIT> current_configuration = liveness_initial_func(cc_g).getConfiguration();
                TOKEN_UNIT distance = std::max((TOKEN_UNIT) 1, current_configuration.at(criticalEdgeId)) ;

                //VERBOSE_INFO("[     ] liveness_next_func_by_dichotomy: starting distance " << distance);

                TOKEN_UNIT minVal = 0;
                TOKEN_UNIT maxVal = sumOfWeight;
                TOKEN_UNIT currentVal = maxVal;
                bool alive_once = false;
                while (maxVal - minVal > 1) {
                    VERBOSE_DEBUG("[     ] liveness_next_func_by_dichotomy: from " << minVal << " to " << maxVal);
                    VERBOSE_DEBUG("[     ] liveness_next_func_by_dichotomy: selected is " << currentVal);
                    cc_g->reset_computation();
                    current_configuration[criticalEdgeId] = currentVal;
                    algorithms::dse::TokenConfiguration new_token_configuration(cc_g, current_configuration);
                    auto res = liveness_performance_func(cc_g, new_token_configuration);

                    if (res.throughput > 0) {
                        VERBOSE_DEBUG("[     ] liveness_next_func_by_dichotomy: " << currentVal << " is alive");
                        alive_once = true;
                        maxVal = currentVal;
                    } else {
                        VERBOSE_DEBUG("[     ] liveness_next_func_by_dichotomy: " << currentVal << " is dead");
                        minVal = currentVal;
                    }
                    currentVal = minVal + (maxVal - minVal) / 2;

                }

                VERBOSE_DEBUG("[     ] liveness_next_func_by_dichotomy: finish with dead " << minVal << " and alive " << maxVal);

                auto new_configuration = starting_point.getConfiguration();
                new_configuration[criticalEdgeId] = maxVal;
                algorithms::dse::TokenConfiguration new_point(df, new_configuration);
                return {new_point};

            }

        public:
            LivenessNextFunc(bool use_dichotomy = true) : use_dichotomy(use_dichotomy) {}

                std::vector<algorithms::dse::TokenConfiguration>  operator()(const algorithms::dse::TokenConfiguration& current) const {

                VERBOSE_DEBUG("liveness_next_func: Start");

                // stopping condition
                if (current.getPerformance().throughput > 0) {
                    VERBOSE_DEBUG("Stopping condition reached, no more points to explore.");
                    return  {};
                }

                const models::Dataflow* df =  current.getDataflow();
                const auto& current_configuration = current.getConfiguration();

                /*
                 *  Special Case 1: only one critical edge, can perform dichotomy
                 */

                std::vector<algorithms::dse::TokenConfiguration::EdgeIdentifier> critical_feedbacks;
                for (algorithms::dse::TokenConfiguration::EdgeIdentifier edgeId : current.getPerformance().critical_edges) {
                    Edge c = df->getEdgeById(edgeId);
                    if (df->getEdgeType(c) != EDGE_TYPE::FEEDBACK_EDGE) continue; // Skip non feedback buffers.
                    critical_feedbacks.push_back(edgeId);
                }

                if (use_dichotomy and critical_feedbacks.size() == 1) {
                    VERBOSE_DEBUG("Special case 1: Run Dichotomy");
                    return  liveness_next_func_by_dichotomy(current);
                }

                /*
                 *  General Case : Step up each critical edge by a gcd step
                 */

                std::vector<algorithms::dse::TokenConfiguration> next_configurations;

                for (algorithms::dse::TokenConfiguration::EdgeIdentifier edgeId : current.getPerformance().critical_edges) {
                    Edge c = df->getEdgeById(edgeId);
                    if (df->getEdgeType(c) != EDGE_TYPE::FEEDBACK_EDGE) continue; // Skip non feedback buffers.
                    auto new_configuration = current_configuration; // Make a copy of the current configuration
                    new_configuration[edgeId] = new_configuration[edgeId] + df->getFineGCD(c); // Increment the buffer size for the current edge by 1

                    // Create a new TokenConfiguration with the updated buffer size for the current edge
                    algorithms::dse::TokenConfiguration new_token_configuration(current.getDataflow(), new_configuration);
                    next_configurations.push_back(new_token_configuration);
                }

                return next_configurations;
            }

        private:
            bool      use_dichotomy;
        };



        algorithms::dse::TokenConfiguration::PerformanceResult liveness_performance_func(models::Dataflow* g, const algorithms::dse::TokenConfiguration& config) {

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




// TODO: for the liveness problem it might make sense, but for buffer sizing it is so dumb.
std::vector<algorithms::dse::TokenConfiguration> liveness_initial_func(const models::Dataflow* dataflow) {
    std::map<ARRAY_INDEX , TOKEN_UNIT> configuration; // Replace Edge with the correct type for your implementation
    { ForEachEdge(dataflow,e) {
            ARRAY_INDEX tid = dataflow->getEdgeId(e);
            if (dataflow->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                configuration[tid] = 0;
            }
        }}
    return {algorithms::dse::TokenConfiguration(dataflow, configuration)};
}


bool liveness_stop_condition(const algorithms::dse::TokenConfiguration& new_config, const algorithms::dse::TokenConfigurationSet& searched_configs) {

    const algorithms::dse::TokenConfiguration* best = searched_configs.getBestPerformancePoint();
    if (best) return ((best->getCost() < new_config.getCost()) && (best->getPerformance().throughput > 0));
    return false;
}

TokenConfigurationSet solve_liveness   (models::Dataflow* const  dataflow,
                                        bool realtime_output,
                                        size_t thread_count,
                                        size_t timeout,
                                        size_t limit,
                                        std::string  filename,
                                        bool use_last,
                                        bool use_dichotomy,
                                        algorithms::dse::TokenConfiguration* tc
) {

            dataflow->precomputeFineGCD();
            algorithms::dse::ModularDSE dse(dataflow,
                                            liveness_performance_func,
                                            LivenessNextFunc(use_dichotomy),
                                            liveness_stop_condition,
                                            thread_count);



            if(filename != "") {
                VERBOSE_INFO("Load previous search points from " << filename);
                dse.import_results(filename);
            } else if (tc) {
                    VERBOSE_INFO("Initial state of the search is forced to " << *tc);
                    dse.add_initial_job(*tc);
            } else {
                    dse.add_initial_jobs(liveness_initial_func(dataflow));
            }

            // Run the DSE exploration for a short period of time (e.g., 100 milliseconds)
            std::future<void> exploration_future = std::async(std::launch::async, [&dse,limit, use_last, realtime_output] { dse.explore(limit, use_last,realtime_output); });

            if (timeout > 0) {
                std::this_thread::sleep_for(std::chrono::seconds (timeout));
                dse.stop();
            }

            exploration_future.wait();
            return dse.getResults();
        }



void liveness_dse   (models::Dataflow* const  dataflow, parameters_list_t params) {

    size_t use_last   = (params.count("use_last") > 0) ? commons::fromString<bool>(params.at("use_last")) : false;
    size_t use_dichotomy   = (params.count("use_dichotomy") > 0) ? commons::fromString<bool>(params.at("use_dichotomy")) : false;
    size_t realtime_output = (params.count("realtime") > 0) ? commons::fromString<bool>(params.at("realtime")) : false;
    size_t thread_count    = (params.count("thread") > 0) ? commons::fromString<size_t>(params.at("thread")) : 1;
    size_t timeout         = (params.count("timeout") > 0) ? commons::fromString<size_t>(params.at("timeout")) : 0;
    size_t limit           = (params.count("limit") > 0) ? commons::fromString<size_t>(params.at("limit")) : 0;
    std::string  filename  = (params.count("import") > 0) ? params.at("import") : "";
    algorithms::dse::TokenConfiguration* tc = (params.count("init") > 0) ? new algorithms::dse::TokenConfiguration(dataflow, commons::split<TOKEN_UNIT>(params.at("init"), ',')) : nullptr;

    VERBOSE_ASSERT(params.count("init")  + params.count("import")  != 2, "Please pass your init inside the import file");

    TokenConfigurationSet result = solve_liveness   (dataflow, realtime_output, thread_count, timeout, limit, filename, use_last,use_dichotomy,  tc);

    delete tc;
}
    } // end of dse namespace
} // end of algorithms namespace

