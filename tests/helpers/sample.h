/*
 * sample.h
 *
 *  Created on: Dec 3, 2019
 *      Author: toky
 */

#ifndef TESTS_HELPERS_SAMPLE_H_
#define TESTS_HELPERS_SAMPLE_H_


#include <models/Dataflow.h>

models::Dataflow * generateDAC2013 () {

    models::Dataflow* new_graph = new models::Dataflow();

    {
        auto new_vertex = new_graph->addVertex(1);
        new_graph->setVertexName(new_vertex,"t1");
        new_graph->setInitPhasesQuantity(new_vertex,0);
        new_graph->setPhasesQuantity(new_vertex,3);
        new_graph->setReentrancyFactor(new_vertex,1);
        new_graph->setVertexDuration(new_vertex,{1,1,1});
    }
    {
        auto new_vertex = new_graph->addVertex(2);
        new_graph->setVertexName(new_vertex,"t2");
        new_graph->setInitPhasesQuantity(new_vertex,0);
        new_graph->setPhasesQuantity(new_vertex,1);
        new_graph->setReentrancyFactor(new_vertex,1);
        new_graph->setVertexDuration(new_vertex,{1});
    }
    {
        auto new_vertex = new_graph->addVertex(3);
        new_graph->setVertexName(new_vertex,"t3");
        new_graph->setInitPhasesQuantity(new_vertex,0);
        new_graph->setPhasesQuantity(new_vertex,2);
        new_graph->setReentrancyFactor(new_vertex,1);
        new_graph->setVertexDuration(new_vertex,{1,1});
    }
    {
        auto new_vertex = new_graph->addVertex(4);
        new_graph->setVertexName(new_vertex,"t4");
        new_graph->setInitPhasesQuantity(new_vertex,0);
        new_graph->setPhasesQuantity(new_vertex,2);
        new_graph->setReentrancyFactor(new_vertex,1);
        new_graph->setVertexDuration(new_vertex,{1,1});
    }
    {
        auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(2));
        new_graph->setEdgeInPhases(new_edge,{0,0,2});
        new_graph->setEdgeOutPhases(new_edge,{3});
        new_graph->setPreload(new_edge,3);
        new_graph->setEdgeName(new_edge,"a1");
    }
    {
        auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(3));
        new_graph->setEdgeInPhases(new_edge,{5});
        new_graph->setEdgeOutPhases(new_edge,{1,1});
        new_graph->setPreload(new_edge,1);
        new_graph->setEdgeName(new_edge,"a2");
    }
    {
        auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(1));
        new_graph->setEdgeInPhases(new_edge,{1,2});
        new_graph->setEdgeOutPhases(new_edge,{1,4,0});
        new_graph->setPreload(new_edge,2);
        new_graph->setEdgeName(new_edge,"a3");
    }
    {
        auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(4));
        new_graph->setEdgeInPhases(new_edge,{2,3,0});
        new_graph->setEdgeOutPhases(new_edge,{3,3});
        new_graph->setPreload(new_edge,0);
        new_graph->setEdgeName(new_edge,"a4");
    }
    {
        auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(2));
        new_graph->setEdgeInPhases(new_edge,{0,4});
        new_graph->setEdgeOutPhases(new_edge,{5});
        new_graph->setPreload(new_edge,7);
        new_graph->setEdgeName(new_edge,"a5");
    }

    return new_graph;
}


