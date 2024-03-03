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

public:
  ALU(std::pair<int, int> &p1, std::pair<int, int> &p2, int no_inst_1, int no_inst_2, memory &m, registers &r1, registers &r2, int core1, int core2);
  void executeInstruction(std::vector<int> instructions, memory &m, registers &r, int core, int &pc);
  void logs(const std::string &message, std::ofstream &outputFile);
  void logi(int number, std::ofstream &outputFile);
  template <typename T>
  void logVariable(const std::string &variableName, T variable, std::ofstream &outputFile);
};
// #endif
