/*
 * VHDLCommons.cpp
 *
 *  Created on: 29 Oct 2024
 *      Author: jkmingwen
 */

#include <cstddef>
#include <filesystem>
#include <map>
#include <string>
#include <fstream>
#include <regex>
#include "commons/commons.h"
#include "commons/verbose.h"
#include "models/Dataflow.h"
#include "models/Scheduling.h"
#include "VHDLCommons.h"

int getOperatorLifespan(const std::string &opType, int opFreq) {
  if (operatorLifespans.at(opFreq).count(opType)) {
    return operatorLifespans.at(opFreq).at(opType);
  } else {
    // some components have no lifespan (e.g. Proj, const_val) but need a
    // positive lifespan for SDF to remain valid
    return 1;
  }
}

/**
   Get order of arguments for operators.
   Order of arguments for operators are denoted by their name (e.g.
   compName_arg0_arg1_arg2).

   @param name Actor's name.

   @return argOrder Vector of names of arguments (actors) in order.
*/
std::vector<std::string> getArgOrderFromName(std::string name) {
  std::vector<std::string> argOrder = commons::split<std::string>(name, '_');
  argOrder.erase(argOrder.begin()); // first element is the actor name

  return argOrder;
}

/**
   Get input data types for given Vertex.

   @param dataflow Graph where Vertex is instantiated.
   @param src Vertex whose input data we wish to derive.

   @return inputType Map reflecting input data type ("real"/"int") and counts
   of their occurances.
*/
std::map<std::string, int> getInputDataTypes(models::Dataflow *const dataflow, Vertex src) {
  std::map<std::string, int> inputTypes;

  {ForInputEdges(dataflow, src, inEdge) {
      std::string portName = dataflow->getEdgeOutputPortName(inEdge);
      std::vector<std::string> type = commons::split<std::string>(portName, '_');
      // edge data type determined by postfix after final '_' delimiter
      // "vect" ignored (artifact of data from input actor)
      if (type.back() == "vect") {
        type.pop_back();
      }
      inputTypes[type.back()]++;
    }}

  return inputTypes;
}

/**
   Get output data types for given Vertex.

   @param dataflow Graph where Vertex is instantiated.
   @param src Vertex whose output data we wish to derive.

   @return outputType Map reflecting output data type ("real"/"int") and counts
   of their occurances.
*/
std::map<std::string, int> getOutputDataTypes(models::Dataflow *const dataflow, Vertex src) {
  std::map<std::string, int> outputTypes;

  {ForOutputEdges(dataflow, src, outEdge) {
      std::string portName = dataflow->getEdgeInputPortName(outEdge);
      std::vector<std::string> type = commons::split<std::string>(portName, '_');
      // edge data type determined by postfix after final '_' delimiter
      // "vect" ignored (artifact of data from input actor)
      if (type.back() == "vect") {
        type.pop_back();
      }
      outputTypes[type.back()]++;
    }}

  return outputTypes;
}

/**
   Determines data type depending on the types of each port.

   @param portTypes Map of types of data on each port and their counts.

   @return opDataType String reflecting resulting data type ("real"/"int").
*/
std::string getOpDataType(std::map<std::string, int> portTypes) {
  std::vector<std::string> supportedTypes {"real", "int"};
  std::string opDataType = "nomatch";
  // ensure only supported data types present
  for (auto const &[type, count] : portTypes) {
    if (find(supportedTypes.begin(), supportedTypes.end(), type) == supportedTypes.end()) {
      VERBOSE_ERROR("\tUndefined output type detected: "
                    << type << ", input types: "
                    << commons::toString(portTypes));
    }
  }

  if (portTypes.find("real") != portTypes.end() &&
      portTypes.find("int") != portTypes.end()) {
    VERBOSE_WARNING("\tBoth Int and Float types detected, setting type to floating point");
    opDataType = "fp";
  } else if (portTypes.find("real") != portTypes.end()) {
    opDataType = "fp";
  } else if (portTypes.find("int") != portTypes.end()) {
    opDataType = "int";
  }

  return opDataType;
}

