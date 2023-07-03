//
// Created by toky on 3/7/23.
//

#include "normalize.h"
#include "algorithms/normalization.h"

void algorithms::transformation::apply_normalization    (models::Dataflow* const dataflow, parameters_list_t  parameters  ) {

    // Doesnt support init phases.
    { ForEachTask(dataflow,t) {
            VERBOSE_ASSERT(dataflow->getInitPhasesQuantity(t) == 0, "Unsupported case.");
    }}

    // First we compute the normalization
    dataflow->reset_computation();
    std::map<ARRAY_INDEX,TOKEN_UNIT> alphas = algorithms::compute_alphas(dataflow);

    //algorithms::normalize(dataflow);

    // Then we modify the graph, every buffer is modified
    {ForEachChannel(dataflow,c) {

        auto bufferId = dataflow->getEdgeId(c);
        auto source = dataflow->getEdgeSource(c);
        auto target = dataflow->getEdgeTarget(c);
        auto sourcePhaseCount = dataflow->getPhasesQuantity(source);
        auto targetPhaseCount = dataflow->getPhasesQuantity(target);

        auto alpha = alphas[bufferId];

        auto inVector =  dataflow->getEdgeInVector(c);
        auto outVector=  dataflow->getEdgeOutVector(c);
        auto bufferPreload = dataflow->getPreload(c);

        bufferPreload = bufferPreload * alpha;

        for (auto inPhase = 0; inPhase < sourcePhaseCount; inPhase++) {
            inVector[inPhase] = inVector[inPhase] * alpha;
            // dataflow->getNormIn(c,inPhase);
        }

        for (auto outPhase = 0; outPhase < targetPhaseCount; outPhase++) {
            outVector[outPhase] = outVector[outPhase] * alpha;
            //outVector[outPhase-1] = dataflow->getNormOut(c,outPhase);
            //(TOKEN_FRACT(outVector[outPhase]) * alpha).denominator();
        }

        dataflow->setEdgeInPhases(c,inVector) ;
        dataflow->setEdgeOutPhases(c,outVector) ;
        dataflow->setPreload(c, bufferPreload);

    }}


}