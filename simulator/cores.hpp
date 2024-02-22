#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
#include<memory.cpp>
#include<memory.h>

using namespace std;
class ALU
{
private:
  int pc = 0;
  vector<string> program;

public:
  void executeInstruction(const int *instruction, memory &m, int core);
};