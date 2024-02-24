#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
#include "registers.hpp"
// #include "memory.cpp"
//#include "memory.hpp"

using namespace std;
class ALU
{
private:
  int pc1 = 0;
  int pc2 = 0;
  vector<string> program;

public:
  ALU(pair<int, int> &p1,pair<int, int> &p2, int no_inst_1,int no_inst_2,memory &m1,memory &m2, registers &r1,registers &r2, int core1, int core2);
  void executeInstruction(vector<int> instructions, memory &m, registers &r, int core,int &pc);
};