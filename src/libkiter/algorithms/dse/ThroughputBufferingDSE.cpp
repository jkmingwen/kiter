//
// Created by toky on 26/4/23.
//

#include "ThroughputBufferingDSE.h"
#include <algorithms/dse/dse_utils.h>
#include <algorithms/dse/kperiodic.h>
#include <algorithms/dse/ModularDSE.h>

namespace algorithms {
    namespace dse {



    // TODO: The copy of the graph is atrocious, one solution would be to pass he dataflow to the function instead.
        algorithms::dse::TokenConfiguration::PerformanceResult throughputbuffering_performance_func(const algorithms::dse::TokenConfiguration& config) {
            //VERBOSE_INFO("Copy graph");
            models::Dataflow g = *config.getDataflow();
            g.reset_computation(); // TODO: So wasteful. Needed because setPreload, makes me hope to have a Kperiodic with preloads as parameters.
            //VERBOSE_INFO("Prepare graph with config " << commons::toString(config));
            {ForEachEdge(&g,e) {
                    if (g.getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                        TOKEN_UNIT v = config.getConfiguration().at(g.getEdgeId(e));
                        g.setPreload(e,  v );
                    }
                }}

            //VERBOSE_INFO("compute_Kperiodic_throughput_and_cycles");
            kperiodic_result_t result_max = algorithms::compute_Kperiodic_throughput_and_cycles(&g);
            //VERBOSE_INFO("Done");
            algorithms::dse::TokenConfiguration::PerformanceResult res (0, {});
            res.throughput = result_max.throughput;
            for (Edge e : result_max.critical_edges) res.critical_edges.insert(g.getEdgeId(e));
            return res;
        }

        algorithms::dse::TokenConfiguration throughputbuffering_initial_func(const models::Dataflow* dataflow) {
            std::map<ARRAY_INDEX , TOKEN_UNIT> configuration; // Replace Edge with the correct type for your implementation
            { ForEachEdge(dataflow,e) {
                    ARRAY_INDEX tid = dataflow->getEdgeId(e);
                    if (dataflow->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                        configuration[tid] = 0;
                    }
                }}
            return algorithms::dse::TokenConfiguration(dataflow, configuration);
        }

// TODO : the kperiodic results is missing here
// TODO : also the step size is not there
        std::vector<algorithms::dse::TokenConfiguration> throughputbuffering_next_func(const algorithms::dse::TokenConfiguration& current) {



            const models::Dataflow* df =  current.getDataflow();
            std::vector<algorithms::dse::TokenConfiguration> next_configurations;
            const auto& current_configuration = current.getConfiguration();
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

        class ThroughputBufferingStopCondition {
        public:
            ThroughputBufferingStopCondition(TIME_UNIT target_th) : target_th(target_th) {}

            bool operator()(const algorithms::dse::TokenConfiguration& new_config, const algorithms::dse::TokenConfigurationSet& searched_configs) const {
                const algorithms::dse::TokenConfiguration* best = searched_configs.getBestPerformancePoint();

                if (best) {
                    return ((best->getCost() < new_config.getCost()) && (best->getPerformance().throughput > target_th));
                }

                return false;
            }

        private:
            TIME_UNIT target_th;
        };


        void solve_throughputbuffering   (models::Dataflow* const  dataflow, parameters_list_t params) {

            int thread_count = (params.count("thread") > 0) ? commons::fromString<int>(params.at("thread")) : 1;
            int timeout      = (params.count("timeout") > 0) ? commons::fromString<int>(params.at("timeout")) : 0;

            // Ensure dataflow has no feedbackbuffers.
            // Create feedbackbuffers with correct initial configuration (taking into account useful preload).

            // Compute the target throughput
            TIME_UNIT target_throughput = algorithms::compute_Kperiodic_throughput_and_cycles(dataflow).throughput;
            models::Dataflow* dataflow_prime = new models::Dataflow(*dataflow);

            algorithms::generateInplaceFeedbackBuffers(dataflow_prime);
            dataflow_prime->precomputeFineGCD();
            ThroughputBufferingStopCondition throughputbuffering_stop_condition(target_throughput);
            algorithms::dse::TokenConfiguration tc =  throughputbuffering_initial_func(dataflow_prime);



            algorithms::dse::ModularDSE dse(dataflow_prime,
                                            throughputbuffering_performance_func,
                                            throughputbuffering_initial_func,
                                            throughputbuffering_next_func,
                                            throughputbuffering_stop_condition,
                                            thread_count);

            dse.add_initial_job(tc);

            // Run the DSE exploration for a short period of time (e.g., 100 milliseconds)
            std::future<void> exploration_future = std::async(std::launch::async, [&dse] { dse.explore(); });

            if (timeout > 0) {
                std::this_thread::sleep_for(std::chrono::seconds (timeout));
                dse.stop();
            }

            exploration_future.wait();

            std::cout << dse.print_space();

        }
    } // end of dse namespace
} // end of algorithms namespace

