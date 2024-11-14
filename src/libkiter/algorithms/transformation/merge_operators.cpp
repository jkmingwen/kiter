/*
 *  merge_operators.cpp
 *
 *  Created on: Sept 5, 2023
 *      Author: jkmingwen
 */


#include "merge_operators.h"
#include "../vhdl_generation/VHDLComponent.h"
#include "../vhdl_generation/VHDLCircuit.h"
#include "../vhdl_generation/VHDLGenerator.h"
#include "../throughput/actor.h"
#include "../throughput/state.h"
#include "../dse/abstract_dep_graph.h"
#include <commons/verbose.h>
#include <models/Dataflow.h>
#include <printers/SDF3Wrapper.h> // to write XML files
#include "algorithms/transformation/merge_output.h"
#include "algorithms/vhdl_generation/VHDLCommons.h"
#include "commons/commons.h"
#include "singleOutput.h"

std::vector<std::string> mergeableOperators = { "fp_add", "fp_prod", "fp_div",
                                                "fp_sqrt", "fp_diff", "fp_pow",
                                                "int_add", "int_diff", "int_prod",
                                                "float2int", "int2float", "fp_floor",
                                                "int_max", "int_min", "fp_max",
                                                "fp_min", "fp_abs" };
std::vector<std::string> mergeStrategies = {"greedy", "smart"};
bool osAsBroadcast = false; // used for when the output selector should act as a simple broadcast
int broadcastBufferCnt = 0;
implType t = TT;

/**
   Applies a specified merging strategy to the given dataflow graph.

   @param dataflow HSDF/SDF graph

   @param params Parameters that dictate how the dataflow graph is transformed
   by the merging operations. Parameters are as follows:
   - MERGE_STRATEGY: Strategy to define which actors are merged -
   smart/greedy (greedy).
   - FREQUENCY: Clock cycle frequency (in MHz) of VHDL design; affects execution
   duration (and thus schedule and mergeability) of actors.
   - BROADCAST: Define as t to set output selectors to act as broadcasters
   (default: false). Scheduled buffers are placed on each output edge of the
   broadcaster instead to imitate functionality of output selector.
   - BUFFER_TYPE: Use specified buffer type instead of scheduled buffers for
   buffer components in VHDL implementation. Options: shiftreg/sbuffer (default).
 */
