/*
 *  iterative_evaluation.cpp
 *
 *  Created on: Aug 2, 2021
 *      Author: jkmingwen
 */


#include "iterative_evaluation.h"
#include <commons/verbose.h>
#include <models/Dataflow.h>
#include <printers/SDF3Wrapper.h> // to write XML files

std::set<std::string> binaryOps = {
  "add", "diff", "prod", "div", "mod", "l_shift",
  "r_shift", "greaterthan", "lessthan", "geq",
  "leq", "equal", "notequal", "AND", "OR", "XOR", "pow"
};

std::set<std::string> castOps = {
  "int", "float"
};

void algorithms::transformation::iterative_evaluate(models::Dataflow* const  dataflow,
                                                    parameters_list_t params) {
  bool changeDetected = true;
  std::string dirName = "./"; // use current directory as default
  std::string outputName = dataflow->getGraphName() + "_simplified" + ".xml"; // use graph name as default

  if (params.find("OUTPUT_DIR") != params.end()) {
    dirName = params["OUTPUT_DIR"];
    VERBOSE_INFO("DIR NAME: " << dirName);
  }
  if (params.find("OUTPUT_NAME") != params.end()) {
    outputName = params["OUTPUT_NAME"] + ".xml";
  }
  outputName = dirName + outputName; // set output file path

  VERBOSE_INFO("Beginning iterative evaluation...");
  models::Dataflow* dataflow_prime = new models::Dataflow(*dataflow);
  while (changeDetected) {
    VERBOSE_INFO("");
    VERBOSE_INFO("Starting new iteration of iterative evaluation:")
    changeDetected = false;
      {ForEachVertex(dataflow_prime, v) {
          std::string opName = dataflow_prime->getVertexType(v);
          VERBOSE_INFO("Visiting " << opName
                       << " (" << dataflow_prime->getVertexName(v) << ")...");
          if (opName == "delay") {
            // identify input signal edge and static delay amount
            int delayAmt;
            Edge inputSig, delayArg;
            if (checkForStaticDelay(dataflow_prime, v, inputSig, delayArg, delayAmt)) {
              VERBOSE_INFO("\tBypassing static delay");
              VERBOSE_INFO("\t\tDelay amount: " << delayAmt
                           << " (" << dataflow_prime->getVertexName(dataflow_prime->getEdgeSource(delayArg)) << ")");
              bypassDelay(dataflow_prime, v, inputSig, delayArg, delayAmt);
              changeDetected = true;
              break;
            }
          }
          // if (opName == "Proj") {
          //   VERBOSE_INFO("\tBypassing Proj");
          //   bypassProj(dataflow_prime, v);
          //   changeDetected = true;
          //   break;
          // }
          if (checkForNumericInputs(dataflow_prime, v)) {
            std::vector<std::string> inputArgs;
            {ForInputEdges(dataflow_prime, v, e) {
                inputArgs.push_back(dataflow_prime->getVertexType(dataflow_prime->getEdgeSource(e)));
              }}
            if (binaryOps.find(opName) != binaryOps.end() && inputArgs.size() == 2) {
              VERBOSE_INFO("\tEvaluating binary operator: " << opName);
              VERBOSE_INFO("\t\tInput arguments: " << inputArgs[0] << ", " << inputArgs[1]);
              applyResult(dataflow_prime, v, evalBinop(opName, inputArgs[0], inputArgs[1]));
              changeDetected = true;
              break;
            } else if (castOps.find(opName) != castOps.end() && inputArgs.size() == 1) {
              VERBOSE_INFO("\tEvaluating cast operator: " << opName);
              VERBOSE_INFO("\t\tInput argument: " << inputArgs[0]);
              applyResult(dataflow_prime, v, evalCast(opName, inputArgs[0]));
              changeDetected = true;
              break;
            } else if (opName.find("OUTPUT") != std::string::npos) {
              VERBOSE_INFO("\tAll inputs to output are constants");
              break;
            } else {
              VERBOSE_INFO("\tEvaluating operator: " << opName);
              VERBOSE_INFO("\t\tInput arguments: ");
              for (auto &i : inputArgs) {
                VERBOSE_INFO("\t\t  " << i);
              }
              applyResult(dataflow_prime, v, evalOther(opName, inputArgs));
              changeDetected = true;
              break;
            }
          }
        }}
  }
  VERBOSE_INFO("No further possible evaluations detected, producing simplified graph");
  VERBOSE_INFO("Simplified graph:" << outputName);
  // TODO use printers::writeSDF3File(path/to/file, dataflow_graph) to produce updated SDF file
  printers::writeSDF3File(outputName, dataflow_prime);
}

