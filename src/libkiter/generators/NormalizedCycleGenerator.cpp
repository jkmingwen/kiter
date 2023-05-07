//
// Created by toky on 24/4/23.
//

#include "NormalizedCycleGenerator.h"
#include <models/Dataflow.h>

models::Dataflow*  generators::new_normalized_cycle(size_t node_count ,
                                                    const std::vector<TOKEN_UNIT>& node_weights,
                                                    const std::vector<TIME_UNIT>&  node_durations,
                                                    const std::vector<TOKEN_UNIT>& edge_preloads) {
    models::Dataflow* to = new models::Dataflow();

    // Check input values
    VERBOSE_ASSERT_EQUALS(node_count, node_weights.size());
    VERBOSE_ASSERT_EQUALS(node_count, node_durations.size());
    VERBOSE_ASSERT_EQUALS(node_count, edge_preloads.size());

    VERBOSE_DEBUG("Generate a graph with "  << node_count << " nodes, "
                                            << commons::toString(node_durations) << " durations, "
                                            << commons::toString(node_weights) << " weights, "
                                            << commons::toString(edge_preloads) << " preloads.");

    // Empty graph case
    if (node_count == 0) return to;

    // generate the nodes, with durations.
    std::vector<ARRAY_INDEX> temp_vertex_list;
    for(size_t i = 0; i < node_count; i++) {
        auto new_vtx = to->addVertex();
        temp_vertex_list.push_back( to->getVertexId(new_vtx) );
        to->setVertexName(new_vtx, "Node_" + commons::toString(i));
        to->setPhasesQuantity(new_vtx,1);
        to->setVertexDuration(new_vtx,{ node_durations.at(i) });
        to->setReentrancyFactor(new_vtx,1);
    }
    temp_vertex_list.push_back(temp_vertex_list.at(0));

    for (size_t i = 0; i < node_count ; i++) {
        auto src = to->getVertexById( temp_vertex_list[i] );
        auto dst = to->getVertexById( temp_vertex_list[i+1] );

        auto e1 = to->addEdge(src, dst);
        to->setEdgeName(e1, "Edge_" + commons::toString(i));
        to->setEdgeInPhases(e1,{node_weights.at(i % node_count)});
        to->setEdgeOutPhases(e1,{node_weights.at((i + 1) % node_count)});
        to->setPreload(e1,edge_preloads[i]);
    }



    to->setEdgeType(to->getFirstEdge(),EDGE_TYPE::FEEDBACK_EDGE);

    return to;
}

models::Dataflow*  generators::generate_normalized_cycle(parameters_list_t   param_list) {

    // Gather parameters (size, weights, durations, preloads)
    size_t node_count = param_list.count("size") > 0 ?  commons::fromString<size_t>(param_list.at("size")) : 0;
    std::vector<TOKEN_UNIT> node_weights   = param_list.count("weights") > 0 ?  commons::split<TOKEN_UNIT>(param_list.at("weights"), ',') : std::vector<TOKEN_UNIT>();
    std::vector<TIME_UNIT>  node_durations = param_list.count("durations") > 0 ?  commons::split<TIME_UNIT>(param_list.at("durations"), ',') : std::vector<TIME_UNIT>();
    std::vector<TOKEN_UNIT> edge_preloads  = param_list.count("preloads") > 0 ?  commons::split<TOKEN_UNIT>(param_list.at("preloads"), ',') : std::vector<TOKEN_UNIT>();

    // Check parameters values
    VERBOSE_ASSERT_EQUALS(node_weights.size(), node_count);
    VERBOSE_ASSERT_EQUALS(node_durations.size(), node_count);
    VERBOSE_ASSERT_EQUALS(edge_preloads.size(), node_count);

    // Run the generation
    return generators::new_normalized_cycle( node_count, node_weights, node_durations, edge_preloads) ;

}
