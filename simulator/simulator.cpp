#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstdarg>
#include "simulator.hpp"
#include <sstream>
#include "memory.hpp"
#include "memory.cpp"
#include <bits/stdc++.h>
using namespace std;
using vs = vector<string>;
using vss = vector<vector<string>>;
using vi = vector<int>;
RISCV::Inst stringToInst(const std::string &s)
{
    static const std::map<std::string, RISCV::Inst> instMap = {
        {"jal", RISCV::jal},
        {"jalr", RISCV::jalr},
        {"beq", RISCV::beq},
        {"bne", RISCV::bne},
        {"blt", RISCV::blt},
        {"bge", RISCV::bge},
        {"bltu", RISCV::bltu},
        {"begu", RISCV::begu},
        {"lb", RISCV::lb},
        {"lw", RISCV::lw},
        {"sb", RISCV::sb},
        {"sw", RISCV::sw},
        {"addi", RISCV::addi},
        {"add", RISCV::add},
        {"sub", RISCV::sub},
        {"ecall", RISCV::ecall},
        {"lbu", RISCV::lbu},
        {"lwu", RISCV::lwu},
    };

    auto it = instMap.find(s);
    if (it != instMap.end())
    {
        return it->second;
    }
    else
    {
        // Handle unknown instruction
        return RISCV::lwu;
    }
}

// Function to convert a string to the corresponding RISCV::reg value
RISCV::reg stringToReg(const std::string &s)
{
    static const std::map<std::string, RISCV::reg> regMap = {
        {"x0", RISCV::x0},
        {"x1", RISCV::x1},
        {"x2", RISCV::x2},
        {"x3", RISCV::x3},
        {"x4", RISCV::x4},
        {"x5", RISCV::x5},
        {"x6", RISCV::x6},
        {"x7", RISCV::x7},
        {"x8", RISCV::x8},
        {"x9", RISCV::x9},
        {"x10", RISCV::x10},
        {"x11", RISCV::x11},
        {"x12", RISCV::x12},
        {"x13", RISCV::x13},
        {"x14", RISCV::x14},
        {"x15", RISCV::x15},
        {"x16", RISCV::x16},
        {"x17", RISCV::x17},
        {"x18", RISCV::x18},
        {"x19", RISCV::x19},
        {"x20", RISCV::x20},
        {"x21", RISCV::x21},
        {"x22", RISCV::x22},
        {"x23", RISCV::x23},
        {"x24", RISCV::x24},
        {"x25", RISCV::x25},
        {"x26", RISCV::x26},
        {"x27", RISCV::x27},
        {"x28", RISCV::x28},
        {"x29", RISCV::x29},
        {"x30", RISCV::x30},
        {"x31", RISCV::x31},
    };

    auto it = regMap.find(s);
    if (it != regMap.end())
    {
        return it->second;
    }
    else
    {
        // Handle unknown register
        return RISCV::x31;
    }
}

vector<string> split_string(const string &line)
{
    istringstream iss(line);
    vector<string> words;

    string word;
    while (getline(iss, word, ' '))
    {
        words.push_back(word);
    }

    return words;
}

int main()
{
    // const string file_name = "test.s";
    const string file_path = "..\\simulator\\test.s";
    // cout << file_path << endl;
    ifstream instructions_prog_1(file_path);
    if (!instructions_prog_1.is_open())
    {
        cout << "Error in opening the file" << endl;
        return 0;
    }
    vs lines_prog_1;
    string line_prog_1;
    int address_1 = 0;
    memory m;
    while (getline(instructions_prog_1, line_prog_1))
    {
        lines_prog_1.push_back(line_prog_1);
    }
    instructions_prog_1.close();
    for (int k = 0; k < lines_prog_1.size(); k++)
    {
        vss lines_program_1;
        lines_program_1.push_back(split_string(lines_prog_1[k]));
        // cout<<lines_prog_1[k]<<endl;
        // cout<<lines_program_1[0][1]<<endl;
        int encode[4] = {0, 0, 0, 0}; // used to store intructions in numerical form.
        for (int i = 0; i < lines_program_1[0].size(); i++)
        {
            if (i == 0)
            {
                RISCV::Inst inst;
                string ss = lines_program_1[0][0];
                inst = stringToInst(ss);
                encode[0] = inst;
                // cout << encode[i] << endl;
            }
            else
            {
                RISCV::reg r;
                //  std::cout << "Register " << regToString(r) << " value: " << (sizeof(r)) << std::endl;
                string sss = lines_program_1[0][i];
                r = stringToReg(sss);
                encode[i] = r;
                // cout << encode[i] << endl;
            }
            
        }
        m.write_instruction(address_1, encode, 1);
           address_1++;
    }
    //  vi vvv = m.read_instruction(1,1);
    //         for(auto i:vvv)
    //         {
    //             cout<<i<<endl;
    //         }
    return 0;
}