// returns true if all input vertices to the given vertex have numeric values
bool algorithms::checkForNumericInputs(models::Dataflow* const dataflow, Vertex v) {
  bool allInputNum = true;
  if (dataflow->getVertexInDegree(v)) {
    {ForInputEdges(dataflow, v, e) {
        std::string actorType = dataflow->getVertexType(dataflow->getEdgeSource(e));
        try {
          float conv = std::stof(actorType);
        } catch(std::invalid_argument const& ex) {
          allInputNum = false;
        }
      }}
  } else {
    allInputNum = false; // no inputs means no numeric inputs
  }
  return allInputNum;
}

// check if the given delay actor is static or variable
// this is determined by checking if there exists an input actor that is an integer value
// simultaneously assigns the edges providing the input signal and delay argument to the
// provided arguments; also assigns the delay amount
bool algorithms::checkForStaticDelay(models::Dataflow* const dataflow, Vertex v,
                                     Edge &inputSig, Edge &delayArg, int &delayAmt) {
  bool intArgFound = false;
  {ForInputEdges(dataflow, v, e) {
      std::string actorType = dataflow->getVertexType(dataflow->getEdgeSource(e));
      // https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
      if (actorType.find_first_not_of( "0123456789" ) == std::string::npos) {
        delayAmt = std::stoi(actorType);
        delayArg = e;
        intArgFound = true;
      } else {
        inputSig = e;
      }
    }}

  return intArgFound;
}

/* evaluate expressions given the operator specified by op
 * note that we assume that there will only ever be 2 arguments to the actor;
 * this is because the isSigBinop function in Faust similarly assumes 2 arguments
 */
std::string algorithms::evalBinop(std::string op, std::string arg1, std::string arg2) {
  float result = 0; // NOTE assume all results are floats for simplicity; might have to make this more flexible
  int arg1Int = std::stoi(arg1);
  int arg2Int = std::stoi(arg2);
  float arg1Float = std::stof(arg1);
  float arg2Float = std::stof(arg2);

  if (op == "add") {
    result = arg1Float + arg2Float;
  } else if (op == "diff") {
    result = arg1Float - arg2Float;
  } else if (op == "prod") {
    result = arg1Float * arg2Float;
  } else if (op == "div") {
    result = arg1Float / arg2Float;
  } else if (op == "mod") {
    result = arg1Int % arg2Int; // NOTE might need to use fmod for float values
  } else if (op == "greaterthan") {
    result = (float) arg1Float > arg2Float;
  } else if (op == "lessthan") {
    result = (float) arg1Float < arg2Float;
  } else if (op == "geq") {
    result = (float) arg1Float >= arg2Float;
  } else if (op == "leq") {
    result = (float) arg1Float <= arg2Float;
  } else if (op == "equal") {
    result = (float) arg1Float == arg2Float;
  } else if (op == "notequal") {
    result = (float) arg1Float != arg2Float;
  } else if (op == "l_shift") { // bitwise operators only work on integral types
    result = arg1Int << arg2Int;
  } else if (op == "r_shift") {
    result = arg1Int >> arg2Int;
  } else if (op == "AND") {
    result = arg1Int & arg2Int;
  } else if (op == "OR") {
    result = arg1Int | arg2Int;
  } else if (op == "XOR") {
    result = arg1Int ^ arg2Int;
  } else if (op == "pow") { // NOTE we assume that power actor only ever has 2 input args, might have to reimplement for more than 2 input args in the future; we also assume that the last operand is always the exponent
    result = pow(arg1Float, arg2Float);
  } else {
    std::cerr << "Specified binary operator " << op << " not currently supported" << std::endl;
  }

  return std::to_string(result);
}

std::string algorithms::evalCast(std::string op, std::string arg) {
  if (op == "int") {
    return std::to_string(std::stoi(arg));
  } else if (op == "float") {
    return std::to_string(std::stof(arg));
  } else {
    std::cerr << "Specified cast " << op << " not currently supported" << std::endl;
    return NULL;
  }
}

