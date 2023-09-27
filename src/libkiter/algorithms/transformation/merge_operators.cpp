/*
 *  iterative_evaluation.cpp
 *
 *  Created on: Aug 2, 2021
 *      Author: jkmingwen
 */


#include "merge_operators.h"
#include "../vhdl_generation/VHDLComponent.h"
#include "../vhdl_generation/VHDLCircuit.h"
#include <commons/verbose.h>
#include <models/Dataflow.h>
#include <printers/SDF3Wrapper.h> // to write XML files

void algorithms::transformation::merge_operators(models::Dataflow* const dataflow,
                                                 parameters_list_t params) {
  // TODO produce vector of operators to merge by operator type
  // TODO iteratively call generateMergedGraph
  bool changeDetected = true;
  bool outputSpecified = false;
  std::string dirName = "./"; // use current directory as default
  std::string outputName = dataflow->getGraphName() + "_merged" + ".xml"; // use graph name as default

  if (params.find("OUTPUT_DIR") != params.end()) {
    outputSpecified = true;
    dirName = params["OUTPUT_DIR"];
    VERBOSE_INFO("DIR NAME: " << dirName);
  }
  if (params.find("OUTPUT_NAME") != params.end()) {
    outputSpecified = true;
    outputName = params["OUTPUT_NAME"] + ".xml";
  }
  outputName = dirName + outputName; // set output file path

  VERBOSE_INFO("Beginning to merge operators...");
  // std::string actorAName = "0x55d9ca2bb7b0_0x55d9ca2b8390_0x7f93140064e0";
  // std::string actorCName = "0x55d9ca2bbc90_0x55d9ca2b9520_0x55d9ca2bb8c0";
  std::string actorAName = "0x55e7fa476eb0_0x55e7fa473160_0x7f541c006ba0";
  std::string actorCName = "0x55e7fa477390_0x55e7fa4742d0_0x55e7fa476fc0";
  std::string actorJName = "0x55e7fa477a20_0x55e7fa474a90_0x55e7fa477720";
  Vertex v1 = dataflow->getVertexByName(actorAName);
  Vertex v2 = dataflow->getVertexByName(actorCName);
  Vertex v3 = dataflow->getVertexByName(actorJName);
  std::vector<Vertex> mergeList{v1, v2, v3};
  int isOffset = 0;
  int osOffset = 0;
  generateMergedGraph(dataflow, mergeList, isOffset, osOffset); // NOTE mergeList of actors needs to be in their expected order of execution
  std::string actorBName = "0x55e7fa476fc0_outputselector0_0x55e7fa473be0";
  std::string actorDName = "0x55e7fa477720_outputselector0_0x55e7fa477630";
  Vertex v4 = dataflow->getVertexByName(actorBName);
  Vertex v5 = dataflow->getVertexByName(actorDName);
  std::vector<Vertex> mergeListTwo{v4, v5};
  generateMergedGraph(dataflow, mergeListTwo, isOffset, osOffset);
  if (outputSpecified) { // only write output file if output directory specified
    VERBOSE_INFO("Merged graph:" << outputName);
    printers::writeSDF3File(outputName, dataflow);
  }
}

/* Given a graph and a vector of vertices, return a new graph where
   the vertices in the vector have been merged.
   The isOffset and osOffset parameters inform the current count of
   input/output selectors to avoid naming conflicts. */
