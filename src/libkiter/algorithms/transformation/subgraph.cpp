//
// Created by toky on 28/4/23.
//

#include "subgraph.h"
#include <models/Dataflow.h>

namespace algorithms {

    models::Dataflow *build_subgraph(const models::Dataflow *g, const std::set<ARRAY_INDEX> &critical_edges) {

        models::Dataflow *cc_g = new models::Dataflow();
        std::set<ARRAY_INDEX> visited_v;
        std::set<ARRAY_INDEX> visited_e;

        // VERBOSE_ASSERT(critical_edges.size() > 0, "Empty Critical cycle");

        for (auto e_id: critical_edges) {

            const Edge original_edge = g->getEdgeById(e_id);
            const Vertex original_source = g->getEdgeSource(original_edge);
            const Vertex original_target = g->getEdgeTarget(original_edge);

            const ARRAY_INDEX original_source_id = g->getVertexId(original_source);
            const ARRAY_INDEX original_target_id = g->getVertexId(original_target);
            const std::string original_source_name = g->getVertexName(original_source);
            const std::string original_target_name = g->getVertexName(original_target);

            if (!visited_v.count(original_source_id)) {
                Vertex src_cc = cc_g->addVertex(original_source_id, original_source_name);
                cc_g->setPhasesQuantity(src_cc, g->getPhasesQuantity(original_source));
                cc_g->setReentrancyFactor(src_cc, g->getReentrancyFactor(original_source));
                cc_g->setVertexDuration(src_cc, g->getVertexPhaseDuration(original_source));
                visited_v.insert(original_source_id);
            }
            if (!visited_v.count(original_target_id)) {
                Vertex trg_cc = cc_g->addVertex(original_target_id, original_target_name);
                cc_g->setPhasesQuantity(trg_cc, g->getPhasesQuantity(original_target));
                cc_g->setReentrancyFactor(trg_cc, g->getReentrancyFactor(original_target));
                cc_g->setVertexDuration(trg_cc, g->getVertexPhaseDuration(original_target));
                visited_v.insert(original_target_id);
            }

            Vertex src_cc = cc_g->getVertexByName(original_source_name);
            Vertex trg_cc = cc_g->getVertexByName(original_target_name);

            Edge e_cc = cc_g->addEdge(src_cc, trg_cc, e_id, g->getEdgeName(original_edge));
            cc_g->setPreload(e_cc, g->getPreload(original_edge));
            cc_g->setEdgeInPhases(e_cc, g->getEdgeInVector(original_edge));
            cc_g->setEdgeOutPhases(e_cc, g->getEdgeOutVector(original_edge));
            cc_g->setEdgeType(e_cc, g->getEdgeType(original_edge));
        }
        return cc_g;
    }
} // end of namespace