std::string algorithms::evalOther(std::string op, std::vector<std::string> args) {
  // convert strings to floats for proper max/min operator behaviour
  if (op == "max") {
    std::vector<float> floatArgs;
    for (auto &arg : args) {
      floatArgs.push_back(std::stof(arg));
    }
    return std::to_string(*std::max_element(floatArgs.begin(), floatArgs.end()));
  } else if (op == "min") {
    std::vector<float> floatArgs;
    for (auto &arg : args) {
      floatArgs.push_back(std::stof(arg));
    }
    return std::to_string(*std::min_element(floatArgs.begin(), floatArgs.end()));
  } else {
    std::cerr << "Specified operator " << op << " not currently supported" << std::endl;
    return NULL;
  }
}

void algorithms::bypassDelay(models::Dataflow* const dataflow, Vertex v,
                             Edge inputSig, Edge delayArg, int delayAmt) {
  Vertex newSource = dataflow->getEdgeSource(inputSig);  // input signal that will be delayed via initial tokens
  std::vector<TOKEN_UNIT> inPhases = dataflow->getEdgeInVector(inputSig);
  std::vector<TOKEN_UNIT> outPhases = dataflow->getEdgeOutVector(inputSig);
  std::string edgeName = dataflow->getEdgeName(inputSig);
  std::string edgeInPort = dataflow->getEdgeInputPortName(inputSig);
  std::string edgeOutPort = dataflow->getEdgeOutputPortName(inputSig);
  std::string delayArgSourceName = dataflow->getVertexName(dataflow->getEdgeSource(delayArg));
  std::string delayName = dataflow->getVertexName(v);
  unsigned int delayArgOutputCnt = dataflow->getVertexOutDegree(dataflow->getEdgeSource(delayArg)); // need to store output count separately to avoid breakage after removing vertices
  Vertex newTarget;
  int edgeCount = 0;

  dataflow->removeEdge(inputSig);
  {ForOutputEdges(dataflow, v, e) { // bypass delay operator with new edge
      newTarget = dataflow->getEdgeTarget(e);
      std::string newEdgeName = edgeName;
      std::string newInPortName = edgeInPort;
      std::string newOutPortName = edgeOutPort;
      if (dataflow->getVertexOutDegree(v) > 1) {
        std::string edgeNumber = std::string("_" + std::to_string(edgeCount) + "_");
        newEdgeName.replace(newEdgeName.find_last_of("_"),
                            1,
                            edgeNumber);
        newInPortName.replace(newInPortName.find_last_of("_"),
                              1,
                              edgeNumber);
        newOutPortName.replace(newOutPortName.find_last_of("_"),
                               1,
                               edgeNumber);
      }
      VERBOSE_INFO("\t\tNew edge: " << dataflow->getVertexType(newSource) << " ("
                   << dataflow->getVertexName(newSource) << ") ---> "
                   << dataflow->getVertexType(newTarget) << " ("
                   << dataflow->getVertexName(newTarget) << ")");
      Edge newEdge = dataflow->addEdge(newSource, newTarget, newEdgeName);
      dataflow->setEdgeInPhases(newEdge, inPhases);
      dataflow->setEdgeOutPhases(newEdge, outPhases);
      dataflow->setEdgeInputPortName(newEdge, newInPortName);
      dataflow->setEdgeOutputPortName(newEdge, newOutPortName);
      dataflow->setPreload(newEdge, delayAmt);
      dataflow->setTokenSize(newEdge, 1);
      edgeCount++;
      // update target name to reflect new input argument if necessary
      // as VHDLGeneration uses the vertex name to determine order of arguments for binary operators
      if (dataflow->getVertexName(newTarget).find(delayName) != std::string::npos) {
        std::string newTargetName = dataflow->getVertexName(newTarget);
        std::string sourceName = dataflow->getVertexName(newSource);
        if (sourceName.find("_") != std::string::npos) { // only need the base name of binary operators
          sourceName = sourceName.substr(0, sourceName.find("_"));
        }
        newTargetName.replace(newTargetName.find(delayName),
                              delayName.length(),
                              sourceName);
        dataflow->setVertexName(newTarget, newTargetName);
      }
    }}

  /* remove vertex providing delay argument:
     note that if the delay arg actor has multiple output edges,
     we just delete the corresponding edge so we don't unnecessarily
     alter the graph */
  VERBOSE_DEBUG("\tnumber of outputs for delay argument: " << delayArgOutputCnt);
  if (delayArgOutputCnt > 1) {
    VERBOSE_INFO("\t\tRemoving edge from delay argument (" << dataflow->getEdgeName(delayArg) << ")");
    dataflow->removeEdge(delayArg);
  } else { // only one output edge
    VERBOSE_INFO("\t\tRemoving delay argument from graph (" << delayArgSourceName << ")");
    dataflow->removeVertex(dataflow->getVertexByName(delayArgSourceName));
  }
  dataflow->removeVertex(dataflow->getVertexByName(delayName));
}

