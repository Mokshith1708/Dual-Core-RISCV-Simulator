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
  int n=2;
  std::vector<std::string> program;
  std::vector<std::vector<int>> fetch;
  std::vector<std::vector<int>> decode[2];
  std::vector<std::vector<int>> execute[2][5];
  std::vector<std::vector<int>> mem;
  std::vector<std::vector<int>> write;

public:
  ALU(std::pair<int, int> &p1, std::pair<int, int> &p2, int no_inst_1, int no_inst_2, memory &m, registers &r1, registers &r2, int core1, int core2);
  void executeInstruction(std::vector<int> instructions, memory &m, registers &r, int core, int &pc);

  // Clock cycles
  int clockCycles;

  // Function for each pipeline stage
  void instructionFetch(memory &m, int core, int &pc);
  void instructionDecode(memory &m, int core);
  void instructionExecute(memory &m, registers &r, int core, int &pc);
  void memoryAccess(memory &m, int core, int &pc);
  void writeBack(memory &m, int core, int &pc);

  // Helper functions for data forwarding and stall detection
  int forwardData(int reg, int stage);
  bool detectDataHazard(const std::pair<std::string, std::vector<int>> &current, const std::pair<std::string, std::vector<int>> &previous);
};
// #endif
