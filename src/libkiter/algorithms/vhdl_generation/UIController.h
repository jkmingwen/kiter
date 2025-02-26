/*
 * UIController.cpp
 *
 *  Created on: 25 February 2025
 *      Author: jkmingwen
 */
#ifndef UI_CONTROLLER_H_
#define UI_CONTROLLER_H_

#include "VHDLCommons.h"
#include "algorithms/vhdl_generation/VHDLComponent.h"

class UIController : public VHDLComponent {
public:
  UIController();

  int addInputUIPort(int dataWidth);
  int addOutputUIPort(int dataWidth);
  std::string genProcess();
  std::string genInternalSignals();
  void writeImplementation(std::ofstream &vhdlOutput);
  void portMappingInit();
  std::string getInterInSigName(int portWidth);
  std::string getInterOutSigName(int portWidth);
  std::map<std::string, int> getIntermediateSignals();

private:
  const int binaryWidth = 1;
  const int numericWidth = 34;
  const std::string extPrefix = "ui_";
  std::map<int, std::string> intermediateInSignal = {
      {binaryWidth, "ui_binary_input_data"}, {numericWidth, "ui_numeric_input_data"}};
  std::map<int, std::string> intermediateOutSignal = {
      {binaryWidth, "ui_binary_output_data"},
      {numericWidth, "ui_numeric_output_data"}};
  int numericInputUICount = 0;
  int binaryInputUICount = 0;
  int numericOutputUICount = 0;
  int binaryOutputUICount = 0;
  std::string inBinaryAddr    = "in_binary_addr";
  std::string inBinaryAddrIdx = "in_binary_addr_int";
  std::string inBinaryMem     = "in_binary_mem";
  std::string inBinaryUIData = "in_binary_ctrl";
  std::string inBinaryExtData = "in_binary_value";
  std::string outBinaryAddr    = "out_binary_addr";
  std::string outBinaryAddrIdx = "out_binary_addr_int";
  std::string outBinaryMem     = "out_binary_mem";
  std::string outBinaryUIData = "out_binary_ctrl";
  std::string outBinaryExtData = "out_binary_value";
  std::string binaryWEnable = "binary_write_enable";
  std::string inNumAddr = "in_numeric_addr";
  std::string inNumAddrIdx = "in_numeric_addr_int";
  std::string inNumMemType = "in_numeric_mem";
  std::string inNumMem = "input_numeric_registers";
  std::string inNumUIData = "in_numeric_ctrl";
  std::string inNumExtData = "in_numeric_value";
  std::string outNumAddr = "out_numeric_addr";
  std::string outNumAddrIdx = "out_numeric_addr_int";
  std::string outNumMemType = "out_numeric_mem";
  std::string outNumMem = "output_numeric_registers";
  std::string outNumUIData = "out_numeric_ctrl";
  std::string outNumExtData = "out_numeric_value";
  std::string numWEnable = "numeric_write_enable";
};
#endif /* UI_CONTROLLER_H_ */