/**
   Derive a standardised name for the category of operator represented by the
   vertex. To classify vertices by the functionality they represent.

   @param dataflow Graph where Vertex is instantiated.
   @param v Vertex whose operation classification we wish to derive.

   @return opType String of operation classification.
*/
std::string deriveOpCat(models::Dataflow *const dataflow, Vertex v) {
  // Test for type of component and update their categories accordingly
  std::string opType = dataflow->getVertexType(v);

  // INPUT/OUTPUT components (graph's source/sink)
  if (opType.find("INPUT") != std::string::npos ||
      opType.find("OUTPUT") != std::string::npos) {
    return opType.substr(0, opType.find("_"));
  }

  // UI components
  if (std::count(uiTypes.begin(), uiTypes.end(), opType)) {
    return opType;
  }

  // Constant values (numbers)
  std::istringstream stringAsNumber(opType);
  float compTypeAsFloat;
  stringAsNumber >> std::noskipws >> compTypeAsFloat;
  if ((stringAsNumber.eof() && !stringAsNumber.fail()) ||
      (opType == "fSamplingFreq" || opType == "fSampleRate")) {
    return "const_value";
  }

  // Arithmetic operators
  if (std::count(arithmeticTypes.begin(), arithmeticTypes.end(), opType)) {
    // data type of arithmetic operators determined by output types
    std::string dataType = getOpDataType(getInputDataTypes(dataflow, v));
    return opType = dataType + "_" + opType;
  }

  // Numeric operators
  if (std::count(numOperatorTypes.begin(), numOperatorTypes.end(), opType)) {
    // data type of numeric operators determined by output types
    std::string dataType = getOpDataType(getInputDataTypes(dataflow, v));
    return opType = dataType + "_" + opType;
  }

  // Routing operators
  if (std::count(routingTypes.begin(), routingTypes.end(), opType)) {
    return opType;
  }

  return opType;
}

std::vector<std::string> getMultiOutputActors(models::Dataflow *const dataflow) {
  std::vector<std::string> actorNames;
  {ForEachVertex(dataflow, v) {
      std::string name = dataflow->getVertexName(v);
      std::string opType = deriveOpCat(dataflow, v);
      int numOutputs = dataflow->getVertexOutDegree(v);
      if (opOutputPorts.find(opType) != opOutputPorts.end() && opType != "output_selector") {
        if (numOutputs > opOutputPorts.at(opType).size()) {
          actorNames.push_back(name);
        }
        if (numOutputs < opOutputPorts.at(opType).size()) {
          VERBOSE_ERROR("Too few outputs for " << name << " (" << opType << ")." );
        }
      } else if (opType == "INPUT" && numOutputs > 1) {
        actorNames.push_back(name);
      } else {
        VERBOSE_INFO("Ignoring multioutput check for " << name << " (" << opType << ")");
      }
    }}
  return actorNames;
}

/**
   Generates a file based on a reference, optionally replacing words in the
   resulting file.

   @param refFile Path to reference file.
   @param dstFile Path to location of file to be generated in.
   @param replacementMap Words to match and their replacements.

   @return void
*/
void copyFileAndReplaceWords(std::string refFile, std::string dstFile,
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

std::string generateTikzFill(std::string name, std::string type,
                             ARRAY_INDEX id,
                             std::vector<TIME_UNIT> startTimes,
                             std::vector<TIME_UNIT> durations,
                             int row, bool truncNames) {
  std::stringstream output;
  std::string fillParams = "[fill=blue!40!white, draw=black]";
  int labelX = 0;
  float labelY = row + 0.5;
  size_t nameMaxLength = 15;
  size_t typeMaxLength = 15;

  // tikz requires underscores to be escaped
  type = std::regex_replace(type, std::regex("_"), "\\_");
  // shorten names by:
  // 1. removing everything after the first underscore (just used to indicate
  // arg order in dadop)
  // 2. add ellipses if name exceeds max length
  if (truncNames) {
    name = std::regex_replace(name, std::regex("_.*"), "");
    if (name.length() > nameMaxLength) {
      name = name.substr(0, nameMaxLength) + "...";
    }
    if (type.length() > typeMaxLength) {
      type = type.substr(0, typeMaxLength) + "...";
    }
  }

  // add labels for actor names and type
  // output << "\\draw (" << labelX << "," << labelY
  //        << ") node[anchor=east, align=right] {\\begin{varwidth}{3cm}" << name << "\\\\"
  //        << "(" << type << ")" << " [ " << id << " ]" << "\\end{varwidth}};" << std::endl;
  output << "\\draw (" << labelX << "," << labelY
         << ") node[anchor=east, align=right] {\\begin{varwidth}{3cm}" << type << "\\\\"
         << " (" << id << ")" << "\\end{varwidth}};" << std::endl;
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

std::string generateTikzSchedule(models::Scheduling schedule) {
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
    ARRAY_INDEX id = refGraph->getVertexId(v);
    std::vector<TIME_UNIT> startTimes = item.second.periodic_starts.second;
    std::vector<TIME_UNIT> durations = refGraph->getVertexPhaseDuration(v);
    int row = find(names.begin(), names.end(), name) -
                names.begin(); // use name order to determine row number
    output << generateTikzFill(name, type, id, startTimes, durations, row, true) << std::endl;
  }

  output << "\\end{tikzpicture}\n"
         << "\\end{document}" << std::endl;
  return output.str();
}
