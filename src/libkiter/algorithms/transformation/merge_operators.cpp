/*
 *  iterative_evaluation.cpp
 *
 *  Created on: Aug 2, 2021
 *      Author: jkmingwen
 */


#include "merge_operators.h"
#include "../vhdl_generation/VHDLComponent.h"
#include "../vhdl_generation/VHDLCircuit.h"
#include "../vhdl_generation/VHDLGenerator.h"
#include "../throughput/actor.h"
#include "../throughput/state.h"
#include "../dse/buffer_sizing.h"
#include "../dse/abstract_dep_graph.h"
#include <commons/verbose.h>
#include <models/Dataflow.h>
#include <printers/SDF3Wrapper.h> // to write XML files
#include "singleOutput.h"

void algorithms::transformation::merge_operators(models::Dataflow* const dataflow,
                                                 parameters_list_t params) {
  // TODO produce vector of operators to merge by operator type
  // TODO iteratively call generateMergedGraph
  bool changeDetected = true;
  bool outputSpecified = false;
  bool mergeStrategySpecified = false;
  std::string dirName = "./"; // use current directory as default
  std::string outputName = dataflow->getGraphName() + "_merged" + ".xml"; // use graph name as default
  std::string mergeStrategy = "greedy";
  int operatorFreq = 125; // default target operator frequency is 125MHz

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
  if (params.find("MERGE_STRATEGY") != params.end()) {
    if (std::find(mergeStrategies.begin(),
                  mergeStrategies.end(),
                  params["MERGE_STRATEGY"]) != mergeStrategies.end()) {
      mergeStrategySpecified = true;
      mergeStrategy = params["MERGE_STRATEGY"];
      VERBOSE_INFO("MERGE STRATEGY: " << params["MERGE_STRATEGY"]);
    } else {
      VERBOSE_ERROR("Specified merge strategy (" << params["MERGE_STRATEGY"]
                    << ") is unsupported."); // TODO list valid merge strategies
    }
  }
  if (!mergeStrategySpecified) {
    VERBOSE_INFO("No merge strategy specified, defaulting to greedy merge.");
  }
  // check if operator frequencies have been specified
  if (params.find("FREQUENCY") != params.end()) {
    VERBOSE_INFO("Operator frequency set to " << params["FREQUENCY"]);
    operatorFreq = std::stoi(params["FREQUENCY"]);
  } else {
    VERBOSE_INFO("Default operator frequency used (" << operatorFreq << "), you can use -p FREQUENCY=frequency_in_MHz to set the operator frequency");
  }

  // check and adjust for any operators with multiple I/Os
  // the merge function currently only works with operators with the same number of inputs/outputs
  VHDLCircuit tmp = generateCircuitObject(dataflow, true, operatorFreq); // set bufferless to true -- inconsequential in this use case
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
    tmp = generateCircuitObject(dataflow, true, operatorFreq); // set bufferless to true -- inconsequential in this use case
  }
  VERBOSE_ASSERT (tmp.getMultiOutActors().size() == 0, "Error while add Dups") ;

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
    if (dataflow->getVertexType(dataflow->getVertexById(ids.front())) == "prod") {
      generateMergedGraph(dataflow, mergeVector, isOffset, osOffset); // NOTE mergeList of actors needs to be in their expected order of execution
    }
  }

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
    // use edge ID to avoid edge name conflicts
    Edge outEdge = dataflow->addEdge(new_is, mergedActor);
    std::string outEdgeName = "channel_" + commons::toString(dataflow->getEdgeId(outEdge)) + "_" + edgeType;
    dataflow->setEdgeName(outEdge, outEdgeName);
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
    Edge mergedToOS = dataflow->addEdge(mergedActor, new_os);
    std::string outEdgeName = "channel_" + commons::toString(dataflow->getEdgeId(mergedToOS)) + "_" + edgeType;
    dataflow->setEdgeName(mergedToOS, outEdgeName);
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
  VHDLCircuit circuit;
  circuit.setOperatorFreq(operatorFreq);

  {ForEachVertex(dataflow, v) {
      VHDLComponent op(dataflow, v);
      circuit.addComponent(op);
      // count occurances of mergeable operator types in graph
      if (std::find(mergeableOperators.begin(),
                    mergeableOperators.end(),
                    op.getType()) != mergeableOperators.end()) {
        opCounts[op.getType()]++;
      }
      // update execution time in dataflow according to component operator type
      TIME_UNIT opLifespan = circuit.getOperatorLifespan(op.getType());
      VERBOSE_INFO("operator lifespan (" << op.getType() << "): " << opLifespan);
      if (opLifespan == 0) { // i.e. no specified lifespan (e.g. const_value, delay, Proj operators)
        std::vector<TIME_UNIT> opLifespans{1};
        if (op.getType() == "input_selector" || op.getType() == "output_selector") {
          opLifespans = dataflow->getVertexPhaseDuration(v); // special case for input and output selectors, which are CSDF components
        }
        dataflow->setVertexDuration(v, opLifespans);
      } else {
        std::vector<TIME_UNIT> opLifespans{opLifespan};
        dataflow->setVertexDuration(v, opLifespans);
      }
    }}
  // track the operator types that we might want to merge (i.e. multiple occurances of the same type)
  for (auto &types : opCounts) {
    if (types.second > 1) {
      typesToMerge.push_back(types.first);
    }
  }

  // to identify all relevant causal dependencies, we look for dependencies from the output actors
  {ForEachVertex(dataflow, v) {
      std::string actorName = dataflow->getVertexName(v);
      if (actorName.find("OUTPUT", 0) != std::string::npos) { // we match by name to avoid having to instantiate new VHDLComponent objects
        outputActorNames.push_back(actorName);
      }
    }}
  VERBOSE_ASSERT(outputActorNames.size() > 0,
                 "No output actors identified; at least 1 output actor needed to run causal dependency detection.");
  for (auto &name : outputActorNames) {
    Vertex outputActor = dataflow->getVertexByName(name);
    findCausalDependency(dataflow, outputActor, absDepGraph);
  }
  VERBOSE_DEBUG(absDepGraph.printStatus());

  // compute execution times using causal dependency graph
  {ForEachVertex(dataflow, v) {
      if (dataflow->getVertexInDegree(v) == 0) { // we only start from actors without causal dependencies (no input edges)
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
      VHDLComponent op(dataflow, dataflow->getVertexById(id));
      if (std::find(typesToMerge.begin(), typesToMerge.end(), op.getType()) != typesToMerge.end()) {
        mergeableIds[op.getType()].push_back(id);
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
  VHDLCircuit circuit;
  circuit.setOperatorFreq(operatorFreq);

  {ForEachVertex(dataflow, v) {
      VHDLComponent op(dataflow, v);
      circuit.addComponent(op);
      // check for occurances of mergeable operator types
      if (std::find(mergeableOperators.begin(),
                    mergeableOperators.end(),
                    op.getType()) != mergeableOperators.end()) {
        opCounts[op.getType()]++;
      }
      // update execution time in dataflow according to component operator type
      TIME_UNIT opLifespan = circuit.getOperatorLifespan(op.getType());
      VERBOSE_INFO("operator lifespan (" << op.getType() << "): " << opLifespan);
      if (opLifespan == 0) { // i.e. no specified lifespan (e.g. const_value, delay, Proj operators)
        std::vector<TIME_UNIT> opLifespans{1};
        if (op.getType() == "input_selector" || op.getType() == "output_selector") {
          opLifespans = dataflow->getVertexPhaseDuration(v); // special case for input and output selectors, which are CSDF components
        }
        dataflow->setVertexDuration(v, opLifespans);
      } else {
        std::vector<TIME_UNIT> opLifespans{opLifespan};
        dataflow->setVertexDuration(v, opLifespans);
      }
    }}
  // track the operator types that we might want to merge (i.e. multiple occurances of the same type)
  for (auto &types : opCounts) {
    if (types.second > 1) {
      typesToMerge.push_back(types.first);
    }
  }

  // to identify all relevant causal dependencies, we look for dependencies from the output actors
  {ForEachVertex(dataflow, v) {
      std::string actorName = dataflow->getVertexName(v);
      if (actorName.find("OUTPUT", 0) != std::string::npos) { // we match by name to avoid having to instantiate new VHDLComponent objects
        outputActorNames.push_back(actorName);
      }
    }}
  VERBOSE_ASSERT(outputActorNames.size() > 0,
                 "No output actors identified; at least 1 output actor needed to run causal dependency detection.");
  for (auto &name : outputActorNames) {
    Vertex outputActor = dataflow->getVertexByName(name);
    findCausalDependency(dataflow, outputActor, absDepGraph);
  }
  VERBOSE_DEBUG(absDepGraph.printStatus());

  // compute execution times using causal dependency graph
  {ForEachVertex(dataflow, v) {
      if (dataflow->getVertexInDegree(v) == 0) { // we only start from actors without causal dependencies (no input edges)
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
    std::map<std::string, bool> opMerged;
    for (auto &type : typesToMerge) { // initialise map to flag when operator type has been detected at the given exec time
      opMerged[type] = false;
    }
    for (auto &id : e.second) {
      VERBOSE_DEBUG("\t" << id << "(" << dataflow->getVertexName(dataflow->getVertexById(id))
                   << ", " << dataflow->getVertexType(dataflow->getVertexById(id)) << ")");
      VHDLComponent op(dataflow, dataflow->getVertexById(id));
      if (std::find(typesToMerge.begin(), typesToMerge.end(), op.getType()) != typesToMerge.end() &&
          !opMerged[op.getType()]) {
        mergeableIds[op.getType()].push_back(id);
        opMerged[op.getType()] = true;
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
// NOTE this only works with HSDFs
void algorithms::findCausalDependency(models::Dataflow* const dataflow, Vertex v,
                                      abstractDepGraph &g) {
  if (dataflow->getVertexInDegree(v) > 0) { // input edges means possible causal dependencies
    {ForInputEdges(dataflow, v, inEdge) {
        Vertex source = dataflow->getEdgeSource(inEdge);
        TOKEN_UNIT consRate = dataflow->getEdgeOutPhase(inEdge, 1);
        TOKEN_UNIT prodRate = dataflow->getEdgeInPhase(inEdge, 1);
        if (dataflow->getPreload(inEdge) < consRate) {
          g.addCausalDep(dataflow->getVertexId(source), dataflow->getVertexId(v));
          findCausalDependency(dataflow, source, g);
        }
      }}
  } else {
    {ForOutputEdges(dataflow, v, outEdge) {
        Vertex target = dataflow->getEdgeTarget(outEdge);
        TOKEN_UNIT consRate = dataflow->getEdgeOutPhase(outEdge, 1);
        TOKEN_UNIT prodRate = dataflow->getEdgeInPhase(outEdge, 1);
        if (dataflow->getPreload(outEdge) < consRate) {
          g.addCausalDep(dataflow->getVertexId(v), dataflow->getVertexId(target));
        }
      }}
    return;
  }
}
