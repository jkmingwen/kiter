/*
 * symbolic_execution.cpp
 *
 *  Created on: 15 December 2020
 *      Author: jkmingwen
 */

#include <algorithm>
#include <map>
#include <vector>
#include <printers/stdout.h>
#include <commons/verbose.h>
#include <commons/commons.h>
#include <models/Dataflow.h>
#include <algorithms/schedulings.h>
#include "symbolic_execution.h"

#include "../../models/Scheduling.h"
#include "actor.h"
#include "state.h"
#include "../scc.h"
#include "../dse/buffer_sizing.h"
#include "../dse/abstract_dep_graph.h"

void algorithms::compute_asap_throughput(models::Dataflow* const dataflow,
                                         parameters_list_t param_list) {
  VERBOSE_ASSERT(dataflow,TXT_NEVER_HAPPEND);
  VERBOSE_ASSERT(computeRepetitionVector(dataflow),"inconsistent graph");
  std::map<int, std::vector<ARRAY_INDEX>> sccMap;
  std::vector<models::Dataflow*> sccDataflows;
  TIME_UNIT minThroughput = LONG_MAX; // NOTE should technically be LDBL_MAX cause TIME_UNIT is of type long double
  std::map<Edge, TOKEN_UNIT> minStepSizes;
  std::map<Edge, std::pair<TOKEN_UNIT, TOKEN_UNIT>> minChannelSizes;
  initSearchParameters(dataflow, minStepSizes, minChannelSizes);
  StorageDistribution initDist(dataflow->getEdgesCount(),
                               0,
                               minChannelSizes,
                               findMinimumDistributionSz(minChannelSizes));

  // generate SCCs if any
  sccMap = computeSCCKosaraju(dataflow);
  if (sccMap.size() > 1) { // if the original graph isn't one SCC, need to break into SCC subgraphs
    sccDataflows = generateSCCs(dataflow, sccMap);
    VERBOSE_INFO("Strongly connected components:");
    for (auto g : sccDataflows) {
      VERBOSE_INFO("Actors in SCC:");
      {ForEachVertex(g, actor) {
          VERBOSE_INFO(g->getVertexName(actor) << "(id:"
                       << g->getVertexId(actor)<< ") ");
        }}
      VERBOSE_INFO("\n");
    }
    for (auto g : sccDataflows) {
      if (g->getEdgesCount() > 0) {
        std::pair<ARRAY_INDEX, EXEC_COUNT> actorInfo;
        VERBOSE_DSE("Compute throughput with following channel sizes:" << std::endl);
        std::map<Edge, TOKEN_UNIT> bufferSizes;
        {ForEachEdge(g, e) {
            bufferSizes[e] = minChannelSizes[e].second;
            VERBOSE_DSE("\tChannel " << g->getEdgeId(e) << ": "
                        << bufferSizes[e] << std::endl);
          }}
        TIME_UNIT componentThroughput = computeComponentThroughput(g, actorInfo, initDist);
        // TIME_UNIT componentThroughput = computeComponentThroughput(g, actorInfo, bufferSizes);
        VERBOSE_INFO("component throughput: " << componentThroughput);
        VERBOSE_INFO("actor ID, repFactor: " << actorInfo.first << ", "
                     << actorInfo.second);
        TIME_UNIT scaledThroughput = (componentThroughput * actorInfo.second) /
          dataflow->getNi(dataflow->getVertexById(actorInfo.first));
        VERBOSE_INFO("scaled throughput: " << scaledThroughput);
        if (scaledThroughput < minThroughput) {
          minThroughput = scaledThroughput;
        }
      } else if (g->getVerticesCount() == 1 && g->getEdgesCount() == 0) {
        /* NOTE this is a workaround from ignoring reentrancy edges --- if this
           condition is met, we assume that we have a single actor with
           re-entrancy, which should therefore have a throughput of 1 */
        ARRAY_INDEX standaloneId;
        EXEC_COUNT standaloneRepFactor;
        {ForEachVertex(g, v) { // FIXME this probably won't be necessary once the getFirstVertex() bug is fixed
            standaloneId = g->getVertexId(v);
            standaloneRepFactor = g-> getPhasesQuantity(v);
          }}
        EXEC_COUNT repFactor = dataflow->getNi(dataflow->getVertexById(standaloneId));
        /* repetition factor for standalone component will be equal to its phase count
           this makes sense because, while it's producing and consuming 1 token in its
           re-entrant edge, it will need to execute its number of phases to arrive
           back at the same state */
        TIME_UNIT componentThroughput = (TIME_UNIT) (1 * standaloneRepFactor) /
          dataflow->getVertexTotalDuration(dataflow->getVertexById(standaloneId));
        TIME_UNIT scaledThroughput = (TIME_UNIT) componentThroughput / repFactor;
        if (scaledThroughput < minThroughput) {
          minThroughput = scaledThroughput;
        }
        VERBOSE_INFO("component throughput: " << componentThroughput);
        VERBOSE_INFO("standalone actor repetition factor: " << standaloneRepFactor);
        VERBOSE_INFO("actor ID, repFactor: " << standaloneId << ", " << repFactor);
        VERBOSE_INFO("scaled throughput: " << scaledThroughput);
      }
    }
    std::cout << "Throughput of graph: " << minThroughput << std::endl;
    return;
  }
  // if graph is strongly connected, just need to use computeComponentThroughput
  std::pair<ARRAY_INDEX, EXEC_COUNT> actorInfo; // look at note for computeComponentThroughput
  std::map<Edge, TOKEN_UNIT> bufferSizes;
  VERBOSE_DSE("Compute throughput with following channel sizes:" << std::endl);
  {ForEachEdge(dataflow, e) {
      bufferSizes[e] = minChannelSizes[e].second;
      if (dataflow->getEdgeId(e) == 1)
        bufferSizes[e] = 10;
      else if (dataflow->getEdgeId(e) == 2)
        bufferSizes[e] = 9;
      else
        bufferSizes[e] = 4;
      VERBOSE_DSE("\tChannel " << dataflow->getEdgeId(e) << ": "
                  << bufferSizes[e] << std::endl);
    }}
  minThroughput = computeComponentThroughput(dataflow, actorInfo, initDist);
  // minThroughput = computeComponentThroughput(dataflow, actorInfo, bufferSizes);
  std::cout << "Throughput of graph: " << minThroughput << std::endl;
  return;
}

