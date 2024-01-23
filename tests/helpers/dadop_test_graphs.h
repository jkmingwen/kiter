/*
 * dadop_test_graphs.cpp
 *
 *  Created on: Jan 11, 2024
 *      Author: jkmingwen
 */
#ifndef TESTS_HELPERS_DADOP_TEST_GRAPHS_H
#define TESTS_HELPERS_DADOP_TEST_GRAPHS_H
#include <models/Dataflow.h>

// Auto-generate by Kiter for Kiter
//
models::Dataflow* generateNoiseSimplified () {
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


// Auto-generate by Kiter for Kiter
//
models::Dataflow *generateSeqArithmeticThreeSimplified() {
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x5616b3483030");
 new_graph->setVertexType(new_vertex,"2.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x5616b3483ab0");
 new_graph->setVertexType(new_vertex,"5.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x5616b34841a0");
 new_graph->setVertexType(new_vertex,"3.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x5616b3484960");
 new_graph->setVertexType(new_vertex,"9.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"0x5616b3486d80_0x5616b3483030_0x7ffb5c006ba0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x5616b3486e90_0x5616b3486d80_0x5616b3483ab0");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x5616b3487260_0x5616b34841a0_0x5616b3486e90");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"0x5616b3487500");
 new_graph->setVertexType(new_vertex,"-7.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"0x5616b34875f0_0x5616b3487260_0x5616b3487500");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(10);
 new_graph->setVertexName(new_vertex,"0x5616b34878f0_0x5616b3484960_0x5616b34875f0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(11);
 new_graph->setVertexName(new_vertex,"0x7ffb5c006ba0");
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
 new_graph->setEdgeInputPortName(new_edge, "in_channel_0_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_0_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(10), new_graph->getVertexById(12));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_10_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_10_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_10_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_1_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_1_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_1_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_2_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_2_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_2_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_3_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_3_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_3_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_4_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_4_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_4_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_5_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_5_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_6_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_6_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_6_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_7_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_7_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_7_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_8_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_8_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_8_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_9_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_9_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_9_real_vect");
}
return new_graph;
}

// Auto-generate by Kiter for Kiter
//
models::Dataflow *generateFIR4Simplified() {
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x55d2fe186800_0x7f75f4008a90_0x7f75f400a7c0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x55d2fe186b40_0x7f75f40075c0_0x7f75f400a7c0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x55d2fe186ef0_0x7f75f40061e0_0x7f75f400a7c0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x55d2fe187220_0x55d2fe186b40_0x55d2fe186ef0");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"0x55d2fe187310_0x55d2fe186800_0x55d2fe187220");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x7f75f40061e0");
 new_graph->setVertexType(new_vertex,"0.750000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x7f75f40075c0");
 new_graph->setVertexType(new_vertex,"0.500000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"0x7f75f4008a90");
 new_graph->setVertexType(new_vertex,"0.250000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"0x7f75f400a7c0");
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
 new_graph->setEdgeInputPortName(new_edge, "in_channel_0_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_0_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(3));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_10_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_10_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_10_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(4));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_14_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_14_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_14_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_15_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_15_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_15_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_16_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_16_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_16_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_4_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_4_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_4_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(2));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_5_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_5_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(4));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_9_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_9_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_9_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(3));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,3);
 new_graph->setEdgeName(new_edge,"channel_11_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_11_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_11_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(2));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,2);
 new_graph->setEdgeName(new_edge,"channel_6_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_6_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_6_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(1));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,1);
 new_graph->setEdgeName(new_edge,"channel_1_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_1_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_1_real_vect");
}
return new_graph;
}

// Auto-generate by Kiter for Kiter
//
models::Dataflow* generateRMS2Simplified() {
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x55fc5d7251d0");
 new_graph->setVertexType(new_vertex,"2");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x55fc5d727a20");
 new_graph->setVertexType(new_vertex,"Proj");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x55fc5d7285b0_0x55fc5d727a20");
 new_graph->setVertexType(new_vertex,"float");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x55fc5d728de0");
 new_graph->setVertexType(new_vertex,"Proj");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"0x55fc5d729830_0x55fc5d728de0");
 new_graph->setVertexType(new_vertex,"float");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x55fc5d72af50");
 new_graph->setVertexType(new_vertex,"1048576.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x55fc5d72fd10_0x7f4824001e70_0x55fc5d7251d0");
 new_graph->setVertexType(new_vertex,"pow");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"0x55fc5d7300a0_0x55fc5d72af50_0x55fc5d72fd10");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"0x55fc5d730190_0x55fc5d7300a0");
 new_graph->setVertexType(new_vertex,"int");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(10);
 new_graph->setVertexName(new_vertex,"0x55fc5d730930_0x55fc5d727a20_0x55fc5d730190");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(11);
 new_graph->setVertexName(new_vertex,"0x55fc5d730a20_0x55fc5d730930_0x55fc5d730190");
 new_graph->setVertexType(new_vertex,"diff");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(12);
 new_graph->setVertexName(new_vertex,"0x55fc5d730fb0");
 new_graph->setVertexType(new_vertex,"0.000000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(13);
 new_graph->setVertexName(new_vertex,"0x55fc5d731120_0x55fc5d730fb0_0x55fc5d7285b0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(14);
 new_graph->setVertexName(new_vertex,"0x55fc5d731210_0x55fc5d731120");
 new_graph->setVertexType(new_vertex,"sqrt");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(15);
 new_graph->setVertexName(new_vertex,"0x55fc5d7316b0_0x7f4824001f60_0x55fc5d7251d0");
 new_graph->setVertexType(new_vertex,"pow");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(16);
 new_graph->setVertexName(new_vertex,"0x55fc5d731950_0x55fc5d72af50_0x55fc5d7316b0");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(17);
 new_graph->setVertexName(new_vertex,"0x55fc5d731a40_0x55fc5d731950");
 new_graph->setVertexType(new_vertex,"int");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(18);
 new_graph->setVertexName(new_vertex,"0x55fc5d732080_0x55fc5d728de0_0x55fc5d731a40");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(19);
 new_graph->setVertexName(new_vertex,"0x55fc5d732170_0x55fc5d732080_0x55fc5d731a40");
 new_graph->setVertexType(new_vertex,"diff");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(20);
 new_graph->setVertexName(new_vertex,"0x55fc5d732610_0x55fc5d730fb0_0x55fc5d729830");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(21);
 new_graph->setVertexName(new_vertex,"0x55fc5d732700_0x55fc5d732610");
 new_graph->setVertexType(new_vertex,"sqrt");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(22);
 new_graph->setVertexName(new_vertex,"0x7f4824001e70");
 new_graph->setVertexType(new_vertex,"INPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(23);
 new_graph->setVertexName(new_vertex,"0x7f4824001f60");
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
 new_graph->setEdgeInputPortName(new_edge, "in_channel_0_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_0_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(10), new_graph->getVertexById(11));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_10_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_10_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_10_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(2));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_15_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_15_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_15_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_19_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_19_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_19_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(13), new_graph->getVertexById(14));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_20_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_20_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_20_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(14), new_graph->getVertexById(24));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_21_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_21_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_21_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(12), new_graph->getVertexById(20));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_22_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_22_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_22_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(16));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_26_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_26_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_26_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(23), new_graph->getVertexById(15));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_27_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_27_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_27_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(15));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_28_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_28_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_28_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(15), new_graph->getVertexById(16));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_29_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_29_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_29_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(16), new_graph->getVertexById(17));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_30_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_30_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_30_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(17), new_graph->getVertexById(18));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_31_int_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_31_int_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_31_int_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(18), new_graph->getVertexById(19));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_32_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_32_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_32_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(19), new_graph->getVertexById(4));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_37_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_37_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_37_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(20));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_41_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_41_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_41_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(20), new_graph->getVertexById(21));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_42_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_42_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_42_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(21), new_graph->getVertexById(25));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_43_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_43_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_43_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_4_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_4_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_4_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(22), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_5_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_5_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_6_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_6_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_6_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_7_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_7_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_7_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_8_real_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_8_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_8_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_9_int_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_9_int_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_9_int_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(3));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_16_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_16_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_16_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_38_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_38_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_38_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,1);
 new_graph->setEdgeName(new_edge,"channel_1_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_1_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_1_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(11));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,1000);
 new_graph->setEdgeName(new_edge,"channel_11_int_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_11_int_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_11_int_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(18));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,1);
 new_graph->setEdgeName(new_edge,"channel_23_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_23_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_23_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(17), new_graph->getVertexById(19));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,1000);
 new_graph->setEdgeName(new_edge,"channel_33_int_vect");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_33_int_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_33_int_vect");
}
return new_graph;
}

// Auto-generate by Kiter for Kiter
//
models::Dataflow *generateEchoSimplified() {
models::Dataflow* new_graph = new models::Dataflow();

{
auto new_vertex = new_graph->addVertex(1);
 new_graph->setVertexName(new_vertex,"0x563184cce020");
 new_graph->setVertexType(new_vertex,"0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(2);
 new_graph->setVertexName(new_vertex,"0x563184cce6b0");
 new_graph->setVertexType(new_vertex,"1");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(3);
 new_graph->setVertexName(new_vertex,"0x563184cd3c30");
 new_graph->setVertexType(new_vertex,"-1");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(4);
 new_graph->setVertexName(new_vertex,"0x563184cf6890");
 new_graph->setVertexType(new_vertex,"Proj");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(5);
 new_graph->setVertexName(new_vertex,"0x563184cfe700_0x7f4c901b82b0_0x7f4c901c2210");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(6);
 new_graph->setVertexName(new_vertex,"0x563184cfe7f0_0x563184cfe700");
 new_graph->setVertexType(new_vertex,"int");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(7);
 new_graph->setVertexName(new_vertex,"0x563184cfea60_0x563184cfe7f0_0x563184cd3c30");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(8);
 new_graph->setVertexName(new_vertex,"0x563184cfeb50_0x563184cce020_0x563184cfea60");
 new_graph->setVertexType(new_vertex,"max");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(9);
 new_graph->setVertexName(new_vertex,"0x563184cfec60_0x7f4c9005aaa0_0x563184cfeb50");
 new_graph->setVertexType(new_vertex,"min");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(10);
 new_graph->setVertexName(new_vertex,"0x563184cfed30_0x563184cfec60");
 new_graph->setVertexType(new_vertex,"int");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(11);
 new_graph->setVertexName(new_vertex,"0x563184cfef10_0x563184cfed30_0x563184cce6b0");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(12);
 new_graph->setVertexName(new_vertex,"0x563184cff580_0x563184cfef10");
 new_graph->setVertexType(new_vertex,"int");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(13);
 new_graph->setVertexName(new_vertex,"0x563184cff630");
 new_graph->setVertexType(new_vertex,"delay");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(14);
 new_graph->setVertexName(new_vertex,"0x563184cff8e0");
 new_graph->setVertexType(new_vertex,"0.010000");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(15);
 new_graph->setVertexName(new_vertex,"0x563184cffa50_0x563184cff8e0_0x7f4c901bf960");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(16);
 new_graph->setVertexName(new_vertex,"0x563184cffe40_0x563184cffa50_0x563184cff630");
 new_graph->setVertexType(new_vertex,"prod");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(17);
 new_graph->setVertexName(new_vertex,"0x563184d000c0_0x7f4c901b9160_0x563184cffe40");
 new_graph->setVertexType(new_vertex,"add");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(18);
 new_graph->setVertexName(new_vertex,"0x7f4c9005aaa0");
 new_graph->setVertexType(new_vertex,"65536");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(19);
 new_graph->setVertexName(new_vertex,"0x7f4c901b82b0");
 new_graph->setVertexType(new_vertex,"44.100002");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(20);
 new_graph->setVertexName(new_vertex,"0x7f4c901b9160");
 new_graph->setVertexType(new_vertex,"INPUT_0");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(21);
 new_graph->setVertexName(new_vertex,"0x7f4c901bf960");
 new_graph->setVertexType(new_vertex,"hslider");
 new_graph->setInitPhasesQuantity(new_vertex,0);
 new_graph->setPhasesQuantity(new_vertex,1);
 new_graph->setReentrancyFactor(new_vertex,1);
 new_graph->setVertexDuration(new_vertex,{1});
}
{
auto new_vertex = new_graph->addVertex(22);
 new_graph->setVertexName(new_vertex,"0x7f4c901c2210");
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
 new_graph->setEdgeInputPortName(new_edge, "in_channel_0_real_vect");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_0_real_vect");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(19), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_13_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_13_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_13_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(22), new_graph->getVertexById(5));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_14_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_14_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_14_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(6));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_15_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_15_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_15_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_16_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_16_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_16_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(7));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_17_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_17_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_17_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_18_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_18_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_18_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_19_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_19_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_19_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(14), new_graph->getVertexById(15));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_1_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_1_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_1_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(10));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_20_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_20_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_20_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(10), new_graph->getVertexById(11));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_21_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_21_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_21_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(11));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_22_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_22_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_22_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(12));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_23_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_23_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_23_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(12), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_24_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_24_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_24_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(13), new_graph->getVertexById(16));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_25_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_25_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_25_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(16), new_graph->getVertexById(17));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_26_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_26_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_26_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(17), new_graph->getVertexById(4));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_28_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_28_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_28_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(21), new_graph->getVertexById(15));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_2_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_2_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_2_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(15), new_graph->getVertexById(16));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_3_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_3_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_3_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(13));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_4_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_4_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_4_real");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(18), new_graph->getVertexById(9));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_5_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_5_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_5_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(8));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_6_int");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_6_int");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_6_int");
}
{
auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(23));
 new_graph->setEdgeInPhases(new_edge,{1});
 new_graph->setEdgeOutPhases(new_edge,{1});
 new_graph->setPreload(new_edge,0);
 new_graph->setEdgeName(new_edge,"channel_29_real");
 new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
 new_graph->setEdgeInputPortName(new_edge, "in_channel_29_real");
 new_graph->setEdgeOutputPortName(new_edge, "out_channel_29_real");
}
return new_graph;
}

#endif //TESTS_HELPERS_DADOP_TEST_GRAPHS_H
