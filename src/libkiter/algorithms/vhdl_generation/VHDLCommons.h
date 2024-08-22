/*
 * VHDLCommons.h
 *
 * Created on: 16 June 2024
 *     Author: jkmingwen
 */
#ifndef VHDL_COMMONS_H_
#define VHDL_COMMONS_H_

#include <cstddef>
#include <map>
#include <string>
#include <fstream>
#include <filesystem>
#include <regex>
#include "commons/commons.h"
#include "commons/verbose.h"
#include "models/Scheduling.h"

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

inline std::string generateTikzFill(std::string name, std::string type,
                                    std::vector<TIME_UNIT> startTimes,
                                    std::vector<TIME_UNIT> durations,
                                    int row, bool truncNames) {
  std::stringstream output;
  std::string fillParams = "[fill=blue!40!white, draw=black]";
  int labelX = 0;
  float labelY = row + 0.5;
  size_t nameMaxLength = 15;
  // shorten names by:
  // 1. removing everything after the first underscore (just used to indicate
  // arg order in dadop)
  // 2. add ellipses if name exceeds max length
  if (truncNames) {
    name = std::regex_replace(name, std::regex("_.*"), "");
    if (name.length() > nameMaxLength) {
      name = name.substr(0, nameMaxLength) + "...";
    }
  }
 // tikz requires underscores to be escaped
  type = std::regex_replace(type, std::regex("_"), "\\_");

  // add labels for actor names and type
  output << "\\draw (" << labelX << "," << labelY
         << ") node[anchor=east, align=right] {\\begin{varwidth}{3cm}" << name << "\\\\"
         << "(" << type << ")" << "\\end{varwidth}};" << std::endl;
  for (int p = 0; p < startTimes.size(); p++) {
    VERBOSE_ASSERT(startTimes.size() == durations.size(), "Vector of start times and durations should have equal number of elements");
    std::string btmLeftCoords = std::to_string((int) startTimes[p]) + "," + std::to_string(row); // x,y
    int duration = durations[p];
    std::string topRightCoords = std::to_string((int) startTimes[p] + duration) + "," + std::to_string(row + 1); // x,y
    float phaseX = startTimes[p] + ((float)duration / 2);
    float phaseY = labelY;

  output << "\\filldraw" << fillParams << " (" << btmLeftCoords
         << ") rectangle (" << topRightCoords << ");" << std::endl;

  // add labels for exec phases
  output << "\\draw (" << phaseX << "," << phaseY << ") node[anchor=center] {"
         << p << "};" << std::endl;
  }
  return output.str();
}

inline std::string
generateTikzSchedule(models::Scheduling schedule) {
  std::stringstream output;
  std::string gridParams = "[step=1cm,gray,very thin]";
  int lX = 0;
  int lY = 0;
  int rX = schedule.getGraphPeriod() + 2; // add some padding
  auto scheduleObj = schedule.getTaskSchedule();
  int rY = scheduleObj.size(); // = actor count
  std::string btmLeftCoords = std::to_string(lX) + "," + std::to_string(lY); // x,y
  std::string topRightCoords = std::to_string(rX) + "," + std::to_string(rY); // x,y
  int widthLimit = 10; // in counts of grids
  int heightLimit = 20; // in counts of grids
  const models::Dataflow* refGraph = schedule.getDataflow();

  // latex headers
  output << "\\documentclass{minimal}\n"
         << "\\usepackage{tikz}\n"
         << "\\usepackage{varwidth}\n"
         << "\\usepackage[paperwidth=597pt, paperheight=845pt]{geometry}"
         << std::endl;
  // add padding to paper dimensions if necessary
  if (rX > widthLimit) {
    output << "\\addtolength{\\paperwidth}{" << rX - widthLimit << "cm}" << std::endl;
  }
  if (rY > heightLimit) {
    output << "\\addtolength{\\paperheight}{" << rY - heightLimit << "cm}" << std::endl;
  }

  // generate tikz diagram of schedule
  output << "\\begin{document}\n"
         << "\\begin{tikzpicture}" << std::endl;

  // generate grid representing schedule times
  // each row represents an actor's executions
  // each column represents a unit of time
  output << "\\draw"
         << gridParams
         << " (" << btmLeftCoords << ") grid ("
         << topRightCoords << ");" << std::endl;
  // add tick marks for time
  output << "\\foreach \\x in {" << lX << ",...," << rX << "}\n"
         << "\\draw (\\x cm,1pt) -- (\\x cm, -1pt) node[anchor=north] {$\\x$};"
         << std::endl;

  // generate order of actors from latest to earliest exec time
  std::vector<int> times;
  std::vector<std::string> names;
  for (const auto &item : scheduleObj) {
    Vertex v = refGraph->getVertexById(item.first);
    std::string name = refGraph->getVertexName(v);
    TIME_UNIT start = item.second.periodic_starts.second.front();
    times.push_back((int) start);
    names.push_back(name);
  }
  // sort here
  for (int i = 0; i < times.size() - 1; i++) {
    int maxId = i;
    for (int j = i + 1; j < times.size(); j++) {
      if (times[j] > times[maxId]) {
        maxId = j;
      }
    }
    if (maxId != i) {
      TIME_UNIT tmpTime = times[i];
      std::string tmpName = names[i];
      times[i] = times[maxId];
      times[maxId] = tmpTime;
      names[i] = names[maxId];
      names[maxId] = tmpName;
    }
  }

  for (const auto &item : scheduleObj) {
    Vertex v = refGraph->getVertexById(item.first);
    std::string name = refGraph->getVertexName(v);
    std::string type = refGraph->getVertexType(v);
    std::vector<TIME_UNIT> startTimes = item.second.periodic_starts.second;
    std::vector<TIME_UNIT> durations = refGraph->getVertexPhaseDuration(v);
    int row = find(names.begin(), names.end(), name) -
                names.begin(); // use name order to determine row number
    output << generateTikzFill(name, type, startTimes, durations, row, true) << std::endl;
  }

  output << "\\end{tikzpicture}\n"
         << "\\end{document}" << std::endl;
  return output.str();
}


#endif /* VHDL_COMMONS_H_ */