void algorithms::generateMergedGraph(models::Dataflow* dataflow,
                                     std::vector<Vertex> &vertices,
                                     int &isOffset, int &osOffset) {
  // Check that operators have the same input/output edges
  VERBOSE_ASSERT(vertices.size() > 1, "Need to have more than 1 actor to perform merging.");
  std::vector<unsigned int> vInDegree;
  std::vector<unsigned int> vOutDegree;
  for (auto v : vertices) {
    vInDegree.push_back(dataflow->getVertexInDegree(v));
    vOutDegree.push_back(dataflow->getVertexOutDegree(v));
  }
  VERBOSE_ASSERT(std::equal(vInDegree.begin() + 1, vInDegree.end(), vInDegree.begin()),
                 "Every actor to be merged needs to have the same number of inputs.");
  VERBOSE_ASSERT(std::equal(vOutDegree.begin() + 1, vOutDegree.end(), vOutDegree.begin()),
                 "Every actor to be merged needs to have the same number of outputs.");
  std::map<int, std::vector<std::string>> argOrder;
  std::map<int, std::vector<std::string>> inEdges;
  std::map<int, std::vector<std::string>> inPorts;
  std::map<int, std::vector<std::string>> outEdges;
  std::map<int, std::vector<std::string>> outPorts;
  std::map<int, std::map<std::string, int>> inDataTypes;
  std::map<int, std::map<std::string, int>> outDataTypes;
  std::vector<std::string> actorNames; // we track these to replace with the name of the output selector

  // store information from each actor that we're going to merge
  int actorCount = 0;
  for (auto v : vertices) {
    std::vector<std::string> inEdgeNames;
    std::vector<std::string> inPortNames;
    std::vector<std::string> outEdgeNames;
    std::vector<std::string> outPortNames;
    VHDLComponent actorInfo(dataflow, v);
    std::string actorBaseName = dataflow->getVertexName(v);
    size_t pos = actorBaseName.find("_");
    actorNames.push_back(actorBaseName.substr(0, pos));
    argOrder[actorCount] = actorInfo.getArgOrder();
    inDataTypes[actorCount] = actorInfo.getInputTypes();
    outDataTypes[actorCount] = actorInfo.getOutputTypes();
    for (auto a : actorInfo.getArgOrder()) { // store the operands (in the form of edges and ports) in the order indicated by argOrder
      std::string actorName = dataflow->getVertexName(v);
      {ForInputEdges(dataflow, dataflow->getVertexByName(actorName), inEdge) {
          Vertex inputActor = dataflow->getEdgeSource(inEdge);
          if (dataflow->getVertexName(inputActor).rfind(a, 0) == 0) { // the edge connects the input actor to this actor
            inEdgeNames.push_back(dataflow->getEdgeName(inEdge));
            inPortNames.push_back(dataflow->getEdgeOutputPortName(inEdge));
          }
        }}
      {ForOutputEdges(dataflow, dataflow->getVertexByName(actorName), outEdge) {
          Vertex outputActor = dataflow->getEdgeTarget(outEdge);
          // NOTE assuming here that there's only ever 1 output edge per vertice
          outEdgeNames.push_back(dataflow->getEdgeName(outEdge));
          outPortNames.push_back(dataflow->getEdgeInputPortName(outEdge));
        }}
    }
    inEdges[actorCount] = inEdgeNames;
    inPorts[actorCount] = inPortNames;
    outEdges[actorCount] = outEdgeNames;
    outPorts[actorCount] = outPortNames;
    actorCount++;
  }

  dataflow->reset_computation(); // necessary to edit dataflow graph
  VERBOSE_INFO("Adding merged operator");
  // Number of input/output selectors added should be equal to the input/output counts of the merged operators
  size_t inDeg = dataflow->getVertexInDegree(vertices.front());
  size_t outDeg = dataflow->getVertexOutDegree(vertices.front());
  // Create the merged operator + its input and output selector
  std::string mergedActorName = "merged";
  for (int i = 0; i < inDeg; i++) {
    mergedActorName += "_inputselector" + commons::toString(i + isOffset);
  }
  Vertex mergedActor = dataflow->addVertex(mergedActorName);
  dataflow->setPhasesQuantity(mergedActor, 1);
  dataflow->setVertexDuration(mergedActor,dataflow->getVertexPhaseDuration(vertices.front())); // just get phase duration of first actor since they're all equal
  dataflow->setVertexType(mergedActor, dataflow->getVertexType(vertices.front())); // merged vertices are of the same type
  // add re-entrancy edges
  addReentrancy(dataflow, mergedActor, mergedActorName, {1});

  // add input selectors to graph
  for (int i = 0; i < inDeg; i++) {
    int isId = i + isOffset;
    std::string inputSelectorName = "inputselector" + commons::toString(isId);
    std::vector<TIME_UNIT> execDurations(vertices.size(), 1);
    std::vector<TOKEN_UNIT> execRates(vertices.size(), 1);
    // input selector 0 will take the first operand from all inputs, input selector 1 will take the second operands, etc.
    for (auto const &arg : argOrder) {
      inputSelectorName += "_" + (arg.second)[i];
    }
    auto new_is = dataflow->addVertex(inputSelectorName);
    dataflow->setPhasesQuantity(new_is, vertices.size());
    dataflow->setVertexDuration(new_is, execDurations);
    dataflow->setVertexType(new_is, "input_selector");

    // input edges of input selectors are grouped according to order of operands
    for (auto const &edge : inEdges) {
      // store information of original edge before removing it
      Edge ogEdge = dataflow->getEdgeByName(edge.second[i]);
      Vertex ogSource = dataflow->getEdgeSource(ogEdge);
      std::vector<TOKEN_UNIT> inPhases = (dataflow->getEdgeInVector(ogEdge));
      std::vector<TOKEN_UNIT> outPhases(vertices.size(), 0);
      TOKEN_UNIT preload = dataflow->getPreload(ogEdge);
      DATA_UNIT tokenSize = dataflow->getTokenSize(ogEdge);
      dataflow->removeEdge(ogEdge);
      Edge newEdge = dataflow->addEdge(ogSource, new_is, (edge.second[i]));
      outPhases[edge.first] = 1; // the order of execution (of its input actor to merged actor) is reflected here
      dataflow->setEdgeInPhases(newEdge, inPhases);
      dataflow->setEdgeOutPhases(newEdge, outPhases);
      dataflow->setEdgeInputPortName(newEdge, ("in_" + edge.second[i]));
      dataflow->setEdgeOutputPortName(newEdge, ("out_" + edge.second[i]));
      dataflow->setPreload(newEdge, preload);
      dataflow->setTokenSize(newEdge, tokenSize);
    }

    // new edge from input selector to merged actor
    std::string edgeType = (inDataTypes[i]).begin()->first; // our current VHDL generation implementation uses the first input edge type to determine arithmetic type
    // use edge count to avoid edge name conflicts
    std::string outEdgeName = "channel_" + commons::toString(dataflow->getEdgesCount() + 1) + "_" + edgeType;
    Edge outEdge = dataflow->addEdge(new_is, mergedActor, outEdgeName);
    dataflow->setEdgeInPhases(outEdge, execRates);
    dataflow->setEdgeOutPhases(outEdge, {1});
    dataflow->setEdgeInputPortName(outEdge, "in_" + outEdgeName);
    dataflow->setEdgeOutputPortName(outEdge, "out_" + outEdgeName);
    dataflow->setPreload(outEdge, 0);
    dataflow->setTokenSize(outEdge, 1);

    // add re-entrancy edges/ports
    addReentrancy(dataflow, new_is, "inputselector" + commons::toString(isId), execRates);
  }
  isOffset += inDeg;

  // add output selectors to graph
  for (int i = 0 ; i < outDeg ; i++) {
    int osId = i + osOffset;
    std::string outputSelectorName = "outputselector" + commons::toString(osId);
    std::vector<TIME_UNIT> execDurations(vertices.size(), 1);
    std::vector<TOKEN_UNIT> execRates(vertices.size(), 1);
    auto new_os = dataflow->addVertex(outputSelectorName);
    dataflow->setPhasesQuantity(new_os, vertices.size());
    dataflow->setVertexDuration(new_os, execDurations);
    dataflow->setVertexType(new_os, "output_selector");
    // connect merged actors to output selector
    std::string edgeType = (outDataTypes[i]).begin()->first;
    std::string outEdgeName = "channel_" + commons::toString(dataflow->getEdgesCount() + 1) + "_" + edgeType;
    Edge mergedToOS = dataflow->addEdge(mergedActor, new_os, outEdgeName);
    dataflow->setEdgeInPhases(mergedToOS, {1});
    dataflow->setEdgeOutPhases(mergedToOS, execRates);
    dataflow->setEdgeInputPortName(mergedToOS, "in_" + outEdgeName);
    dataflow->setEdgeOutputPortName(mergedToOS, "out_" + outEdgeName);
    dataflow->setPreload(mergedToOS, 0);
    dataflow->setTokenSize(mergedToOS, 1);

    // output edges from output selector are the target actors of the merged actors
    for (auto const &edge : outEdges) {
      // store information of original edge before removing it
      Edge ogEdge = dataflow->getEdgeByName(edge.second[i]);
      Vertex ogTarget = dataflow->getEdgeTarget(ogEdge);
      std::vector<TOKEN_UNIT> inPhases(vertices.size(), 0);
      std::vector<TOKEN_UNIT> outPhases = (dataflow->getEdgeOutVector(ogEdge));
      TOKEN_UNIT preload = dataflow->getPreload(ogEdge);
      DATA_UNIT tokenSize = dataflow->getTokenSize(ogEdge);
      dataflow->removeEdge(ogEdge);
      Edge newEdge = dataflow->addEdge(new_os, ogTarget, (edge.second[i]));
      inPhases[edge.first] = 1; // the order of execution is reflected here
      dataflow->setEdgeInPhases(newEdge, inPhases);
      dataflow->setEdgeOutPhases(newEdge, outPhases);
      dataflow->setEdgeInputPortName(newEdge, ("in_" + edge.second[i]));
      dataflow->setEdgeOutputPortName(newEdge, ("out_" + edge.second[i]));
      dataflow->setPreload(newEdge, preload);
      dataflow->setTokenSize(newEdge, tokenSize);
    }

    // add re-entrancy edges/ports
    addReentrancy(dataflow, new_os, "outputselector" + commons::toString(osId), execRates);
    // rename affected actors (targets of merged vertices) with updated source actor name (of output selector)
    for (auto name : actorNames) {
      {ForEachVertex(dataflow, v) {
          std::string newName = replaceActorName(dataflow->getVertexName(v),
                                                 name,
                                                 "outputselector" + commons::toString(osId));
          dataflow->setVertexName(v, newName);
        }}
    }
  }
  osOffset += outDeg;

  // remove merged vertices
  std::vector<ARRAY_INDEX> verticesToRemove;
  for (auto v : vertices) { // necessary workaround to remove vertices
    verticesToRemove.push_back(dataflow->getVertexId(v));
  }
  for (auto i : verticesToRemove) {
    dataflow->removeVertex(dataflow->getVertexById(i));
  }
}

