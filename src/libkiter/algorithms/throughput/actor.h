/*
 * actor.h
 *
 *  Created on: 22 December 2020
 *      Author: jkmingwen
 */
#ifndef ACTOR_H_
#define ACTOR_H_

#include "../dse/abstract_dep_graph.h"
#include <models/Dataflow.h>

#include <vector>

namespace models {
  class Dataflow;
}

class abstractDepGraph;
class State;

class Actor {
 public:
  Actor();
  Actor(models::Dataflow* const dataflow, Vertex a); // actor must be associated to a graph

  EXEC_COUNT getPhaseCount();
  PHASE_INDEX getPhase();
  EXEC_COUNT getRepFactor();
  ARRAY_INDEX getId();
  void setId(ARRAY_INDEX newId);
  TOKEN_UNIT getExecRate(Edge e);
  TOKEN_UNIT getExecRate(Edge e, PHASE_INDEX p);
  EXEC_COUNT getNumExecutions();
  bool isReadyForExec(State& s);
  bool isReadyToEndExec(State& s);
  // int isReadyForExecWithMod(State s);
  // bool isReadyToEndExecWithMod(State s, models::Dataflow* const dataflow, std::map<std::pair<ARRAY_INDEX, ARRAY_INDEX>, long> cond, long step);
  void execStart(models::Dataflow* const dataflow, State &s);
  // void execStartWithMod(models::Dataflow* const dataflow, State &s, std::map<TIME_UNIT, std::map<ARRAY_INDEX, std::pair<long, bool>>> *buffer,
  // TIME_UNIT step, long slots, std::map<ARRAY_INDEX, long> condition);
  // void execNextStartWithMod(models::Dataflow* const dataflow, State &s, std::map<TIME_UNIT, std::map<ARRAY_INDEX, std::pair<long, bool>>> *buffer,
  // TIME_UNIT step, long slots, std::map<ARRAY_INDEX, long> condition);
  void execEnd(models::Dataflow* const dataflow, State &s);
  void execEndWithMod(models::Dataflow* const dataflow, State &s, std::deque<std::pair<TIME_UNIT, std::pair<ARRAY_INDEX, long>>>* buffer,
  TIME_UNIT step, long slots, std::map<ARRAY_INDEX, long> condition);
  void computeCausalDeps(models::Dataflow* const dataflow, State &prevState,
                         abstractDepGraph &absDepGraph);
  std::string printStatus(models::Dataflow* const dataflow);
 private:
  Vertex actor;
  EXEC_COUNT numExecs; // track number of executions
  PHASE_INDEX phaseCount; // number of phases of execution
  EXEC_COUNT repFactor;
  ARRAY_INDEX id;
  bool isExecuting; // FIXME this is a workaround for preventing actors from executing more than once in a single time frame
  std::map<Edge, std::vector<TOKEN_UNIT>> prodExecRate;
  std::map<Edge, std::vector<TOKEN_UNIT>> consExecRate;
};

typedef  std::vector<Actor> ActorMap_t;

#endif /* ACTOR_H_ */