// Compute throughput of given component, and return info of actor used to compute throughput
/* NOTE the minActorInfo argument is a workaround as 'getFirstVertex' is currently not working
   as expected. Ideally, we would call 'getFirstVertex' in 'compute_asap_throughput' in order to
   find the ID of one of the actors in the SCC component, and subsequently use that to get its
   repetition factor */
TIME_UNIT algorithms::computeComponentThroughput(models::Dataflow* const dataflow,
                                                 std::pair<ARRAY_INDEX, EXEC_COUNT> &minActorInfo,
                                                 StorageDistribution &storDist) {
  VERBOSE_ASSERT(dataflow,TXT_NEVER_HAPPEND);
  VERBOSE_ASSERT(computeRepetitionVector(dataflow),"inconsistent graph");
  StateList visitedStates;
  EXEC_COUNT minRepFactor = INT_MAX; // EXEC_COUNT is of type long int, and LONG_MAX has same max value
  ARRAY_INDEX minRepActorId;
  TOKEN_UNIT minRepActorExecCount = 0;
  TIME_UNIT timeStep;

  // initialise actors
  std::map<ARRAY_INDEX, Actor> actorMap;
  {ForEachTask(dataflow, t) {
      actorMap[dataflow->getVertexId(t)] = Actor(dataflow, t);
      VERBOSE_INFO("\n");
    }}
  State prevState(dataflow, actorMap, storDist);
  State currState(dataflow, actorMap, storDist);
  VERBOSE_INFO("Printing initial state status");
  VERBOSE_INFO(prevState.print(dataflow));
  VERBOSE_INFO("Printing actor statuses:");
  VERBOSE_INFO(printStatus(dataflow, prevState));
  {ForEachTask(dataflow, t) {
      VERBOSE_INFO(actorMap[dataflow->getVertexId(t)].printStatus(dataflow));
      // track actor with lowest repetition factor (determines when states are stored)
      if (actorMap[dataflow->getVertexId(t)].getRepFactor() < minRepFactor) {
        minRepFactor = actorMap[dataflow->getVertexId(t)].getRepFactor();
        minRepActorId = actorMap[dataflow->getVertexId(t)].getId();
      }
    }}
  VERBOSE_INFO("Actor with ID " << minRepActorId
               << " is actor with lowest repetition factor ("
               << minRepFactor << ")");
  minActorInfo = std::make_pair(minRepActorId, minRepFactor);
  // Start ASAP execution loop
  while (true) {
    {ForEachEdge(dataflow, e) {
        prevState.setTokens(e, currState.getTokens(e));
        if (currState.hasBoundedBuffers()) {
          prevState.setBufferSpace(e, currState.getBufferSpace(e));
        }
      }}
    // end actor firing
    {ForEachTask(dataflow, t) {
        while (actorMap[dataflow->getVertexId(t)].isReadyToEndExec(currState)) {
          if (actorMap[dataflow->getVertexId(t)].getId() == minRepActorId) {
            minRepActorExecCount++;
            if (minRepActorExecCount == minRepFactor) {
              VERBOSE_INFO("Adding the following state to list of visited states:");
              VERBOSE_INFO(currState.print(dataflow));
              if (!visitedStates.addState(currState)) {
                VERBOSE_INFO("ending execution and computing throughput");
                // compute throughput using recurrent state
                TIME_UNIT thr = visitedStates.computeThroughput();
                // TODO function to find storage dependencies for periodic phase
                computePeriodicStorageDeps(dataflow, currState, prevState, actorMap,
                                           minRepActorId, minRepFactor, minRepActorExecCount);
                return thr;
              }
              currState.setTimeElapsed(0);
              minRepActorExecCount = 0;
            }
          }
          actorMap[dataflow->getVertexId(t)].execEnd(dataflow, currState);
          currState.updateState(dataflow, actorMap); // NOTE updating tokens/phase in state done separately from execEnd function, might be a cause for bugs
        }
      }}
    VERBOSE_INFO("Printing current state status after ending firing");
    VERBOSE_INFO(currState.print(dataflow));
    VERBOSE_INFO("Printing Actor Statuses:");
    {ForEachTask(dataflow, t) {
        VERBOSE_INFO(actorMap[dataflow->getVertexId(t)].printStatus(dataflow));
      }}
    VERBOSE_INFO(printStatus(dataflow, currState));
    // start actor firing
    {ForEachTask(dataflow, t) {
        while (actorMap[dataflow->getVertexId(t)].isReadyForExec(currState)) {
          actorMap[dataflow->getVertexId(t)].execStart(dataflow, currState);
          currState.updateState(dataflow, actorMap);
        }
      }}
    VERBOSE_INFO("Printing Current State Status");
    VERBOSE_INFO(currState.print(dataflow));
    VERBOSE_INFO("Printing Actor Statuses:");
    {ForEachTask(dataflow, t) {
        VERBOSE_INFO(actorMap[dataflow->getVertexId(t)].printStatus(dataflow));
      }}
    VERBOSE_INFO(printStatus(dataflow, currState));
    // advance time and check for deadlocks
    timeStep = currState.advanceTime();
    if (timeStep == LONG_MAX) { // NOTE should technically be LDBL_MAX cause TIME_UNIT is of type long double
      VERBOSE_INFO("Deadlock found!");
      VERBOSE_DSE("Graph deadlocked; looking for causal dependencies" << std::endl);
      computeDeadlockStorageDeps(dataflow, currState, actorMap);
      return 0;
    }
  }
}

