//
// Created by toky on 24/4/23.
//

#include <algorithms/dse/kperiodic.h>
#include <algorithms/dse/ModularDSE.h>
#include <algorithms/dse/LivenessModularDSE.h>

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
// TODO: the kperiodic_results should be stored somewhere
TIME_UNIT dummy_performance_func(const algorithms::dse::TokenConfiguration& config) {
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
    return result_max.throughput;
}

// TODO: for the liveness problem it might make sense, but for buffer sizing it is so dumb.
algorithms::dse::TokenConfiguration dummy_initial_func(const models::Dataflow* dataflow) {
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
std::vector<algorithms::dse::TokenConfiguration> dummy_next_func(const algorithms::dse::TokenConfiguration& current) {

    // stopping condition
    if (current.getThroughput() > 0) {
        //VERBOSE_INFO("Stopping condition reached");
        return  {};
    }

    const models::Dataflow* df =  current.getDataflow();
    std::vector<algorithms::dse::TokenConfiguration> next_configurations;
    const auto& current_configuration = current.getConfiguration();

    for (const auto& edge_buffer_pair : current_configuration) {
        auto new_configuration = current_configuration; // Make a copy of the current configuration
        Edge c = df->getEdgeById(edge_buffer_pair.first);
        new_configuration[edge_buffer_pair.first] = edge_buffer_pair.second + df->getFineGCD(c); // Increment the buffer size for the current edge by 1

        // Create a new TokenConfiguration with the updated buffer size for the current edge
        algorithms::dse::TokenConfiguration new_token_configuration(current.getDataflow(), new_configuration);
        next_configurations.push_back(new_token_configuration);
    }

    return next_configurations;
}

void solve_liveness   (models::Dataflow* const  dataflow, parameters_list_t params) {

    int thread_count = (params.count("thread") > 0) ? commons::fromString<int>(params.at("thread")) : 1;
    int timeout      = (params.count("timeout") > 0) ? commons::fromString<int>(params.at("timeout")) : 0;


    dataflow->precomputeFineGCD();
    algorithms::dse::ModularDSE dse(dataflow,
                                    dummy_performance_func,
                                    dummy_initial_func,
                                    dummy_next_func, thread_count);

    algorithms::dse::TokenConfiguration tc = dummy_initial_func(dataflow);
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