models::Dataflow * generateNormDAC2013 () {

    models::Dataflow* new_graph = new models::Dataflow();

    {
        auto new_vertex = new_graph->addVertex(1);
        new_graph->setVertexName(new_vertex,"t1");
        new_graph->setInitPhasesQuantity(new_vertex,0);
        new_graph->setPhasesQuantity(new_vertex,3);
        new_graph->setReentrancyFactor(new_vertex,1);
        new_graph->setVertexDuration(new_vertex,{1,1,1});
    }
    {
        auto new_vertex = new_graph->addVertex(2);
        new_graph->setVertexName(new_vertex,"t2");
        new_graph->setInitPhasesQuantity(new_vertex,0);
        new_graph->setPhasesQuantity(new_vertex,1);
        new_graph->setReentrancyFactor(new_vertex,1);
        new_graph->setVertexDuration(new_vertex,{1});
    }
    {
        auto new_vertex = new_graph->addVertex(3);
        new_graph->setVertexName(new_vertex,"t3");
        new_graph->setInitPhasesQuantity(new_vertex,0);
        new_graph->setPhasesQuantity(new_vertex,2);
        new_graph->setReentrancyFactor(new_vertex,1);
        new_graph->setVertexDuration(new_vertex,{1,1});
    }
    {
        auto new_vertex = new_graph->addVertex(4);
        new_graph->setVertexName(new_vertex,"t4");
        new_graph->setInitPhasesQuantity(new_vertex,0);
        new_graph->setPhasesQuantity(new_vertex,2);
        new_graph->setReentrancyFactor(new_vertex,1);
        new_graph->setVertexDuration(new_vertex,{1,1});
    }
    {
        auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(2));
        new_graph->setEdgeInPhases(new_edge,{0,0,10});
        new_graph->setEdgeOutPhases(new_edge,{15});
        new_graph->setPreload(new_edge,15);
        new_graph->setEdgeName(new_edge,"a1");
    }
    {
        auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(3));
        new_graph->setEdgeInPhases(new_edge,{15});
        new_graph->setEdgeOutPhases(new_edge,{3,3});
        new_graph->setPreload(new_edge,3);
        new_graph->setEdgeName(new_edge,"a2");
    }
    {
        auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(1));
        new_graph->setEdgeInPhases(new_edge,{2,4});
        new_graph->setEdgeOutPhases(new_edge,{2,8,0});
        new_graph->setPreload(new_edge,4);
        new_graph->setEdgeName(new_edge,"a3");
    }
    {
        auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(4));
        new_graph->setEdgeInPhases(new_edge,{4,6,0});
        new_graph->setEdgeOutPhases(new_edge,{6,6});
        new_graph->setPreload(new_edge,0);
        new_graph->setEdgeName(new_edge,"a4");
    }
    {
        auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(2));
        new_graph->setEdgeInPhases(new_edge,{0,12});
        new_graph->setEdgeOutPhases(new_edge,{15});
        new_graph->setPreload(new_edge,21);
        new_graph->setEdgeName(new_edge,"a5");
    }

    return new_graph;
}

models::Dataflow * generateSampleCycle () {
	// Auto-generate by Kiter for Kiter
	//

	models::Dataflow* new_graph = new models::Dataflow();

	{
	auto new_vertex = new_graph->addVertex(1);
	 new_graph->setVertexName(new_vertex,"A");
	 new_graph->setInitPhasesQuantity(new_vertex,0);
	 new_graph->setPhasesQuantity(new_vertex,2);
	 new_graph->setReentrancyFactor(new_vertex,1);
	 new_graph->setVertexDuration(new_vertex,{3,1});
	}
	{
	auto new_vertex = new_graph->addVertex(2);
	 new_graph->setVertexName(new_vertex,"B");
	 new_graph->setInitPhasesQuantity(new_vertex,0);
	 new_graph->setPhasesQuantity(new_vertex,3);
	 new_graph->setReentrancyFactor(new_vertex,1);
	 new_graph->setVertexDuration(new_vertex,{2,1,2});
	}
	{
	auto new_vertex = new_graph->addVertex(3);
	 new_graph->setVertexName(new_vertex,"C");
	 new_graph->setInitPhasesQuantity(new_vertex,0);
	 new_graph->setPhasesQuantity(new_vertex,1);
	 new_graph->setReentrancyFactor(new_vertex,1);
	 new_graph->setVertexDuration(new_vertex,{1});
	}
//    {
//    auto new_vertex = new_graph->addVertex(4);
//     new_graph->setVertexName(new_vertex,"D");
//     new_graph->setInitPhasesQuantity(new_vertex,0);
//     new_graph->setPhasesQuantity(new_vertex,2);
//     new_graph->setReentrancyFactor(new_vertex,1);
//     new_graph->setVertexDuration(new_vertex,{3,1});
//    }
	{
	auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(2));
	 new_graph->setEdgeInPhases(new_edge,{3,5});
	 new_graph->setEdgeOutPhases(new_edge,{1,1,4});
	 new_graph->setPreload(new_edge,0);
	 new_graph->setEdgeName(new_edge,"channel_1");
	}
	{
	auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(3));
	 new_graph->setEdgeInPhases(new_edge,{6,2,1});
	 new_graph->setEdgeOutPhases(new_edge,{6});
	 new_graph->setPreload(new_edge,0);
	 new_graph->setEdgeName(new_edge,"channel_2");
	}
	{
	auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(4));
	 new_graph->setEdgeInPhases(new_edge,{2});
	 new_graph->setEdgeOutPhases(new_edge,{1,3});
	 new_graph->setPreload(new_edge,4);
	 new_graph->setEdgeName(new_edge,"channel_3");
	}
//    {
//    auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(1));
//     new_graph->setEdgeInPhases(new_edge,{2});
//     new_graph->setEdgeOutPhases(new_edge,{1,3});
//     new_graph->setPreload(new_edge,0);
//     new_graph->setEdgeName(new_edge,"channel_4");
//    }

	return new_graph;
}