void algorithms::addReentrancy(models::Dataflow* const dataflow, Vertex v,
                               std::string actorName, std::vector<TOKEN_UNIT> execRates) {
  Edge selfLoop = dataflow->addEdge(v, v, "R" + actorName);
  dataflow->setEdgeInPhases(selfLoop, execRates);
  dataflow->setEdgeOutPhases(selfLoop, execRates);
  dataflow->setEdgeInputPortName(selfLoop, "in_R" + actorName);
  dataflow->setEdgeOutputPortName(selfLoop, "out_R" + actorName);
  dataflow->setPreload(selfLoop, 1); // reentrancy edges need an initial token
  dataflow->setTokenSize(selfLoop, 1);
}

// replace all occurances of actor name with replacement name
// from https://stackoverflow.com/questions/5343190/how-do-i-replace-all-instances-of-a-string-with-another-string
std::string algorithms::replaceActorName(std::string originalName, const std::string& toReplace,
                                         const std::string& replacement) {
    size_t pos = 0;
    while ((pos = originalName.find(toReplace, pos)) != std::string::npos) {
      originalName.replace(pos, toReplace.length(), replacement);
      pos += replacement.length();
    }
    return originalName;
}

std::vector<std::vector<ARRAY_INDEX>> algorithms::greedyMerge(models::Dataflow* const dataflow) {
  std::vector<std::vector<ARRAY_INDEX>> matchingOperators;
  std::vector<ARRAY_INDEX> tmpMatches;
  std::map<std::string, int> opCounts;

  // track counts of each type of operator in dataflow graph
  {ForEachVertex(dataflow, v) {
      VHDLComponent op(dataflow, v);
      if (std::find(mergeableOperators.begin(),
                    mergeableOperators.end(),
                    op.getType()) != mergeableOperators.end()) {
        opCounts[op.getType()]++;
      }
    }}
  // each vertex type with multiple occurances gets stored in a vector
  for (auto &types : opCounts) {
    if (types.second > 1) {
      {ForEachVertex(dataflow, v) {
          VHDLComponent op(dataflow, v);
          if (op.getType() == types.first) {
            tmpMatches.push_back(dataflow->getVertexId(v));
          }
        }}
      matchingOperators.push_back(tmpMatches);
      tmpMatches.clear();
    }
  }

  return matchingOperators;
}