// takes in original dataflow graph and map of its SCCs, outputs vector of SCC dataflow subgraphs
std::vector<models::Dataflow*> algorithms::generateSCCs(models::Dataflow* const dataflow,
                                                        std::map<int, std::vector<ARRAY_INDEX>> sccMap) {
  std::vector<models::Dataflow*> sccDataflows;
  for (auto const& component : sccMap) {
    models::Dataflow* sccDataflow = new models::Dataflow(*dataflow);
    sccDataflow->reset_computation();
    // remove all actors not in component, as well as edges/ports associated with them
    {ForEachVertex(dataflow, v) {
        // check if given actor in current SCC
        ARRAY_INDEX actorId = dataflow->getVertexId(v);
        if (std::find(component.second.begin(),
                      component.second.end(),
                      actorId) == component.second.end()) { // actor not in SCC
          sccDataflow->removeVertex(sccDataflow->getVertexById(actorId));
        }
      }}
    sccDataflows.push_back(sccDataflow);
  }

  return sccDataflows;
}

void algorithms::computePeriodicStorageDeps(models::Dataflow* const dataflow,
                                            State &currState,
                                            State &prevState,
                                            std::map<ARRAY_INDEX, Actor> actorMap,
                                            ARRAY_INDEX minRepActorId,
                                            EXEC_COUNT minRepFactor,
                                            TOKEN_UNIT minRepActorExecCount) {
  abstractDepGraph absDepGraph(dataflow); // initialise abstract dependency graph
  State recurrentState = currState;
  TIME_UNIT timeStep;
  VERBOSE_DEBUG("Recurrent state:" << std::endl);
  VERBOSE_DEBUG(recurrentState.print(dataflow));

  currState.setTimeElapsed(0);
  minRepActorExecCount = -1;

  // end actor firings
  {ForEachTask(dataflow, t) {
      while (actorMap[dataflow->getVertexId(t)].isReadyToEndExec(currState)) {
        if (actorMap[dataflow->getVertexId(t)].getId() == minRepActorId) {
          minRepActorExecCount++;
          if (minRepActorExecCount == minRepFactor) {
            currState.setTimeElapsed(0);
            minRepActorExecCount = 0;
          }
        }
        actorMap[dataflow->getVertexId(t)].execEnd(dataflow, currState);
        currState.updateState(dataflow, actorMap);
      }
    }}

  // start actor firings
  while (true) {
    {ForEachTask(dataflow, t) {
        while (actorMap[dataflow->getVertexId(t)].isReadyForExec(currState)) {
          VERBOSE_DEBUG("Actor " << dataflow->getVertexId(t)
                        << " ready to fire; looking for causal dependencies" << std::endl);
          actorMap[dataflow->getVertexId(t)].computeCausalDeps(dataflow, prevState, absDepGraph);
          actorMap[dataflow->getVertexId(t)].execStart(dataflow, currState);
          currState.updateState(dataflow, actorMap);
        }
      }}
    timeStep = currState.advanceTime(); // TODO try without assigning to timeStep (cause we don't use it)
    {ForEachEdge(dataflow, e) {
        prevState.setTokens(e, currState.getTokens(e));
        if (currState.hasBoundedBuffers()) {
          prevState.setBufferSpace(e, currState.getBufferSpace(e));
        }
      }}
    // end actor firing
    {ForEachTask(dataflow, t) {
        while (actorMap[dataflow->getVertexId(t)].isReadyToEndExec(currState)) {
          if (actorMap[dataflow->getVertexId(t)].getId() == minRepActorId) {
            minRepActorExecCount++;
            if (minRepActorExecCount == minRepFactor) {
              VERBOSE_INFO("Adding the following state to list of visited states:");
              VERBOSE_INFO(currState.print(dataflow));
              if (currState == recurrentState) {
                VERBOSE_INFO("ending execution and computing throughput");
                // TODO call function to find storage dependencies
                std::set<Edge> storageDeps = absDepGraph.computeStorageDependencies(dataflow);
                return;
              }
              currState.setTimeElapsed(0);
              minRepActorExecCount = 0;
            }
          }
          actorMap[dataflow->getVertexId(t)].execEnd(dataflow, currState);
          currState.updateState(dataflow, actorMap); // NOTE updating tokens/phase in state done separately from execEnd function, might be a cause for bugs
        }
      }}
  }
}

