//
// Created by toky on 26/4/23.
//

#include "dse_utils.h"
#include <models/Dataflow.h>

void algorithms::generateInplaceFeedbackBuffers(models::Dataflow* g){
/* Generate dataflow_prime */

    g->reset_computation();
// add feedback channels in new graph to model bounded channel quantities
    {ForEachEdge(g, c) {

// Only feedback for non-feedback;
            if (g->getEdgeType(c) == EDGE_TYPE::FEEDBACK_EDGE) {continue;};
            if (g->edgeExist(g->getEdgeTarget(c),
                             g->getEdgeSource(c))) {
                FAILED("Unsupported case for sanity reason.");
            }

            Edge original_edge = g->getEdgeById(g->getEdgeId(c));
            Edge new_edge = g->addEdge(g->getEdgeTarget(c),
                                       g->getEdgeSource(c));
            g->setEdgeInPhases(new_edge,
                               g->getEdgeOutVector(c));
            g->setEdgeOutPhases(new_edge,
                                g->getEdgeInVector(c));
            g->setPreload(new_edge, 0);
            g->setEdgeName(new_edge,
                           g->getEdgeName(c) + "_prime");
            g->setEdgeType(new_edge, EDGE_TYPE::FEEDBACK_EDGE);

    }}

}

std::pair<models::Dataflow*, std::map<Edge,Edge> > genGraphWFeedbackEdgesWithPairs(models::Dataflow* g){
/* Generate dataflow_prime */

std::map<Edge,Edge>  matching; // <dataflow_prime -> dataflow>
models::Dataflow* dataflow_prime = new models::Dataflow(*g);
dataflow_prime->reset_computation();
// add feedback channels in new graph to model bounded channel quantities
{ForEachEdge(g, c) {

// Only feedback for non-feedback;
if (g->getEdgeType(c) == EDGE_TYPE::FEEDBACK_EDGE) {continue;};
if (g->edgeExist(dataflow_prime->getEdgeTarget(c),
        dataflow_prime->getEdgeSource(c))) {
FAILED("Unsupported case for sanity reason.");
}


Edge original_edge = dataflow_prime->getEdgeById(g->getEdgeId(c));

Edge new_edge = dataflow_prime->addEdge(dataflow_prime->getEdgeTarget(c),
                                        dataflow_prime->getEdgeSource(c));
dataflow_prime->setEdgeInPhases(new_edge,
        dataflow_prime->getEdgeOutVector(c));
dataflow_prime->setEdgeOutPhases(new_edge,
        dataflow_prime->getEdgeInVector(c));
dataflow_prime->setPreload(new_edge, 0);
dataflow_prime->setEdgeName(new_edge,
        dataflow_prime->getEdgeName(c) + "_prime");
dataflow_prime->setEdgeType(new_edge, EDGE_TYPE::FEEDBACK_EDGE);

matching[original_edge] = c;
matching[new_edge] = c;

}}

return std::pair<models::Dataflow*, std::map<Edge,Edge>>(dataflow_prime, matching);

}


void algorithms::add_feedback_buffers(models::Dataflow *const dataflow, parameters_list_t ) {
    algorithms::generateInplaceFeedbackBuffers(dataflow);
}
