/*
 * repetition_vector.cpp
 *
 *  Created on: 22 avr. 2013
 *      Author: toky
 */


#include "repetition_vector.h"
#include <models/Dataflow.h>

#define CRAZY


bool calcFractionsConnectedActors(const models::Dataflow *from, std::map<Vertex,EXEC_COUNT_FRACT> &fractions, Vertex a, TOKEN_UNIT ratePeriod) {

    EXEC_COUNT_FRACT fractionA = fractions[a];

    // Inconsistent graph?
    if (fractionA == EXEC_COUNT_FRACT(0,1)) {
        VERBOSE_ERROR("fractionA == EXEC_COUNT_FRACT(0,1)");
        return false;
    }

    // Calculate the rate for each actor 'b' connected to actor 'a'
    {ForConnectedEdges(from,a,c) {
                Vertex src  = from->getEdgeSource(c);
                Vertex dest = from->getEdgeTarget(c);

                Vertex b = (src==a) ? dest : src;

                // Tokens produced or consumed by actor A in one rate period
                TOKEN_UNIT rateA = from->getEdgeIn(c) ;
                rateA = rateA * (ratePeriod / from->getEdgeInPhasesCount(c));

                // Tokens produced or consumed by actor B in one rate period
                TOKEN_UNIT rateB =  from->getEdgeOut(c);
                rateB = rateB * (ratePeriod / from->getEdgeOutPhasesCount(c));

                if (dest==a) {
                    TOKEN_UNIT tmp = rateB;
                    rateB = rateA;
                    rateA = tmp;
                }

                // Graph inconsistent?
                if (rateA == 0 || rateB == 0)
                {
                    fractions.clear();
                    VERBOSE_ERROR("rateA == 0 || rateB == 0");
                    return false;
                }

                // Calculate firing rate 'b'
                EXEC_COUNT_FRACT ratioAB = EXEC_COUNT_FRACT(rateA, rateB);
                EXEC_COUNT_FRACT fractionB = fractionA * ratioAB;

                // Known firing rate for 'b'
                EXEC_COUNT_FRACT knownFractionB = fractions[b];

                // Compare known and calculated firing rate of 'b'
                if (knownFractionB != EXEC_COUNT_FRACT(0)
                    && fractionB != knownFractionB)  {
                    // Inconsistent graph, set all fractions to 0
                    fractions.clear();
                    VERBOSE_ERROR("(knownFractionB(" << knownFractionB << ") != EXEC_COUNT_FRACT(0,1) && fractionB(" << fractionB << ") != knownFractionB)");
                    return false;
                }
                else if (knownFractionB == EXEC_COUNT_FRACT(0,1))
                {
                    // Set the firing rate of actor 'b'
                    fractions[b] = fractionB;

                    // Calculate firing rate for all actors connnected to 'b'
                    calcFractionsConnectedActors(from, fractions, b, ratePeriod);

                    // Is graph inconsistent?
                    if (fractions[b] == EXEC_COUNT_FRACT(0,1))
                    {
                        VERBOSE_ERROR("(fractions[b] == EXEC_COUNT_FRACT(0,1))");
                        return false;
                    }
                }
            }}
    return true;
}


bool calcRepetitionVector(models::Dataflow *from,std::map<Vertex,EXEC_COUNT_FRACT>& fractions, EXEC_COUNT ratePeriod) {

    std::map<Vertex,EXEC_COUNT> repetitionVector;
    EXEC_COUNT l = 1;

    // Find lowest common multiple (lcm) of all denominators
    {ForEachVertex(from,v) {
            l = std::lcm(l,fractions[v].denominator());
        }}

    // Zero vector?
    if (l == 0) {
        VERBOSE_ERROR("Zero vector ?");
        return false;
    }


    // Calculate non-zero repetition vector
    {ForEachVertex(from,v) {
            repetitionVector[v] = (fractions[v].numerator() * l) /  fractions[v].denominator();
        }}


    // Find greatest common divisor (gcd)
    EXEC_COUNT g = repetitionVector.begin()->second;

    {ForEachVertex(from,v) {
            g = std::gcd(g, repetitionVector[v]);
        }}

    VERBOSE_ASSERT(g > 0, TXT_NEVER_HAPPEND);

    // Minimize the repetition vector using the gcd
    {ForEachVertex(from,v) {
            repetitionVector[v] = repetitionVector[v] / g;
        }}

    {ForEachVertex(from,v) {
            repetitionVector[v] = repetitionVector[v] * ratePeriod;
        }}

#ifdef CRAZY
    // Workaround for repetition vector issues
    EXEC_COUNT subrate = ratePeriod;
    {ForEachVertex(from,v) {
            subrate =  std::gcd(subrate, repetitionVector[v] / from->getPhasesQuantity(v));
        }}
    VERBOSE_INFO("SubRate = " << subrate);

    {ForEachVertex(from,v) {
            repetitionVector[v] = repetitionVector[v] / subrate;
        }}
#endif

    VERBOSE_DEBUG("Repetition Vector :");
    {ForEachVertex(from,v) {
            from->setNi(v,repetitionVector[v]);
            VERBOSE_DEBUG(from->getVertexName(v) << " \t: " << repetitionVector[v] / from->getPhasesQuantity(v) << "\tx " << from->getPhasesQuantity(v) << "\t = " << repetitionVector[v] << " \t(" << ((from->getReentrancyFactor(v) > 0)?"lb":"") << ")");
        }}

    return true;
}


