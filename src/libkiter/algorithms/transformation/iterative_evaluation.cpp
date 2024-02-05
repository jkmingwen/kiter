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
  "float2int", "int2float", "abs"
};

std::set<std::string> trigOps = {
  "cos", "sin", "tan"
};

std::set<std::string> nonEvalOps = { // operators that can't be evaluated
  "select2", "input_selector", "output_selector"
};

void algorithms::transformation::iterative_evaluate(models::Dataflow* const  dataflow,
                                                    parameters_list_t params) {
  bool changeDetected = true;

  VERBOSE_INFO("Beginning iterative evaluation...");
  // Bruno Edit: I removed the cached dataflow, now directly output to the original one
  // models::Dataflow* dataflow_prime = new models::Dataflow(*dataflow);
  models::Dataflow* dataflow_prime = dataflow;

  while (changeDetected) {
    VERBOSE_INFO("");
    VERBOSE_INFO("Starting new iteration of iterative evaluation:");
    changeDetected = false;
      {ForEachVertex(dataflow_prime, v) {
          std::string opName = dataflow_prime->getVertexType(v);
          VERBOSE_INFO("Visiting " << opName
                       << " (" << dataflow_prime->getVertexName(v) << ")...");
          if (dataflow_prime->getVertexDegree(v) == 0) { // remove vertices with no edges
            VERBOSE_INFO(opName << "(" << dataflow_prime->getVertexName(v) << ") has no input/output edges, removing...");
            dataflow_prime->removeVertex(v);
            changeDetected = true;
            break;
          }
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
            } else if (trigOps.find(opName) != trigOps.end() && inputArgs.size() == 1) {
              VERBOSE_INFO("\tEvaluating trig operator: " << opName);
              VERBOSE_INFO("\t\tInput argument: " << inputArgs[0]);
              applyResult(dataflow_prime, v, evalTrig(opName, inputArgs[0]));
              changeDetected = true;
              break;
            } else if (nonEvalOps.find(opName) != nonEvalOps.end()) {
              VERBOSE_INFO("\tNon-evaluated operator: " << opName);
              VERBOSE_INFO("\t\tInput arguments: ");
              for (auto &i : inputArgs) {
                VERBOSE_INFO("\t\t  " << i);
              }
              changeDetected = false; // no change as actors aren't removed
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
  VERBOSE_INFO("No further possible evaluations detected");
  VERBOSE_INFO("Checking for delays with more than one output:");
  /* Check for delays with multiple outputs only after iterative evaluation
     complete to prevent unnecessarily generating Proj operators when the
     delay could've just been bypassed */
  changeDetected = true;
  while (changeDetected) {
    changeDetected = false;
    {ForEachVertex(dataflow_prime, v) {
        std::string opName = dataflow_prime->getVertexType(v);
        VERBOSE_INFO("Visiting " << opName
                     << " (" << dataflow_prime->getVertexName(v) << ")...");
        if (opName == "delay" && dataflow_prime->getVertexOutDegree(v) > 1) {
          routeMultiOutDelay(dataflow_prime, v);
          changeDetected = true;
          break;
        }
      }}
  }
  VERBOSE_INFO("No further possible reductions detected, producing simplified graph");
}

bool isFloatingPoint(const std::string& input) {
    std::istringstream iss(input);
    float f;
    iss >> std::noskipws >> f; // noskipws means not to skip white spaces

    // Check if the entire string was consumed and if there are no errors
    return iss.eof() && !iss.fail();
}

// returns true if all input vertices to the given vertex have numeric values
bool algorithms::checkForNumericInputs(models::Dataflow* const dataflow, Vertex v) {
  bool allInputNum = true;

      if (dataflow->getVertexInDegree(v)) {
        {ForInputEdges(dataflow, v, e) {
            std::string actorType = dataflow->getVertexType(dataflow->getEdgeSource(e));
            if (!isFloatingPoint(actorType)) return false;
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
  VERBOSE_INFO("Checking " << dataflow->getVertexName(v) << " for static delay");
  VERBOSE_DEBUG_ASSERT(dataflow->getVertexInDegree(v) == 2,
                       "Expected number of input arguments for delay is 2; number of inputs detected: "
                       << dataflow->getVertexInDegree(v));
  bool intArgFound = false;
  std::vector<Edge> inputEdges;
  std::string inputArgType;
  {ForInputEdges(dataflow, v, e) {
      inputEdges.push_back(e);
    }}
  /* NOTE we assume that the channel with the smaller channel number is the delayed signal
     while the larger is the delay arg; this has been true based off our observations but
     might not always be in the future */
  if (getChannelNumber(dataflow->getEdgeName(inputEdges.front())) < getChannelNumber(dataflow->getEdgeName(inputEdges.back()))) {
    VERBOSE_INFO("\tParsing input edge: " << dataflow->getEdgeName(inputEdges.back()));
    inputArgType = dataflow->getVertexType(dataflow->getEdgeSource(inputEdges.back()));
    delayArg = inputEdges.back();
    inputSig = inputEdges.front();
  } else {
    VERBOSE_INFO("\tParsing input edge: " << dataflow->getEdgeName(inputEdges.front()));
    inputArgType = dataflow->getVertexType(dataflow->getEdgeSource(inputEdges.front()));
    delayArg = inputEdges.front();
    inputSig = inputEdges.back();
  }
  VERBOSE_INFO("\tInput edge source actor is of type " << inputArgType);

  // https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
  if (inputArgType.find_first_not_of("0123456789") == std::string::npos) {
    delayAmt = std::stoi(inputArgType);
    intArgFound = true;
    VERBOSE_INFO("\t\tFound integer argument of value " << delayAmt);
    VERBOSE_INFO("\t\tInput edge set to " << dataflow->getEdgeName(inputSig));
  }

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
  if (op == "float2int") {
    return std::to_string(std::stoi(arg));
  } else if (op == "int2float") {
    return std::to_string(std::stof(arg));
  } else if (op == "abs") {
    return std::to_string(std::abs(std::stof(arg)));
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

/* evaluate trigonometric expressions given the operator specified by op
 * note that we assume that there will only ever be 1 argument to the actor;
 * we also assume that the argument is given in radians rather than degrees
 */
std::string algorithms::evalTrig(std::string op, std::string arg) {
  float result = 0; // NOTE assume all results are floats for simplicity; might have to make this more flexible
  float argRadians = std::stof(arg);

  if (op == "cos") {
    result = cos(argRadians);
  } else if (op == "sin") {
    result = sin(argRadians);
  } else if (op == "tan") {
    result = tan(argRadians);
  } else {
    std::cerr << "Specified trigonometric operator " << op << " not currently supported" << std::endl;
  }

  return std::to_string(result);
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

// Route the output of delays with more than 1 output to Proj operators
// The idea is to ensure that delays only ever have 1 output
void algorithms::routeMultiOutDelay(models::Dataflow* const dataflow, Vertex v) {
  ARRAY_INDEX cnt = 0;
  while (dataflow->getVertexOutDegree(v) > 1) {
    VERBOSE_INFO("Routing multi-output delay");
    // add duplicate (Proj operator)
    std::string ogName = dataflow->getVertexName(v);
    std::string newDupName = ogName;
    if (newDupName.find("_") != std::string::npos) { // only use the base name for updating target actor name
      newDupName = newDupName.substr(0, newDupName.find("_"));
      ogName = newDupName;
      VERBOSE_INFO("Base name of actor whose output we're merging: " << newDupName);
    }
    newDupName = newDupName  + "Dup" + commons::toString(cnt++);
    Vertex newDup = dataflow->addVertex(newDupName);
    dataflow->setPhasesQuantity(newDup,1);
    dataflow->setVertexDuration(newDup, {1});
    dataflow->setVertexType(newDup, "Proj");

    // connect edge to Proj
    VERBOSE_INFO("Create new edge from source to new Proj operator");
    Edge ne = dataflow->addEdge(v, newDup);
    // get one of the port names, and strip in/out to get port id and thus name edge
    dataflow->setEdgeInPhases(ne, {1});
    dataflow->setEdgeOutPhases(ne, {1});
    // generate output edges for Proj
    Edge c1;
    Edge c2;
    int cnt = 0;
    std::string c1_type;
    std::string c2_type;
    {ForOutputEdges(dataflow, v, c)	{
        cnt++;
        std::string edgeName = dataflow->getEdgeName(c);
        // ignore "vect" to find the actual data type
        if (edgeName.substr(edgeName.find_last_of("_") + 1) == "vect") {
          edgeName.erase(edgeName.find_last_of("_"), std::string::npos);
        }
        if (cnt == 1) {
          c1 =  c;
          c1_type = edgeName.substr(edgeName.find_last_of("_") + 1);
        }
        if (cnt == 2) {
          c2 =  c;
          c2_type = edgeName.substr(edgeName.find_last_of("_") + 1);
        }
        if (cnt >= 3) break; // only take 2 at a time
      }}
    // workaround for manual channel naming
    std::string neName = "channel_" + commons::toString(dataflow->getEdgeId(ne)) + "_0_" + dataflow->getVertexName(v) + "_" + c1_type; // NOTE assuming here that all outputs are the same type
    VERBOSE_INFO("\tSetting new output edge name to " << neName);
    dataflow->setEdgeName(ne, neName);
    dataflow->setEdgeInputPortName(ne, "in_" + neName);
    dataflow->setEdgeOutputPortName(ne, "out_" + neName);

    VERBOSE_INFO("\tCreate new edges to targets");
    Edge nc1 = dataflow->addEdge(newDup, dataflow->getEdgeTarget(c1));
    dataflow->setEdgeInPhases(nc1, {1});
    dataflow->setEdgeOutPhases(nc1, {1});
    // workaround for manual channel naming
    std::string nc1Name = "channel_" + commons::toString(dataflow->getEdgeId(nc1)) + "_1_" + dataflow->getVertexName(v) + "_" + c1_type;
    dataflow->setEdgeName(nc1, nc1Name);
    dataflow->setEdgeInputPortName(nc1, "in_" + nc1Name);
    dataflow->setEdgeOutputPortName(nc1, "out_" + nc1Name);
    // update name of target actor if its name defines input arg order (i.e. actorname_arg1_arg2)
    /* iterate through the name of the target actor of c1/c2
       (assigned to nc1/2TargetName); once we find a string that
       matches the name of our original source actor (ogName),
       replace that with the name of the new source actor (newDupName) */
    /* NOTE current implementation is designed to only replace the first instance
       of a substring matching the original source actor name as we expect that multiple
       inputs from the same original actor would then come from another newly
       constructed source actor*/
    std::string nc1TargetName = dataflow->getVertexName(dataflow->getEdgeTarget(c1));
    std::string nc1OriginalName = nc1TargetName;
    if (nc1TargetName.find("_") != std::string::npos) {
      VERBOSE_INFO("\tUpdating name of target actor from " << nc1TargetName << " (searching for " << ogName << ")");
      VERBOSE_INFO("\tNew source name is " << newDupName);
      std::string nc1NewName;
      bool targetNameUpdated = false; // track when we've identified the part of the target name we need to update
      size_t pos = nc1TargetName.find("_");
      while ((pos = nc1TargetName.find("_")) != std::string::npos) {
        std::string partialName;
        partialName = nc1TargetName.substr(0, pos); // break up check into substrings delimited by underscore
        if (partialName == ogName && !targetNameUpdated) {
          VERBOSE_INFO("\t\tFound matching partial name for " << ogName
                       << ", changing it to " << newDupName);
          partialName = newDupName; // update the part of the target name that we're replacing with the new actor
          targetNameUpdated = true;
        }
        nc1NewName += partialName; // reconstruct new actor name by parts
        nc1TargetName.erase(0, pos + 1);
        if (nc1TargetName.find("_") != std::string::npos) {
          nc1NewName += "_";
        } else {
          if (nc1TargetName == ogName && !targetNameUpdated) { // separate check for last substring
            partialName = newDupName;
            targetNameUpdated = true;
          } else {
            partialName = nc1TargetName;
          }
          nc1NewName += "_" + partialName;
        }
        VERBOSE_INFO("\t\tCurrent constructed new name: " << nc1NewName);
      }
      VERBOSE_INFO("\tto " << nc1NewName);
      dataflow->setVertexName(dataflow->getEdgeTarget(c1), nc1NewName);
    }
    Edge nc2 = dataflow->addEdge(newDup,  dataflow->getEdgeTarget(c2));
    dataflow->setEdgeInPhases(nc2, {1});
    dataflow->setEdgeOutPhases(nc2, {1});
    // workaround for manual channel naming
    std::string nc2Name = "channel_" + commons::toString(dataflow->getEdgeId(nc2)) + "_2_" + dataflow->getVertexName(v) + "_" + c2_type;
    dataflow->setEdgeName(nc2, nc2Name);
    dataflow->setEdgeInputPortName(nc2, "in_" + nc2Name);
    dataflow->setEdgeOutputPortName(nc2, "out_" + nc2Name);
    std::string nc2TargetName = dataflow->getVertexName(dataflow->getEdgeTarget(c2));
    std::string nc2OriginalName = nc2TargetName;
    if (nc2TargetName.find("_") != std::string::npos) {
      VERBOSE_INFO("Updating name of target actor from " << nc2TargetName << " (searching for " << ogName << ")");
      VERBOSE_INFO("New source name is " << newDupName);
      std::string nc2NewName;
      bool targetNameUpdated = false; // track when we've identified the part of the target name we need to update
      size_t pos = nc2TargetName.find("_");
      while ((pos = nc2TargetName.find("_")) != std::string::npos) { // separate check for last substring
        std::string partialName;
        partialName = nc2TargetName.substr(0, pos); // break up check into substrings delimited by underscore
        if (partialName == ogName && !targetNameUpdated) {
          VERBOSE_INFO("\t\tFound matching partial name for " << ogName << ", changing it to " << newDupName);
          partialName = newDupName; // update the part of the target name that we're replacing with the new actor
          targetNameUpdated = true;
        }
        nc2NewName += partialName; // reconstruct new actor name by parts
        nc2TargetName.erase(0, pos + 1);
        if (nc2TargetName.find("_") != std::string::npos) {
          nc2NewName += "_";
        } else {
          if (nc2TargetName == ogName && !targetNameUpdated) {
            partialName = newDupName;
            targetNameUpdated = true;
          } else {
            partialName = nc2TargetName;
          }
          nc2NewName += "_" + partialName;
        }
        VERBOSE_INFO("\t\tCurrent constructed new name: " << nc2NewName);
      }
      VERBOSE_INFO("\tto " << nc2NewName);
      dataflow->setVertexName(dataflow->getEdgeTarget(c2), nc2NewName);
    }
    // remote the two buffer
    VERBOSE_INFO("Remove original edges");
    dataflow->removeEdge(c1);
    dataflow->removeEdge(c2);
  }
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

// helper function for comparing ordering of channels (via channel name rather than ID)
int algorithms::getChannelNumber(std::string channelName) {
  std::string prefix = "channel_";

  VERBOSE_DEBUG_ASSERT(channelName.find("channel_") != std::string::npos,
                       "Cannot identify channel number if channel name doesn't start with specified prefix");
  channelName.replace(channelName.find(prefix), prefix.length(), ""); // remove prefix
  channelName.erase(channelName.find("_"));
  VERBOSE_DEBUG_ASSERT(channelName.find_first_not_of("0123456789") == std::string::npos,
                       "Channel number extracted is not a number");

  return std::stoi(channelName);
}
