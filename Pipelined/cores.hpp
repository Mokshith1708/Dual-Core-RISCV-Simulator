#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include "registers.hpp"
// #include "memory.cpp"
// #include "memory.hpp"
class memory;
class ALU
{
private:
  int pc1 = 0;
  int pc2 = 0;
  std::vector<std::string> program;
  std::vector<int> fetch1;
  std::vector<int> decode1;
  std::vector<int> execute1;
  std::vector<int> mem1;
  std::vector<int> write1;
  std::vector<int> fetch2;
  std::vector<int> decode2;
  std::vector<int> execute2;
  std::vector<int> mem2;
  std::vector<int> write2;
  std::vector<int>tempReg1;
  std::vector<int>tempReg2;

public:
  ALU(std::pair<int, int> &p1, std::pair<int, int> &p2, int no_inst_1, int no_inst_2, memory &m, registers &r1, registers &r2, int core1, int core2);
  void executeInstruction(std::vector<int> instructions, memory &m, registers &r, int core, int &pc);
  ALU();
  // Clock cycles
  int clockCycles1=0;
  int clockCycles2=0;
  bool dataforwarding1, dataforwarding2;
  // Function for each pipeline stage
  void instructionFetch(memory &m, int core, int &pc);
  std::vector<int> instructionDecode(memory &m, int core,registers& r,int& pc, std::vector<int>& tempReg);
  std::vector<int> instructionExecute(std::vector<int>v,memory &m, registers &r, int core, int &pc, std::vector<int>& tempReg);
  void memoryAccess(std::vector<int>k,memory &m, int core, int &pc);
  void writeBack(std::vector<int>k,memory &m, int core, int &pc, registers &r);

  // Helper functions for data forwarding and stall detection
  int forwardData(int reg, int stage);
  bool detectDataHazard(const std::pair<std::string, std::vector<int>> &current, const std::pair<std::string, std::vector<int>> &previous);
};
// #endif
