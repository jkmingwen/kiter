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


	Vertex t = dataflow->getVertexByName(parameters["name"]);

	VERBOSE_INFO("Out degree = " << dataflow->getVertexOutDegree(t) );

	ARRAY_INDEX cnt = 0;
	while (dataflow->getVertexOutDegree(t) > 1) {

		VERBOSE_INFO("remove outputs..");


		// add a Duplicate,
                std::string newDup_name = dataflow->getVertexName(t) + "_Dup" + commons::toString(cnt++);
		Vertex newDup = dataflow->addVertex(newDup_name);
		dataflow->setPhasesQuantity(newDup,1);
		dataflow->setVertexDuration(newDup, {1});
		dataflow->setVertexType(newDup, "Proj");
		//connect one to duplicate from the task

		VERBOSE_INFO("Create new edge from source");
		Edge ne = dataflow->addEdge(t, newDup);
                // get one of the port names, and strip in/out to get port id and thus name edge
		dataflow->setEdgeInPhases(ne, {1});
		dataflow->setEdgeOutPhases(ne, {1});

		VERBOSE_INFO("Collect edges");
		Edge c1;
		Edge c2;
		int cnt = 0;
                std::string c1_type;
                std::string c2_type;
		{ForOutputEdges(dataflow,t,c)	{
			cnt++;
                        std::string edgeName = dataflow->getEdgeName(c);
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
                VERBOSE_INFO("Setting new output edge name to " << neName);
                dataflow->setEdgeName(ne, neName);
                dataflow->setEdgeInputPortName(ne, "in_" + neName);
                dataflow->setEdgeOutputPortName(ne, "out_" + neName);


		VERBOSE_INFO("Create new edge to targets");
		Edge nc1 = dataflow->addEdge(newDup,  dataflow->getEdgeTarget(c1));
		dataflow->setEdgeInPhases(nc1, {1});
		dataflow->setEdgeOutPhases(nc1, {1});
                // workaround for manual channel naming
                std::string nc1Name = "channel_" + commons::toString(dataflow->getEdgeId(nc1)) + "_1_" + dataflow->getVertexName(t) + "_" + c1_type;
                dataflow->setEdgeName(nc1, nc1Name);
                dataflow->setEdgeInputPortName(nc1, "in_" + nc1Name);
                dataflow->setEdgeOutputPortName(nc1, "out_" + nc1Name);
		Edge nc2 = dataflow->addEdge(newDup,  dataflow->getEdgeTarget(c2));
		dataflow->setEdgeInPhases(nc2, {1});
		dataflow->setEdgeOutPhases(nc2, {1});
                // workaround for manual channel naming
                std::string nc2Name = "channel_" + commons::toString(dataflow->getEdgeId(nc2)) + "_2_" + dataflow->getVertexName(t) + "_" + c2_type;
                dataflow->setEdgeName(nc2, nc2Name);
                dataflow->setEdgeInputPortName(nc2, "in_" + nc2Name);
                dataflow->setEdgeOutputPortName(nc2, "out_" + nc2Name);

		// remote the two buffer
		VERBOSE_INFO("Remove edges");
		dataflow->removeEdge(c1);
		dataflow->removeEdge(c2);



	}





}