void algorithms::smartMerge(models::Dataflow* const dataflow) {
  abstractDepGraph absDepGraph(dataflow); // initialise abstract dependency graph
  std::map<ARRAY_INDEX, Actor> actorMap;
  {ForEachVertex(dataflow, v) {
      VERBOSE_INFO("Actor ID: " << dataflow->getVertexId(v));
      actorMap[dataflow->getVertexId(v)] = Actor(dataflow, v);
      VERBOSE_INFO("\n");
    }}
  VERBOSE_INFO("Actor map size: " << actorMap.size());
  VERBOSE_INFO("Initialising states:");
  State prevState(dataflow, actorMap);
  State currState(dataflow, actorMap);
  VERBOSE_INFO("Set current state time to 0:");
  currState.setTimeElapsed(0);
  // VERBOSE_INFO("Actor at 1: " << actorMap[1].printStatus(dataflow));
  // actorMap[1].computeCausalDeps(dataflow, prevState, absDepGraph);
  VERBOSE_INFO("Actor at 4: " << actorMap[4].printStatus(dataflow));
  actorMap[4].computeCausalDeps(dataflow, prevState, absDepGraph);
  {ForEachVertex(dataflow, v) {
      // while (actorMap[dataflow->getVertexId(v)].isReadyForExec(currState)) {
      //   VERBOSE_DEBUG("Actor " << dataflow->getVertexId(t)
      //                 << " ready to fire; looking for causal dependencies" << std::endl);
      actorMap[1].computeCausalDeps(dataflow, prevState, absDepGraph);
      VERBOSE_INFO("Compute causal deps for Actor ID: " << dataflow->getVertexId(v));
      actorMap.at(dataflow->getVertexId(v)).computeCausalDeps(dataflow, prevState, absDepGraph);
      //   actorMap[dataflow->getVertexId(v)].execStart(dataflow, currState);
      //   currState.updateState(dataflow, actorMap);
      // }
    }}
  return;
}
