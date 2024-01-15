/*
 * dadop_test_graphs.cpp
 *
 *  Created on: Jan 11, 2024
 *      Author: jkmingwen
 */
#ifndef TESTS_HELPERS_DADOP_TEST_GRAPHS_H
#define TESTS_HELPERS_DADOP_TEST_GRAPHS_H
#include <models/Dataflow.h>

models::Dataflow* generateFIR4 () {
 // Auto-generate by Kiter for Kiter
 //
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x5564909776b0");
 new_graph->setVertexType(new_vertex,"1");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x556490977e20");
 new_graph->setVertexType(new_vertex,"3");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x5564909a2800_0x7f9bcc008a90_0x7f9bcc22afd0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x5564909a2b40_0x7f9bcc0075c0_0x7f9bcc229ca0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"0x5564909a2ef0_0x7f9bcc0061e0_0x7f9bcc228a40");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x5564909a3220_0x5564909a2b40_0x5564909a2ef0");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x5564909a3310_0x5564909a2800_0x5564909a3220");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"0x7f9bcc005ad0");
 new_graph->setVertexType(new_vertex,"2");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"0x7f9bcc0061e0");
 new_graph->setVertexType(new_vertex,"0.750000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(10);
 new_graph->setVertexName(new_vertex,"0x7f9bcc0075c0");
 new_graph->setVertexType(new_vertex,"0.500000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(11);
 new_graph->setVertexName(new_vertex,"0x7f9bcc008a90");
 new_graph->setVertexType(new_vertex,"0.250000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(12);
 new_graph->setVertexName(new_vertex,"0x7f9bcc00a7c0");
 new_graph->setVertexType(new_vertex,"INPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(13);
 new_graph->setVertexName(new_vertex,"0x7f9bcc228a40");
 new_graph->setVertexType(new_vertex,"delay");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(14);
 new_graph->setVertexName(new_vertex,"0x7f9bcc229ca0");
 new_graph->setVertexType(new_vertex,"delay");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(15);
 new_graph->setVertexName(new_vertex,"0x7f9bcc22afd0");
 new_graph->setVertexType(new_vertex,"delay");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(16);
 new_graph->setVertexName(new_vertex,"OUTPUT_0");
 new_graph->setVertexType(new_vertex,"OUTPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(3));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_10_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(12), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_11_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_12_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(13), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_13_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_14_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_15_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(16));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_16_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(12), new_graph->getVertexById(15));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_1_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(15));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_2_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(15), new_graph->getVertexById(3));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_3_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_4_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(10), new_graph->getVertexById(4));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(12), new_graph->getVertexById(14));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_6_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(14));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_7_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(14), new_graph->getVertexById(4));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_8_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_9_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
return new_graph;
}

models::Dataflow* generateEcho () {
 // Auto-generate by Kiter for Kiter
 //
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x55cd2ca61020");
 new_graph->setVertexType(new_vertex,"0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x55cd2ca616b0");
 new_graph->setVertexType(new_vertex,"1");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x55cd2ca629b0");
 new_graph->setVertexType(new_vertex,"1.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x55cd2ca66c30");
 new_graph->setVertexType(new_vertex,"-1");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"0x55cd2ca89890");
 new_graph->setVertexType(new_vertex,"Proj");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x55cd2ca8af30");
 new_graph->setVertexType(new_vertex,"delay");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x55cd2ca91700_0x7f433c1b82b0_0x7f433c1c2210");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"0x55cd2ca917f0_0x55cd2ca91700");
 new_graph->setVertexType(new_vertex,"int");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"0x55cd2ca91a60_0x55cd2ca917f0_0x55cd2ca66c30");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(10);
 new_graph->setVertexName(new_vertex,"0x55cd2ca91b50_0x55cd2ca61020_0x55cd2ca91a60");
 new_graph->setVertexType(new_vertex,"max");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(11);
 new_graph->setVertexName(new_vertex,"0x55cd2ca91c60_0x7f433c05aaa0_0x55cd2ca91b50");
 new_graph->setVertexType(new_vertex,"min");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(12);
 new_graph->setVertexName(new_vertex,"0x55cd2ca91d30_0x55cd2ca91c60");
 new_graph->setVertexType(new_vertex,"int");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(13);
 new_graph->setVertexName(new_vertex,"0x55cd2ca91f10_0x55cd2ca91d30_0x55cd2ca616b0");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(14);
 new_graph->setVertexName(new_vertex,"0x55cd2ca92580_0x55cd2ca91f10");
 new_graph->setVertexType(new_vertex,"int");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(15);
 new_graph->setVertexName(new_vertex,"0x55cd2ca92630");
 new_graph->setVertexType(new_vertex,"delay");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(16);
 new_graph->setVertexName(new_vertex,"0x55cd2ca928e0");
 new_graph->setVertexType(new_vertex,"0.010000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(17);
 new_graph->setVertexName(new_vertex,"0x55cd2ca92a50_0x55cd2ca928e0_0x7f433c1bf960");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(18);
 new_graph->setVertexName(new_vertex,"0x55cd2ca92e40_0x55cd2ca92a50_0x55cd2ca92630");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(19);
 new_graph->setVertexName(new_vertex,"0x55cd2ca930c0_0x7f433c1b9160_0x55cd2ca92e40");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(20);
 new_graph->setVertexName(new_vertex,"0x7f433c05aaa0");
 new_graph->setVertexType(new_vertex,"65536");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(21);
 new_graph->setVertexName(new_vertex,"0x7f433c160660");
 new_graph->setVertexType(new_vertex,"192000.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(22);
 new_graph->setVertexName(new_vertex,"0x7f433c1b5790");
 new_graph->setVertexType(new_vertex,"44100");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(23);
 new_graph->setVertexName(new_vertex,"0x7f433c1b5d40_0x55cd2ca629b0_0x7f433c1b5790");
 new_graph->setVertexType(new_vertex,"max");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(24);
 new_graph->setVertexName(new_vertex,"0x7f433c1b6310_0x7f433c160660_0x7f433c1b5d40");
 new_graph->setVertexType(new_vertex,"min");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(25);
 new_graph->setVertexName(new_vertex,"0x7f433c1b7fd0");
 new_graph->setVertexType(new_vertex,"0.001000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(26);
 new_graph->setVertexName(new_vertex,"0x7f433c1b82b0_0x7f433c1b7fd0_0x7f433c1b6310");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(27);
 new_graph->setVertexName(new_vertex,"0x7f433c1b9160");
 new_graph->setVertexType(new_vertex,"INPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(28);
 new_graph->setVertexName(new_vertex,"0x7f433c1bf960");
 new_graph->setVertexType(new_vertex,"hslider");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(29);
 new_graph->setVertexName(new_vertex,"0x7f433c1c2210");
 new_graph->setVertexType(new_vertex,"hslider");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(30);
 new_graph->setVertexName(new_vertex,"OUTPUT_0");
 new_graph->setVertexType(new_vertex,"OUTPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(27), new_graph->getVertexById(19));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_0_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(22), new_graph->getVertexById(23));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_10_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(23), new_graph->getVertexById(24));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_11_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(24), new_graph->getVertexById(26));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_12_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(26), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_13_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(29), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_14_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_15_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_16_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_17_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_18_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(10), new_graph->getVertexById(11));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_19_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(16), new_graph->getVertexById(17));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_1_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(12));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_20_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(12), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_21_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_22_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(13), new_graph->getVertexById(14));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_23_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(14), new_graph->getVertexById(15));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_24_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(15), new_graph->getVertexById(18));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_25_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(18), new_graph->getVertexById(19));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_26_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(19), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_28_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_29_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(28), new_graph->getVertexById(17));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_2_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_30_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(30));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_31_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(17), new_graph->getVertexById(18));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_3_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(15));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_4_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(20), new_graph->getVertexById(11));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_6_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(25), new_graph->getVertexById(26));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_7_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(21), new_graph->getVertexById(24));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_8_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(23));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_9_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
return new_graph;
}

models::Dataflow* generateNoise () {
 // Auto-generate by Kiter for Kiter
 //
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x55f86c414020");
 new_graph->setVertexType(new_vertex,"0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x55f86c4146b0");
 new_graph->setVertexType(new_vertex,"1");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x55f86c43ad10");
 new_graph->setVertexType(new_vertex,"Proj");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x55f86c43b910");
 new_graph->setVertexType(new_vertex,"delay");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"0x55f86c43f2d0_0x55f86c43b910");
 new_graph->setVertexType(new_vertex,"float");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x55f86c43f6c0");
 new_graph->setVertexType(new_vertex,"delay");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x55f86c43fa50_0x7f85a000c370_0x55f86c43f6c0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"0x55f86c43fcd0_0x55f86c43fa50_0x7f85a000c0c0");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"0x55f86c440100");
 new_graph->setVertexType(new_vertex,"0.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(10);
 new_graph->setVertexName(new_vertex,"0x55f86c4402b0_0x55f86c440100_0x55f86c43f2d0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(11);
 new_graph->setVertexName(new_vertex,"0x7f85a000c0c0");
 new_graph->setVertexType(new_vertex,"12345");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(12);
 new_graph->setVertexName(new_vertex,"0x7f85a000c370");
 new_graph->setVertexType(new_vertex,"1103515245");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(13);
 new_graph->setVertexName(new_vertex,"OUTPUT_0");
 new_graph->setVertexType(new_vertex,"OUTPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(4));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_10_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_11_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_12_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(10), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_13_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(12), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_1_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_2_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_3_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_4_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_6_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(3));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_8_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(4));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_9_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
return new_graph;
}

models::Dataflow* generateSeqArithmeticThree () {
 // Auto-generate by Kiter for Kiter
 //
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x55c393e42030");
 new_graph->setVertexType(new_vertex,"2.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x55c393e42ab0");
 new_graph->setVertexType(new_vertex,"5.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x55c393e431a0");
 new_graph->setVertexType(new_vertex,"3.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x55c393e43960");
 new_graph->setVertexType(new_vertex,"9.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"0x55c393e45d80_0x55c393e42030_0x7f7d78006ba0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x55c393e45e90_0x55c393e45d80_0x55c393e42ab0");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x55c393e46260_0x55c393e431a0_0x55c393e45e90");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"0x55c393e46500");
 new_graph->setVertexType(new_vertex,"-7.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"0x55c393e465f0_0x55c393e46260_0x55c393e46500");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(10);
 new_graph->setVertexName(new_vertex,"0x55c393e468f0_0x55c393e43960_0x55c393e465f0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(11);
 new_graph->setVertexName(new_vertex,"0x7f7d78006ba0");
 new_graph->setVertexType(new_vertex,"INPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(12);
 new_graph->setVertexName(new_vertex,"OUTPUT_0");
 new_graph->setVertexType(new_vertex,"OUTPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(10), new_graph->getVertexById(12));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_10_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_1_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_2_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_3_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_4_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_6_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_7_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_8_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_9_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
return new_graph;
}

models::Dataflow* generateFIR4Simplified () {
 // Auto-generate by Kiter for Kiter
 //
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x5564909a2800_0x7f9bcc008a90_0x7f9bcc00a7c0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x5564909a2b40_0x7f9bcc0075c0_0x7f9bcc00a7c0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x5564909a2ef0_0x7f9bcc0061e0_0x7f9bcc00a7c0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x5564909a3220_0x5564909a2b40_0x5564909a2ef0");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"0x5564909a3310_0x5564909a2800_0x5564909a3220");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x7f9bcc0061e0");
 new_graph->setVertexType(new_vertex,"0.750000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x7f9bcc0075c0");
 new_graph->setVertexType(new_vertex,"0.500000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"0x7f9bcc008a90");
 new_graph->setVertexType(new_vertex,"0.250000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"0x7f9bcc00a7c0");
 new_graph->setVertexType(new_vertex,"INPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(10);
 new_graph->setVertexName(new_vertex,"OUTPUT_0");
 new_graph->setVertexType(new_vertex,"OUTPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(1));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(3));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_10_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(4));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_14_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_15_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_16_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_4_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(2));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(4));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_9_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(3));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,3);
 new_graph->setEdgeName(new_edge,"channel_11_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(2));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,2);
 new_graph->setEdgeName(new_edge,"channel_6_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(1));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,1);
 new_graph->setEdgeName(new_edge,"channel_1_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
return new_graph;
}

models::Dataflow* generateEchoSimplified () {
 // Auto-generate by Kiter for Kiter
 //
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x55cd2ca61020");
 new_graph->setVertexType(new_vertex,"0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x55cd2ca616b0");
 new_graph->setVertexType(new_vertex,"1");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x55cd2ca66c30");
 new_graph->setVertexType(new_vertex,"-1");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x55cd2ca89890");
 new_graph->setVertexType(new_vertex,"Proj");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"0x55cd2ca91700_0x7f433c1b82b0_0x7f433c1c2210");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x55cd2ca917f0_0x55cd2ca91700");
 new_graph->setVertexType(new_vertex,"int");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x55cd2ca91a60_0x55cd2ca917f0_0x55cd2ca66c30");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"0x55cd2ca91b50_0x55cd2ca61020_0x55cd2ca91a60");
 new_graph->setVertexType(new_vertex,"max");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"0x55cd2ca91c60_0x7f433c05aaa0_0x55cd2ca91b50");
 new_graph->setVertexType(new_vertex,"min");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(10);
 new_graph->setVertexName(new_vertex,"0x55cd2ca91d30_0x55cd2ca91c60");
 new_graph->setVertexType(new_vertex,"int");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(11);
 new_graph->setVertexName(new_vertex,"0x55cd2ca91f10_0x55cd2ca91d30_0x55cd2ca616b0");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(12);
 new_graph->setVertexName(new_vertex,"0x55cd2ca92580_0x55cd2ca91f10");
 new_graph->setVertexType(new_vertex,"int");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(13);
 new_graph->setVertexName(new_vertex,"0x55cd2ca92630");
 new_graph->setVertexType(new_vertex,"delay");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(14);
 new_graph->setVertexName(new_vertex,"0x55cd2ca928e0");
 new_graph->setVertexType(new_vertex,"0.010000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(15);
 new_graph->setVertexName(new_vertex,"0x55cd2ca92a50_0x55cd2ca928e0_0x7f433c1bf960");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(16);
 new_graph->setVertexName(new_vertex,"0x55cd2ca92e40_0x55cd2ca92a50_0x55cd2ca92630");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(17);
 new_graph->setVertexName(new_vertex,"0x55cd2ca930c0_0x7f433c1b9160_0x55cd2ca92e40");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(18);
 new_graph->setVertexName(new_vertex,"0x7f433c05aaa0");
 new_graph->setVertexType(new_vertex,"65536");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(19);
 new_graph->setVertexName(new_vertex,"0x7f433c1b82b0");
 new_graph->setVertexType(new_vertex,"44.100002");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(20);
 new_graph->setVertexName(new_vertex,"0x7f433c1b9160");
 new_graph->setVertexType(new_vertex,"INPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(21);
 new_graph->setVertexName(new_vertex,"0x7f433c1bf960");
 new_graph->setVertexType(new_vertex,"hslider");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(22);
 new_graph->setVertexName(new_vertex,"0x7f433c1c2210");
 new_graph->setVertexType(new_vertex,"hslider");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(23);
 new_graph->setVertexName(new_vertex,"OUTPUT_0");
 new_graph->setVertexType(new_vertex,"OUTPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(20), new_graph->getVertexById(17));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_0_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(19), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_13_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(22), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_14_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_15_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_16_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_17_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_18_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_19_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(14), new_graph->getVertexById(15));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_1_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_20_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(10), new_graph->getVertexById(11));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_21_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(11));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_22_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(12));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_23_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(12), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_24_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(13), new_graph->getVertexById(16));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,1);
 new_graph->setEdgeName(new_edge,"channel_25_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(16), new_graph->getVertexById(17));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_26_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(17), new_graph->getVertexById(4));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_28_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(21), new_graph->getVertexById(15));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_2_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(15), new_graph->getVertexById(16));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_3_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_4_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(18), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_6_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(23));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_29_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
return new_graph;
}

models::Dataflow* generateNoiseSimplified () {
 // Auto-generate by Kiter for Kiter
 //
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x55f86c43ad10");
 new_graph->setVertexType(new_vertex,"Proj");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x55f86c43f2d0_0x55f86c43ad10");
 new_graph->setVertexType(new_vertex,"float");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x55f86c43fa50_0x7f85a000c370_0x55f86c43ad10");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x55f86c43fcd0_0x55f86c43fa50_0x7f85a000c0c0");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"0x55f86c440100");
 new_graph->setVertexType(new_vertex,"0.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x55f86c4402b0_0x55f86c440100_0x55f86c43f2d0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x7f85a000c0c0");
 new_graph->setVertexType(new_vertex,"12345");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"0x7f85a000c370");
 new_graph->setVertexType(new_vertex,"1103515245");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"OUTPUT_0");
 new_graph->setVertexType(new_vertex,"OUTPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_12_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_13_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(3));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_1_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(4));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(4));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_6_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(1));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_8_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(2));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_9_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(3));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,1);
 new_graph->setEdgeName(new_edge,"channel_2_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
return new_graph;
}

models::Dataflow* generateSeqArithmeticThreeSimplified () {
 // Auto-generate by Kiter for Kiter
 //
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x55c393e42030");
 new_graph->setVertexType(new_vertex,"2.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x55c393e42ab0");
 new_graph->setVertexType(new_vertex,"5.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x55c393e431a0");
 new_graph->setVertexType(new_vertex,"3.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x55c393e43960");
 new_graph->setVertexType(new_vertex,"9.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"0x55c393e45d80_0x55c393e42030_0x7f7d78006ba0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x55c393e45e90_0x55c393e45d80_0x55c393e42ab0");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x55c393e46260_0x55c393e431a0_0x55c393e45e90");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"0x55c393e46500");
 new_graph->setVertexType(new_vertex,"-7.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"0x55c393e465f0_0x55c393e46260_0x55c393e46500");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(10);
 new_graph->setVertexName(new_vertex,"0x55c393e468f0_0x55c393e43960_0x55c393e465f0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(11);
 new_graph->setVertexName(new_vertex,"0x7f7d78006ba0");
 new_graph->setVertexType(new_vertex,"INPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(12);
 new_graph->setVertexName(new_vertex,"OUTPUT_0");
 new_graph->setVertexType(new_vertex,"OUTPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(10), new_graph->getVertexById(12));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_10_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_1_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_2_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_3_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_4_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_6_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_7_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_8_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_9_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
return new_graph;
}

models::Dataflow* generateFIR4GreedyMerge () {
 // Auto-generate by Kiter for Kiter
 //
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x7f9bcc0061e0");
 new_graph->setVertexType(new_vertex,"0.750000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x7f9bcc0075c0");
 new_graph->setVertexType(new_vertex,"0.500000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x7f9bcc008a90");
 new_graph->setVertexType(new_vertex,"0.250000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x7f9bcc00a7c0");
 new_graph->setVertexType(new_vertex,"INPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"OUTPUT_0");
 new_graph->setVertexType(new_vertex,"OUTPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x7f9bcc00a7c0Dup0");
 new_graph->setVertexType(new_vertex,"Proj");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,0);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x7f9bcc00a7c0Dup1");
 new_graph->setVertexType(new_vertex,"Proj");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,0);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"merged_inputselector0_inputselector1");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{3});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"inputselector0_outputselector1_outputselector1");
 new_graph->setVertexType(new_vertex,"input_selector");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,2);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1,1});
}
{
auto new_vertex = new_graph->addVertex(10);
 new_graph->setVertexName(new_vertex,"inputselector1_outputselector1_outputselector0");
 new_graph->setVertexType(new_vertex,"input_selector");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,2);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1,1});
}
{
auto new_vertex = new_graph->addVertex(11);
 new_graph->setVertexName(new_vertex,"outputselector0");
 new_graph->setVertexType(new_vertex,"output_selector");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,2);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1,1});
}
{
auto new_vertex = new_graph->addVertex(12);
 new_graph->setVertexName(new_vertex,"merged_inputselector2_inputselector3");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(13);
 new_graph->setVertexName(new_vertex,"inputselector2_0x7f9bcc008a90_0x7f9bcc0075c0_0x7f9bcc0061e0");
 new_graph->setVertexType(new_vertex,"input_selector");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,3);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1,1,1});
}
{
auto new_vertex = new_graph->addVertex(14);
 new_graph->setVertexName(new_vertex,"inputselector3_0x7f9bcc00a7c0Dup1_0x7f9bcc00a7c0Dup0_0x7f9bcc00a7c0Dup0");
 new_graph->setVertexType(new_vertex,"input_selector");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,3);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1,1,1});
}
{
auto new_vertex = new_graph->addVertex(15);
 new_graph->setVertexName(new_vertex,"outputselector1");
 new_graph->setVertexType(new_vertex,"output_selector");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,3);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1,1,1});
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_15_0_0x7f9bcc00a7c0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_17_2_0x7f9bcc00a7c0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1,1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_36_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(10), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1,1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_42_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(11));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1,1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_46_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1,0});
 new_graph->setEdgeOutPhases(new_edge,{0,1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_15_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{0,1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_16_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1,0,0});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{0,1,0});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{0,0,1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_10_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(13), new_graph->getVertexById(12));
 new_graph->setEdgeInPhases(new_edge,{1,1,1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_57_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(14));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1,0,0});
 new_graph->setPreload(new_edge,1);
 new_graph->setEdgeName(new_edge,"channel_16_1_0x7f9bcc00a7c0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(14));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{0,1,0});
 new_graph->setPreload(new_edge,2);
 new_graph->setEdgeName(new_edge,"channel_14_2_0x7f9bcc00a7c0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(14));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{0,0,1});
 new_graph->setPreload(new_edge,3);
 new_graph->setEdgeName(new_edge,"channel_13_1_0x7f9bcc00a7c0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(14), new_graph->getVertexById(12));
 new_graph->setEdgeInPhases(new_edge,{1,1,1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_64_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(12), new_graph->getVertexById(15));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1,1,1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_68_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(15), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1,0,0});
 new_graph->setEdgeOutPhases(new_edge,{0,1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_4_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(15), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{0,1,0});
 new_graph->setEdgeOutPhases(new_edge,{1,0});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_9_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(15), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{0,0,1});
 new_graph->setEdgeOutPhases(new_edge,{1,0});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_14_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
return new_graph;
}

models::Dataflow* generateFIR4SmartMerge() {
 // Auto-generate by Kiter for Kiter
 //
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x7f9bcc0061e0");
 new_graph->setVertexType(new_vertex,"0.750000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x7f9bcc0075c0");
 new_graph->setVertexType(new_vertex,"0.500000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x7f9bcc008a90");
 new_graph->setVertexType(new_vertex,"0.250000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x7f9bcc00a7c0");
 new_graph->setVertexType(new_vertex,"INPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"OUTPUT_0");
 new_graph->setVertexType(new_vertex,"OUTPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x7f9bcc00a7c0Dup0");
 new_graph->setVertexType(new_vertex,"Proj");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,0);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x7f9bcc00a7c0Dup1");
 new_graph->setVertexType(new_vertex,"Proj");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,0);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"merged_inputselector0_inputselector1");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{3});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"inputselector0_outputselector1_outputselector1");
 new_graph->setVertexType(new_vertex,"input_selector");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,2);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1,1});
}
{
auto new_vertex = new_graph->addVertex(10);
 new_graph->setVertexName(new_vertex,"inputselector1_outputselector1_outputselector0");
 new_graph->setVertexType(new_vertex,"input_selector");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,2);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1,1});
}
{
auto new_vertex = new_graph->addVertex(11);
 new_graph->setVertexName(new_vertex,"outputselector0");
 new_graph->setVertexType(new_vertex,"output_selector");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,2);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1,1});
}
{
auto new_vertex = new_graph->addVertex(12);
 new_graph->setVertexName(new_vertex,"merged_inputselector2_inputselector3");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(13);
 new_graph->setVertexName(new_vertex,"inputselector2_0x7f9bcc008a90_0x7f9bcc0075c0_0x7f9bcc0061e0");
 new_graph->setVertexType(new_vertex,"input_selector");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,3);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1,1,1});
}
{
auto new_vertex = new_graph->addVertex(14);
 new_graph->setVertexName(new_vertex,"inputselector3_0x7f9bcc00a7c0Dup1_0x7f9bcc00a7c0Dup0_0x7f9bcc00a7c0Dup0");
 new_graph->setVertexType(new_vertex,"input_selector");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,3);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1,1,1});
}
{
auto new_vertex = new_graph->addVertex(15);
 new_graph->setVertexName(new_vertex,"outputselector1");
 new_graph->setVertexType(new_vertex,"output_selector");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,3);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1,1,1});
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_15_0_0x7f9bcc00a7c0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_17_2_0x7f9bcc00a7c0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1,1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_36_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(10), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1,1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_42_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(11));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1,1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_46_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1,0});
 new_graph->setEdgeOutPhases(new_edge,{0,1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_15_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{0,1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_16_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1,0,0});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{0,1,0});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{0,0,1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_10_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(13), new_graph->getVertexById(12));
 new_graph->setEdgeInPhases(new_edge,{1,1,1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_57_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(14));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1,0,0});
 new_graph->setPreload(new_edge,1);
 new_graph->setEdgeName(new_edge,"channel_16_1_0x7f9bcc00a7c0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(14));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{0,1,0});
 new_graph->setPreload(new_edge,2);
 new_graph->setEdgeName(new_edge,"channel_14_2_0x7f9bcc00a7c0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(14));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{0,0,1});
 new_graph->setPreload(new_edge,3);
 new_graph->setEdgeName(new_edge,"channel_13_1_0x7f9bcc00a7c0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(14), new_graph->getVertexById(12));
 new_graph->setEdgeInPhases(new_edge,{1,1,1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_64_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(12), new_graph->getVertexById(15));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1,1,1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_68_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(15), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1,0,0});
 new_graph->setEdgeOutPhases(new_edge,{0,1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_4_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(15), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{0,1,0});
 new_graph->setEdgeOutPhases(new_edge,{1,0});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_9_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(15), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{0,0,1});
 new_graph->setEdgeOutPhases(new_edge,{1,0});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_14_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
return new_graph;
}

models::Dataflow *generateRMS2Simplified() {
  // Auto-generate by Kiter for Kiter
  //
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x555f4c8441d0");
 new_graph->setVertexType(new_vertex,"2");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x555f4c846a20");
 new_graph->setVertexType(new_vertex,"Proj");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x555f4c8475b0_0x555f4c846a20");
 new_graph->setVertexType(new_vertex,"float");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x555f4c847de0");
 new_graph->setVertexType(new_vertex,"Proj");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"0x555f4c848830_0x555f4c847de0");
 new_graph->setVertexType(new_vertex,"float");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x555f4c849f50");
 new_graph->setVertexType(new_vertex,"1048576.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x555f4c84ed10_0x7ff80c001e70_0x555f4c8441d0");
 new_graph->setVertexType(new_vertex,"pow");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"0x555f4c84f0a0_0x555f4c849f50_0x555f4c84ed10");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"0x555f4c84f190_0x555f4c84f0a0");
 new_graph->setVertexType(new_vertex,"int");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(10);
 new_graph->setVertexName(new_vertex,"0x555f4c84f930_0x555f4c846a20_0x555f4c84f190");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(11);
 new_graph->setVertexName(new_vertex,"0x555f4c84fa20_0x555f4c84f930_0x555f4c84f190");
 new_graph->setVertexType(new_vertex,"diff");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(12);
 new_graph->setVertexName(new_vertex,"0x555f4c84ffb0");
 new_graph->setVertexType(new_vertex,"0.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(13);
 new_graph->setVertexName(new_vertex,"0x555f4c850120_0x555f4c84ffb0_0x555f4c8475b0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(14);
 new_graph->setVertexName(new_vertex,"0x555f4c850210_0x555f4c850120");
 new_graph->setVertexType(new_vertex,"sqrt");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(15);
 new_graph->setVertexName(new_vertex,"0x555f4c8506b0_0x7ff80c001f60_0x555f4c8441d0");
 new_graph->setVertexType(new_vertex,"pow");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(16);
 new_graph->setVertexName(new_vertex,"0x555f4c850950_0x555f4c849f50_0x555f4c8506b0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(17);
 new_graph->setVertexName(new_vertex,"0x555f4c850a40_0x555f4c850950");
 new_graph->setVertexType(new_vertex,"int");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(18);
 new_graph->setVertexName(new_vertex,"0x555f4c851080_0x555f4c847de0_0x555f4c850a40");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(19);
 new_graph->setVertexName(new_vertex,"0x555f4c851170_0x555f4c851080_0x555f4c850a40");
 new_graph->setVertexType(new_vertex,"diff");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(20);
 new_graph->setVertexName(new_vertex,"0x555f4c851610_0x555f4c84ffb0_0x555f4c848830");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(21);
 new_graph->setVertexName(new_vertex,"0x555f4c851700_0x555f4c851610");
 new_graph->setVertexType(new_vertex,"sqrt");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(22);
 new_graph->setVertexName(new_vertex,"0x7ff80c001e70");
 new_graph->setVertexType(new_vertex,"INPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(23);
 new_graph->setVertexName(new_vertex,"0x7ff80c001f60");
 new_graph->setVertexType(new_vertex,"INPUT_1");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(24);
 new_graph->setVertexName(new_vertex,"OUTPUT_0");
 new_graph->setVertexType(new_vertex,"OUTPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(25);
 new_graph->setVertexName(new_vertex,"OUTPUT_1");
 new_graph->setVertexType(new_vertex,"OUTPUT_1");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(12), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_0_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(10), new_graph->getVertexById(11));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_10_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(2));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_15_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_19_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(13), new_graph->getVertexById(14));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_20_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(14), new_graph->getVertexById(24));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_21_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(12), new_graph->getVertexById(20));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_22_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(16));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_26_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(23), new_graph->getVertexById(15));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_27_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(15));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_28_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(15), new_graph->getVertexById(16));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_29_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(16), new_graph->getVertexById(17));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_30_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(17), new_graph->getVertexById(18));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_31_int_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(18), new_graph->getVertexById(19));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_32_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(19), new_graph->getVertexById(4));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_37_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(20));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_41_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(20), new_graph->getVertexById(21));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_42_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(21), new_graph->getVertexById(25));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_43_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_4_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(22), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_6_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_7_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_8_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_9_int_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(3));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_16_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_38_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,1);
 new_graph->setEdgeName(new_edge,"channel_1_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(11));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,1000);
 new_graph->setEdgeName(new_edge,"channel_11_int_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(18));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,1);
 new_graph->setEdgeName(new_edge,"channel_23_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(17), new_graph->getVertexById(19));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,1000);
 new_graph->setEdgeName(new_edge,"channel_33_int_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
}
return new_graph;
}

#endif //TESTS_HELPERS_DADOP_TEST_GRAPHS_H
