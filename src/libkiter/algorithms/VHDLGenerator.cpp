/*
 * VHDLGenerator.h
 *
 *  Created on: 14 June 2021
 *      Author: jkmingwen
 */

#include "VHDLGenerator.h"

void algorithms::generateVHDL(models::Dataflow* const dataflow,
                              parameters_list_t param_list) {
  {ForEachVertex(dataflow, actor) {
      std::cout << "Actor name, type, ID:" << std::endl;
      std::cout << "\t" << dataflow->getVertexName(actor) << ", "
                << dataflow->getVertexType(actor) << ", "
                << dataflow->getVertexId(actor) << std::endl;
      {ForInputEdges(dataflow, actor, inEdge) {
          std::cout << "\tInput edges:" << std::endl;
          std::cout << "\t\tName, ID: " << dataflow->getEdgeName(inEdge)
                    << ", " << dataflow->getEdgeId(inEdge) << std::endl;
          std::cout << "\t\tSrcPort, DstPort: " << dataflow->getEdgeInputPortName(inEdge)
                    << ", " << dataflow->getEdgeOutputPortName(inEdge) << std::endl;
        }}
      {ForOutputEdges(dataflow, actor, outEdge) {
          std::cout << "\tOutput edges:" << std::endl;
          std::cout << "\t\tName, ID: " << dataflow->getEdgeName(outEdge)
                    << ", " << dataflow->getEdgeId(outEdge) << std::endl;
          std::cout << "\t\tSrcPort, DstPort: " << dataflow->getEdgeInputPortName(outEdge)
                    << ", " << dataflow->getEdgeOutputPortName(outEdge) << std::endl;
        }}
    }}
  return;
}
