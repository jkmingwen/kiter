/*
 * VHDLMergeTest.cpp
 *
 *  Created on: Sep 6, 2023
 *      Author: toky
 */


#define BOOST_TEST_MODULE VHDLMergeTest
#include "helpers/test_classes.h"
#include <printers/stdout.h>
#include "helpers/mergetester.h"
#include <models/Dataflow.h>
#include <algorithms/schedulings.h>
#include <printers/SDF3Wrapper.h>
#include <algorithms/transformation/merging.h>


void merge_vhdl_operators (models::Dataflow *graph , std::string name_of_merge, Vertex v1, Vertex v2) {

    // Check same operators
    VERBOSE_ASSERT_EQUALS(graph->getVertexInDegree(v1),graph->getVertexInDegree(v2));
    VERBOSE_ASSERT_EQUALS(graph->getVertexOutDegree(v1) ,graph->getVertexOutDegree(v2));
    // VERBOSE_ASSERT_EQUALS(graph->getVertexPhaseDuration(v1) ,graph->getVertexPhaseDuration(v2));


    graph->reset_computation();
    //Create the merged operator + its input and output selector
    Vertex merged = graph->addVertex(name_of_merge);
    graph->setPhasesQuantity(merged, 1);
    graph->setVertexDuration(merged,graph->getVertexPhaseDuration(v1));

    size_t inDeg = graph->getVertexInDegree(v1);
    size_t outDeg = graph->getVertexOutDegree(v1);


    for (size_t i = 0 ; i < inDeg ; i++) {
        // TODO Check for name conflict
        std::string input_selector_name = "IS_" + name_of_merge  + "_" + commons::toString(i);
        auto new_is = graph->addVertex(input_selector_name);
        graph->setPhasesQuantity(new_is,2);
        graph->setVertexDuration(new_is, {1,1});
        auto e = graph->addEdge(new_is, merged, "IS_" + name_of_merge  + "_" + commons::toString(i));
        graph->setEdgeInPhases(e, {1,1});
        graph->setEdgeOutPhases(e, {1});
        graph->setPreload(e, 0);

    }

    for (size_t i = 0 ; i < outDeg ; i++) {
        // TODO Check for name conflict
        std::string output_selector_name = "OS_" + name_of_merge  + "_" + commons::toString(i);
        auto new_os = graph->addVertex(output_selector_name);
        graph->setPhasesQuantity(new_os,2);
        graph->setVertexDuration(new_os, {1,1});
        auto e = graph->addEdge(merged, new_os, output_selector_name);


        graph->setEdgeInPhases(e, {1});
        graph->setEdgeOutPhases(e, {1,1});
        graph->setPreload(e, 0);

    }

    // The input and output selectors are ready to be used


    auto v1_input_edges = graph->getInputEdges(v1);
    auto v2_input_edges = graph->getInputEdges(v2);

    auto v1_in_end = v1_input_edges.second;
    auto v2_in_end = v2_input_edges.second;
    auto v2_in_it = v2_input_edges.first;
    auto v1_in_it = v1_input_edges.first;

    for (size_t i = 0 ; i < inDeg ; i++) {
        std::string input_selector_name = "IS_" + name_of_merge  + "_" + commons::toString(i);
        Vertex input_selector = graph->getVertexByName(input_selector_name);
        Edge v1_edge = it2Edge(v1_in_it); v1_in_it++;
        Edge v2_edge = it2Edge(v2_in_it); v2_in_it++;

        Vertex v1_source = graph->getEdgeSource(v1_edge);
        Vertex v2_source = graph->getEdgeSource(v2_edge);

        Edge e1 = graph->addEdge(v1_source, input_selector,input_selector_name + "_V1_source");
        graph->setEdgeInPhases(e1, graph->getEdgeInVector(v1_edge));
        graph->setEdgeOutPhases(e1, {1,0});
        graph->setPreload(e1, 0);


        Edge e2 = graph->addEdge(v2_source, input_selector,input_selector_name + "_V2_source");
        graph->setEdgeInPhases(e2, graph->getEdgeInVector(v2_edge));
        graph->setEdgeOutPhases(e2, {0,1});
        graph->setPreload(e2, 0);
    }






    auto v1_output_edges = graph->getOutputEdges(v1);
    auto v2_output_edges = graph->getOutputEdges(v2);
    auto v1_out_end = v1_output_edges.second;
    auto v2_out_end = v2_output_edges.second;
    auto v2_out_it = v2_output_edges.first;
    auto v1_out_it = v1_output_edges.first;
    for (size_t j = 0 ; j < outDeg ; j++) {
        std::string output_selector_name = "OS_" + name_of_merge  + "_" + commons::toString(j);
        Vertex output_selector = graph->getVertexByName(output_selector_name);
        Edge v1_edge = it2Edge(v1_out_it); v1_out_it++;
        Edge v2_edge = it2Edge(v2_out_it); v2_out_it++;
        Vertex v1_target = graph->getEdgeTarget(v1_edge);
        Vertex v2_target = graph->getEdgeTarget(v2_edge);

        Edge e1 = graph->addEdge(output_selector , v1_target, output_selector_name + "V1_target");
        graph->setEdgeOutPhases(e1, graph->getEdgeOutVector(v1_edge));
        graph->setEdgeInPhases(e1, {1,0});
        graph->setPreload(e1, 0);

        Edge e2 = graph->addEdge(output_selector, v2_target, output_selector_name + "V2_target");
        graph->setEdgeOutPhases(e2, graph->getEdgeOutVector(v2_edge));
        graph->setEdgeInPhases(e2, {0,1});
        graph->setPreload(e2, 0);
    }

    auto id = graph->getVertexId(v2);
    graph->removeVertex(v1);
    graph->removeVertex(graph->getVertexById(id));
}


