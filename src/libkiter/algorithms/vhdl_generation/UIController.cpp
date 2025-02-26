/*
 * UIController.cpp
 *
 *  Created on: 25 February 2025
 *      Author: jkmingwen
 */

#include "UIController.h"

UIController::UIController() {
  portMapName = "ui_ctrl";
  implRefName = "ui_controller";
}

int UIController::addInputUIPort(int dataWidth) {
  int compAddr = 0;
  if (dataWidth == binaryWidth) {
    compAddr = binaryInputUICount;
    binaryInputUICount++;
  } else if (dataWidth == numericWidth) {
    compAddr = numericInputUICount;
    numericInputUICount++;
  } else {
    VERBOSE_ERROR("Attempting to add unsupported UI data width: " << dataWidth);
  }

  return compAddr;
}

int UIController::addOutputUIPort(int dataWidth) {
  int compAddr = 0;
  if (dataWidth == binaryWidth) {
    compAddr = binaryOutputUICount;
    binaryOutputUICount++;
  } else if (dataWidth == numericWidth) {
    compAddr = numericOutputUICount;
    numericOutputUICount++;
  } else {
    VERBOSE_ERROR("Attempting to add unsupported UI data width: " << dataWidth);
  }

  return compAddr;
}

void UIController::portMappingInit() {
  // trigger port inititalised and updated in addExecution
  addGenericMapping("ram_width", "34", "integer");
  addPortMapping("clk", "sys_clk_sig", "std_logic", "in");
  // iterate through circuit's UI components and instantiate generic
  // numeric/binary input/output counts
  if (binaryInputUICount) {
    addGenericMapping("binary_input_count", std::to_string(binaryInputUICount),
                      "integer");
    addPortMapping(inBinaryUIData, intermediateInSignal.at(binaryWidth), "std_logic_vector",
                   "out", binaryInputUICount);
    addPortMapping(inBinaryAddr, extPrefix + inBinaryAddr, "std_logic_vector",
                   "in", binaryInputUICount);
    addPortMapping(inBinaryExtData, extPrefix + inBinaryExtData, "std_logic",
                   "in");
    addExternalPort(extPrefix + inBinaryAddr, inBinaryAddr, "std_logic_vector",
                    "in", binaryInputUICount);
    addExternalPort(extPrefix + inBinaryExtData, inBinaryExtData, "std_logic",
                    "in");
  }
  if (binaryOutputUICount) {
    addGenericMapping("binary_output_count",
                      std::to_string(binaryOutputUICount), "integer");
    addPortMapping(outBinaryUIData, intermediateOutSignal.at(binaryWidth), "std_logic_vector",
                   "in", binaryOutputUICount);
    addPortMapping(outBinaryAddr, extPrefix + outBinaryAddr, "std_logic_vector",
                   "in", binaryOutputUICount);
    addPortMapping(outBinaryExtData, extPrefix + outBinaryExtData, "std_logic",
                   "out");
    addExternalPort(extPrefix + outBinaryAddr, outBinaryAddr,
                    "std_logic_vector", "in", binaryOutputUICount);
    addExternalPort(extPrefix + outBinaryExtData, outBinaryExtData, "std_logic", "out");
  }
  if (numericInputUICount) {
    int numPortWidth = numericWidth * numericInputUICount;
    int numAddrWidth = numericInputUICount;
    addGenericMapping("numeric_input_count",
                      std::to_string(numericInputUICount), "integer");
    addPortMapping(inNumUIData, intermediateInSignal.at(numericWidth), "std_logic_vector",
                   "out", numPortWidth);
    addPortMapping(inNumAddr, extPrefix + inNumAddr, "std_logic_vector", "in",
                   numAddrWidth);
    addPortMapping(inNumExtData, extPrefix + inNumExtData, "std_logic_vector",
                   "in");
    addExternalPort(extPrefix + inNumAddr, inNumAddr, "std_logic_vector", "in", numAddrWidth);
    addExternalPort(extPrefix + inNumExtData, inNumExtData, "std_logic_vector", "in");
  }
  if (numericOutputUICount) {
    int numPortWidth = numericWidth * numericOutputUICount;
    int numAddrWidth = numericOutputUICount;
    addGenericMapping("numeric_output_count",
                      std::to_string(numericOutputUICount), "integer");
    addPortMapping(outNumUIData, intermediateOutSignal.at(numericWidth), "std_logic_vector",
                   "in", numPortWidth);
    addPortMapping(outNumAddr, extPrefix + outNumAddr, "std_logic_vector",
                   "in", numAddrWidth);
    addPortMapping(outNumExtData, extPrefix + outNumExtData, "std_logic_vector",
                   "out");
    addExternalPort(extPrefix + outNumAddr, outNumAddr, "std_logic_vector",
                    "in", numAddrWidth);
    addExternalPort(extPrefix + outNumExtData, outNumExtData, "std_logic_vector", "out");
  }
  // enable signals
  if (binaryInputUICount || binaryOutputUICount) {
    addPortMapping(binaryWEnable, extPrefix + binaryWEnable, "std_logic", "in");
    addExternalPort(extPrefix + binaryWEnable, binaryWEnable, "std_logic", "in");
  }
  if (numericInputUICount || numericOutputUICount) {
    addPortMapping(numWEnable, extPrefix + numWEnable, "std_logic", "in");
    addExternalPort(extPrefix + numWEnable, numWEnable, "std_logic", "in");
  }
}

