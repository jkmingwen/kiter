/*
 * VHDLGenerator.h
 *
 *  Created on: 14 June 2021
 *      Author: jkmingwen
 */

#include <set>
#include <string>
#include "VHDLGenerator.h"
#include "VHDLComponent.h"

void algorithms::generateVHDL(models::Dataflow* const dataflow,
                              parameters_list_t param_list) {
  std::set<std::string> componentSet; // store components to generate VHDL code for
  std::vector<VHDLComponent> componentList;
  // TODO populate list of actors and channels
  {ForEachVertex(dataflow, actor) {
      // std::cout << "Actor name, type, ID:" << std::endl;
      // std::cout << "\t" << dataflow->getVertexName(actor) << ", "
      //           << dataflow->getVertexType(actor) << ", "
      //           << dataflow->getVertexId(actor) << std::endl;
      std::string actorType = dataflow->getVertexType(actor);
      actorType = actorType.substr(0, actorType.find("_")); // separate function of actor from iterator
      componentSet.insert(actorType);
      componentList.push_back(VHDLComponent(dataflow, actor));
      // {ForInputEdges(dataflow, actor, inEdge) {
      //     std::cout << "\tInput edges:" << std::endl;
      //     std::cout << "\t\tName, ID: " << dataflow->getEdgeName(inEdge)
      //               << ", " << dataflow->getEdgeId(inEdge) << std::endl;
      //     std::cout << "\t\tSrcPort, DstPort: " << dataflow->getEdgeInputPortName(inEdge)
      //               << ", " << dataflow->getEdgeOutputPortName(inEdge) << std::endl;
      //   }}
      // {ForOutputEdges(dataflow, actor, outEdge) {
      //     std::cout << "\tOutput edges:" << std::endl;
      //     std::cout << "\t\tName, ID: " << dataflow->getEdgeName(outEdge)
      //               << ", " << dataflow->getEdgeId(outEdge) << std::endl;
      //     std::cout << "\t\tSrcPort, DstPort: " << dataflow->getEdgeInputPortName(outEdge)
      //               << ", " << dataflow->getEdgeOutputPortName(outEdge) << std::endl;
      //   }}
    }}
  std::cout << "Components to produce: " << std::endl;
  for (auto element : componentSet) {
    std::cout << "\t" << element << std::endl;
  }
  std::cout << "VHDL Component list: " << std::endl;
  for (auto comp : componentList) {
    std::cout << comp.printStatus() << std::endl;
  }
  return;
}