#include <models/NaiveDataflow.h>

bool generic_calc_fractions_connected_actors(const models::NaiveDataflow *from, std::map<models::vertex_id_t ,EXEC_COUNT_FRACT> &fractions, models::NaiveVertexRef a, TOKEN_UNIT ratePeriod) {

    EXEC_COUNT_FRACT fractionA = fractions.at(from->getVertexId(a));

    // Inconsistent graph?
    if (fractionA == EXEC_COUNT_FRACT(0,1)) {
        VERBOSE_ERROR("fractionA == EXEC_COUNT_FRACT(0,1)");
        return false;
    }

    // Calculate the rate for each actor 'b' connected to actor 'a'
    for(auto c : from->getConnectedEdges(a)) {
                auto src  = from->getEdgeSource(c);
                auto dest = from->getEdgeTarget(c);

                auto b = (src==a) ? dest : src;

                // Tokens produced or consumed by actor A in one rate period
                TOKEN_UNIT rateA = from->getEdgeIn(c) ;
                rateA = rateA * (ratePeriod / from->getEdgeInPhasesCount(c));

                // Tokens produced or consumed by actor B in one rate period
                TOKEN_UNIT rateB =  from->getEdgeOut(c);
                rateB = rateB * (ratePeriod / from->getEdgeOutPhasesCount(c));

                if (dest==a) {
                    TOKEN_UNIT tmp = rateB;
                    rateB = rateA;
                    rateA = tmp;
                }

                // Graph inconsistent?
                if (rateA == 0 || rateB == 0)
                {
                    fractions.clear();
                    VERBOSE_ERROR("rateA == 0 || rateB == 0");
                    return false;
                }

                // Calculate firing rate 'b'
                EXEC_COUNT_FRACT ratioAB = EXEC_COUNT_FRACT(rateA, rateB);
                EXEC_COUNT_FRACT fractionB = fractionA * ratioAB;

                // Known firing rate for 'b'
                EXEC_COUNT_FRACT knownFractionB = fractions[from->getVertexId(b)];

                // Compare known and calculated firing rate of 'b'
                if (knownFractionB != EXEC_COUNT_FRACT(0)
                    && fractionB != knownFractionB)  {
                    // Inconsistent graph, set all fractions to 0
                    fractions.clear();
                    VERBOSE_ERROR("(knownFractionB(" << knownFractionB << ") != EXEC_COUNT_FRACT(0,1) && fractionB(" << fractionB << ") != knownFractionB)");
                    return false;
                }
                else if (knownFractionB == EXEC_COUNT_FRACT(0,1))
                {
                    // Set the firing rate of actor 'b'
                    fractions[from->getVertexId(b)] = fractionB;

                    // Calculate firing rate for all actors connnected to 'b'
                    generic_calc_fractions_connected_actors(from, fractions, b, ratePeriod);

                    // Is graph inconsistent?
                    if (fractions[from->getVertexId(b)] == EXEC_COUNT_FRACT(0,1))
                    {
                        VERBOSE_ERROR("(fractions[b] == EXEC_COUNT_FRACT(0,1))");
                        return false;
                    }
                }
            }
    return true;
}


