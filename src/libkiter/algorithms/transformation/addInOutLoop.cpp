//
// Created by toky on 11/5/23.
//


#include<algorithms/transformation/addInOutLoop.h>
#include <models/Dataflow.h>

void algorithms ::transformation::add_input_output_loop    (models::Dataflow* const g, parameters_list_t  params  ) {

    std::vector<ARRAY_INDEX> sources;
    std::vector<ARRAY_INDEX> targets;

    if (params.count("sources")) {
        sources = commons::split<ARRAY_INDEX>(params.at("sources"), ',');
    } else {
        {ForEachVertex(g, t){
                if (g->getVertexType(t).substr(0,5) == "INPUT") {
                    sources.push_back(g->getVertexId(t));
                }
            }}
    }

    if (params.count("targets")) {
        targets = commons::split<ARRAY_INDEX>(params.at("targets"), ',');
    } else {
        {ForEachVertex(g, t){
                if (g->getVertexType(t).substr(0,6) == "OUTPUT") {
                    targets.push_back(g->getVertexId(t));
                }
            }}
    }

    if (!sources.size()) {
        sources.push_back(g->getVertexId(g->getFirstVertex()));
        VERBOSE_WARNING("No sources provided or found, default to first task. Please specify sources using -psources=x,y,..");
    }
    if (!targets.size()) VERBOSE_ERROR("No targets provided or found. Please specify targets using -ptargets=x,y,..");

    return algorithms::transformation::add_input_output_loop(g, sources, targets);
}

void algorithms ::transformation::add_input_output_loop    (models::Dataflow* const dataflow,
                                                            const std::vector<ARRAY_INDEX>& inputs,
                                                            const std::vector<ARRAY_INDEX>&  output  ) {

    models::Dataflow  &g_prime = *dataflow;

    // Set the feedback buffers to force the entries to wait for exit to finish.
    for (auto output_task_id : output) {
        for (auto input_task_id : inputs) {

            Vertex output_task = g_prime.getVertexById(output_task_id);
            Vertex input_task = g_prime.getVertexById(input_task_id);


            std::vector<TOKEN_UNIT> input_phases (g_prime.getPhasesQuantity(input_task));
            input_phases[0] = 1;

            std::vector<TOKEN_UNIT> output_phases (g_prime.getPhasesQuantity(output_task));
            output_phases[0] = 1;

            Edge feedback = g_prime.addEdge(output_task,input_task);
            g_prime.setPreload(feedback, 1);
            g_prime.setEdgeInPhases(feedback, output_phases);
            g_prime.setEdgeOutPhases(feedback, input_phases);

        }
    }

}