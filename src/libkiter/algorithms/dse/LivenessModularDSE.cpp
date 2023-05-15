//
// Created by toky on 24/4/23.
//

#include <algorithms/dse/kperiodic.h>
#include <algorithms/dse/ModularDSE.h>
#include <algorithms/dse/LivenessModularDSE.h>
#include "algorithms/transformation/subgraph.h"

namespace algorithms {
    namespace dse {


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
algorithms::dse::TokenConfiguration liveness_initial_func(const models::Dataflow* dataflow) {
    std::map<ARRAY_INDEX , TOKEN_UNIT> configuration; // Replace Edge with the correct type for your implementation
    { ForEachEdge(dataflow,e) {
            ARRAY_INDEX tid = dataflow->getEdgeId(e);
            if (dataflow->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                configuration[tid] = 0;
            }
        }}
    return algorithms::dse::TokenConfiguration(dataflow, configuration);
}




std::vector<algorithms::dse::TokenConfiguration> liveness_next_func_by_dichotomy(const algorithms::dse::TokenConfiguration& starting_point) {

    VERBOSE_INFO("[start] liveness_next_func_by_dichotomy:" << starting_point.to_csv_line());

    const models::Dataflow* df =  starting_point.getDataflow();


    // 1. build a minimal graph with the critical edges only.
    VERBOSE_INFO("[start] liveness_next_func_by_dichotomy: Build cc");
    models::Dataflow* cc_g = algorithms::build_subgraph(df, starting_point.getPerformance().critical_edges);
    VERBOSE_INFO("[start] liveness_next_func_by_dichotomy: cc built");

    // 1bis. find critical feedback_edge
    std::vector<algorithms::dse::TokenConfiguration::EdgeIdentifier> critical_feedbacks;
    {ForEachEdge(cc_g, c) {
        auto edgeId = cc_g->getEdgeId(c);
        if (cc_g->getEdgeType(c) != EDGE_TYPE::FEEDBACK_EDGE) continue; // Skip non feedback buffers.
        critical_feedbacks.push_back(edgeId);
        VERBOSE_INFO("[start] Found a FEEDBACK_EDGE");
    }}

    VERBOSE_ASSERT((critical_feedbacks.size() > 0), "Unsupported case.");
    VERBOSE_ASSERT((critical_feedbacks.size() == 1), "Unsupported case.");


    // 2. Check the critical edge
    auto criticalEdgeId = *critical_feedbacks.begin();
    Edge criticalEdge = cc_g->getEdgeById(criticalEdgeId);
    VERBOSE_ASSERT(cc_g->getEdgeType(criticalEdge) == EDGE_TYPE::FEEDBACK_EDGE, "Unsupported case.");
    VERBOSE_INFO("[start] liveness_next_func_by_dichotomy: edge check");


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

    bool alive_once = false;
    while (true) {
        cc_g->reset_computation();
        algorithms::dse::TokenConfiguration new_token_configuration(cc_g, current_configuration);
        auto res = liveness_performance_func(cc_g, new_token_configuration);

        if (res.throughput > 0) {
            alive_once = true;
            distance = distance / 2;
            //VERBOSE_INFO("[     ] liveness_next_func_by_dichotomy: alive, new distance is " << distance);
            if (distance == 0)  return {new_token_configuration};
            current_configuration[criticalEdgeId] = current_configuration[criticalEdgeId] - distance; // Increment the buffer size for the current edge by 1
        } else {
            if (!alive_once) distance = distance * 2;
            //VERBOSE_INFO("[     ] liveness_next_func_by_dichotomy: dead, new distance is " << distance);
            current_configuration[criticalEdgeId] = current_configuration[criticalEdgeId] + distance; // Increment the buffer size for the current edge by 1
        }
        //VERBOSE_INFO("[     ] liveness_next_func_by_dichotomy: new token size " << current_configuration[criticalEdgeId]);

    }



}

std::vector<algorithms::dse::TokenConfiguration> liveness_next_func(const algorithms::dse::TokenConfiguration& current) {

    // stopping condition
    if (current.getPerformance().throughput > 0) {
        //VERBOSE_INFO("Stopping condition reached");
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

    if (critical_feedbacks.size() == 1) {
        VERBOSE_DEBUG("Special case 1: Run Dichotomy");
        return liveness_next_func_by_dichotomy(current);
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
                                        algorithms::dse::TokenConfiguration* tc
) {

            dataflow->precomputeFineGCD();
            algorithms::dse::ModularDSE dse(dataflow,
                                            liveness_performance_func,
                                            liveness_initial_func,
                                            liveness_next_func,
                                            liveness_stop_condition,
                                            thread_count);


            if(filename != "") {
                VERBOSE_INFO("Load previous search points from " << filename);
                dse.import_results(filename);
            } else if (tc) {
                    VERBOSE_INFO("Initial state of the search is forced to " << *tc);
                    dse.add_initial_job(*tc);
            } else {
                    dse.add_initial_job(liveness_initial_func(dataflow));
            }

            // Run the DSE exploration for a short period of time (e.g., 100 milliseconds)
            std::future<void> exploration_future = std::async(std::launch::async, [&dse,limit, realtime_output] { dse.explore(limit, realtime_output); });

            if (timeout > 0) {
                std::this_thread::sleep_for(std::chrono::seconds (timeout));
                dse.stop();
            }

            exploration_future.wait();
            return dse.getResults();
        }



void liveness_dse   (models::Dataflow* const  dataflow, parameters_list_t params) {

    size_t realtime_output = (params.count("realtime") > 0) ? commons::fromString<bool>(params.at("realtime")) : false;
    size_t thread_count    = (params.count("thread") > 0) ? commons::fromString<size_t>(params.at("thread")) : 1;
    size_t timeout         = (params.count("timeout") > 0) ? commons::fromString<size_t>(params.at("timeout")) : 0;
    size_t limit           = (params.count("limit") > 0) ? commons::fromString<size_t>(params.at("limit")) : 0;
    std::string  filename  = (params.count("import") > 0) ? params.at("import") : "";
    algorithms::dse::TokenConfiguration* tc = (params.count("init") > 0) ? new algorithms::dse::TokenConfiguration(dataflow, commons::split<TOKEN_UNIT>(params.at("init"), ',')) : nullptr;

    VERBOSE_ASSERT(params.count("init")  + params.count("import")  != 2, "Please pass your init inside the import file");

    TokenConfigurationSet result = solve_liveness   (dataflow, realtime_output, thread_count, timeout, limit, filename, tc);

    delete tc;
}
    } // end of dse namespace
} // end of algorithms namespace