std::string UIController::genProcess() {
  std::ostringstream codeOut;
  std::string pad = "    ";

  if (binaryInputUICount) {
    codeOut << pad << inBinaryAddrIdx << " <= to_integer(unsigned(" << inBinaryAddr << "));\n";
    codeOut << "process(clk)\n"
            << "begin\n"
            << "if rising_edge(clk) then\n"
            << pad << "if " << binaryWEnable << " = '1' then\n"
            << pad + pad
            << inBinaryMem << "(" << inBinaryAddrIdx << ") <= " << inBinaryExtData << ";\n"
            << pad << "end if;\n"
            << "end if;\n"
            << "end process;" << std::endl;
  }
  if (binaryOutputUICount) {
    codeOut << pad << outBinaryAddrIdx << " <= to_integer(unsigned(" << outBinaryAddr << "));\n";
    codeOut << "process(" << outBinaryAddrIdx << ")\n"
            << "begin\n"
            << pad << outBinaryExtData << " <= " << outBinaryMem << "(" << outBinaryAddrIdx << ");\n"
            << "end process;"
            << std::endl;
  }
  if (numericInputUICount) {
    codeOut << pad
            << inNumAddrIdx << " <= to_integer(unsigned(" << inNumAddr << "));\n";
    codeOut << "process(clk)\n"
            << "begin\n"
            << "if rising_edge(clk) then\n"
            << pad << "if " << numWEnable << " = '1' then\n"
            << pad + pad
            << inNumMem << "(" << inNumAddrIdx << ") <= " << inNumExtData << ";\n"
            << pad << "end if;\n"
            << "end if;\n"
            << "end process;" << std::endl;
  }
  if (numericOutputUICount) {
    codeOut << pad << outNumAddrIdx << " <= to_integer(unsigned(" << outNumAddr << "));\n";
    codeOut << "process(" << outNumAddrIdx << ")\n"
            << "begin\n"
            << pad << outNumExtData << " <= " << outNumMem << "(" << outNumAddrIdx << ");\n"
            << "end process;"
            << std::endl;
  }

  return codeOut.str();
}

std::string UIController::genInternalSignals() {
  std::ostringstream codeOut;
  std::string pad = "    ";
  if (binaryInputUICount) {
    int memSize = binaryInputUICount - 1;
    codeOut << pad << "signal " << inBinaryMem << " :  std_logic_vector(" << memSize << " downto 0);\n"
            << pad << "signal " << inBinaryAddrIdx << " : integer range 0 to " << memSize << ";" << std::endl;
  }
  if (binaryOutputUICount) {
    int memSize = binaryOutputUICount - 1;
    codeOut << pad << "signal " << outBinaryMem << " :  std_logic_vector(" << memSize << " downto 0);\n"
            << pad << "signal " << outBinaryAddrIdx << " : integer range 0 to " << memSize << ";" << std::endl;
  }
  if (numericInputUICount) {
    int memSize = numericInputUICount - 1;
    codeOut << pad << "type " << inNumMemType << " is array (0 to " << memSize
            << ") of std_logic_vector(33 downto 0);\n"
            << pad
            << "signal " << inNumMem << " : " << inNumMemType << " := (others => "
               "(others => '0'));\n"
            << pad << "signal " << inNumAddrIdx << " : integer range 0 to "
            << memSize << ";" << std::endl;
  }
  if (numericOutputUICount) {
    int memSize = numericOutputUICount - 1;
    codeOut << pad << "type " << outNumMemType << " is array (0 to " << memSize
            << ") of std_logic_vector(33 downto 0);\n"
            << pad
            << "signal " << outNumMem << " : " << outNumMemType << " := (others => "
      "(others => '0'));\n"
            << pad << "signal " << outNumAddrIdx << " : integer range 0 to "
            << memSize << ";" << std::endl;
  }

  return codeOut.str();
}

void UIController::writeImplementation(std::ofstream &vhdlOutput) {
  std::string pad = "    ";
  portMappingInit();

  vhdlOutput << "library ieee;\n"
             << "use ieee.std_logic_1164.all;\n"
             << "use ieee.numeric_std.all;\n"
             << std::endl;

  vhdlOutput << genEntityDecl() << std::endl;

  vhdlOutput << "architecture Behavioral of " << implRefName
             << " is\n"
             << genInternalSignals()
             << "begin\n"
             << std::endl;
  vhdlOutput << genProcess() << std::endl;
  vhdlOutput << "end Behavioral;" << std::endl;
}

std::string UIController::getInterInSigName(int portWidth) {
  return this->intermediateInSignal.at(portWidth);
}

std::string UIController::getInterOutSigName(int portWidth) {
  return this->intermediateOutSignal.at(portWidth);
}

std::map<std::string, int> UIController::getIntermediateSignals() {
  std::map<std::string, int> intermediateSignals; // signal name -> width
  if (binaryInputUICount) {
    intermediateSignals[getInterInSigName(binaryWidth)] = binaryInputUICount;
  }
  if (binaryOutputUICount) {
    intermediateSignals[getInterOutSigName(binaryWidth)] = binaryOutputUICount;
  }
  if (numericInputUICount) {
    intermediateSignals[getInterInSigName(numericWidth)] =
        numericInputUICount * numericWidth;
  }
  if (numericOutputUICount) {
    intermediateSignals[getInterOutSigName(numericWidth)] =
        numericOutputUICount * numericWidth;
  }
  return intermediateSignals;
}