void algorithms::bypassProj(models::Dataflow* const dataflow, Vertex v) {
  std::string projName = dataflow->getVertexName(v);

  if (dataflow->getVertexInDegree(v) > 1) { // multiple inputs to Proj operator: map to outputs in order of inputs
    VERBOSE_DEBUG_ASSERT(dataflow->getVertexInDegree(v) == dataflow->getVertexOutDegree(v),
                         "Number of inputs (" << dataflow->getVertexInDegree(v)
                         << ") doesn't match number of outputs"
                         << dataflow->getVertexOutDegree(v) << "for Proj operator");
    VERBOSE_INFO("\t\tBypassing Proj with multiple inputs:");
    std::vector<Vertex> newSources;
    Vertex newTarget;
    std::vector<Edge> oldInputEdges;
    std::vector<std::string> edgeNames, edgeInPorts, edgeOutPorts;
    std::vector<std::vector<TOKEN_UNIT>> inPhases, outPhases;
    std::vector<int> initTokens;
    int currentEdge = 0;
    {ForInputEdges(dataflow, v, e) {
        newSources.push_back(dataflow->getEdgeSource(e));
        edgeNames.push_back(dataflow->getEdgeName(e));
        edgeInPorts.push_back(dataflow->getEdgeInputPortName(e));
        edgeOutPorts.push_back(dataflow->getEdgeOutputPortName(e));
        inPhases.push_back(dataflow->getEdgeInVector(e));
        outPhases.push_back(dataflow->getEdgeOutVector(e));
        initTokens.push_back(dataflow->getPreload(e));
        oldInputEdges.push_back(e);
      }}
    dataflow->removeEdge(oldInputEdges[currentEdge]);
    {ForOutputEdges(dataflow, v, e) {
        newTarget =  dataflow->getEdgeTarget(e);
        std::string newEdgeName = edgeNames[currentEdge];
        std::string newInPortName = edgeInPorts[currentEdge];
        std::string newOutPortName = edgeOutPorts[currentEdge];
        VERBOSE_INFO("\t\tNew edge: "
                     << dataflow->getVertexType(newSources[currentEdge]) << " ("
                     << dataflow->getVertexName(newSources[currentEdge]) << ") ---> "
                     << dataflow->getVertexType(newTarget) << " ("
                     << dataflow->getVertexName(newTarget) << ")");
        Edge newEdge = dataflow->addEdge(newSources[currentEdge], newTarget, newEdgeName);
        dataflow->setEdgeInPhases(newEdge, inPhases[currentEdge]);
        dataflow->setEdgeOutPhases(newEdge, outPhases[currentEdge]);
        dataflow->setEdgeInputPortName(newEdge, newInPortName);
        dataflow->setEdgeOutputPortName(newEdge, newOutPortName);
        dataflow->setPreload(newEdge, initTokens[currentEdge]);
        dataflow->setTokenSize(newEdge, 1);
        currentEdge++;
        // update target name to reflect new input argument if necessary
        // as VHDLGeneration uses the vertex name to determine order of arguments for binary operators
        if (dataflow->getVertexName(newTarget).find(projName) != std::string::npos) {
          std::string newTargetName = dataflow->getVertexName(newTarget);
          std::string sourceName = dataflow->getVertexName(newSources[currentEdge]);
          if (sourceName.find("_") != std::string::npos) { // only need the base name of binary operators
            sourceName = sourceName.substr(0, sourceName.find("_"));
          }
          newTargetName.replace(newTargetName.find(projName),
                                projName.length(),
                                sourceName);
          dataflow->setVertexName(newTarget, newTargetName);
        }
      }}
  } else {
    Vertex newSource, newTarget;
    Edge oldEdge;
    std::string edgeName, edgeInPort, edgeOutPort;
    std::vector<TOKEN_UNIT> inPhases, outPhases;
    int initTokens;
    int edgeCount = 0;
    {ForInputEdges(dataflow, v, e) {
        newSource = dataflow->getEdgeSource(e);
        edgeName = dataflow->getEdgeName(e);
        edgeInPort = dataflow->getEdgeInputPortName(e);
        edgeOutPort = dataflow->getEdgeOutputPortName(e);
        inPhases = dataflow->getEdgeInVector(e);
        outPhases = dataflow->getEdgeOutVector(e);
        initTokens = dataflow->getPreload(e);
        oldEdge = e;
      }}
    dataflow->removeEdge(oldEdge);
    {ForOutputEdges(dataflow, v, e) {
        newTarget = dataflow->getEdgeTarget(e);
        std::string newEdgeName = edgeName;
        std::string newInPortName = edgeInPort;
        std::string newOutPortName = edgeOutPort;
        if (dataflow->getVertexOutDegree(v) > 1) {
          std::string edgeNumber = std::string("_" + std::to_string(edgeCount) + "_");
          newEdgeName.replace(newEdgeName.find_last_of("_"),
                              1,
                              edgeNumber);
          newInPortName.replace(newInPortName.find_last_of("_"),
                                1,
                                edgeNumber);
          newOutPortName.replace(newOutPortName.find_last_of("_"),
                                 1,
                                 edgeNumber);
        }
        VERBOSE_INFO("\t\tNew edge: " << dataflow->getVertexType(newSource) << " ("
                     << dataflow->getVertexName(newSource) << ") ---> "
                     << dataflow->getVertexType(newTarget) << " ("
                     << dataflow->getVertexName(newTarget) << ")");
        Edge newEdge = dataflow->addEdge(newSource, newTarget, newEdgeName);
        dataflow->setEdgeInPhases(newEdge, inPhases);
        dataflow->setEdgeOutPhases(newEdge, outPhases);
        dataflow->setEdgeInputPortName(newEdge, newInPortName);
        dataflow->setEdgeOutputPortName(newEdge, newOutPortName);
        dataflow->setPreload(newEdge, initTokens);
        dataflow->setTokenSize(newEdge, 1);
        edgeCount++;
        // update target name to reflect new input argument if necessary
        // as VHDLGeneration uses the vertex name to determine order of arguments for binary operators
        if (dataflow->getVertexName(newTarget).find(projName) != std::string::npos) {
          std::string newTargetName = dataflow->getVertexName(newTarget);
          std::string sourceName = dataflow->getVertexName(newSource);
          if (sourceName.find("_") != std::string::npos) { // only need the base name of binary operators
            sourceName = sourceName.substr(0, sourceName.find("_"));
          }
          newTargetName.replace(newTargetName.find(projName),
                                projName.length(),
                                sourceName);
          dataflow->setVertexName(newTarget, newTargetName);
        }
      }}
  }

  dataflow->removeVertex(dataflow->getVertexByName(projName));
}

