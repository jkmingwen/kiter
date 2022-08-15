/*
 *  iterative_evaluation.cpp
 *
 *  Created on: Aug 2, 2021
 *      Author: jkmingwen
 */


#include "iterative_evaluation.h"
#include <models/Dataflow.h>
#include <printers/SDF3Wrapper.h> // to write XML files

std::set<std::string> binaryOps = {
  "add", "diff", "prod", "div", "mod", "l_shift",
  "r_shift", "greaterthan", "lessthan", "geq",
  "leq", "equal", "notequal", "AND", "OR", "XOR"
};

std::set<std::string> castOps = {
  "int", "float"
};

void algorithms::transformation::iterative_evaluate(models::Dataflow* const  dataflow,
                                                    parameters_list_t params) {
  bool changeDetected = true;
  std::string dirName = "./"; // use current directory as default
  std::string outputName = dirName + dataflow->getGraphName() + "_simplified" + ".xml"; // use graph name as default

  if (params.find("OUTPUT_NAME") != params.end()) {
    outputName = dirName + params["OUTPUT_NAME"] + ".xml";
  }
  models::Dataflow* dataflow_prime = new models::Dataflow(*dataflow);
  while (changeDetected) {
    dataflow_prime = new models::Dataflow(*dataflow_prime);
    changeDetected = false;
      {ForEachVertex(dataflow_prime, v) {
          std::string opName = dataflow_prime->getVertexType(v);
          if (opName == "delay") {
            // identify input signal edge and static delay amount
            int delayAmt;
            Edge inputSig, delayArg;
            if (checkForStaticDelay(dataflow_prime, v, inputSig, delayArg, delayAmt)) {
              bypassDelay(dataflow_prime, v, inputSig, delayArg, delayAmt);
              changeDetected = true;
              break;
            }
          }
          if (checkForNumericInputs(dataflow_prime, v)) {
            std::vector<std::string> inputArgs;
            {ForInputEdges(dataflow_prime, v, e) {
                inputArgs.push_back(dataflow_prime->getVertexType(dataflow_prime->getEdgeSource(e)));
              }}
            if (binaryOps.find(opName) != binaryOps.end() && inputArgs.size() == 2) {
              applyResult(dataflow_prime, v, evalBinop(opName, inputArgs[0], inputArgs[1]));
              changeDetected = true;
              break;
            } else if (castOps.find(opName) != castOps.end() && inputArgs.size() == 1) {
              applyResult(dataflow_prime, v, evalCast(opName, inputArgs[0]));
              changeDetected = true;
              break;
            } else {
              applyResult(dataflow_prime, v, evalOther(opName, inputArgs));
              changeDetected = true;
              break;
            }
          }
        }}
  }
  std::cout << "no more changes detected" << std::endl;
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
  std::string delayArgSourceName = dataflow->getVertexName(dataflow->getEdgeSource(delayArg));
  std::string delayName = dataflow->getVertexName(v);
  unsigned int delayArgOutputCnt = dataflow->getVertexOutDegree(dataflow->getEdgeSource(delayArg)); // need to store output count separately to avoid breakage after removing vertices
  Vertex newTarget;

  if (dataflow->getVertexOutDegree(v) > 1) {
    std::cerr << "Bypassing delay with multiple outputs not yet supported" << std::endl;
  } else { // NOTE could be adapted to multiple outputs by storing a vector of edge sources and targets
    {ForOutputEdges(dataflow, v, e) {
        newTarget = dataflow->getEdgeTarget(e);
        std::cout << "\tOutput edge: " << dataflow->getEdgeName(e) << std::endl;
        std::cout << "\tNew target for " << dataflow->getVertexType(newSource)
                  << ": " << dataflow->getVertexType(newTarget) << std::endl;
      }}
  }
  std::cout << "Replace edge with bypass edge" << std::endl;
  std::cout << "\tSource: " << dataflow->getVertexName(newSource)
            << ", " << dataflow->getVertexType(newSource) << std::endl;
  std::cout << "\tTarget: " << dataflow->getVertexName(newTarget)
            << ", " << dataflow->getVertexType(newTarget) << std::endl;
  // bypass delay operator with new edge
  dataflow->removeEdge(inputSig);
  Edge newEdge = dataflow->addEdge(newSource, newTarget, edgeName);
  dataflow->setEdgeInPhases(newEdge, inPhases);
  dataflow->setEdgeOutPhases(newEdge, outPhases);
  dataflow->setPreload(newEdge, delayAmt);
  dataflow->setTokenSize(newEdge, 1);

  /* remove vertex providing delay argument:
     note that if the delay arg actor has multiple output edges,
     we just delete the corresponding edge so we don't unnecessarily
     alter the graph */
  std::cout << "\tnumber of outputs for delay argument: " << delayArgOutputCnt << std::endl;
  if (delayArgOutputCnt > 1) {
    std::cout << "\t\tremoving edge from delay argument" << std::endl;
    dataflow->removeEdge(delayArg);
  } else { // only one output edge
    std::cout << "\t\tremoving delay argument from graph" << std::endl;
    dataflow->removeVertex(dataflow->getVertexByName(delayArgSourceName));
  }
  dataflow->removeVertex(dataflow->getVertexByName(delayName));
}

// update vertex with the specified result, removing any input arguments
void algorithms::applyResult(models::Dataflow* const dataflow, Vertex v,
                             std::string result) {
  std::vector<std::string> inputVertices; // store names of vertices acting as input args to v
  {ForInputEdges(dataflow, v, e) {
      inputVertices.push_back(dataflow->getVertexName(dataflow->getEdgeSource(e)));
    }}
  dataflow->setVertexType(v, result); // update with given result
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
