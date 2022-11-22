/*
 * singleOutput.cpp
 *
 *  Created on: 12 Aug 2021
 *      Author: toky
 */

#include <models/Dataflow.h>
#include "singleOutput.h"

void algorithms::transformation::singleOutput    (models::Dataflow* const dataflow, parameters_list_t  parameters  ) {

	// Check arguments

	if (parameters.find("name") == parameters.end() ) {
		VERBOSE_ERROR("Please specify the 'name' argument.");
		return;
	}

        // split into substrings, if necessary
        std::vector<std::string> verticesToMerge;
        std::string argument = parameters["name"];
        if (argument.find(",") != std::string::npos) {
          VERBOSE_DEBUG("List of vertex names detected, splitting into substrings with \",\" as delimiter...");
          size_t pos = argument.find(",");
          while ((pos = argument.find(",")) != std::string::npos) {
            std::string vArg = argument.substr(0, pos);
              VERBOSE_DEBUG("\tVertex name: " << vArg);
            verticesToMerge.push_back(vArg);
            argument.erase(0, pos + 1);
          }
        }
        verticesToMerge.push_back(argument); // add final substring to vector of arguments regardless
    VERBOSE_DEBUG("Found " << verticesToMerge.size() << " vertex names as arguments");
        for (auto &s : verticesToMerge) {
            VERBOSE_DEBUG("\t" << s);
        }
        for (unsigned i = 0; i < verticesToMerge.size(); i++) {
          Vertex t = dataflow->getVertexByName(verticesToMerge.at(i));

            VERBOSE_DEBUG("Out degree = " << dataflow->getVertexOutDegree(t) );

          ARRAY_INDEX cnt = 0;
          while (dataflow->getVertexOutDegree(t) > 1) {

              VERBOSE_DEBUG("remove outputs..");

            // add a Duplicate
            std::string newDupName = dataflow->getVertexName(t);
            std::string ogName = dataflow->getVertexName(t); // for updating target actor name
            if (newDupName.find("_") != std::string::npos) { // only use the base name for updating target actor name
              newDupName = newDupName.substr(0, newDupName.find("_"));
              ogName = newDupName;
                VERBOSE_DEBUG("Base name of actor whose output we're merging: " << newDupName);
            }
            newDupName = newDupName  + "Dup" + commons::toString(cnt++);
            Vertex newDup = dataflow->addVertex(newDupName);
            dataflow->setPhasesQuantity(newDup,1);
            dataflow->setVertexDuration(newDup, {1});
            dataflow->setVertexType(newDup, "Proj");
            //connect one to duplicate from the task

              VERBOSE_DEBUG("Create new edge from source");
            Edge ne = dataflow->addEdge(t, newDup);
            // get one of the port names, and strip in/out to get port id and thus name edge
            dataflow->setEdgeInPhases(ne, {1});
            dataflow->setEdgeOutPhases(ne, {1});

              VERBOSE_DEBUG("Collect edges");
            Edge c1;
            Edge c2;
            int cnt = 0;
            std::string c1_type;
            std::string c2_type;
            {ForOutputEdges(dataflow,t,c)	{
                cnt++;
                std::string edgeName = dataflow->getEdgeName(c);
                // ignore "vect" to find the actual data type
                if (edgeName.substr(edgeName.find_last_of("_") + 1) == "vect") {
                  edgeName.erase(edgeName.find_last_of("_"), std::string::npos);
                }
                if (cnt == 1) {
                  c1 =  c;
                  c1_type = edgeName.substr(edgeName.find_last_of("_") + 1);
                }
                if (cnt == 2) {
                  c2 =  c;
                  c2_type = edgeName.substr(edgeName.find_last_of("_") + 1);
                }
                if (cnt >= 3) break;
              }}
            // workaround for manual channel naming
            std::string neName = "channel_" + commons::toString(dataflow->getEdgeId(ne)) + "_0_" + dataflow->getVertexName(t) + "_" + c1_type; // NOTE assuming here that all outputs are the same type
              VERBOSE_DEBUG("Setting new output edge name to " << neName);
            dataflow->setEdgeName(ne, neName);
            dataflow->setEdgeInputPortName(ne, "in_" + neName);
            dataflow->setEdgeOutputPortName(ne, "out_" + neName);


              VERBOSE_DEBUG("Create new edge to targets");
            Edge nc1 = dataflow->addEdge(newDup,  dataflow->getEdgeTarget(c1));
            dataflow->setEdgeInPhases(nc1, {1});
            dataflow->setEdgeOutPhases(nc1, {1});
            // workaround for manual channel naming
            std::string nc1Name = "channel_" + commons::toString(dataflow->getEdgeId(nc1)) + "_1_" + dataflow->getVertexName(t) + "_" + c1_type;
            dataflow->setEdgeName(nc1, nc1Name);
            dataflow->setEdgeInputPortName(nc1, "in_" + nc1Name);
            dataflow->setEdgeOutputPortName(nc1, "out_" + nc1Name);
            // update name of target actor if its name defines input arg order (i.e. actorname_arg1_arg2)
            /* iterate through the name of the target actor of c1/c2
               (assigned to nc1/2TargetName); once we find a string that
               matches the name of our original source actor (ogName),
               replace that with the name of the new source actor (newDupName) */
            /* NOTE current implementation is designed to only replace the first instance
               of a substring matching the original source actor name as we expect that multiple
               inputs from the same original actor would then come from another newly
               constructed source actor*/
            std::string nc1TargetName = dataflow->getVertexName(dataflow->getEdgeTarget(c1));
            std::string nc1OriginalName = nc1TargetName;
            if (nc1TargetName.find("_") != std::string::npos) {
                VERBOSE_DEBUG("Updating name of target actor from " << nc1TargetName << " (searching for " << ogName << ")");
                VERBOSE_DEBUG("New source name is " << newDupName);
              std::string nc1NewName;
              bool targetNameUpdated = false; // track when we've identified the part of the target name we need to update
              size_t pos = nc1TargetName.find("_");
              while ((pos = nc1TargetName.find("_")) != std::string::npos) {
                std::string partialName;
                partialName = nc1TargetName.substr(0, pos); // break up check into substrings delimited by underscore
                if (partialName == ogName && !targetNameUpdated) {
                    VERBOSE_DEBUG("\t\tFound matching partial name for " << ogName << ", changing it to " << newDupName);
                  partialName = newDupName; // update the part of the target name that we're replacing with the new actor
                  targetNameUpdated = true;
                }
                nc1NewName += partialName; // reconstruct new actor name by parts
                nc1TargetName.erase(0, pos + 1);
                if (nc1TargetName.find("_") != std::string::npos) {
                  nc1NewName += "_";
                } else {
                  if (nc1TargetName == ogName && !targetNameUpdated) { // separate check for last substring
                    partialName = newDupName;
                    targetNameUpdated = true;
                  } else {
                    partialName = nc1TargetName;
                  }
                  nc1NewName += "_" + partialName;
                }
                  VERBOSE_DEBUG("\t\tCurrent constructed new name: " << nc1NewName);
              }
                VERBOSE_DEBUG("\tto " << nc1NewName);
              dataflow->setVertexName(dataflow->getEdgeTarget(c1), nc1NewName);
              std::replace(verticesToMerge.begin(), verticesToMerge.end(), nc1OriginalName, nc1NewName);
            }
            Edge nc2 = dataflow->addEdge(newDup,  dataflow->getEdgeTarget(c2));
            dataflow->setEdgeInPhases(nc2, {1});
            dataflow->setEdgeOutPhases(nc2, {1});
            // workaround for manual channel naming
            std::string nc2Name = "channel_" + commons::toString(dataflow->getEdgeId(nc2)) + "_2_" + dataflow->getVertexName(t) + "_" + c2_type;
            dataflow->setEdgeName(nc2, nc2Name);
            dataflow->setEdgeInputPortName(nc2, "in_" + nc2Name);
            dataflow->setEdgeOutputPortName(nc2, "out_" + nc2Name);
            std::string nc2TargetName = dataflow->getVertexName(dataflow->getEdgeTarget(c2));
            std::string nc2OriginalName = nc2TargetName;
            if (nc2TargetName.find("_") != std::string::npos) {
                VERBOSE_DEBUG("Updating name of target actor from " << nc2TargetName << " (searching for " << ogName << ")");
                VERBOSE_DEBUG("New source name is " << newDupName);
              std::string nc2NewName;
              bool targetNameUpdated = false; // track when we've identified the part of the target name we need to update
              size_t pos = nc2TargetName.find("_");
              while ((pos = nc2TargetName.find("_")) != std::string::npos) { // separate check for last substring
                std::string partialName;
                partialName = nc2TargetName.substr(0, pos); // break up check into substrings delimited by underscore
                if (partialName == ogName && !targetNameUpdated) {
                    VERBOSE_DEBUG("\t\tFound matching partial name for " << ogName << ", changing it to " << newDupName);
                  partialName = newDupName; // update the part of the target name that we're replacing with the new actor
                  targetNameUpdated = true;
                }
                nc2NewName += partialName; // reconstruct new actor name by parts
                nc2TargetName.erase(0, pos + 1);
                if (nc2TargetName.find("_") != std::string::npos) {
                  nc2NewName += "_";
                } else {
                  if (nc2TargetName == ogName && !targetNameUpdated) {
                    partialName = newDupName;
                    targetNameUpdated = true;
                  } else {
                    partialName = nc2TargetName;
                  }
                  nc2NewName += "_" + partialName;
                }
                  VERBOSE_DEBUG("\t\tCurrent constructed new name: " << nc2NewName);
              }
                VERBOSE_DEBUG("\tto " << nc2NewName);
              dataflow->setVertexName(dataflow->getEdgeTarget(c2), nc2NewName);
              std::replace(verticesToMerge.begin(), verticesToMerge.end(), nc2OriginalName, nc2NewName);
            }
            // remote the two buffer
              VERBOSE_DEBUG("Remove edges");
            dataflow->removeEdge(c1);
            dataflow->removeEdge(c2);



          }
        }






}


