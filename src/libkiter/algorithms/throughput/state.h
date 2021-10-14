/*
 * state.h
 *
 *  Created on: 3 January 2021
 *      Author: jkmingwen
 */

#ifndef STATE_H_
#define STATE_H_

#include "actor.h"
#include <models/Dataflow.h>

namespace models {
  class Dataflow;
}

class Actor;

class State {
 public:
  State();
  State(models::Dataflow* const dataflow,
        std::map<ARRAY_INDEX, Actor> actorMap);
  State(models::Dataflow* const dataflow,
        std::map<ARRAY_INDEX, Actor> actorMap,
        std::map<Edge, TOKEN_UNIT> &bufferSizes);
  PHASE_INDEX getPhase(Vertex a) const; // returns current phase of actor
  TOKEN_UNIT getTokens(Edge e) const; // returns current tokens in edge
  TOKEN_UNIT getBufferSize(Edge e) const; // returns maximum token capacity of edge
  TOKEN_UNIT getBufferSpace(Edge e) const; // returns amount of space left in buffer
  std::list<std::pair<TIME_UNIT, PHASE_INDEX>> getRemExecTime(Vertex a) const; // returns amount of time left for execution
  std::map<Vertex, std::list<std::pair<TIME_UNIT, PHASE_INDEX>>> getExecQueue();
  void removeFrontExec(Vertex a);
  TIME_UNIT getTimeElapsed() const; // returns total elapsed time in state
  void setPhase(Vertex a, PHASE_INDEX newPhase);
  void setTokens(Edge e, TOKEN_UNIT newTokens);
  void setBufferSpace(Edge e, TOKEN_UNIT newSpace);
  void addExecution(Vertex a, std::pair<TIME_UNIT, PHASE_INDEX> newExec);
  void advanceRemExecTime(Vertex a, TIME_UNIT timeStep);
  void setTimeElapsed(TIME_UNIT time);
  void updateState(models::Dataflow* const dataflow,
                   std::map<ARRAY_INDEX, Actor> actorMap); // updates state with current status of graph
  TIME_UNIT advanceTime();
  TIME_UNIT advanceTimeWithMod();
  bool hasBoundedBuffers();
  bool operator==(const State& s) const;
  std::string print(models::Dataflow* const dataflow);

  /* bool addToVisitedStates(std::vector<State> &visitedStates); // checks list of visited states and adds to list if new state */

 private:
  std::map<Vertex, std::list<std::pair<TIME_UNIT, PHASE_INDEX>>> executingActors; // track actor's currently executing and the time before completing execution
  std::map<Vertex, PHASE_INDEX> actorPhases; // track the current phases of each actor
  std::map<Edge, TOKEN_UNIT> currentTokens; // track number of tokens in each channel
  std::map<Edge, TOKEN_UNIT> bufferCapacities; // track maximum token capacity for each channel
  std::map<Edge, TOKEN_UNIT> bufferSpaces; // track how many more tokens can be stored for each channel
  std::vector<Vertex> actors; // store actors in state
  TIME_UNIT timeElapsed; // track time elapsed given state
  bool isBounded; // whether the graph is modelled with bounded buffers --- needs to be used when computing storage dependencies for throughput-buffersizing tradeoff analysis

};

class StateList {
public:
  StateList();
  StateList(State s);
  bool addState(State s);
  std::list<State>::iterator getRepeatedState();
  TIME_UNIT computeThroughput();
  int computeIdx(State s);

private:
  std::list<State> visitedStates;
  std::list<State>::iterator repeatedState;
};
#endif /* STATE_H_ */
