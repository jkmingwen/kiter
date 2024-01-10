//
// Created by toky on 26/5/23.
//

#include  <algorithms/liveness/SC.h>
#include <models/EventGraph.h>
#include <models/Dataflow.h>
#include "algorithms/normalization.h"


void generateSC1Constraint(const models::Dataflow *const dataflow, models::EventGraph *g, Edge c) {

    ARRAY_INDEX id = dataflow->getEdgeId(c);

    Vertex source = dataflow->getEdgeSource(c);
    Vertex target = dataflow->getEdgeTarget(c);

    std::string sourceName = dataflow->getVertexName(source);
    std::string targetName = dataflow->getVertexName(target);

    const ARRAY_INDEX source_id = dataflow->getVertexId(source);
    const ARRAY_INDEX target_id = dataflow->getVertexId(target);

    EXEC_COUNT source_phase_count = dataflow->getEdgeInPhasesCount(c);
    EXEC_COUNT target_phase_count = dataflow->getEdgeOutPhasesCount(c);


    VERBOSE_ASSERT_EQUALS(dataflow->getAlpha(c).denominator(), 1);
    const TOKEN_UNIT stepa = dataflow->getFineGCD(c) * dataflow->getAlpha(c).numerator();
    TOKEN_UNIT normmop = commons::floor(dataflow->getNormMop(c), stepa);

    TOKEN_UNIT normdapkm1 = 0;
    TOKEN_UNIT normdapk = 0;

    for (EXEC_COUNT pi = 1; pi <= source_phase_count; pi++) {
        TOKEN_UNIT normdamkp = 0;
        const TOKEN_UNIT wak = dataflow->getNormIn(c, pi);
        normdapk += wak;
        models::EventGraphVertex source_event = g->getEventGraphVertex(source_id, pi, 1);

        for (EXEC_COUNT pj = 1; pj <= target_phase_count; pj++) {

            const TOKEN_UNIT vakp = dataflow->getNormOut(c, pj);
            normdamkp += vakp;


            models::EventGraphVertex target_event = g->getEventGraphVertex(target_id, pj, 1);


            // from DAC2013
            // W1(u) = D_a^-(t^j_{k_j}) - D_a^+Pred(t^i_{k_i}) - step_a - Mo(a)
            // D1(u) = 1

            TIME_UNIT w1 = normdamkp - normdapkm1 - stepa - normmop;
            TIME_UNIT d1 = 1;
            g->addEventConstraint(source_event, target_event, w1, d1, id);

        }
        normdapkm1 = normdapk;
    }
}


models::EventGraph *generate_SC1_EventGraph(const models::Dataflow *const dataflow) {

    models::EventGraph *g = new models::EventGraph();

    /* generate nodes */
    {
        ForEachVertex(dataflow, t) {
            const ARRAY_INDEX tid = dataflow->getVertexId(t);

            EXEC_COUNT init_phase_count = dataflow->getInitPhasesQuantity(t);
            EXEC_COUNT periodic_phase_count = dataflow->getPhasesQuantity(t);

            for (EXEC_COUNT i = 1; i <= init_phase_count; i++) {
                g->addEvent(models::SchedulingEvent(tid, 1 - i, 1));
            }

            for (EXEC_COUNT i = 1; i <= periodic_phase_count; i++) {
                g->addEvent(models::SchedulingEvent(tid, i, 1));
            }
        }
    }


    // DEFINITION DES CONTRAINTES DE PRECEDENCES
    //******************************************************************
    {
        ForEachChannel(dataflow, c) {
            generateSC1Constraint(dataflow, g, c);
        }
    }

    return g;

}

