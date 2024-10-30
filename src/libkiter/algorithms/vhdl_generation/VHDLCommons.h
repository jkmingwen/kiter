/*
 * VHDLCommons.h
 *
 * Created on: 16 June 2024
 *     Author: jkmingwen
 */
#ifndef VHDL_COMMONS_H_
#define VHDL_COMMONS_H_

#include "models/Dataflow.h"
#include "models/Scheduling.h"
#include <map>
#include <string>
#include <vector>
enum implType { TT, DD }; // TT: time triggered, DD: data driven

/* Each component has a specific lifespan and name that needs to be defined in
   the generated AXI interface --- we track them using a predefined map where
   the keys are the operating frequencies. The "types" of the operators,
   which are taken from the actor types in the SDF file generated from the Faust
   application, then map to the number of clock cycles taken for them to complete */
inline std::map<int, std::map<std::string, int>> operatorLifespans =
  {
    {50,
     {{"fp_add", 1}, {"fp_prod", 1}, {"fp_div", 3}, {"fp_sqrt", 1},
      {"fp_diff", 1}, {"fp_pow", 3}, {"int_add", 1}, {"int_prod", 1},
      {"int_diff", 1}, {"float2int", 1}, {"int2float", 1}, {"sbuffer", 2},
      {"fix2fp", 1}, {"fp2fix", 1}, {"shiftreg", 1},
      // NOTE unimplemented operators from here:
      {"fp_floor", 2}, {"int_max", 1}, {"int_min", 1}, {"fp_max", 1},
      {"fp_min", 1}, {"fp_abs", 1}, {"select2", 1},
      // {"select3", 1},
      {"attach", 1}, {"int_abs", 1}, {"vbargraph", 1}}},
    {125,
     {{"fp_add", 3}, {"fp_prod", 1}, {"fp_div", 8}, {"fp_sqrt", 5},
      {"fp_diff", 3}, {"fp_pow", 8}, {"int_add", 1}, {"int_prod", 1},
      {"int_diff", 1}, {"float2int", 1}, {"int2float", 1}, {"sbuffer", 2},
      {"fix2fp", 1}, {"fp2fix", 1}, {"shiftreg", 1},
      // NOTE unimplemented operators from here:
      {"fp_floor", 2}, {"int_max", 1}, {"int_min", 1}, {"fp_max", 1},
      {"fp_min", 1}, {"fp_abs", 1}, {"select2", 1},
      // {"select3", 1},
      {"attach", 1}, {"int_abs", 1}, {"vbargraph", 1}}},
    {250,
     {{"fp_add", 6}, {"fp_prod", 1}, {"fp_div", 18}, {"fp_sqrt", 10},
      {"fp_diff", 6}, {"fp_pow", 18}, {"int_add", 1}, {"int_prod", 1},
      {"int_diff", 1}, {"float2int", 2}, {"int2float", 3}, {"sbuffer", 2},
      {"fix2fp", 1}, {"fp2fix", 2}, {"shiftreg", 1},
      // NOTE unimplemented operators from here:
      {"fp_floor", 5}, {"int_max", 1}, {"int_min", 1}, {"fp_max", 1},
      {"fp_min", 1}, {"fp_abs", 1}, {"select2", 1},
      // {"select3", 1},
      {"attach", 1}, {"int_abs", 1}, {"vbargraph", 1}}}
  };

inline std::vector<std::string> uiTypes = {"button",   "checkbox", "hslider",
                                    "vslider",  "nentry",   "vbargraph",
                                    "hbargraph"};

inline std::vector<std::string> arithmeticTypes = {"add",  "prod", "diff", "div",
                                                   "prod", "sqrt", "pow"};

inline std::vector<std::string> numOperatorTypes = {"floor", "min", "max", "abs"};

inline std::vector<std::string> routingTypes = {"select2", "select3", "attach"};

int getOperatorLifespan(const std::string &opType, int opFreq);
std::vector<std::string> getArgOrderFromName(std::string name);
std::map<std::string, int> getInputDataTypes(models::Dataflow *const dataflow,
                                             Vertex src);
std::map<std::string, int> getOutputDataTypes(models::Dataflow *const dataflow,
                                              Vertex src);
std::string getOpDataType(std::map<std::string, int> portTypes);
std::string deriveOpCat(models::Dataflow *const dataflow,
                        Vertex v);
void copyFileAndReplaceWords(
    std::string refFile, std::string dstFile,
    const std::map<std::string, std::string> &replacementMap);
std::string generateTikzFill(std::string name, std::string type, ARRAY_INDEX id,
                             std::vector<TIME_UNIT> startTimes,
                             std::vector<TIME_UNIT> durations, int row,
                             bool truncNames);
std::string generateTikzSchedule(models::Scheduling schedule);


#endif /* VHDL_COMMONS_H_ */