models::Dataflow * generateSamplePipeline () {
	// Auto-generate by Kiter for Kiter
	//
	models::Dataflow* new_graph = new models::Dataflow();

	{
		auto new_vertex = new_graph->addVertex(1);
		new_graph->setVertexName(new_vertex,"a");
		new_graph->setPhasesQuantity(new_vertex,1);
		new_graph->setReentrancyFactor(new_vertex,1);
		new_graph->setVertexDuration(new_vertex,{1});
	}
	{
		auto new_vertex = new_graph->addVertex(2);
		new_graph->setVertexName(new_vertex,"b");
		new_graph->setPhasesQuantity(new_vertex,1);
		new_graph->setReentrancyFactor(new_vertex,1);
		new_graph->setVertexDuration(new_vertex,{2});
	}
	{
		auto new_vertex = new_graph->addVertex(3);
		new_graph->setVertexName(new_vertex,"c");
		new_graph->setPhasesQuantity(new_vertex,1);
		new_graph->setReentrancyFactor(new_vertex,1);
		new_graph->setVertexDuration(new_vertex,{2});
	}
	{
		auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(2));
		new_graph->setEdgeInPhases(new_edge,{2});
		new_graph->setEdgeOutPhases(new_edge,{3});
		new_graph->setPreload(new_edge,0);
		new_graph->setEdgeName(new_edge,"alpha");
	}
	{
		auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(3));
		new_graph->setEdgeInPhases(new_edge,{1});
		new_graph->setEdgeOutPhases(new_edge,{2});
		new_graph->setPreload(new_edge,0);
		new_graph->setEdgeName(new_edge,"beta");
	}

	return new_graph;
}


models::Dataflow* generateSampleMultSCC () {
  models::Dataflow* new_graph = new models::Dataflow();

  {
    auto new_vertex = new_graph->addVertex(1);
    new_graph->setVertexName(new_vertex, "A");
    new_graph->setInitPhasesQuantity(new_vertex,0);
    new_graph->setPhasesQuantity(new_vertex,2);
    new_graph->setReentrancyFactor(new_vertex,1);
    new_graph->setVertexDuration(new_vertex,{1,1});
  }
  {
    auto new_vertex = new_graph->addVertex(2);
    new_graph->setVertexName(new_vertex,"B");
    new_graph->setInitPhasesQuantity(new_vertex,0);
    new_graph->setPhasesQuantity(new_vertex,3);
    new_graph->setReentrancyFactor(new_vertex,1);
    new_graph->setVertexDuration(new_vertex,{1,1,1});
  }
  {
    auto new_vertex = new_graph->addVertex(3);
    new_graph->setVertexName(new_vertex,"C");
    new_graph->setInitPhasesQuantity(new_vertex,0);
    new_graph->setPhasesQuantity(new_vertex,1);
    new_graph->setReentrancyFactor(new_vertex,1);
    new_graph->setVertexDuration(new_vertex,{1});
  }
  {
    auto new_vertex = new_graph->addVertex(4);
    new_graph->setVertexName(new_vertex,"D");
    new_graph->setInitPhasesQuantity(new_vertex,0);
    new_graph->setPhasesQuantity(new_vertex,1);
    new_graph->setReentrancyFactor(new_vertex,1);
    new_graph->setVertexDuration(new_vertex,{1});
  }
  {
    auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(2));
    new_graph->setEdgeInPhases(new_edge,{3,5});
    new_graph->setEdgeOutPhases(new_edge,{1,1,4});
    new_graph->setPreload(new_edge,0);
    new_graph->setEdgeName(new_edge,"channel_1");
  }
  {
    auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(1));
    new_graph->setEdgeInPhases(new_edge,{2});
    new_graph->setEdgeOutPhases(new_edge,{1,3});
    new_graph->setPreload(new_edge,4);
    new_graph->setEdgeName(new_edge,"channel_2");
  }
  {
    auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(4));
    new_graph->setEdgeInPhases(new_edge,{3,5});
    new_graph->setEdgeOutPhases(new_edge,{24});
    new_graph->setPreload(new_edge,13);
    new_graph->setEdgeName(new_edge,"channel_3");
  }
  {
    auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(3));
    new_graph->setEdgeInPhases(new_edge,{36});
    new_graph->setEdgeOutPhases(new_edge,{6});
    new_graph->setPreload(new_edge,6);
    new_graph->setEdgeName(new_edge,"channel_4");
  }

  return new_graph;

}

#endif /* TESTS_HELPERS_SAMPLE_H_ */
