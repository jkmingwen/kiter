//
// Created by Bruno on 07/10/24.
//

#define BOOST_TEST_MODULE FIRScheduleTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "helpers/random_generator.h"
#include "algorithms/experiments/expansion_experiment.h"
#include "algorithms/normalization.h"
#include "algorithms/transformation/iterative_evaluation.h"
#include <algorithms/schedulings.h>


BOOST_FIXTURE_TEST_SUITE( FIRScheduleTest , WITH_SAMPLE)
// Auto-generate by Kiter for Kiter
//

// Auto-generate by Kiter for Kiter
//
    models::Dataflow* generateGraph () {
        models::Dataflow* new_graph = new models::Dataflow();

        {
            auto new_vertex = new_graph->addVertex(1);
            new_graph->setVertexName(new_vertex,"0x2c5b300_0x7f577000dea0_0x7f5770243840INIT1");
            new_graph->setVertexType(new_vertex,"prod");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(2);
            new_graph->setVertexName(new_vertex,"0x2c5b640_0x7f577000c9d0_0x7f5770242510INIT2");
            new_graph->setVertexType(new_vertex,"prod");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(3);
            new_graph->setVertexName(new_vertex,"0x2c5b9f0_0x7f577000b500_0x7f57702411f0INIT3");
            new_graph->setVertexType(new_vertex,"prod");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(4);
            new_graph->setVertexName(new_vertex,"0x2c5bf80_0x7f577000a070_0x7f577023feb0INIT4");
            new_graph->setVertexType(new_vertex,"prod");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(5);
            new_graph->setVertexName(new_vertex,"0x2c5c600_0x7f5770008ba0_0x7f577023eb80INIT5");
            new_graph->setVertexType(new_vertex,"prod");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(6);
            new_graph->setVertexName(new_vertex,"0x2c5cdb0_0x7f5770007600_0x7f577023d850INIT6");
            new_graph->setVertexType(new_vertex,"prod");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(7);
            new_graph->setVertexName(new_vertex,"0x2c5d650_0x7f5770006220_0x7f577023c5f0INIT7");
            new_graph->setVertexType(new_vertex,"prod");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(8);
            new_graph->setVertexName(new_vertex,"0x2c5da00_0x2c5cdb0_0x2c5d650");
            new_graph->setVertexType(new_vertex,"add");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(9);
            new_graph->setVertexName(new_vertex,"0x2c5daf0_0x2c5c600_0x2c5da00");
            new_graph->setVertexType(new_vertex,"add");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(10);
            new_graph->setVertexName(new_vertex,"0x2c5dbe0_0x2c5bf80_0x2c5daf0");
            new_graph->setVertexType(new_vertex,"add");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(11);
            new_graph->setVertexName(new_vertex,"0x2c5dcd0_0x2c5b9f0_0x2c5dbe0");
            new_graph->setVertexType(new_vertex,"add");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(12);
            new_graph->setVertexName(new_vertex,"0x2c5ddc0_0x2c5b640_0x2c5dcd0");
            new_graph->setVertexType(new_vertex,"add");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(13);
            new_graph->setVertexName(new_vertex,"0x2c5deb0_0x2c5b300_0x2c5ddc0");
            new_graph->setVertexType(new_vertex,"add");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(14);
            new_graph->setVertexName(new_vertex,"0x7f5770006220");
            new_graph->setVertexType(new_vertex,"0.875000");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(15);
            new_graph->setVertexName(new_vertex,"0x7f5770007600");
            new_graph->setVertexType(new_vertex,"0.750000");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(16);
            new_graph->setVertexName(new_vertex,"0x7f5770008ba0");
            new_graph->setVertexType(new_vertex,"0.625000");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(17);
            new_graph->setVertexName(new_vertex,"0x7f577000a070");
            new_graph->setVertexType(new_vertex,"0.500000");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(18);
            new_graph->setVertexName(new_vertex,"0x7f577000b500");
            new_graph->setVertexType(new_vertex,"0.375000");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(19);
            new_graph->setVertexName(new_vertex,"0x7f577000c9d0");
            new_graph->setVertexType(new_vertex,"0.250000");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(20);
            new_graph->setVertexName(new_vertex,"0x7f577000dea0");
            new_graph->setVertexType(new_vertex,"0.125000");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(21);
            new_graph->setVertexName(new_vertex,"0x7f577000fbd0");
            new_graph->setVertexType(new_vertex,"INPUT_0");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(22);
            new_graph->setVertexName(new_vertex,"0x7f577023c5f0INIT7");
            new_graph->setVertexType(new_vertex,"sbuffer");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(23);
            new_graph->setVertexName(new_vertex,"0x7f577023d850INIT6");
            new_graph->setVertexType(new_vertex,"sbuffer");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(24);
            new_graph->setVertexName(new_vertex,"0x7f577023eb80INIT5");
            new_graph->setVertexType(new_vertex,"sbuffer");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(25);
            new_graph->setVertexName(new_vertex,"0x7f577023feb0INIT4");
            new_graph->setVertexType(new_vertex,"sbuffer");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(26);
            new_graph->setVertexName(new_vertex,"0x7f57702411f0INIT3");
            new_graph->setVertexType(new_vertex,"sbuffer");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(27);
            new_graph->setVertexName(new_vertex,"0x7f5770242510INIT2");
            new_graph->setVertexType(new_vertex,"sbuffer");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(28);
            new_graph->setVertexName(new_vertex,"0x7f5770243840INIT1");
            new_graph->setVertexType(new_vertex,"sbuffer");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_vertex = new_graph->addVertex(29);
            new_graph->setVertexName(new_vertex,"OUTPUT_0");
            new_graph->setVertexType(new_vertex,"OUTPUT_0");
            new_graph->setInitPhasesQuantity(new_vertex,0);
            new_graph->setPhasesQuantity(new_vertex,1);
            new_graph->setReentrancyFactor(new_vertex,1);
            new_graph->setVertexDuration(new_vertex,{1});
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(20), new_graph->getVertexById(1));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_0_real");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(18), new_graph->getVertexById(3));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_10_real");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(21), new_graph->getVertexById(26));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_11_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(26), new_graph->getVertexById(3));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_13_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(3), new_graph->getVertexById(11));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_14_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(17), new_graph->getVertexById(4));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_15_real");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(21), new_graph->getVertexById(25));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_16_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(25), new_graph->getVertexById(4));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_18_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(4), new_graph->getVertexById(10));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_19_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(21), new_graph->getVertexById(28));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_1_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(16), new_graph->getVertexById(5));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_20_real");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(21), new_graph->getVertexById(24));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_21_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(24), new_graph->getVertexById(5));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_23_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(5), new_graph->getVertexById(9));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_24_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(15), new_graph->getVertexById(6));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_25_real");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(21), new_graph->getVertexById(23));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_26_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(23), new_graph->getVertexById(6));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_28_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(6), new_graph->getVertexById(8));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_29_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(14), new_graph->getVertexById(7));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_30_real");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(21), new_graph->getVertexById(22));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_31_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(22), new_graph->getVertexById(7));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_33_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(7), new_graph->getVertexById(8));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_34_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(8), new_graph->getVertexById(9));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_35_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(9), new_graph->getVertexById(10));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_36_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(10), new_graph->getVertexById(11));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_37_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(11), new_graph->getVertexById(12));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_38_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(12), new_graph->getVertexById(13));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_39_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(28), new_graph->getVertexById(1));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_3_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(13), new_graph->getVertexById(29));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_40_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(1), new_graph->getVertexById(13));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_4_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(19), new_graph->getVertexById(2));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_5_real");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(21), new_graph->getVertexById(27));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_6_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(27), new_graph->getVertexById(2));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_8_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        {
            auto new_edge = new_graph->addEdge(new_graph->getVertexById(2), new_graph->getVertexById(12));
            new_graph->setEdgeInPhases(new_edge,{1});
            new_graph->setEdgeOutPhases(new_edge,{1});
            new_graph->setPreload(new_edge,0);
            new_graph->setEdgeName(new_edge,"channel_9_real_vect");
            new_graph->setEdgeType(new_edge, EDGE_TYPE::NORMAL_EDGE);
        }
        return new_graph;
    }


    BOOST_AUTO_TEST_CASE( prepare_graph_test )
    {
        parameters_list_t params;
        models::Dataflow *graph = generateGraph ();
        VERBOSE_ASSERT(computeRepetitionVector(graph), "inconsistent graph");
        models::Scheduling res1 = algorithms::scheduling::CSDF_1PeriodicScheduling(graph, 5208);

        graph->reset_computation();

        algorithms::transformation::generate_audio_components(graph,params);
        VERBOSE_ASSERT(computeRepetitionVector(graph), "inconsistent graph");
        models::Scheduling res2 = algorithms::scheduling::CSDF_1PeriodicScheduling(graph, 0);

        std::cout <<  res1.asText() << std::endl;
        std::cout <<  res2.asText() << std::endl;

    }



BOOST_AUTO_TEST_SUITE_END()
