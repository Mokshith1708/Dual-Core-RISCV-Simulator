#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include "registers.hpp"
using std::string;
// #include "memory.cpp"
// #include "memory.hpp"
class memory;
class ALU
{
private:
  int pc1 = 0;
  int pc2 = 0;
  int no_inst_1, no_inst_2;
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
  std::vector<int> tempReg1;
  std::vector<int> tempReg2;

public:
  ALU(std::map<string, int> &latency_map, std::pair<int, int> &p1, std::pair<int, int> &p2, int no_inst_1, int no_inst_2, memory &m, registers &r1, registers &r2, int core1, int core2,bool dataforwarding_on);
  void executeInstruction(std::vector<int> instructions, memory &m, registers &r, int core, int &pc);
  
  int clockCycles1 = 0;
  int clockCycles2 = 0;
  int t_f_1,t_f_2;
  int count1=0;
  int count2=0;
  int ggg;
  int ggg1, ggg2, ggg3, ggg4;
  int gg;
  int gg1, gg2, gg3, gg4;
  int add_lat,addi_lat,sub_lat,mul_lat,muli_lat;
  bool dataforwarding1 = true;
  bool dataforwarding2;
  void executeInstructiondummy(std::vector<int> instruction, memory &m, registers &r, int core, int pc,std::vector<int> &tempReg);
  std::vector<int> prevpc1, prevpc2;
  void instructionFetch(memory &m, int core, int &pc, registers &r, std::vector<int> &tempReg);
  std::vector<int> instructionDecode(memory &m, int core, registers &r, int &pc, std::vector<int> &tempReg);
  std::vector<int> instructionExecute(std::vector<int> v, memory &m, registers &r, int core, int &pc, std::vector<int> &tempReg);
  void memoryAccess(std::vector<int> k, memory &m, int core, int &pc);
  void writeBack(std::vector<int> k, memory &m, int core, int &pc, registers &r);
  int RAW_Hazard(std::vector<int>v1,std::vector<int>v);
  int typeOf(int k);
  bool predictor(int pc,memory& m);
  void stallManagement(memory &m, int core, registers &r, int &pc, std::vector<int> &tempReg);
};