void algorithms::transformation::merge_operators(models::Dataflow* const dataflow,
                                                 parameters_list_t params) {
  std::string mergeStrategy = "greedy"; // default strategy used if none specified
  int operatorFreq = 250;               // operator compute frequency in MHz
  std::string bufferImpl = "sbuffer";   // default buffer type if none specified

  if (params.find("MERGE_STRATEGY") != params.end()) {
    VERBOSE_INFO("MERGE STRATEGY: " << params["MERGE_STRATEGY"]);
    if (std::find(mergeStrategies.begin(),
                  mergeStrategies.end(),
                  params["MERGE_STRATEGY"]) != mergeStrategies.end()) {
      mergeStrategy = params["MERGE_STRATEGY"];
    } else {
      VERBOSE_ERROR("Specified merge strategy (" << params["MERGE_STRATEGY"]
                    << ") is unsupported."); // TODO list valid merge strategies
    }
  } else {
    VERBOSE_INFO("No merge strategy specified, defaulting to: " << mergeStrategy);
  }
  // check if operator frequencies have been specified
  if (params.find("FREQUENCY") != params.end()) {
    VERBOSE_INFO("Operator frequency set to " << params["FREQUENCY"]);
    operatorFreq = std::stoi(params["FREQUENCY"]);
  } else {
    VERBOSE_INFO("Default operator frequency used (" << operatorFreq << "), you can use -p FREQUENCY=frequency_in_MHz to set the operator frequency");
  }

  if (params.find("BROADCAST") != params.end()) {
    VERBOSE_INFO("Adding gates to the output edges of output selectors");
    osAsBroadcast = true;
  }

  // Necessary to update placeholder buffer type to a specified implementation
  // as VHDLComponent types are instantiated during merge
  // NOTE might not be necessary anymore
  if (params.find("BUFFER_TYPE") != params.end()) {
    VERBOSE_INFO("Set buffer implementation to type: " << params["BUFFER_TYPE"]);
    bufferImpl = params["BUFFER_TYPE"];
  }
  {ForEachVertex(dataflow, v) {
      if (dataflow->getVertexType(v) == "buffer") {
        dataflow->setVertexType(v, bufferImpl);
      }
    }}

  if (params.find("DATA_DRIVEN") != params.end()) {
    VERBOSE_INFO("Setting implementation type to data-driven");
    t = DD;
  }

  // check and adjust for any operators with multiple I/Os
  // the merge function currently only works with operators with the same number
  // of inputs/outputs
  if (params.find("NORMALISE_OUTPUTS") != params.end()) {
    VHDLCircuit tmp = generateCircuitObject(dataflow, t);
    while (tmp.getMultiOutActors().size() > 0) {

      VERBOSE_INFO("getMultiOutActors is not empty");
      /*  This block remove multiIO actors and replace them  */
      for (std::string actorName: tmp.getMultiOutActors()) {
        parameters_list_t parameters;
        parameters["name"] = actorName;
        VERBOSE_INFO("singleOutput actor " << actorName);
        try {
          singleOutput(dataflow, parameters);
        } catch (...) {
          VERBOSE_WARNING("actor missing!");
        }
      }
      VERBOSE_INFO("Regenerate Circuit");
      tmp = generateCircuitObject(dataflow, t);
    }
    VERBOSE_ASSERT (tmp.getMultiOutActors().size() == 0, "Error while add Dups") ;
  }

  while (getMultiOutputActors(dataflow).size() > 0) {
    VERBOSE_INFO("getMultiOutputActors is not empty");
    for (std::string actorName : getMultiOutputActors(dataflow)) {
      parameters_list_t parameters;
      parameters["name"] = actorName;
      VERBOSE_INFO("merge output for actor " << actorName);
      try {
        merge_output(dataflow, parameters);
      } catch (...) {
        VERBOSE_WARNING("actor missing!");
      }
    }
    VERBOSE_INFO("Regenerate Circuit");
  }

  // Begin merge operation
  int isOffset = 0;
  int osOffset = 0;
  std::vector<std::vector<ARRAY_INDEX>> mergeVectorIds;
  if (mergeStrategy == "greedy") {
    mergeVectorIds = greedyMerge(dataflow, operatorFreq);
  } else if (mergeStrategy == "smart") {
    mergeVectorIds = smartMerge(dataflow, operatorFreq);
  }
  VERBOSE_DEBUG("Merge actors generated using merge strategy: " << mergeStrategy);
  // just for debugging purposes
  for (auto &ids : mergeVectorIds) {
    VERBOSE_DEBUG("\tMerge group:");
    for (auto &id : ids) {
      VERBOSE_DEBUG("\t\t" << id << "(" << dataflow->getVertexName(dataflow->getVertexById(id))
                    << ", " << dataflow->getVertexType(dataflow->getVertexById(id)) << ")");
    }
  }

  for (auto &ids : mergeVectorIds) { // repeatedly call generateMergedGraph for each group of actors
    VERBOSE_DEBUG("\tMerging:");
    std::vector<Vertex> mergeVector;
    // storing IDs and retrieving vertices just before merging is the only way I've been able to get this to work
    for (auto &id : ids) {
      mergeVector.push_back(dataflow->getVertexById(id));
      VERBOSE_DEBUG("\t\t" << id << "(" << dataflow->getVertexName(dataflow->getVertexById(id))
                   << ", " << dataflow->getVertexType(dataflow->getVertexById(id)) << ")");
    }
    generateMergedGraph(dataflow, mergeVector, isOffset, osOffset); // NOTE mergeList of actors needs to be in their expected order of execution
  }

  // revert buffer type to generic placeholder after transform
  {ForEachVertex(dataflow, v) {
      if (dataflow->getVertexType(v) == bufferImpl) {
        dataflow->setVertexType(v, "buffer");
      }
    }}

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
  std::vector<TIME_UNIT> mergedDurations;
  for (auto v : vertices) {
    vInDegree.push_back(dataflow->getVertexInDegree(v));
    vOutDegree.push_back(dataflow->getVertexOutDegree(v));
    mergedDurations.push_back(dataflow->getVertexDuration(v));
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
  std::map<int, std::map<std::string, int>> outDataTypes;
  std::vector<std::string> actorNames; // we track these to replace with the name of the output selector

  // store information from each actor that we're going to merge
  int actorCount = 0;
  for (auto v : vertices) {
    std::vector<std::string> inEdgeNames;
    std::vector<std::string> inPortNames;
    std::vector<std::string> outEdgeNames;
    std::vector<std::string> outPortNames;
    std::string actorBaseName = commons::split<std::string>(dataflow->getVertexName(v), '_').front();
    actorNames.push_back(actorBaseName);
    argOrder[actorCount] = getArgOrderFromName(dataflow->getVertexName(v));
    outDataTypes[actorCount] = getOutputDataTypes(dataflow, v);
    std::string actorName = dataflow->getVertexName(v);
    for (auto argActorName : argOrder[actorCount]) { // store the operands (in the form of edges and ports) in the order indicated by argOrder
      {ForInputEdges(dataflow, v, inEdge) {
          Vertex inputActor = dataflow->getEdgeSource(inEdge);
          std::string inputActorName = commons::split<std::string> (dataflow->getVertexName(inputActor), '_').front();
          if (inputActorName == argActorName) { // the edge connects the input actor to this actor
            std::string edgeName = dataflow->getEdgeName(inEdge);
            std::string inPortName = dataflow->getEdgeOutputPortName(inEdge);
            if (std::find(inEdgeNames.begin(), inEdgeNames.end(), edgeName) ==
                inEdgeNames.end() &&
                std::find(inPortNames.begin(), inPortNames.end(), inPortName) ==
                inPortNames.end()) { // avoid adding duplicate edge/port names
              inEdgeNames.push_back(edgeName);
              inPortNames.push_back(inPortName);
            }
          }
        }
      }
    }
    VERBOSE_ASSERT(dataflow->getVertexOutDegree(v) == 1,
                   "Out degree for: " << dataflow->getVertexName(v) << ": "
                                      << dataflow->getVertexOutDegree(v));
    {ForOutputEdges(dataflow, v, outEdge) {
        // NOTE assuming here that there's only ever 1 output edge per vertice
        outEdgeNames.push_back(dataflow->getEdgeName(outEdge));
        outPortNames.push_back(dataflow->getEdgeInputPortName(outEdge));
      }}
    inEdges[actorCount] = inEdgeNames;
    inPorts[actorCount] = inPortNames;
    outEdges[actorCount] = outEdgeNames;
    outPorts[actorCount] = outPortNames;
    actorCount++;
    VERBOSE_ASSERT(dataflow->getVertexInDegree(v) == inEdgeNames.size(),
                   "Vertex in degree unequal for " << dataflow->getVertexName(v) << ", " << dataflow->getVertexType(v) << ": " << dataflow->getVertexInDegree(v) << "!=" << inEdgeNames.size());
    VERBOSE_ASSERT(dataflow->getVertexOutDegree(v) == outEdgeNames.size(),
                   "Vertex out degree unequal for "  << dataflow->getVertexName(v)  << ", " << dataflow->getVertexType(v) << ": " << dataflow->getVertexOutDegree(v) << "!=" << outEdgeNames.size() << "\n" << commons::toString(outEdgeNames));
  }

  dataflow->reset_computation(); // necessary to edit dataflow graph
  // Number of input/output selectors added should be equal to the input/output counts of the merged operators
  size_t inDeg = dataflow->getVertexInDegree(vertices.front());
  size_t outDeg = dataflow->getVertexOutDegree(vertices.front());
  // Create the merged operator + its input and output selector
  std::string mergedActorName = "merged";
  for (int i = 0; i < inDeg; i++) {
    mergedActorName += "_inputselector" + commons::toString(i + isOffset);
  }
  Vertex mergedActor = dataflow->addVertex(mergedActorName);
  dataflow->setPhasesQuantity(mergedActor, vertices.size());
  dataflow->setVertexDuration(mergedActor, mergedDurations);
  dataflow->setVertexType(mergedActor, dataflow->getVertexType(vertices.front())); // merged vertices are of the same type
  // add re-entrancy edges
  dataflow->setReentrancyFactor(mergedActor, 1);

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
    std::string edgeType = "";
    std::map<std::string, int> inputDataTypes;
    {ForInputEdges(dataflow, new_is, inEdge) {
        std::string inEdgeName = dataflow->getEdgeName(inEdge);
        if (inEdgeName.substr(inEdgeName.rfind("_") + 1) == "vect") {
          inputDataTypes["real_vect"]++;
        } else {
          inputDataTypes[inEdgeName.substr(inEdgeName.rfind("_") + 1)]++;
        }
      }}
    if (inputDataTypes.size() > 1) {
      VERBOSE_WARNING(inputDataTypes.size() << " input types on input selector " << isId << "; setting to real.");
      edgeType = "real"; // TODO make this not hard-coded
    } else {
      edgeType = inputDataTypes.begin()->first;
    }
    // use edge ID to avoid edge name conflicts
    Edge outEdge = dataflow->addEdge(new_is, mergedActor);
    std::string outEdgeName = "channel_" + commons::toString(dataflow->getEdgeId(outEdge) + dataflow->getEdgesCount()) + "_" + edgeType;
    VERBOSE_WARNING("Output edge from input selector " << isId << "set to: " << outEdgeName);
    dataflow->setEdgeName(outEdge, outEdgeName);
    dataflow->setEdgeInPhases(outEdge, execRates);
    dataflow->setEdgeOutPhases(outEdge, execRates);
    dataflow->setEdgeInputPortName(outEdge, "in_" + outEdgeName);
    dataflow->setEdgeOutputPortName(outEdge, "out_" + outEdgeName);
    dataflow->setPreload(outEdge, 0);
    dataflow->setTokenSize(outEdge, 1);

    // add re-entrancy edges/ports
    dataflow->setReentrancyFactor(new_is, 1);
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
    Edge mergedToOS = dataflow->addEdge(mergedActor, new_os);
    std::string outEdgeName = "channel_" + commons::toString(dataflow->getEdgeId(mergedToOS) + dataflow->getEdgesCount()) + "_" + edgeType;
    dataflow->setEdgeName(mergedToOS, outEdgeName);
    dataflow->setEdgeInPhases(mergedToOS, execRates);
    dataflow->setEdgeOutPhases(mergedToOS, execRates);
    dataflow->setEdgeInputPortName(mergedToOS, "in_" + outEdgeName);
    dataflow->setEdgeOutputPortName(mergedToOS, "out_" + outEdgeName);
    dataflow->setPreload(mergedToOS, 0);
    dataflow->setTokenSize(mergedToOS, 1);

    // output edges from output selector connect to target actors of the merged actors
    for (auto const &edge : outEdges) {
      // store information of original edge before removing it
      Edge ogEdge = dataflow->getEdgeByName(edge.second[i]);
      std::string baseName =
          dataflow->getVertexName(dataflow->getEdgeSource(ogEdge));
      baseName = baseName.substr(0, baseName.find("_"));
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
    dataflow->setReentrancyFactor(new_os, 1);
    // batch renaming of affected actors (targets of merged vertices)
    std::map<Vertex, std::string> newNames;
    {ForEachVertex(dataflow, v) {
        newNames[v] = dataflow->getVertexName(v);
      }}
    for (const auto &name : actorNames) {
      for (const auto &[vertex, newName] : newNames) {
        std::string replName = replaceActorName(
            newNames[vertex], name, "outputselector" + commons::toString(osId));
        // store and retain name updates as we iterate
        if (newNames[vertex] != replName) {
          newNames[vertex] = replName;
        }
      }
    }
    for (const auto &[vertex, newName] : newNames) {
      dataflow->setVertexName(vertex, newName);
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

/**
   Replace occurances of a string with a specified replacement

   @param targetString String to operate on.
   @param toReplace Phrase to replace in targetString.
   @param replacement Replacement for string specified by toReplace.
   @param replacementMask Mask defining which phrases to replace given multiple matches.
**/ // TODO break into modular functions: e.g. splitIntoPhrases, replacePhrase, replaceActorName
std::string algorithms::replaceActorName(std::string targetString,
                                         const std::string &toReplace,
                                         const std::string &replacement,
                                         std::vector<TOKEN_UNIT> replacementMask) {
  std::vector<std::string> actorNames =
      commons::split<std::string>(targetString, '_');
  VERBOSE_ASSERT(actorNames.size() > 0, "Non-empty string must be used.");
  if (actorNames.size() == 1) {
    return targetString; // nothing to replace if no args listed after base name
  }

  if (replacementMask.size() == 1) { // replace all occurances of toReplace with replacement
    // actorNames.erase(actorNames.begin());
    for (auto i = 1; i < actorNames.size(); i++) { // only elements after the first are its args
      if (actorNames[i] == toReplace) {
        actorNames[i] = replacement;
      }
    }
    targetString = commons::join(actorNames.begin(), actorNames.end(), std::string("_"));
  } else if (replacementMask.size() > 1) { // only replace at unmasked positions // TODO update this clause section to use split and join functions
    std::vector<size_t> argStartPos;
    size_t startPos = targetString.find("_");
    if (startPos != std::string::npos) {
      while ((startPos = targetString.find("_", startPos)) !=
             std::string::npos) {
        // populate vector with start positions of matched replacement
        argStartPos.push_back(startPos + 1);
        startPos += 1;
      }
      // use replacementMask vector to define matched instances to replace
      for (auto i = 0; i < replacementMask.size(); i++) {
        if (replacementMask[i]) {
          VERBOSE_ASSERT(
              targetString.substr(argStartPos[i], toReplace.length()) ==
                  toReplace,
              "argument identified for replacement ("
                  << targetString.substr(argStartPos[i], toReplace.length())
                  << ") doesn't match intended replacement: " << toReplace);
          targetString.replace(argStartPos[i], toReplace.length(), replacement);
        }
      }
    }
  }

  return targetString;
}

std::vector<std::vector<ARRAY_INDEX>> algorithms::greedyMerge(models::Dataflow* const dataflow,
                                                              int operatorFreq) {
  std::vector<std::vector<ARRAY_INDEX>> matchingOperators;
  std::vector<std::string> typesToMerge;
  std::map<std::string, std::vector<ARRAY_INDEX>> mergeableIds;
  std::vector<ARRAY_INDEX> tmpMatches;
  abstractDepGraph absDepGraph(dataflow); // initialise abstract dependency graph
  std::map<ARRAY_INDEX, int> execTimes; // vertex ID -> exec time
  std::map<int, std::vector<ARRAY_INDEX>> executionTime; // execution time, vector of vertex IDs
  std::vector<std::string> outputActorNames;
  std::map<std::string, int> opCounts;

  {ForEachVertex(dataflow, v) {
      std::string opType = deriveOpCat(dataflow, v);
      // count occurances of mergeable operator types in graph
      if (std::find(mergeableOperators.begin(),
                    mergeableOperators.end(),
                    opType) != mergeableOperators.end()) {
        opCounts[opType]++;
      }
      // update execution time in dataflow according to component operator type
      std::vector<TIME_UNIT> opLifespans(
          dataflow->getVertexPhaseDuration(v).size(),
         getOperatorLifespan(opType, operatorFreq));
        dataflow->setVertexDuration(v, opLifespans);
    }}
  // track the operator types that we might want to merge (i.e. multiple occurances of the same type)
  for (auto &types : opCounts) {
    if (types.second > 1) {
      typesToMerge.push_back(types.first);
    }
  }

  // to identify all relevant causal dependencies, we look for dependencies from the output actors
  std::map<ARRAY_INDEX, bool> visited;
  {ForEachVertex(dataflow, v) {
      std::string actorName = dataflow->getVertexName(v);
      visited[dataflow->getVertexId(v)] = false;
      if (actorName.find("OUTPUT", 0) != std::string::npos) { // we match by name to avoid having to instantiate new VHDLComponent objects
        outputActorNames.push_back(actorName);
      }
    }}
  VERBOSE_ASSERT(outputActorNames.size() > 0,
                 "No output actors identified; at least 1 output actor needed to run causal dependency detection.");
  for (auto &name : outputActorNames) {
    Vertex outputActor = dataflow->getVertexByName(name);
    findCausalDependency(dataflow, outputActor, absDepGraph, visited);
  }
  VERBOSE_DEBUG(absDepGraph.printStatus());

  // compute execution times using causal dependency graph
  {ForEachVertex(dataflow, v) {
      // start from actors without causal dependencies to compute execution time
      if (!absDepGraph.hasDependency(dataflow->getVertexId(v))) {
        absDepGraph.computeExecTime(dataflow, dataflow->getVertexId(v), execTimes);
      }
    }}
  // group actor IDs by execution times so we know which ones execute at the same time
  for (auto &time : execTimes) {
    executionTime[time.second].push_back(time.first);
  }

  // generate mergeable operators
  VERBOSE_DEBUG("Execution times: ");
  for (auto &e : executionTime) {
    VERBOSE_DEBUG("Time: " << e.first);
    for (auto &id : e.second) {
      VERBOSE_DEBUG("\t" << id << "(" << dataflow->getVertexName(dataflow->getVertexById(id))
                    << ", " << dataflow->getVertexType(dataflow->getVertexById(id)) << ")");
      std::string opType = deriveOpCat(dataflow, dataflow->getVertexById(id));
      if (std::find(typesToMerge.begin(), typesToMerge.end(), opType) != typesToMerge.end()) {
        mergeableIds[opType].push_back(id);
      }
    }
  }

  for (auto &ids : mergeableIds) {
    if (ids.second.size() > 1) { // only add to merge list if there's more than one operator
      matchingOperators.push_back(ids.second);
    }
  }

  return matchingOperators;
}

std::vector<std::vector<ARRAY_INDEX>> algorithms::smartMerge(models::Dataflow* const dataflow,
                                                             int operatorFreq) {
  std::vector<std::vector<ARRAY_INDEX>> matchingOperators;
  std::vector<std::string> typesToMerge;
  std::map<std::string, std::vector<ARRAY_INDEX>> mergeableIds;
  std::vector<ARRAY_INDEX> tmpMatches;
  abstractDepGraph absDepGraph(dataflow); // initialise abstract dependency graph
  std::map<ARRAY_INDEX, int> execTimes; // vertex ID -> exec time
  std::map<int, std::vector<ARRAY_INDEX>> executionTime; // execution time, vector of vertex IDs
  std::vector<std::string> outputActorNames;
  std::map<std::string, int> opCounts;

  {ForEachVertex(dataflow, v) {
      std::string opType = deriveOpCat(dataflow, v);
      // check for occurances of mergeable operator types
      if (std::find(mergeableOperators.begin(),
                    mergeableOperators.end(),
                    opType) != mergeableOperators.end()) {
        opCounts[opType]++;
      }
      // update execution time in dataflow according to component operator type
      std::vector<TIME_UNIT> opLifespans(
                                         dataflow->getVertexPhaseDuration(v).size(),
                                         getOperatorLifespan(opType, operatorFreq));
      dataflow->setVertexDuration(v, opLifespans);
    }}
  // track the operator types that we might want to merge (i.e. multiple occurances of the same type)
  for (auto &types : opCounts) {
    if (types.second > 1) {
      typesToMerge.push_back(types.first);
    }
  }

  // to identify all relevant causal dependencies, we look for dependencies from the output actors
  std::map<ARRAY_INDEX, bool> visited;
  {ForEachVertex(dataflow, v) {
      std::string actorName = dataflow->getVertexName(v);
      visited[dataflow->getVertexId(v)] = false;
      if (actorName.find("OUTPUT", 0) != std::string::npos) { // we match by name to avoid having to instantiate new VHDLComponent objects
        outputActorNames.push_back(actorName);
      }
    }}
  VERBOSE_ASSERT(outputActorNames.size() > 0,
                 "No output actors identified; at least 1 output actor needed to run causal dependency detection.");
  for (auto &name : outputActorNames) {
    Vertex outputActor = dataflow->getVertexByName(name);
    findCausalDependency(dataflow, outputActor, absDepGraph, visited);
  }
  VERBOSE_DEBUG(absDepGraph.printStatus());

  // compute execution times using causal dependency graph
  {ForEachVertex(dataflow, v) {
      // start from actors without causal dependencies to compute execution time
      if (!absDepGraph.hasDependency(dataflow->getVertexId(v))) {
        absDepGraph.computeExecTime(dataflow, dataflow->getVertexId(v), execTimes);
      }
    }}
  // group actor IDs by execution times so we know which ones execute at the same time
  for (auto &time : execTimes) {
    executionTime[time.second].push_back(time.first); // exec time -> actor ID
  }

  // generate mergeable operators
  VERBOSE_DEBUG("Execution times: ");
  for (auto &e : executionTime) {
    VERBOSE_DEBUG("Time: " << e.first);
    std::map<std::string, int> opCountForExecTime; // occurances of each operator type executing at this time
    for (auto &type : typesToMerge) {
      opCountForExecTime[type] = 0;
    }
    for (auto &id : e.second) {
      VERBOSE_DEBUG(
          "\t" << id << "("
               << dataflow->getVertexName(dataflow->getVertexById(id)) << ", "
               << dataflow->getVertexType(dataflow->getVertexById(id)) << ")");
      std::string opType = deriveOpCat(dataflow, dataflow->getVertexById(id));
      if (std::find(typesToMerge.begin(), typesToMerge.end(), opType) !=
          typesToMerge.end()) {
        // group by occurance count to generate separate merge groups for
        // operators (of the same type) executing in parallel
        mergeableIds[opType + std::to_string(opCountForExecTime[opType])].push_back(id);
        opCountForExecTime[opType]++;
      }
    }
  }

  for (auto &ids : mergeableIds) {
    if (ids.second.size() > 1) { // only add to merge list if there's more than one operator
      matchingOperators.push_back(ids.second);
    }
  }

  return matchingOperators;
}

// recursively look for causal dependencies starting from the specified Vertex v
void algorithms::findCausalDependency(models::Dataflow *const dataflow,
                                      Vertex v, abstractDepGraph &g,
                                      std::map<ARRAY_INDEX, bool> &visited) {
  visited[dataflow->getVertexId(v)] = true;
  {ForInputEdges(dataflow, v, inEdge) {
      Vertex source = dataflow->getEdgeSource(inEdge);
      TOKEN_UNIT consRate = dataflow->getEdgeOutPhase(inEdge, 1);
      if (dataflow->getPreload(inEdge) < consRate) {
        g.addCausalDep(dataflow->getVertexId(source), dataflow->getVertexId(v));
      }
      if (!visited[dataflow->getVertexId(source)]) {
        findCausalDependency(dataflow, source, g, visited);
      }
    }}
}

// place vertices along the same path, thereby enforcing a sequential operation
// the output of the first vertex is broadcasted to the second vertex as well as
// the original target vertex
void algorithms::sequentialiseVertices(models::Dataflow *const dataflow,
                                       Vertex v1, Vertex v2) {
  Edge a1, a2, b1, b2;
  {ForInputEdges(dataflow, v1, e) { a1 = e; }}
  {ForOutputEdges(dataflow, v1, e) { a2 = e; }}
  {ForInputEdges(dataflow, v2, e) { b1 = e; }}
  {ForOutputEdges(dataflow, v2, e) { b2 = e; }}
  Vertex v1Target = dataflow->getEdgeTarget(a2);
  std::string a2Name = dataflow->getEdgeName(a2);
  std::string b1Name = dataflow->getEdgeName(b1);
  // TODO test with using just base name of v1
  Vertex broadcast = dataflow->addVertex("broadcast" + dataflow->getVertexName(v1));
  dataflow->setVertexType(broadcast, "broadcast");
  dataflow->setPhasesQuantity(broadcast, 1);
  dataflow->setVertexDuration(broadcast, {0});
  dataflow->setReentrancyFactor(broadcast, 1);
  std::vector<TOKEN_UNIT> a2InPhases = dataflow->getEdgeInVector(a2);
  std::vector<TOKEN_UNIT> a2OutPhases = dataflow->getEdgeOutVector(a2);
  dataflow->removeEdge(a2);
  dataflow->removeEdge(b1);

  // connect broadcast actor to graph (between edges a2 and b1)
  Edge a2New = dataflow->addEdge(v1, broadcast, a2Name);
  dataflow->setEdgeInPhases(a2New, {1});
  dataflow->setEdgeOutPhases(a2New, {1});
  dataflow->setEdgeInputPortName(a2New, ("in_" + a2Name));
  dataflow->setEdgeOutputPortName(a2New, ("out_" + a2Name));
  Edge a2Prime = dataflow->addEdge(broadcast, v1Target, "broadcast" + a2Name);
  dataflow->setEdgeInPhases(a2Prime, a2InPhases);
  dataflow->setEdgeOutPhases(a2Prime, a2OutPhases);
  dataflow->setEdgeInputPortName(a2Prime, ("in_broadcast" + a2Name));
  dataflow->setEdgeOutputPortName(a2Prime, ("out_broadcast" + a2Name));
  Edge b1New = dataflow->addEdge(broadcast, v2, b1Name);
  dataflow->setEdgeInPhases(b1New, {1});
  dataflow->setEdgeOutPhases(b1New, {1});
  dataflow->setEdgeInputPortName(b1New, ("in_" + b1Name));
  dataflow->setEdgeOutputPortName(b1New, ("out_" + b1Name));
  // dataflow->setPreload(b1New, 1); // need initial token to maintain schedule

  // update initial tokens
  dataflow->setPreload(a2New, 1);
  dataflow->setPreload(b2, 1);

  std::string srcName = dataflow->getVertexName(broadcast);
  // TODO test with replacing with just base name of actors
  std::string v1TargetNewName = replaceActorName(
      dataflow->getVertexName(v1Target), dataflow->getVertexName(v1), srcName);
  dataflow->setVertexName(v1Target, v1TargetNewName);
}

/**
   Replace output selectors with broadcast components with buffers on each
   output edge.

   @param dataflow HSDF/SDF graph.
   @param params Parameters that dictate how the dataflow graph is transformed
   by the given algorithm. Parameter options (if they exist) are as follows:
 */
void algorithms::transformation::broadcast_os(models::Dataflow *const dataflow,
                                              parameters_list_t params) {
  int bufferCount = 0; // append count to names to avoid naming conflicts
  {ForEachVertex(dataflow, v) {
      if (dataflow->getVertexType(v) == "output_selector") {
        // Update number of phases:
        // broadcast just needs 1 phase of execution as opposed to OS
        EXEC_COUNT numPhases =
            dataflow->getPhasesQuantity(v); // passed on to buffers
        std::vector<TOKEN_UNIT> osInPhases;
        dataflow->setVertexType(v, "broadcast");
        dataflow->setPhasesQuantity(v, 1);
        dataflow->setVertexDuration(v, {1});
        std::string osName = dataflow->getVertexName(v); // store for replacement later in affected target actors
        {ForInputEdges(dataflow, v, inputEdge) {
            osInPhases = dataflow->getEdgeOutVector(inputEdge); // passed on to buffer
            dataflow->setEdgeOutPhases(inputEdge, {1});
          }}

        // Add buffers on each output edge between OS and target
        // OS ---{toBuffer}--> buffer --{newEdge}--> target
        std::vector<Edge> outputEdges;
        std::map<Vertex, std::vector<std::string>> newArgNames; // track names to update
        {ForOutputEdges(dataflow, v, e) { outputEdges.push_back(e); }}
        for (auto e : outputEdges) {
          bufferCount++;
          std::string edgeName = dataflow->getEdgeName(e);
          Vertex ogTarget = dataflow->getEdgeTarget(e);
          TOKEN_UNIT preload = dataflow->getPreload(e);
          DATA_UNIT tokenSize = dataflow->getTokenSize(e);
          Vertex buffer = dataflow->addVertex(
              "osbuffer" + std::to_string(bufferCount));
          dataflow->setReentrancyFactor(buffer, 1);
          dataflow->setVertexType(buffer, "buffer"); // buffer type defined during VHDL generation
          dataflow->setPhasesQuantity(buffer, numPhases);
          dataflow->setVertexDuration(buffer,
                                      std::vector<TIME_UNIT>(numPhases, 1));
          newArgNames[ogTarget].push_back("osbuffer" + std::to_string(bufferCount)); // add new name for updating later
          Edge toBuffer = dataflow->addEdge(v, buffer, "broadcast" + edgeName);
          dataflow->setEdgeInPhases(toBuffer, {1});
          dataflow->setEdgeOutPhases(toBuffer, osInPhases);
          dataflow->setEdgeInputPortName(toBuffer, ("in_broadcast" + edgeName));
          dataflow->setEdgeOutputPortName(toBuffer, ("out_broadcast" + edgeName));
          dataflow->setPreload(toBuffer, 0);
          dataflow->setTokenSize(toBuffer, tokenSize);
          Edge newEdge = dataflow->addEdge(buffer, ogTarget, "tmp"); // rename later to avoid naming with same name
          dataflow->setEdgeInPhases(newEdge, dataflow->getEdgeInVector(e));
          dataflow->setEdgeOutPhases(newEdge, dataflow->getEdgeOutVector(e));
          dataflow->setEdgeInputPortName(newEdge, ("in_" + edgeName));
          dataflow->setEdgeOutputPortName(newEdge, ("out_" + edgeName));
          dataflow->setPreload(newEdge, preload);
          dataflow->setTokenSize(newEdge, tokenSize);

          // restore original name to new edge
          dataflow->removeEdge(e);
          dataflow->setEdgeName(newEdge, edgeName);
        }
        // update names for arg order
        for (const auto &[vertex, names] : newArgNames) {
          std::vector<std::string> newArgOrder;
          std::string vertexName = dataflow->getVertexName(vertex);
          newArgOrder.push_back(commons::split<std::string>(vertexName, '_').front());
          std::vector<std::string> argOrder = getArgOrderFromName(vertexName);
          int i = 0;
          for (auto const &oldArg : argOrder) {
            if (oldArg == osName) { // replace occurances of output selector name with new arg
              newArgOrder.push_back(names[i]);
              i++;
            } else {
              newArgOrder.push_back(oldArg);
            }
          }
          dataflow->setVertexName(vertex, commons::join(newArgOrder, "_"));
        }
      }
    }}
}

/**
   Sequentialises buffers with parallel data storage to multiple instances of
   data storage.

   @param dataflow HSDF/SDF graph
   @param params Parameters that dictate how the dataflow graph is transformed
   by the given algorithm. Parameter options (if they exist) are as follows:
 */
void algorithms::transformation::pipeline_buffers(models::Dataflow *const dataflow,
                                                  parameters_list_t params) {
  // TODO parameterise source actor selection
  Vertex src;
  bool isInitialised = false;
  {ForEachVertex(dataflow, v) {
      std::string vertexType = deriveOpCat(dataflow, v);
      if (vertexType == "INPUT") {
        src = v;
        isInitialised = true;
      } else if (vertexType == "broadcast") {
        // also pipeline buffers from broadcast components
        // connected to the top input components
        bool broadcastInput = false;
        {ForInputEdges(dataflow, v, inEdge) {
            Vertex inputVertex = dataflow->getEdgeSource(inEdge);
            std::string inputType = deriveOpCat(dataflow, inputVertex);
            VERBOSE_INFO(inputType);
            if (inputType == "INPUT") {
              broadcastInput = true;
            } else {
              broadcastInput = false;
              break;
            }
          }}
        if (broadcastInput) {
          src = v;
          isInitialised = true;
        }
      }
    }}
  if (!isInitialised) {
    VERBOSE_WARNING("No input actor found. Transformation bypassed.");
    return;
  }

  // sort buffers on output edges of src actor by init tokens
  std::map<int, std::vector<Vertex>> buffers;
  {ForOutputEdges(dataflow, src, e) {
    Vertex targetActor = dataflow->getEdgeTarget(e);
    if (dataflow->getVertexType(targetActor) != "buffer") {
      VERBOSE_WARNING("Pipelining only works when all target actors are "
                      "buffers. Transformation bypassed.");
      return;
    }
    {ForOutputEdges(dataflow, targetActor, outEdge) {
        buffers[(int)dataflow->getPreload(outEdge)].push_back(targetActor);
      }}
    }}
    // checks for whether pipelining can be performed
    if (buffers.size() < 2) {
      VERBOSE_WARNING("Insufficient buffers to perform pipelining. "
                      "Transformation bypassed.");
      return;
    }
    int expectedTokens = buffers.begin()->first;
    for (const auto &[initTokens, bufferVertices] : buffers) {
      if (initTokens != expectedTokens) {
        VERBOSE_WARNING("Pipelining not supported when tokens not in running "
                        "order. Transformation bypassed.");
        return;
      }
      expectedTokens++;
    }

  for (auto it = buffers.begin(); it != buffers.end(); it++) {
    auto next = std::next(it);
    if (next != buffers.end()) {
      sequentialiseVertices(dataflow, it->second.front(), next->second.front());
    }
  }
}