// compute and return an abstract dependency graph given a deadlocked state
void algorithms::computeDeadlockStorageDeps(models::Dataflow* const dataflow,
                                           State &s,
                                           std::map<ARRAY_INDEX, Actor> actorMap) {
  VERBOSE_DSE(s.print(dataflow));
  abstractDepGraph absDepGraph(dataflow); // initialise abstract dependency graph
  // populate abstract dependency graph
  {ForEachEdge(dataflow, e) {
      Vertex source = dataflow->getEdgeSource(e);
      Vertex target = dataflow->getEdgeTarget(e);
      ARRAY_INDEX sourceId = dataflow->getVertexId(source);
      ARRAY_INDEX targetId = dataflow->getVertexId(target);
      VERBOSE_DEBUG("\t\tChannel " << dataflow->getEdgeId(e) << " ("
                    << sourceId << "->" << targetId
                    << "):" << std::endl);
      VERBOSE_DEBUG("\t\t " << s.getTokens(e) << " tokens available, "
                    << actorMap[targetId].getExecRate(e) << " required (p "
                    << actorMap[targetId].getPhase(e) << ")" << std::endl);
      VERBOSE_DEBUG("\t\t " << s.getBufferSpace(e) << " spaces available, "
                    << actorMap[sourceId].getExecRate(e) << " required (p "
                    << actorMap[sourceId].getPhase(e) << ")" << std::endl);
      if (s.getTokens(e) < actorMap[targetId].getExecRate(e)) {
        VERBOSE_DEBUG("\t\t\tCausal dep between " << targetId << " and "
                      << sourceId << std::endl);
        absDepGraph.addCausalDep(targetId, sourceId);
      }

      if (s.getBufferSpace(e) < actorMap[sourceId].getExecRate(e)) {
        VERBOSE_DEBUG("\t\t\tCausal dep between " << sourceId << " and "
                      << targetId << std::endl);
        absDepGraph.addCausalDep(sourceId, targetId);
      }
    }}
  VERBOSE_DSE(absDepGraph.printStatus() << std::endl);
  std::set<Edge> storageDeps = absDepGraph.computeStorageDependencies(dataflow);

}

