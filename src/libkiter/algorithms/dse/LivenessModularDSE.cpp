//
// Created by toky on 24/4/23.
//

#include <algorithms/dse/kperiodic.h>
#include <algorithms/dse/ModularDSE.h>
#include <algorithms/dse/LivenessModularDSE.h>
namespace algorithms {
    namespace dse {

// This is getFineGCD
TOKEN_UNIT getStepSize(models::Dataflow *dataflow, Edge c) {
    TOKEN_UNIT stepSize;
    stepSize = dataflow->getEdgeInVector(c)[0]; // initialise with first value
    for (EXEC_COUNT i = 0; i < dataflow->getEdgeInPhasesCount(c); i++)
        stepSize = std::gcd(stepSize, dataflow->getEdgeInVector(c)[i]);
    for (EXEC_COUNT i = 0; i < dataflow->getEdgeOutPhasesCount(c); i++)
        stepSize = std::gcd(stepSize, dataflow->getEdgeOutVector(c)[i]);
    return stepSize;
}

TOKEN_UNIT getMinBufferSize(models::Dataflow *dataflow, Edge c) {

    TOKEN_UNIT buffer_size = INT_MAX; // NOTE (should use ULONG_MAX but it's a really large value)

    //TODO: This is not the correct value I believe this is an artefact from previous algorithm implementation (Jaime?)
    TOKEN_UNIT ratePeriod = (TOKEN_UNIT) std::gcd(dataflow->getEdgeInPhasesCount(c),
                                                  dataflow->getEdgeOutPhasesCount(c));

    for (TOKEN_UNIT i = 0; i < ratePeriod; i++) {
        // might want to change variables to p, c, and t for legibility
        TOKEN_UNIT tokensProduced = dataflow->getEdgeInVector(c)[i % dataflow->getEdgeInPhasesCount(c)];
        TOKEN_UNIT tokensConsumed = dataflow->getEdgeOutVector(c)[i % dataflow->getEdgeOutPhasesCount(c)];
        TOKEN_UNIT tokensInitial = dataflow->getPreload(c);
        VERBOSE_DEBUG_DSE("p, c, t: " << tokensProduced << ", "
                                      << tokensConsumed << ", " << tokensInitial );
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
        if (lowerBound < buffer_size) {
            buffer_size = lowerBound;
        }
    }
    return buffer_size;
}



// TODO: The copy of the graph is atrocious
algorithms::dse::TokenConfiguration::PerformanceResult liveness_performance_func(const algorithms::dse::TokenConfiguration& config) {
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

// TODO : the kperiodic results is missing here
// TODO : also the step size is not there
std::vector<algorithms::dse::TokenConfiguration> liveness_next_func(const algorithms::dse::TokenConfiguration& current) {

    // stopping condition
    if (current.getPerformance().throughput > 0) {
        //VERBOSE_INFO("Stopping condition reached");
        return  {};
    }

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

bool liveness_stop_condition(const algorithms::dse::TokenConfiguration& new_config, const algorithms::dse::TokenConfigurationSet& searched_configs) {

    const algorithms::dse::TokenConfiguration* best = searched_configs.getBestPerformancePoint();
    if (best) return ((best->getCost() < new_config.getCost()) && (best->getPerformance().throughput > 0));
    return false;
}

void solve_liveness   (models::Dataflow* const  dataflow, parameters_list_t params) {

    size_t thread_count = (params.count("thread") > 0) ? commons::fromString<size_t>(params.at("thread")) : 1;
    size_t timeout      = (params.count("timeout") > 0) ? commons::fromString<size_t>(params.at("timeout")) : 0;
    size_t limit        = (params.count("limit") > 0) ? commons::fromString<size_t>(params.at("limit")) : 0;
    std::string  filename = (params.count("import") > 0) ? params.at("import") : "";
    algorithms::dse::TokenConfiguration tc = (params.count("init") > 0) ? algorithms::dse::TokenConfiguration(dataflow, commons::split<TOKEN_UNIT>(params.at("init"), ',')) : liveness_initial_func(dataflow);

    VERBOSE_ASSERT(params.count("init")  + params.count("import")  != 2, "Please pass your init inside the import file");
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
    } else {
        VERBOSE_INFO("Initial state of the search is going to be " << tc);
        dse.add_initial_job(tc);
    }

    // Run the DSE exploration for a short period of time (e.g., 100 milliseconds)
    std::future<void> exploration_future = std::async(std::launch::async, [&dse,limit] { dse.explore(limit); });

    if (timeout > 0) {
        std::this_thread::sleep_for(std::chrono::seconds (timeout));
        dse.stop();
    }

    exploration_future.wait();

    std::cout << dse.print_space();

}
    } // end of dse namespace
} // end of algorithms namespace

