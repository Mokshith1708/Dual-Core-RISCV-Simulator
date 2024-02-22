#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
#include "registers.hpp"
// #include "memory.cpp"
// #include "memory.h"

using namespace std;
class ALU
{
private:
  int pc = 0;
  vector<string> program;

public:
  ALU(pair<int, int> &p, int no_inst, memory &m,registers &r1,int core );
  // countLines(const string &filename);
  void executeInstruction(vector<int>instructions, memory &m,registers &r1);
  //int registerfetch(int regValue,registers &r1);
};