// prints current status of dataflow graph
std::string algorithms::printStatus(models::Dataflow* const dataflow, State &s) {
  std::stringstream outputStream;

  outputStream << "\nToken counts:" << std::endl;
  {ForEachEdge(dataflow, e) {
      outputStream << "\tChannel " << dataflow->getEdgeName(e) << " ("
                   << dataflow->getVertexName(dataflow->getEdgeSource(e))
                   << "->"
                   << dataflow->getVertexName(dataflow->getEdgeTarget(e))
                   << "): " << s.getTokens(e) << std::endl;
    }}

  return outputStream.str();
}

// Modification of original program to output scheduling object
std::pair<TIME_UNIT, scheduling_t> algorithms::computeComponentThroughputSchedule(models::Dataflow* const dataflow,
                                                 std::pair<ARRAY_INDEX, EXEC_COUNT> &minActorInfo, scheduling_t schedule) {
  VERBOSE_ASSERT(dataflow,TXT_NEVER_HAPPEND);
  VERBOSE_ASSERT(computeRepetitionVector(dataflow),"inconsistent graph");
  StateList visitedStates;
  EXEC_COUNT minRepFactor = INT_MAX; // EXEC_COUNT is of type long int, and LONG_MAX has same max value
  ARRAY_INDEX minRepActorId;
  TOKEN_UNIT minRepActorExecCount = 0;
  TIME_UNIT timeStep;

  int periodic_state_idx;
  TIME_UNIT thr;
  bool end_check = false; // temp workaround before replacing with mathematical solution
  int actors_left = 0; // seems more efficient to use a counter check than to check through the array
  std::map<ARRAY_INDEX, TIME_UNIT> actors_check;
  {ForEachTask(dataflow, t){
    ++actors_left;
    actors_check[dataflow->getVertexId(t)] = -1;
  }}

  TIME_UNIT curr_step = 0;
  std::map<ARRAY_INDEX, std::vector<std::vector<TIME_UNIT>>> starts; //{task idx : [[state 0 starts], [state 1 ...]]}
  std::map<ARRAY_INDEX, std::vector<TIME_UNIT>> state_start = {};  //{task idx : [ starts]},

  // initialise actors
  std::map<ARRAY_INDEX, Actor> actorMap;
  {ForEachTask(dataflow, t) {
      actorMap[dataflow->getVertexId(t)] = Actor(dataflow, t);
      VERBOSE_INFO("\n");
    }}
  State prevState(dataflow, actorMap);
  State currState(dataflow, actorMap);
  VERBOSE_INFO("Printing initial state status");
  VERBOSE_INFO(prevState.print(dataflow));
  VERBOSE_INFO("Printing actor statuses:");
  VERBOSE_INFO(printStatus(dataflow, prevState));
  {ForEachTask(dataflow, t) {
      VERBOSE_INFO(actorMap[dataflow->getVertexId(t)].printStatus(dataflow));
      // track actor with lowest repetition factor (determines when states are stored)
      if (actorMap[dataflow->getVertexId(t)].getRepFactor() < minRepFactor) {
        minRepFactor = actorMap[dataflow->getVertexId(t)].getRepFactor();
        minRepActorId = actorMap[dataflow->getVertexId(t)].getId();
      }
    }}
  VERBOSE_INFO("Actor with ID " << minRepActorId
               << " is actor with lowest repetition factor ("
               << minRepFactor << ")");
  minActorInfo = std::make_pair(minRepActorId, minRepFactor);
  // Start ASAP execution loop
  while (true) {
    {ForEachEdge(dataflow, e) {
        prevState.setTokens(e, currState.getTokens(e));
        if (currState.hasBoundedBuffers()) {
          prevState.setBufferSpace(e, currState.getBufferSpace(e));
        }
      }}
    // end actor firing
    {ForEachTask(dataflow, t) {
        while (actorMap[dataflow->getVertexId(t)].isReadyToEndExec(currState)) {
          if (actorMap[dataflow->getVertexId(t)].getId() == minRepActorId) {
            minRepActorExecCount++;
            if (minRepActorExecCount == minRepFactor) {
              VERBOSE_INFO("Adding the following state to list of visited states:");
              VERBOSE_INFO(currState.print(dataflow));

              if (!end_check){
                if (!visitedStates.addState(currState)) {
                  VERBOSE_INFO("ending execution and computing throughput");
                  // compute throughput using recurrent state

                  thr = visitedStates.computeThroughput();
                  periodic_state_idx = visitedStates.computeIdx(currState); //idx for repeated state
                  end_check = true;
                }
              }
              if (!end_check){
                {ForEachTask(dataflow, task){ //start new state vector in starts
                  starts[dataflow->getVertexId(task)].push_back({});
                }}
              }
              currState.setTimeElapsed(0);
              minRepActorExecCount = 0;
            }
          }
          actorMap[dataflow->getVertexId(t)].execEnd(dataflow, currState);
          currState.updateState(dataflow, actorMap); // NOTE updating tokens/phase in state done separately from execEnd function, might be a cause for bugs
        }
      }}
    VERBOSE_INFO("Printing current state status after ending firing");
    VERBOSE_INFO(currState.print(dataflow));
    VERBOSE_INFO("Printing Actor Statuses:");
    {ForEachTask(dataflow, t) {
        VERBOSE_INFO(actorMap[dataflow->getVertexId(t)].printStatus(dataflow));
      }}
    VERBOSE_INFO(printStatus(dataflow, currState));
    // start actor firing
    {ForEachTask(dataflow, t) {
        while (actorMap[dataflow->getVertexId(t)].isReadyForExec(currState)) {
          actorMap[dataflow->getVertexId(t)].execStart(dataflow, currState);
          currState.updateState(dataflow, actorMap);

          if (end_check){
            if (actors_check[dataflow->getVertexId(t)] < 0){
              actors_check[dataflow->getVertexId(t)] = curr_step;
              starts[dataflow->getVertexId(t)].back().push_back(curr_step);
              --actors_left;
            }
            if (actors_left == 0){
              {ForEachTask(dataflow, task){ //Creates schedule after additional actor is fired after end of period
                static_task_schedule_t initials;
                periodic_task_schedule_t periodics;
                for(std::size_t i = 0; i < starts[dataflow->getVertexId(task)].size(); ++i){
                  if (i < periodic_state_idx){ //initials
                    initials.insert(initials.end(),starts[dataflow->getVertexId(task)][i].begin(),starts[dataflow->getVertexId(task)][i].end());
                  } else { //periodic
                    periodics.second.insert(periodics.second.end(),starts[dataflow->getVertexId(task)][i].begin(),starts[dataflow->getVertexId(task)][i].end());
                  }
                }
                periodics.first = actors_check[dataflow->getVertexId(task)] - periodics.second[0];
                task_schedule_t sched_struct = {initials,periodics};
                schedule.set(dataflow->getVertexId(task), sched_struct);
                std::cout << dataflow->getVertexName(task) << ": initial starts=" << commons::toString(initials) << ", periodic starts=" << commons::toString(periodics) << std::endl;
              }}
              std::string separator(60, '-');
              std::cout << separator << std::endl;
              return std::make_pair(thr, schedule);
            }

          } else {
            if (starts[dataflow->getVertexId(t)].size() == 0){
              starts[dataflow->getVertexId(t)].push_back({curr_step}); //push into current state's task start list
            } else {
              starts[dataflow->getVertexId(t)].back().push_back(curr_step);
            }
          }
        }
      }}
    VERBOSE_INFO("Printing Current State Status");
    VERBOSE_INFO(currState.print(dataflow));
    VERBOSE_INFO("Printing Actor Statuses:");
    {ForEachTask(dataflow, t) {
        VERBOSE_INFO(actorMap[dataflow->getVertexId(t)].printStatus(dataflow));
      }}
    VERBOSE_INFO(printStatus(dataflow, currState));
    // advance time and check for deadlocks
    timeStep = currState.advanceTime();
    curr_step+= timeStep;
    if (timeStep == LONG_MAX) { // NOTE should technically be LDBL_MAX cause TIME_UNIT is of type long double
      VERBOSE_INFO("Deadlock found!");
      return std::make_pair(0, schedule);
    }
  }
}