// update vertex with the specified result, removing any input arguments
void algorithms::applyResult(models::Dataflow* const dataflow, Vertex v,
                             std::string result) {
  VERBOSE_INFO("\tReplacing " << dataflow->getVertexType(v) << " with " << result);
  std::string vName = dataflow->getVertexName(v);
  std::vector<std::string> inputVertices; // store names of vertices acting as input args to v
  {ForInputEdges(dataflow, v, e) {
      inputVertices.push_back(dataflow->getVertexName(dataflow->getEdgeSource(e)));
    }}
  dataflow->setVertexType(v, result); // update with given result
  dataflow->setVertexName(v, vName.substr(0, vName.find("_"))); // remove extended name indicating input argument order after applying result
  {ForInputEdges(dataflow, v, e) {
      Vertex inputVertex = dataflow->getEdgeSource(e);
      if (dataflow->getVertexOutDegree(inputVertex) > 1) {
        dataflow->removeEdge(e); // don't remove vertex if it supplies input args to others
        std::vector<std::string>::iterator it = find(inputVertices.begin(),
                                                     inputVertices.end(),
                                                     dataflow->getVertexName(inputVertex));
        inputVertices.erase(it); // don't remove the vertices whose edges we've removed
      }
    }}
  // remove all remaining vertices by name
  for (auto &input : inputVertices) {
    Vertex i = dataflow->getVertexByName(input);
    dataflow->removeVertex(i);
  }
}
