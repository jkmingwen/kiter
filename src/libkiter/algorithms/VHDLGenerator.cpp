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
#include "VHDLConnection.h"

void algorithms::generateVHDL(models::Dataflow* const dataflow,
                              parameters_list_t param_list) {
  std::set<std::string> componentSet; // store components to generate VHDL code for
  std::vector<VHDLComponent> componentList;
  std::vector<VHDLConnection> connectionList;

  {ForEachVertex(dataflow, actor) {
      std::string actorType = dataflow->getVertexType(actor);
      actorType = actorType.substr(0, actorType.find("_")); // separate function of actor from iterator
      componentSet.insert(actorType);
      componentList.push_back(VHDLComponent(dataflow, actor));
    }}
  
  {ForEachEdge(dataflow, edge) {
      connectionList.push_back(VHDLConnection(dataflow, edge));
    }}
  std::cout << "Components to produce: " << std::endl;
  for (auto element : componentSet) {
    std::cout << "\t" << element << std::endl;
  }
  std::cout << "VHDL Component list: " << std::endl;
  for (auto comp : componentList) {
    std::cout << comp.printStatus() << std::endl;
  }
  std::cout << "VHDL Connection list: " << std::endl;
  for (auto comp : connectionList) {
    std::cout << comp.printStatus() << std::endl;
  }
  return;
}
