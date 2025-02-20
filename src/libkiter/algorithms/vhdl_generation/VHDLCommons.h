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
enum implType { TT, DD, GS }; // TT: time triggered, DD: data driven, GS: globally scheduled
enum direction { in, out };

/* Each component has a specific lifespan and name that needs to be defined in
   the generated AXI interface --- we track them using a predefined map where
   the keys are the operating frequencies. The "types" of the operators,
   which are taken from the actor types in the SDF file generated from the Faust
   application, then map to the number of clock cycles taken for them to complete */
inline std::map<int, std::map<std::string, int>> operatorLifespans =
  {
    {50,
     {{"fp_add", 1}, {"fp_prod", 1}, {"fp_div", 3}, {"fp_sqrt", 1},
      {"fp_diff", 1}, {"fp_pow", 3}, {"fp_exp", 1}, {"int_add", 1},
      {"int_prod", 1}, {"int_diff", 1}, {"float2int", 1}, {"int2float", 1},
      {"sbuffer", 2}, {"fix2fp", 1}, {"fp2fix", 1}, {"shiftreg", 1},
      {"fp_floor", 1}, {"int_max", 2}, {"int_min", 2}, {"fp_max", 1},
      {"fp_min", 1}, {"fp_abs", 1}, {"int_abs", 1},
      // NOTE unimplemented operators from here:
      {"select2", 1}, {"select3", 1}, {"checkbox", 1}, {"button", 1},
      {"attach", 1}, {"vbargraph", 1}, {"hbargraph", 1}, {"delay", 2}}},
    {125,
     {{"fp_add", 3}, {"fp_prod", 1}, {"fp_div", 8}, {"fp_sqrt", 5},
      {"fp_diff", 3}, {"fp_pow", 8}, {"fp_exp", 3}, {"int_add", 1},
      {"int_prod", 1}, {"int_diff", 1}, {"float2int", 1}, {"int2float", 1},
      {"sbuffer", 2}, {"fix2fp", 1}, {"fp2fix", 1}, {"shiftreg", 1},
      {"fp_floor", 2}, {"int_max", 2}, {"int_min", 2}, {"fp_max", 1},
      {"fp_min", 1}, {"fp_abs", 1}, {"int_abs", 1},
      // NOTE unimplemented operators from here:
      {"select2", 1}, {"select3", 1}, {"checkbox", 1}, {"button", 1},
      {"attach", 1}, {"vbargraph", 1}, {"hbargraph", 1}, {"delay", 2}}},
    {250,
     {{"fp_add", 6}, {"fp_prod", 1}, {"fp_div", 18}, {"fp_sqrt", 10},
      {"fp_diff", 6}, {"fp_pow", 18}, {"fp_exp", 6}, {"int_add", 1},
      {"int_prod", 1}, {"int_diff", 1}, {"float2int", 2}, {"int2float", 3},
      {"sbuffer", 2}, {"fix2fp", 1}, {"fp2fix", 2}, {"shiftreg", 1},
      {"fp_floor", 5}, {"int_max", 2}, {"int_min", 2}, {"fp_max", 1},
      {"fp_min", 1}, {"fp_abs", 1}, {"int_abs", 1},
      // NOTE unimplemented operators from here:
      {"select2", 1}, {"select3", 1}, {"checkbox", 1}, {"button", 1},
      {"attach", 1}, {"vbargraph", 1}, {"hbargraph", 1}, {"delay", 2}}}
  };

inline std::map<std::string, std::vector<std::string>> opInputPorts = {
  // arithmetic types
  {"fp_add", {"X", "Y"}}, {"fp_prod", {"X", "Y"}},
  {"fp_div", {"X", "Y"}}, {"fp_sqrt", {"X"}},
  {"fp_diff", {"X", "Y"}}, {"fp_pow", {"X", "Y"}},
  {"fp_exp", {"X"}}, {"int_add", {"X", "Y"}},
  {"int_prod", {"X", "Y"}}, {"int_diff", {"X", "Y"}},
  // numeric types
  {"float2int", {"X"}}, {"int2float", {"X"}},
  {"int_max", {"X", "Y"}}, {"int_min", {"X", "Y"}},
  {"fp_max", {"X", "Y"}}, {"fp_min", {"X", "Y"}},
  {"fp_abs", {"X"}}, {"int_abs", {"X"}},
  // routing types
  {"select2", {"S", "X", "Y"}},
  {"select3", {"S", "X", "Y", "Z"}},
  {"attach", {"X", "Y"}}, {"fp_floor", {"X"}},
  // ui types
  {"button", {"X"}}, {"checkbox", {"X"}},
  {"hslider", {"X"}}, {"vslider", {"X"}},
  {"nentry", {"X"}}, {"vbargraph", {"X"}},
  {"hbargraph", {"X"}},
  // others
  {"sbuffer", {"in_data"}}, {"shiftreg", {"in_data"}},
  {"buffer", {"in_data"}}, {"delay", {"op_in_data_0", "op_in_data_1"}}
  };