models::EventGraph *generate_SC2_EventGraph(const models::Dataflow *const dataflow) {

    models::EventGraph *g = new models::EventGraph();

    /* generate nodes */
    {ForEachEdge(dataflow, c) {
            const ARRAY_INDEX cid = dataflow->getEdgeId(c);
            g->addEvent(models::SchedulingEvent(cid, 1, 1));
    }}

    {ForEachVertex(dataflow, t) {

        // for every input, output couple, we need a constraint

            { ForInputEdges(dataflow, t, a) {
                    { ForOutputEdges(dataflow, t, ap) {

                            // From DAC2013
                            // a is the input edge
                            // ap is the output edge
                            // W2(e) = −stepa − M0(a)+ maxk∈{1,··· ,φ(t)}[D−a 〈tk , 1〉 − D+a′ P red〈tk , 1〉] .
                            ARRAY_INDEX tid = dataflow->getVertexId(t);
                            const TOKEN_UNIT stepa = dataflow->getFineGCD(a) * dataflow->getAlpha(a).numerator();
                            TOKEN_UNIT mopa = commons::floor(dataflow->getNormMop(a), stepa);

                            TOKEN_UNIT normdam     = 0;
                            TOKEN_UNIT normdappred = 0;
                            TOKEN_UNIT max_normdam_normdappred = std::numeric_limits<TOKEN_UNIT>::min();
                            for (auto phase = 1 ; phase <= dataflow->getPhasesQuantity(t); phase ++) {
                                normdam += dataflow->getNormOut(a, phase);
                                max_normdam_normdappred = std::max(max_normdam_normdappred, normdam - normdappred);
                                normdappred += dataflow->getNormIn(ap, phase);
                            }
                            TIME_UNIT w2 = -stepa - mopa + max_normdam_normdappred;
                            TIME_UNIT d2 = 1;
                            models::EventGraphVertex source_event = g->getEventGraphVertex(dataflow->getEdgeId(a), 1, 1);
                            models::EventGraphVertex target_event = g->getEventGraphVertex(dataflow->getEdgeId(ap), 1, 1);
                            g->addEventConstraint(source_event, target_event, w2, d2, tid);

                        }}
            }}

    }}

    return g;

}

bool algorithms::liveness::CSDF_SC1(const models::Dataflow *const dataflow) {

    VERBOSE_ASSERT(dataflow->has_repetition_vector(), "Need repetition vector.");
    VERBOSE_ASSERT(dataflow->is_normalized(), "Need normalization.");

    models::EventGraph *eg = generate_SC1_EventGraph(dataflow);

    VERBOSE_INFO("EventGraph Latex\n" << eg->printTikz());

    std::pair<TIME_UNIT, std::vector<models::EventGraphEdge> > howard_res = eg->MaxCycleRatio();

    TIME_UNIT res = howard_res.first;
    VERBOSE_INFO("res = " << res);
    VERBOSE_INFO("res = " << commons::toString(howard_res.second));
    std::vector<models::EventGraphEdge> critical_circuit = howard_res.second;

    return (res < 0.0);

}
bool algorithms::liveness::CSDF_SC2(const models::Dataflow *const dataflow) {

    VERBOSE_ASSERT(dataflow->has_repetition_vector(), "Need repetition vector.");
    VERBOSE_ASSERT(dataflow->is_normalized(), "Need normalization.");

    models::EventGraph *eg = generate_SC2_EventGraph(dataflow);

    VERBOSE_INFO("EventGraph Latex\n" << eg->printTikz());

    std::pair<TIME_UNIT, std::vector<models::EventGraphEdge> > howard_res = eg->MaxCycleRatio();

    TIME_UNIT res = howard_res.first;
    VERBOSE_INFO("res = " << res);
    VERBOSE_INFO("res = " << commons::toString(howard_res.second));
    std::vector<models::EventGraphEdge> critical_circuit = howard_res.second;

    return (res < 0.0);

}


void algorithms::liveness::LivenessSC(models::Dataflow * dataflow,  parameters_list_t) {

    // Ensure the graph is normalized
    VERBOSE_ASSERT(computeRepetitionVector(dataflow), "Consistency failed");
    VERBOSE_ASSERT(algorithms::normalize(dataflow), "Normalization failed");

    bool sc1 = algorithms::liveness::CSDF_SC1(dataflow) ;
    bool sc2 = algorithms::liveness::CSDF_SC2(dataflow) ;

    std::cout << "SC1 Result: " << (sc1?"true":"false") << std::endl;
    std::cout << "SC2 Result: " << (sc2?"true":"false") << std::endl;

}