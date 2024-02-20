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

// First Pass: Collect Labels and Addresses
void collectLabels(const vs &lines, map<string, int> &labelMap)
{
    int address = 0;
    for (const string &line : lines)
    {
        vs words = split_string(line);
        if (!words.empty() && words[0].back() == ':')
        {
            // Found a label
            string label = words[0].substr(0, words[0].size() - 1);
            labelMap[label] = address;
            address++;
        }
        else
        {
            // Increment address for non-label lines
            address++;
        }
    }
}

// Second Pass: Generate Machine Code
void generateMachineCode(vs &lines, map<string, int> &labelMap, memory &m)
{
    int address = 0;
    for (const string &line : lines)
    {
        vs words = split_string(line);
        int encode[4] = {0, 0, 0, 0}; // Used to store instructions in numerical form.
        if (!words.empty() && words[0].back() == ':')
        {
            string label = words[0].substr(0, words[0].size() - 1);
            encode[0] = labelMap[label];
            m.write_instruction(address, encode, 1);
            address++;
            continue;
        }

        for (int i = 0; i < (int)words.size(); i++)
        {
            if (i == 0)
            {
                // Handle instruction
                switch (stringToInst(words[i]))
                {
                case RISCV::beq:
                case RISCV::bne:
                case RISCV::blt:
                case RISCV::bge:
                case RISCV::bltu:
                case RISCV::begu:
                    // Label cases
                    encode[0] = stringToInst(words[i]);
                    encode[1] = stringToReg(words[i + 1]);
                    encode[2] = stringToReg(words[i + 2]);
                    if (labelMap.find(words[i + 3]) != labelMap.end())
                    {
                        encode[3] = labelMap[words[i + 3]] + 1;
                    }
                    else
                    {
                        throw std::invalid_argument("Label not found: " + words[i + 3]);
                    }
                    i += 3; // Skip label and operands
                    break;
                case RISCV::j:
                    encode[0] = stringToInst(words[i]);
                    if (labelMap.find(words[i + 1]) != labelMap.end())
                    {
                        encode[1] = labelMap[words[i + 1]] + 1;
                    }
                    else
                    {
                        throw std::invalid_argument("Label not found: " + words[i + 1]);
                    }
                    i += 3;
                    break;
                case RISCV::jal:
                    encode[0] = stringToInst(words[i]);
                    encode[1] = stringToReg(words[i + 1]);
                    if (labelMap.find(words[i + 2]) != labelMap.end())
                    {
                        encode[2] = labelMap[words[i + 2]] + 1;
                    }
                    else
                    {
                        throw std::invalid_argument("Label not found: " + words[i + 2]);
                    }
                    i += 3; // Skip label and operands
                    break;
                case RISCV::jalr:
                    // Label cases
                    encode[0] = stringToInst(words[i]);
                    encode[1] = stringToReg(words[i + 1]);
                    encode[2] = stringToReg(words[i + 2]);
                    encode[3] = stoi(words[i + 3]);
                    i += 3; // Skip label and operands
                    break;
                case RISCV::ecall:
                    encode[i] = stringToInst(words[i]);
                    i += 3;
                    break;
                case RISCV::lw:
                case RISCV::sw:
                case RISCV::lwu:
                case RISCV::lbu:
                case RISCV::lb:
                case RISCV::sb:
                    encode[0] = stringToInst(words[i]);
                    encode[1] = stringToReg(words[i + 1]);
                    encode[2] = stoi(words[2].substr(0, words[2].find('(')));
                    encode[3] = stringToReg(words[2].substr(words[2].find('(') + 1, words[2].find(')') - words[2].find('(') - 1));
                    i = i + 3;
                    break;
                default:
                    // Handle non-branch instructions
                    encode[0] = stringToInst(words[i]);
                    break;
                }
            }
            else
            {
                // Handle registers
                RISCV::reg r = stringToReg(words[i]);
                encode[i] = r;
            }
        }

        m.write_instruction(address, encode, 1);
        address++;
    }
}

int main()
{
    const string file_path = "..\\simulator\\test.s";
    ifstream instructions_prog_1(file_path);
    if (!instructions_prog_1.is_open())
    {
        cout << "Error in opening the file" << endl;
        return 0;
    }

    vs lines_prog_1;
    string line_prog_1;
    while (getline(instructions_prog_1, line_prog_1))
    {
        lines_prog_1.push_back(line_prog_1);
    }
    instructions_prog_1.close();

    map<string, int> labelMap;
    collectLabels(lines_prog_1, labelMap);

    memory m;

    // Second pass to generate machine code
    generateMachineCode(lines_prog_1, labelMap, m);
    vi vvv = m.read_instruction(5, 1);
    for (auto i : vvv)
    {
        cout << i << endl;
    }
    return 0;
}
