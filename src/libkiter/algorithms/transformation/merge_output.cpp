/*
 * merge_output.cpp
 *
 *  Created on: 28 Oct 2024
 *      Author: jkmingwen
 */

#include <models/Dataflow.h>
#include "merge_output.h"
#include "algorithms/transformation/merge_operators.h"
#include "algorithms/vhdl_generation/VHDLCommons.h"
#include "commons/commons.h"

void algorithms::transformation::merge_output    (models::Dataflow* const dataflow, parameters_list_t  parameters  ) {

	// Argument should specify name of source actor
	if (parameters.find("name") == parameters.end() ) {
		VERBOSE_ERROR("Please specify the 'name' argument.");
		return;
        }
        Vertex src = dataflow->getVertexByName(parameters["name"]);
        std::string srcBaseName = getBaseName(dataflow->getVertexName(src));
        Vertex bcSrc = dataflow->addVertex("broadcast" + srcBaseName);
        dataflow->setVertexType(bcSrc, "broadcast");
        std::map<std::string, int> outputTypes;
        std::vector<std::string> toRemove;
        {ForOutputEdges(dataflow, src, outEdge) {
            // save parameters of edge so we can remap it
            std::string edgeName = dataflow->getEdgeName(outEdge);
            Vertex edgeTarget = dataflow->getEdgeTarget(outEdge);
            std::vector<TOKEN_UNIT> inPhases = dataflow->getEdgeInVector(outEdge);
            std::vector<TOKEN_UNIT> outPhases =
                dataflow->getEdgeOutVector(outEdge);
            TOKEN_UNIT initTokens = dataflow->getPreload(outEdge);

            // replace output edge
            dataflow->setEdgeName(outEdge, "old" + edgeName); // placeholder allows us to reuse edge name
            toRemove.push_back("old" + edgeName); // store for later removal
            Edge bcEdge = dataflow->addEdge(bcSrc, edgeTarget, edgeName);
            dataflow->setEdgeInPhases(bcEdge, inPhases);
            dataflow->setEdgeOutPhases(bcEdge, outPhases);
            dataflow->setEdgeInputPortName(bcEdge, "in_" + edgeName);
            dataflow->setEdgeOutputPortName(bcEdge, "out_" + edgeName);
            dataflow->setPreload(bcEdge, initTokens);

            // rename target to reflect broadcast as source
            std::vector<std::string> oldNames = commons::split<std::string>(dataflow->getVertexName(src), '_');
            std::vector<std::string> names = commons::split<std::string>(dataflow->getVertexName(bcSrc), '_');
            for (auto i = 0; i < oldNames.size(); i++) { oldNames.at(i) = splitByString(oldNames.at(i), "PARAM").front(); }
            for (auto i = 0; i < names.size(); i++) { names.at(i) = splitByString(names.at(i), "PARAM").front(); }
            std::string newName =
                replaceActorName(dataflow->getVertexName(edgeTarget),
                                 oldNames.front(), names.front());
            dataflow->setVertexName(edgeTarget, newName);

            // log edge data type
            if (edgeName.substr(edgeName.find_last_of('_') + 1) == "vect") {
              edgeName.erase(edgeName.find_last_of('_'), std::string::npos);
            }
            outputTypes[edgeName.substr(edgeName.find_last_of('_') + 1)]++;
          }}
        // delete only after iterating through all edges so loop doesn't break
        for (auto &name : toRemove) {
          dataflow->removeEdge(dataflow->getEdgeByName(name));
        }

        // add edge from original source to broadcast actor
        bool isInt = false, isFloat = false;
        std::string bcEdgeName = "broadcast_" + srcBaseName;
        for (auto &type : outputTypes) {
          if (type.first == "real") {
            isFloat = true;
          } else if (type.first == "int") {
            isInt = true;
          } else {
            VERBOSE_ERROR("Output type error on " << dataflow->getVertexName(src));
            VERBOSE_ERROR("\tUndefined output type detected: "
                          << type.first << ", input types: "
                          << commons::toString(outputTypes));
          }
        }
        if (isInt && isFloat) {
          VERBOSE_WARNING("\tBoth Int and Float types detected for "
                          << dataflow->getVertexName(src) << "("
                          << dataflow->getVertexType(src)
                          << "), setting type to floating point");
          bcEdgeName += "_real";
        } else if (isFloat) {
          bcEdgeName += "_real";
        } else if (isInt) {
          bcEdgeName += "_int";
        }
        Edge bcEdge = dataflow->addEdge(src, bcSrc, bcEdgeName);
        // hardcoded to 1 phase because I can't think of a better way to
        // determine this (each output edge could have a different number of phases)
        dataflow->setEdgeInPhases(bcEdge, {1});
        dataflow->setEdgeOutPhases(bcEdge, {1});
        dataflow->setEdgeInputPortName(bcEdge , "in_" + bcEdgeName);
        dataflow->setEdgeOutputPortName(bcEdge, "out_" + bcEdgeName);
        dataflow->setPhasesQuantity(bcSrc,1);
        dataflow->setVertexDuration(bcSrc, {1});
}


