/*
 * VHDLCommons.h
 *
 * Created on: 16 June 2024
 *     Author: jkmingwen
 */
#ifndef VHDL_COMMONS_H_
#define VHDL_COMMONS_H_

#include <map>
#include <string>
#include <fstream>
#include <filesystem>
#include "commons/verbose.h"

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
      {"fix2fp", 1}, {"fp2fix", 1},
      // NOTE unimplemented operators from here:
      {"fp_floor", 2}, {"int_max", 1}, {"int_min", 1}, {"fp_max", 1},
      {"fp_min", 1}, {"fp_abs", 1}, {"select2", 1},
      // {"select3", 1},
      {"attach", 1}, {"int_abs", 1}, {"vbargraph", 1}}},
    {125,
     {{"fp_add", 3}, {"fp_prod", 1}, {"fp_div", 8}, {"fp_sqrt", 5},
      {"fp_diff", 3}, {"fp_pow", 8}, {"int_add", 1}, {"int_prod", 1},
      {"int_diff", 1}, {"float2int", 1}, {"int2float", 1}, {"sbuffer", 2},
      {"fix2fp", 1}, {"fp2fix", 1},
      // NOTE unimplemented operators from here:
      {"fp_floor", 2}, {"int_max", 1}, {"int_min", 1}, {"fp_max", 1},
      {"fp_min", 1}, {"fp_abs", 1}, {"select2", 1},
      // {"select3", 1},
      {"attach", 1}, {"int_abs", 1}, {"vbargraph", 1}}},
    {250,
     {{"fp_add", 6}, {"fp_prod", 1}, {"fp_div", 18}, {"fp_sqrt", 10},
      {"fp_diff", 6}, {"fp_pow", 18}, {"int_add", 1}, {"int_prod", 1},
      {"int_diff", 1}, {"float2int", 2}, {"int2float", 3}, {"sbuffer", 2},
      {"fix2fp", 1}, {"fp2fix", 2},
      // NOTE unimplemented operators from here:
      {"fp_floor", 5}, {"int_max", 1}, {"int_min", 1}, {"fp_max", 1},
      {"fp_min", 1}, {"fp_abs", 1}, {"select2", 1},
      // {"select3", 1},
      {"attach", 1}, {"int_abs", 1}, {"vbargraph", 1}}}
  };

// void copyFileAndReplaceWords(
//     std::string refFile, std::string dstFile,
//     const std::map<std::string, std::string> &replacementMap);

/**
   Generates a file based on a reference, optionally replacing words in the
   resulting file.

   @param refFile Path to reference file.
   @param dstFile Path to location of file to be generated in.
   @param replacementMap Words to match and their replacements.

   @return void
*/
inline void copyFileAndReplaceWords(std::string refFile, std::string dstFile,
                                    const std::map<std::string, std::string> &replacementMap) {
  if (replacementMap.size() > 0) {
    std::ifstream fileStream(refFile);
    std::ofstream fileCopy(dstFile);
    std::string lineContent;
    if (fileStream.is_open()) {
      while (std::getline(fileStream, lineContent)) {
        for (auto const &[match, replacement] : replacementMap) {
          size_t pos = 0;
          while ((pos = lineContent.find(match, pos)) != std::string::npos) {
            lineContent.replace(pos, match.length(), replacement);
            pos += replacement.length();
          }
        }
        fileCopy << lineContent << std::endl;
      }
      fileStream.close();
      fileCopy.close();
    } else {
      VERBOSE_ERROR("Failed to access " << refFile);
    }
  } else {
    const auto copyOptions = std::filesystem::copy_options::update_existing |
      std::filesystem::copy_options::recursive;
    std::filesystem::copy(refFile, dstFile, copyOptions);
  }
}

#endif /* VHDL_COMMONS_H_ */