BOOST_FIXTURE_TEST_SUITE( use_case_test , WITH_VERBOSE)

BOOST_AUTO_TEST_CASE( vhdl_merge_test )
{

    // Create the input graph
	models::Dataflow graph;
    graph.addVertex("in");
    auto A = graph.addVertex("A");
    graph.addVertex("B");
    auto C = graph.addVertex("C");
    graph.addVertex("D");
    graph.addVertex("E");
    graph.addVertex("F");
    graph.addVertex("G");
    graph.addVertex("H");
    graph.addVertex("J");
    graph.addVertex("K");
    graph.addVertex("out");
    auto e1 = graph.addEdge(graph.getVertexByName("in"), graph.getVertexByName("A"), "e1");
    auto e2 = graph.addEdge(graph.getVertexByName("A"), graph.getVertexByName("B"), "e2");
    auto e3 = graph.addEdge(graph.getVertexByName("B"), graph.getVertexByName("C"), "e3");
    auto e4 = graph.addEdge(graph.getVertexByName("C"), graph.getVertexByName("D"), "e4");
    auto e5 = graph.addEdge(graph.getVertexByName("D"), graph.getVertexByName("J"), "e5");
    auto e6 = graph.addEdge(graph.getVertexByName("J"), graph.getVertexByName("out"), "e6");

    auto e7 = graph.addEdge(graph.getVertexByName("E"), graph.getVertexByName("A"), "e7");
    auto e8 = graph.addEdge(graph.getVertexByName("G"), graph.getVertexByName("B"), "e8");
    auto e9 = graph.addEdge(graph.getVertexByName("F"), graph.getVertexByName("C"), "e9");
    auto e10 = graph.addEdge(graph.getVertexByName("H"), graph.getVertexByName("D"), "e10");
    auto e11 = graph.addEdge(graph.getVertexByName("K"), graph.getVertexByName("J"), "e11");

    {ForEachVertex(&graph, v) {
        graph.setPhasesQuantity(v, 1);
        graph.setVertexDuration(v, {1});
    }}

    {ForEachEdge(&graph, e) {
        graph.setEdgeInPhases(e, {1});
        graph.setEdgeOutPhases(e, {1});
        graph.setPreload(e, 0);
    }}

    // printers::printXML(&graph);

    std::ofstream mfile;
    mfile.open ("/tmp/input.dot");
    mfile << printers::GenerateGraphDOT(&graph) << std::endl;
    mfile.close();

    merge_vhdl_operators (&graph, "AC" , A, C);

    mfile.open ("/tmp/output.dot");
    mfile << printers::GenerateGraphDOT(&graph) << std::endl;
    mfile.close();


    merge_vhdl_operators (&graph, "ACJ" , graph.getVertexByName("AC"), graph.getVertexByName("J"));

    mfile.open ("/tmp/output2.dot");
    mfile << printers::GenerateGraphDOT(&graph) << std::endl;
    mfile.close();

    mfile.open ("/tmp/output2.xml");
    mfile << printers::generateSDF3XML(&graph) << std::endl;
    mfile.close();
}

BOOST_AUTO_TEST_SUITE_END()






