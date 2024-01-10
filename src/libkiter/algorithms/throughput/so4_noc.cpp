/*
 * so4_noc.cpp taken from "A Statically Scheduled Time-Division-Multiplexed
 *  Network-on-Chip for Real-Time Systems"
 *
 *  Created on: May 2021
 *      Author: katwinkl3
 */

#include <models/Dataflow.h>
#include <iostream>
#include "actor.h"
#include "state.h"
#include "so4_noc.h"
#include <models/Scheduling.h>
#include <printers/so4nocWriter.h>
#include "../scc.h"

std::pair<TIME_UNIT, scheduling_t> algorithms::computeComponentSo4Schedule(models::Dataflow* const dataflow,
                                       std::pair<ARRAY_INDEX, EXEC_COUNT> &minActorInfo, scheduling_t schedule, std::string filename, std::set<ARRAY_INDEX> new_edges) {
  VERBOSE_ASSERT(dataflow,TXT_NEVER_HAPPEND);
  VERBOSE_ASSERT(computeRepetitionVector(dataflow),"inconsistent graph");
  StateList visitedStates;
  EXEC_COUNT minRepFactor = INT_MAX; // EXEC_COUNT is of type long int, and LONG_MAX has same max value
  ARRAY_INDEX minRepActorId;
  TOKEN_UNIT minRepActorExecCount = 0;
  TIME_UNIT timeStep; // total time passed
  size_t periodic_state_idx; // idx of first periodic state in statelist
  TIME_UNIT thr;
  bool end_check = false; //
  int actors_left = 0;
  std::map<ARRAY_INDEX, TIME_UNIT> actors_check;
  {ForEachTask(dataflow, t){
    ++actors_left;
    actors_check[dataflow->getVertexId(t)] = -1;
  }}

  std::map<ARRAY_INDEX, long> condition;
  long slots;
  std::tie(condition, slots) = printers::generateConditions(dataflow, filename, new_edges);

  //auto *buffer = new std::map<TIME_UNIT, std::map<ARRAY_INDEX, std::pair<long, bool>>>();; //slot: [channel (actor's edge): {tokens released, if actor needs to execute}]
  auto *n_buffer = new std::deque<std::pair<TIME_UNIT, std::pair<ARRAY_INDEX, long>>>();; //[timeslot, channel, token]
  scheduling_t task_schedule; // {vertex_id : {time_unit: [time_unit]}}
  //ARRAY_INDEX skip_edge = 0;
  //ARRAY_INDEX skip_vertex = 0;
  //TIME_UNIT skip_time = 0;
  TIME_UNIT curr_step = 0;
  std::map<ARRAY_INDEX, std::vector<std::vector<TIME_UNIT>>> starts; //{task idx : [[state 0 starts], [state 1 ...]]}
  std::map<ARRAY_INDEX, std::vector<TIME_UNIT>> state_start = {};  //{task idx : [ starts]},
  //bool periodic_state = false;
  // initialise actors

  ActorMap_t actorMap (static_cast<unsigned long>(dataflow->getMaxVertexId() + 1));
  {ForEachTask(dataflow, t) {
      actorMap[static_cast<unsigned long>(dataflow->getVertexId(t))] = Actor(dataflow, t);
    }}
  State prevState(dataflow, actorMap);
  State currState(dataflow, actorMap);
  {ForEachTask(dataflow, t) {
      // track actor with lowest repetition factor (determines when states are stored)
      if (actorMap[static_cast<unsigned long>(dataflow->getVertexId(t))].getRepFactor() < minRepFactor) {
        minRepFactor = actorMap[static_cast<unsigned long>(dataflow->getVertexId(t))].getRepFactor();
        minRepActorId = actorMap[static_cast<unsigned long>(dataflow->getVertexId(t))].getId();
      }
    }}

  minActorInfo = std::make_pair(minRepActorId, minRepFactor);
  // Start ASAP execution loop
  while (true) {
    {ForEachEdge(dataflow, e) {
      if (new_edges.find(dataflow->getEdgeId(e)) != new_edges.end()){
        continue;
      }
        prevState.setTokens(e, currState.getTokens(e));
    }}
    // end actor firing
    {ForEachTask(dataflow, t) {
        while (actorMap[dataflow->getVertexId(t)].isReadyToEndExec(currState)) {
          if (actorMap[dataflow->getVertexId(t)].getId() == minRepActorId) {
            minRepActorExecCount++;
            if (minRepActorExecCount == minRepFactor) {
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
          actorMap[dataflow->getVertexId(t)].execEndWithMod(dataflow, currState, n_buffer, curr_step, slots, condition);
          // NOTE updating tokens/phase in state done separately from execEnd function, might be a cause for bugs
          // std::cout <<"END FIRING\n"<< std::endl;
          // std::cout << currState.print(dataflow) << std::endl;
        }
      }}

    // set preload from ended firings
    size_t cast_out_idx;
    for (cast_out_idx = 0; cast_out_idx < (*n_buffer).size(); ++cast_out_idx){
      if ((*n_buffer)[cast_out_idx].first <= curr_step){
        Edge e = dataflow->getEdgeById((*n_buffer)[cast_out_idx].second.first);
        currState.setTokens(e,
                            currState.getTokens(e) + (*n_buffer)[cast_out_idx].second.second);
      }
      else{
        break;
      }
    }
    while (cast_out_idx > 0){
      (*n_buffer).pop_front();
      cast_out_idx--;
    }
    currState.updateState(dataflow, actorMap);

    {ForEachTask(dataflow, t) {

      // if (skip_time != 0 && dataflow->getVertexId(t) != skip_vertex){
      //   continue;
      // }
        if(actorMap[dataflow->getVertexId(t)].isReadyForExec(currState)) {
            actorMap[dataflow->getVertexId(t)].execStart(dataflow, currState);
              currState.updateState(dataflow, actorMap);
              // std::cout <<"FIRING\n"<< std::endl;
              // std::cout << currState.print(dataflow) << std::endl;
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
                        continue;
                      }
                      if (i == starts[dataflow->getVertexId(task)].size() - 1){
                        periodics.second.insert(periodics.second.end(),starts[dataflow->getVertexId(task)][i].begin(),starts[dataflow->getVertexId(task)][i].end()-1);
                      } else { //periodic
                        periodics.second.insert(periodics.second.end(),starts[dataflow->getVertexId(task)][i].begin(),starts[dataflow->getVertexId(task)][i].end());
                      }
                    }
                    periodics.first = actors_check[dataflow->getVertexId(task)] - periodics.second[0];
                    task_schedule_t sched_struct = {initials,periodics};
                    schedule.set(dataflow->getVertexId(task), sched_struct);
                    // std::cout << dataflow->getVertexName(task) << ": initial starts=" << commons::toString(initials) << ", periodic starts=" << commons::toString(periodics) << std::endl;
                  }}
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
    VERBOSE_INFO(currState.print(dataflow));
    timeStep = currState.advanceTimeWithMod();
    curr_step+= timeStep;
    if (timeStep == LONG_MAX) { // NOTE should technically be LDBL_MAX cause TIME_UNIT is of type long double
      VERBOSE_INFO("Deadlock found!");
      return std::make_pair(0, schedule);
    }
  }
}

void algorithms::scheduling::So4Scheduling(models::Dataflow* const dataflow,
                                         parameters_list_t param_list) {
  VERBOSE_ASSERT(dataflow,TXT_NEVER_HAPPEND);
  VERBOSE_ASSERT(computeRepetitionVector(dataflow),"inconsistent graph");

  models::Dataflow* const temp_df = new models::Dataflow();
  *temp_df = *dataflow;

  std::map<int, std::vector<ARRAY_INDEX>> sccMap;
  std::vector<models::Dataflow*> sccDataflows;
  TIME_UNIT minThroughput = LONG_MAX; // NOTE should technically be LDBL_MAX cause TIME_UNIT is of type long double

  scheduling_t scheduling_result;
  size_t line_size = param_list.count("LINE") ? commons::fromString<size_t>(param_list["LINE"]) : 80;

  // Temp fix to turn graph into 1 SCC
  std::set<ARRAY_INDEX> new_edges = {};//add_vbuffers(dataflow, param_list);

  // generate SCCs if any
  sccMap = computeSCCKosaraju(dataflow);
  if (sccMap.size() > 1) { // if the original graph isn't one SCC, need to break into SCC subgraphs
    sccDataflows = generateSCCs(dataflow, sccMap);
    for (auto g : sccDataflows) {
      if (g->getEdgesCount() > 0) {
        std::pair<ARRAY_INDEX, EXEC_COUNT> actorInfo;
        auto res_pair = computeComponentSo4Schedule(g, actorInfo, scheduling_result, param_list["TDMA"], new_edges);
        TIME_UNIT componentThroughput = res_pair.first;
        scheduling_result = res_pair.second;
        TIME_UNIT scaledThroughput = (componentThroughput * actorInfo.second) /
          dataflow->getNi(dataflow->getVertexById(actorInfo.first));
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

    std::cout << res.asASCII(line_size);
    std::cout << res.asText();
    // std::cout << printers::Scheduling2Tikz(res);

    std::cout << "So4 throughput is  " << minThroughput << std::endl;
    std::cout << "So4 period is  " << omega << std::endl;

    temp_df->is_consistent();
    models::Scheduling test = models::Scheduling(temp_df, omega, scheduling_result);
    std::cout << "Schedule Check: " << test.is_valid_schedule() << std::endl;

    return;
  }
  // if graph is strongly connected, just need to use computeComponentThroughput
  std::pair<ARRAY_INDEX, EXEC_COUNT> actorInfo; // look at note for computeComponentThroughput
  auto res_pair = computeComponentSo4Schedule(dataflow, actorInfo, scheduling_result, param_list["TDMA"], new_edges);
  minThroughput = res_pair.first;
  scheduling_result = res_pair.second;
  std::cout << "Throughput of graph: " << minThroughput << std::endl;
  TIME_UNIT omega = 1.0 / minThroughput ;

  temp_df->is_consistent();

  models::Scheduling res = models::Scheduling(dataflow, omega, scheduling_result);
  std::cout << res.asASCII(line_size);
  std::cout << res.asText();

  std::cout << "So4 throughput is  " << minThroughput << std::endl;
  std::cout << "So4 period is  " << omega << std::endl;

  return;
}