std::map<models::vertex_id_t ,EXEC_COUNT> generic_calc_repetition_vector(models::NaiveDataflow *from,std::map<models::vertex_id_t ,EXEC_COUNT_FRACT>& fractions, EXEC_COUNT ratePeriod) {

    std::map<models::vertex_id_t ,EXEC_COUNT> repetitionVector;
    EXEC_COUNT l = 1;

    // Find lowest common multiple (lcm) of all denominators
    for (auto v : from->getVertices()) {
            l = std::lcm(l,fractions[from->getVertexId(v)].denominator());
    }

    // Zero vector?
    if (l == 0) {
        VERBOSE_ERROR("Zero vector ?");
        VERBOSE_FAILURE();
    }


    // Calculate non-zero repetition vector
    for (auto v : from->getVertices()) {
            repetitionVector[from->getVertexId(v)] = (fractions[from->getVertexId(v)].numerator() * l) /  fractions[from->getVertexId(v)].denominator();
    }


    // Find greatest common divisor (gcd)
    EXEC_COUNT g = repetitionVector.begin()->second;

    for (auto v : from->getVertices()) {
            g = std::gcd(g, repetitionVector[from->getVertexId(v)]);
    }

    VERBOSE_ASSERT(g > 0, TXT_NEVER_HAPPEND);

    // Minimize the repetition vector using the gcd
    for (auto v : from->getVertices()) {
            repetitionVector[from->getVertexId(v)] = repetitionVector[from->getVertexId(v)] / g;
    }

    for (auto v : from->getVertices()) {
            repetitionVector[from->getVertexId(v)] = repetitionVector[from->getVertexId(v)] * ratePeriod;
    }

#ifdef CRAZY
    // Workaround for repetition vector issues
    EXEC_COUNT subrate = ratePeriod;
    for (auto v : from->getVertices()) {
            subrate =  std::gcd(subrate, repetitionVector[from->getVertexId(v)] / from->getPhasesQuantity(v));
    }
    VERBOSE_INFO("SubRate = " << subrate);

    for (auto v : from->getVertices()) {
            repetitionVector[from->getVertexId(v)] = repetitionVector[from->getVertexId(v)] / subrate;
    }
#endif

    return repetitionVector;
}


/**
 * getRepetitionVector ()
 * The function calculates and returns the repetition vector of the graph.
 *
 */
bool computeRepetitionVector(models::Dataflow *from) {
	if (from->has_repetition_vector()) return true;
	from->set_read_only();
    std::map<Vertex,EXEC_COUNT_FRACT> fractions;
    {ForEachVertex(from,v) {
    	fractions[v] =  EXEC_COUNT_FRACT(0,1);
    }}
    EXEC_COUNT ratePeriod = 1;

    // Compute period of repetition for rate vectors (dangerous way ...)
    {ForEachEdge(from,c) {
    	ratePeriod = std::lcm(ratePeriod,from->getEdgeInPhasesCount(c));
    	ratePeriod = std::lcm(ratePeriod,from->getEdgeOutPhasesCount(c));
    }}
    VERBOSE_ASSERT(ratePeriod > 0 , TXT_NEVER_HAPPEND);
    VERBOSE_INFO("Rate Period = " << ratePeriod);


    // Calculate firing ratio (as fraction) for each actor
    {ForEachVertex(from,v) {
    	if (fractions[v] == EXEC_COUNT_FRACT(0,1)) {
    		 fractions[v] = EXEC_COUNT_FRACT(1,1);
        	 if (!calcFractionsConnectedActors(from, fractions, v, ratePeriod)) return false;
    	}
    }}

    // Calculate repetition vector based on firing ratios
    bool res =  calcRepetitionVector(from,fractions, ratePeriod);
    if (res) from->set_repetition_vector();
    return res;
}

bool generic_repetition_vector (models::NaiveDataflow *from) {

    std::map<models::vertex_id_t ,EXEC_COUNT_FRACT> fractions;

    for (auto v : from->getVertices()) {
        fractions[from->getVertexId(v)] = EXEC_COUNT_FRACT(0, 1);
    }
    EXEC_COUNT ratePeriod = 1;

    // Compute period of repetition for rate vectors (dangerous way ...)
    for (auto c : from->getEdges()) {
            ratePeriod = std::lcm(ratePeriod,from->getEdgeInPhasesCount(c));
            ratePeriod = std::lcm(ratePeriod,from->getEdgeOutPhasesCount(c));
    }
    VERBOSE_ASSERT(ratePeriod > 0 , TXT_NEVER_HAPPEND);
    VERBOSE_INFO("Rate Period = " << ratePeriod);


    // Calculate firing ratio (as fraction) for each actor
    for (auto v : from->getVertices()) {
            if (fractions[from->getVertexId(v)] == EXEC_COUNT_FRACT(0,1)) {
                fractions[from->getVertexId(v)] = EXEC_COUNT_FRACT(1,1);
                if (!generic_calc_fractions_connected_actors(from, fractions, v, ratePeriod)) return false;
            }
        }


    VERBOSE_INFO("Calc done");

    // Calculate repetition vector based on firing ratios
    auto repetitionVector =  generic_calc_repetition_vector(from,fractions, ratePeriod);
    VERBOSE_INFO("Repetition Vector done");
    for (auto v : from->getVertices()) {
            from->setNi(v,repetitionVector[from->getVertexId(v)]);
            VERBOSE_DEBUG(from->getVertexName(v) << " \t: " << repetitionVector[from->getVertexId(v)] / from->getPhasesQuantity(v) << "\tx " << from->getPhasesQuantity(v) << "\t = " << repetitionVector[from->getVertexId(v)] << " \t(" << ((from->getReentrancyFactor(v) > 0)?"lb":"") << ")");
    }

    return true;
}
// template <> bool generic_repetition_vector  (models::NaiveDataflow *) ;
