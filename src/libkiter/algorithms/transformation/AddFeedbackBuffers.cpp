//
// Created by toky on 26/5/23.
//

#include "AddFeedbackBuffers.h"
#include <models/Dataflow.h>

void algorithms::transformation::generateInplaceFeedbackBuffers(models::Dataflow* g){
/* Generate dataflow_prime */

    g->reset_computation();
// add feedback channels in new graph to model bounded channel quantities
    {ForEachEdge(g, c) {

// Only feedback for non-feedback;
            if (g->getEdgeType(c) == EDGE_TYPE::FEEDBACK_EDGE) {continue;};

//            if (g->edgeExist(g->getEdgeTarget(c),
//                             g->getEdgeSource(c))) {
//                FAILED("Unsupported case for sanity reason.");
//            }

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

void algorithms::transformation::add_feedback_buffers(models::Dataflow *const dataflow, parameters_list_t ) {
    algorithms::transformation::generateInplaceFeedbackBuffers(dataflow);
}

