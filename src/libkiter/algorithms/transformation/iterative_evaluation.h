/*
 *  iterative_evaluation.h
 *
 *  Created on: Aug 2, 2022
 *      Author: jkmingwen
 */

#ifndef SRC_LIBKITER_ALGORITHMS_TRANSFORMATION_ITERATIVE_EVALUATION_H_
#define SRC_LIBKITER_ALGORITHMS_TRANSFORMATION_ITERATIVE_EVALUATION_H_



#include <string>
#include <vector>
#include <commons/commons.h>
#include <models/Dataflow.h>
#include <commons/KiterRegistry.h>



namespace models {
  class Dataflow;
  class Scheduling;
}

namespace algorithms {
  namespace transformation {
    void iterative_evaluate(models::Dataflow* const dataflow, parameters_list_t  parameters);
    void use_scheduled_buffers(models::Dataflow *const dataflow,
                               parameters_list_t parameters);
  }
  bool checkForNumericInputs(models::Dataflow* const dataflow, Vertex v);
  bool checkForStaticDelay(models::Dataflow* const dataflow, Vertex v,
                           Edge &inputSig, Edge &delayArg, int &delayAmt);
  std::string evalBinop(std::string op, std::string arg1, std::string arg2);
  std::string evalCast(std::string op, std::string arg);
  std::string evalOther(std::string op, std::vector<std::string> args);
  std::string evalTrig(std::string op, std::string arg);
  void bypassDelay(models::Dataflow *const dataflow, Vertex v, Edge inputSig,
                   Edge delayArg, int delayAmt);
  void delayToBuffer(models::Dataflow *const dataflow, Vertex v, Edge inputSig,
                     Edge delayArg, int delayAmt);
  void bypassProj(models::Dataflow *const dataflow, Vertex v);
  void routeMultiOutDelay(models::Dataflow* const dataflow, Vertex v);
  void applyResult(models::Dataflow* const dataflow, Vertex v, std::string result);
  int getChannelNumber(std::string channelName);
}

ADD_TRANSFORMATION(
    IterativeEvaluation,
    transformation_t({"IterativeEvaluation",
                      "Faust to SDF: simplify graph by iteratively evaluating "
                      "expressions with constant inputs.",
                      algorithms::transformation::iterative_evaluate}));
ADD_TRANSFORMATION(
    ScheduledBuffers,
    transformation_t(
        {"ScheduledBuffers",
         "Faust to SDF: Use scheduled buffers in SDF and simplify graph.",
         algorithms::transformation::use_scheduled_buffers}));



#endif /* SRC_LIBKITER_ALGORITHMS_TRANSFORMATION_ITERATIVE_EVALUATION_H_ */