void algorithms::scheduling::ASAPScheduling(models::Dataflow* const dataflow,
                                         parameters_list_t param_list) {
  VERBOSE_ASSERT(dataflow,TXT_NEVER_HAPPEND);
  VERBOSE_ASSERT(computeRepetitionVector(dataflow),"inconsistent graph");
  std::map<int, std::vector<ARRAY_INDEX>> sccMap;
  std::vector<models::Dataflow*> sccDataflows;
  TIME_UNIT minThroughput = LONG_MAX; // NOTE should technically be LDBL_MAX cause TIME_UNIT is of type long double

  scheduling_t scheduling_result;
  int linesize = param_list.count("LINE")? commons::fromString<int>(param_list["LINE"]) : 80;

  // generate SCCs if any
  sccMap = computeSCCKosaraju(dataflow);
  if (sccMap.size() > 1) { // if the original graph isn't one SCC, need to break into SCC subgraphs
    sccDataflows = generateSCCs(dataflow, sccMap);
    VERBOSE_INFO("Strongly connected components:");
    for (auto g : sccDataflows) {
      VERBOSE_INFO("Actors in SCC:");
      {ForEachVertex(g, actor) {
          VERBOSE_INFO(g->getVertexName(actor) << "(id:"
                       << g->getVertexId(actor)<< ") ");
        }}
      VERBOSE_INFO("\n");
    }
    for (auto g : sccDataflows) {
      if (g->getEdgesCount() > 0) {
        std::pair<ARRAY_INDEX, EXEC_COUNT> actorInfo;
        auto res_pair = computeComponentThroughputSchedule(g, actorInfo, scheduling_result);
        TIME_UNIT componentThroughput = res_pair.first;
        scheduling_result = res_pair.second;
        VERBOSE_INFO("component throughput: " << componentThroughput);
        VERBOSE_INFO("actor ID, repFactor: " << actorInfo.first << ", "
                     << actorInfo.second);
        TIME_UNIT scaledThroughput = (componentThroughput * actorInfo.second) /
          dataflow->getNi(dataflow->getVertexById(actorInfo.first));
        VERBOSE_INFO("scaled throughput: " << scaledThroughput);
        if (scaledThroughput < minThroughput) {
          minThroughput = scaledThroughput;
        }
      } else if (g->getVerticesCount() == 1 && g->getEdgesCount() == 0) {
        /* NOTE this is a workaround from ignoring reentrancy edges --- if this
           condition is met, we assume that we have a single actor with re-entrancy */
        ARRAY_INDEX standaloneId = g->getVertexId(g->getFirstVertex());
        EXEC_COUNT standaloneRepFactor = g->getPhasesQuantity(g->getFirstVertex());
        Vertex vertexInDataflow = dataflow->getVertexById(standaloneId);
        /* repetition factor for standalone component will be equal to its phase count:
           this makes sense because, while it's producing and consuming 1 token in its
           re-entrant edge, it will need to execute its number of phases to arrive
           back at the same state */
        EXEC_COUNT repFactor = dataflow->getNi(vertexInDataflow);
        TIME_UNIT componentThroughput = (TIME_UNIT) (1 * standaloneRepFactor) /
          dataflow->getVertexTotalDuration(vertexInDataflow);
        TIME_UNIT scaledThroughput = (TIME_UNIT) componentThroughput / repFactor;
        if (scaledThroughput < minThroughput) {
          minThroughput = scaledThroughput;
        }
      }
    }

    TIME_UNIT omega = 1.0 / minThroughput ;
    models::Scheduling res = models::Scheduling(dataflow, omega, scheduling_result);
    std::cout << res.asASCII(linesize);
    std::cout << res.asText();

    std::cout << "ASAP throughput is  " << minThroughput << std::endl;
    std::cout << "ASAP period is  " << omega << std::endl;

    return;
  }
  // if graph is strongly connected, just need to use computeComponentThroughput
  std::pair<ARRAY_INDEX, EXEC_COUNT> actorInfo; // look at note for computeComponentThroughput
  auto res_pair = computeComponentThroughputSchedule(dataflow, actorInfo, scheduling_result);
  minThroughput = res_pair.first;
  scheduling_result = res_pair.second;
  std::cout << "Throughput of graph: " << minThroughput << std::endl;
  TIME_UNIT omega = 1.0 / minThroughput ;
  models::Scheduling res = models::Scheduling(dataflow, omega, scheduling_result);
  std::cout << res.asASCII(linesize);
  std::cout << res.asText();

  std::cout << "ASAP throughput is  " << minThroughput << std::endl;
  std::cout << "ASAP period is  " << omega << std::endl;

  return;
}