inline std::map<std::string, std::vector<std::string>> opOutputPorts = {
  // arithmetic types
  {"fp_add", {"R"}},    {"fp_prod", {"R"}},   {"fp_div", {"R"}},
  {"fp_sqrt", {"R"}},   {"fp_diff", {"R"}},   {"fp_pow", {"R"}},
  {"fp_exp", {"R"}},  {"int_add", {"R"}}, {"int_prod", {"R"}},
  {"int_diff", {"R"}},
  // numeric types
  {"float2int", {"R"}}, {"int2float", {"R"}},
  {"int_max", {"R"}},  {"int_min", {"R"}},   {"fp_max", {"R"}},
  {"fp_min", {"R"}},  {"fp_abs", {"R"}},    {"int_abs", {"R"}},
  {"fp_floor", {"R"}},
  // routing types
  {"select2", {"R"}}, {"select3", {"R"}}, {"attach", {"R"}},
  // ui types
  {"button", {"R"}}, {"checkbox", {"R"}},
  {"hslider", {"R"}}, {"vslider", {"R"}}, {"nentry", {"R"}},
  {"vbargraph", {"R"}}, {"hbargraph", {"R"}},
  // others
  {"sbuffer", {"out_data"}}, {"shiftreg", {"out_data"}},
  {"buffer", {"out_data"}}, {"delay", {"op_out_data_0"}}
};

inline std::map<std::string, std::string> implementationNames = {
    // arithmetic types
    {"fp_add", "fp_add_flopoco"}, {"fp_prod", "fp_prod_flopoco"},
    {"fp_div", "fp_div_flopoco"}, {"fp_sqrt", "fp_sqrt_flopoco"},
    {"fp_diff", "fp_diff_flopoco"}, {"fp_pow", "fp_pow_flopoco"},
    {"fp_exp", "fp_exp_flopoco"},
    {"int_add", "int_add_flopoco"}, {"int_diff", "int_diff_flopoco"},
    {"int_prod", "int_prod_flopoco"},
    // numeric types
    {"float2int", "float2int_flopoco"}, {"int2float", "int2float_flopoco"},
    {"fp_floor", "fp_floor_flopoco"}, {"int_max", "int_max"},
    {"int_min", "int_min"}, {"fp_max", "fp_max"},
    {"fp_min", "fp_min"}, {"fp_abs", "fp_abs"},
    {"int_abs", "int_abs"},
    // routing types
    {"select2", "select2"}, {"select3", "select3"},
    {"attach", "attach"},
    // ui types
    {"vbargraph", "vbargraph"}, {"hbargraph", "hbargraph"},
    {"hslider", "hslider"}, {"vslider", "vslider"},
    {"nentry", "nentry"}, {"checkbox", "checkbox"},
    {"button", "button"},
    // others
    {"delay", "delay"}, {"sbuffer", "sbuffer"},
    {"Proj", "hs_splitter"}, {"shiftreg", "pipo_shift_reg"},
    {"hs_fifo", "hs_fifo"}
};

inline std::vector<std::string> uiTypes = {"button",   "checkbox", "hslider",
                                           "vslider",  "nentry",   "vbargraph",
                                           "hbargraph"};

inline std::vector<std::string> rangeUITypes = {"hslider", "vslider",  "nentry",
                                                "vbargraph", "hbargraph"};

inline std::vector<std::string> switchUITypes = {"button", "checkbox"};

inline std::vector<std::string> arithmeticTypes = {"add",  "prod", "diff", "div",
                                                   "prod", "sqrt", "pow", "exp"};

inline std::vector<std::string> numOperatorTypes = {"floor", "min", "max", "abs"};

inline std::vector<std::string> routingTypes = {"select2", "select3", "attach"};

inline std::unordered_set<std::string> unsupportedOperators = {"cos", "sin"};

int getOperatorLifespan(const std::string &opType, int opFreq);
std::vector<std::string> getArgOrderFromName(std::string name);
std::map<std::string, float> getParamsFromName(std::string name);
std::map<std::string, int> getInputDataTypes(models::Dataflow *const dataflow,
                                             Vertex src);
std::map<std::string, int> getOutputDataTypes(models::Dataflow *const dataflow,
                                              Vertex src);
std::string getOpDataType(std::map<std::string, int> portTypes);
std::string deriveOpCat(models::Dataflow *const dataflow, Vertex v);
std::vector<std::string> getMultiOutputActors(models::Dataflow *const dataflow);
std::string getFullNameFromBaseName(models::Dataflow *const dataflow,
                                   const std::string &partialName);
std::string getBaseName(std::string fullName);
void copyFileAndReplaceWords(
    std::string refFile, std::string dstFile,
    const std::map<std::string, std::string> &replacementMap);
std::string generateTikzFill(std::string name, std::string type, ARRAY_INDEX id,
                             std::vector<TIME_UNIT> startTimes,
                             std::vector<TIME_UNIT> durations, int row,
                             bool truncNames);
std::string generateTikzSchedule(models::Scheduling schedule);
std::string tab(int level);
std::string fpcFloatToBinaryString(float f);
std::vector<std::string> splitByString(const std::string& str, const std::string& delimiter);


#endif /* VHDL_COMMONS_H_ */
