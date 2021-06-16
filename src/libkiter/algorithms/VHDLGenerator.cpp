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
#include "VHDLCircuit.h"

void algorithms::generateVHDL(models::Dataflow* const dataflow,
                              parameters_list_t param_list) {
  std::set<std::string> componentSet; // store components to generate VHDL code for
  std::vector<std::unique_ptr<VHDLComponent>> componentList;
  std::vector<std::unique_ptr<VHDLConnection>> connectionList;
  VHDLCircuit circuit(dataflow);

  {ForEachVertex(dataflow, actor) {
      std::string actorType = dataflow->getVertexType(actor);
      actorType = actorType.substr(0, actorType.find("_")); // separate function of actor from iterator
      componentSet.insert(actorType);
      std::unique_ptr<VHDLComponent> newComp = new VHDLComponent(dataflow, actor); 
      componentList.push_back(newComp);
      circuit.addComponent(newComp)
    }}
  
  {ForEachEdge(dataflow, edge) {
      std::unique_ptr<VHDLConnection> newConn = new VHDLConnection(dataflow, edge);
      connectionList.push_back(newConn);
      circuit.addConnection(newConn);
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
  // std::cout << "VHDL Circuit: " << std::endl;
  // for (auto it : circuit.getComponentMap()) {
  //   std::cout << it.second.printStatus() << std::endl;
  // }
  //std::cout << circuit.printStatus() << std::endl;
  return;
}
