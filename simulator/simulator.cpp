#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstdarg>
#include "simulator.hpp"
#include <sstream>
#include <cctype>
#include "memory.hpp"
#include "memory.cpp"
#include "cores.cpp"
#include "cores.hpp"
#include <bits/stdc++.h>
#include "registers.hpp"
#include "registers.cpp"
using namespace std;
using vs = vector<string>;
using vss = vector<vector<string>>;
using vi = vector<int>;

RISCV::Inst stringToInst(const string &s)
{
    static const map<string, RISCV::Inst> instMap = {
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
        {"j", RISCV::j},
        {"la", RISCV::la},
        {"li", RISCV::li},
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
RISCV::reg stringToReg(const string &s)
{
    static const map<string, RISCV::reg> regMap = {
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
void collectLabels(vs &lines, map<string, int> &labelMap, map<string, int> &dataSizes, pair<int, int> &p)
{
    int address = 0;
    bool inDataSection = false;
    for (string &line : lines)
    {      if(line.empty())
            {
                address++;
                continue;
            }
        vs words = split_string(line);
        if (!words.empty() && words[0] == ".data")
        {
            inDataSection = true;
            p.first = address;
            address++;
            continue;
        }
        if (!words.empty() && words[0] == ".text")
        {
            inDataSection = false;
            p.second = address;
            address++;
            continue;
        }
        if (inDataSection)
        {
            if (!words.empty() && words[0].back() == ':')
            {
                string label = words[0].substr(0, words[0].size() - 1);
                labelMap[label] = address;
                dataSizes[label] = words.size() - 2;
               // cout<<dataSizes[label]<<endl;
                address++;
            }
           
            // else if (!words.empty() && words[0] == ".text")
            // {
            //     // End of .data
            //     inDataSection = false;
            //     p.second = address;
            //     address++;
            // }
        }
        else
        {
            if (!words.empty() && words[0].back() == ':')
            {
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
}

// Second Pass: Generate Machine Code
int generateMachineCode(vs &lines, map<string, int> &labelMap, memory &m, pair<int, int> &p,int core)
{
    int address = 0;
    int address_str = 0;
    int address_memory = 0;
    for (string &line : lines)
    {
        vs words = split_string(line);
        int encode[4] = {0, 0, 0, 0};
        if(line.empty() || line[0]=='#')
        {    
            m.write_instruction(address, encode, core);
            address++;
            continue;
        }
        if (address >= p.first && address < p.second)
        {
            if (words[0] == ".data")
            {
                m.write_instruction(address, encode, core);
                address++;
                continue;
            }
            if (!words.empty() && words[0].back() == ':')
            {
                if (words[1] == ".word")
                {
                    string label = words[0].substr(0, words[0].size() - 1);
                    encode[1] = address_memory;
                    encode[2] = address_memory + words.size() - 3;
                    encode[0] = -2;
                    for (int j = 2; j < words.size(); j++)
                    {
                        m.write_memory(address_memory, stoi(words[j]), core);
                        address_memory++;
                    }
                    m.write_instruction(address, encode, core);
                    address++;
                    continue;
                }
                else if (words[1] == ".string")
                {
                    string str;
                    for (size_t j = 2; j< words.size(); ++j)
                    {
                        str += words[j];
                        if (j != words.size() - 1)
                            str += " ";
                    }
                    m.write_str(words[0].substr(0, words[0].size() - 1), str, address_str, 1);
                    encode[0] = -1;
                    encode[1] = address_str;
                    address_str++;
                    m.write_instruction(address, encode, core);
                    address++;
                    continue;
                }
            }
        }
        if (address >= p.second)
        {

            // Used to store instructions in numerical form.
            if (words[0] == ".text")
            {
                m.write_instruction(address, encode, core);
                address++;
                continue;
            }
            if (!words.empty() && words[0].back() == ':')
            {
                string label = words[0].substr(0, words[0].size() - 1);
                encode[0] = labelMap[label];
                m.write_instruction(address, encode, core);
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
                    case RISCV::la:
                         encode[0] = stringToInst(words[i]);
                         encode[1] = stringToReg(words[i+1]);
                         encode[2] = labelMap[words[i+2]];
                        i += 3; // Skip label and operands
                        break;
                    case RISCV::li:
                         encode[0] = stringToInst(words[i]);
                         encode[1] = stringToReg(words[i+1]);
                         encode[2] = stoi(words[i+2]);
                        i += 3; // Skip label and operands
                        break;
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
                            throw invalid_argument("Label not found: " + words[i + 3]);
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
                            throw invalid_argument("Label not found: " + words[i + 1]);
                        }
                        // cout<<encode[0]<<" "<<encode[1]<<endl;
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
                            throw invalid_argument("Label not found: " + words[i + 2]);
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
                    case RISCV::addi:
                        encode[0] = stringToInst(words[i]);
                        encode[1] = stringToReg(words[i + 1]);
                        encode[2] = stringToReg(words[i + 2]);
                        encode[3] = stoi(words[i + 3]);
                        i += 3;
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
        }

        m.write_instruction(address, encode, core);
        address++;
    }
    return address;
}

// void printRegisterTable(int registers[]) {
//     std::cout << "Register Table:" << std::endl;
//     std::cout << "===============" << std::endl;
//     for (int i = 0; i < 32; ++i) {
//         std::cout << "x" << i << ": " << registers[i] << std::endl;
//     }
//     std::cout << "===============" << std::endl;
// }

int main()
{   map<string, int> labelMap_1,labelMap_2;
    map<string, int> dataSizes_1,dataSizes_2;
    pair<int, int> p1,p2;
    registers r1, r2;
    memory m;


    // for first file.
    const string file_path = "..\\simulator\\test.s";
    ifstream instructions_prog_1(file_path);
    if (!instructions_prog_1.is_open())
    {
        cout << "Error in opening the file 1" << endl;
        return 0;
    }
    vs lines_prog_1;
    string line_prog_1;
    while (getline(instructions_prog_1, line_prog_1))
    {
        lines_prog_1.push_back(line_prog_1);
    }

    instructions_prog_1.close();
    collectLabels(lines_prog_1, labelMap_1, dataSizes_1, p1);
    int no_inst_1 = generateMachineCode(lines_prog_1, labelMap_1, m, p1,1);


    // for second file
    const string file_path_2 = "..\\simulator\\test_2.s";
    ifstream instructions_prog_2(file_path_2);
    if (!instructions_prog_2.is_open())
    {
        cout << "Error in opening the file 2" << endl;
        return 0;
    }

    vs lines_prog_2;
    string line_prog_2;
    while (getline(instructions_prog_2, line_prog_2))
    {
        lines_prog_2.push_back(line_prog_2);
    }

    instructions_prog_2.close();
    collectLabels(lines_prog_2, labelMap_2, dataSizes_2, p2);
    int no_inst_2 = generateMachineCode(lines_prog_2, labelMap_2, m, p2,2);

/// #### ///

    ALU alui(p1,p2,no_inst_1,no_inst_2, m, r1,r2, 1,2);

    std::cout << "Register Table:" << std::endl;
    std::cout << "===============" << std::endl;
    for (int i = 0; i < 32; ++i)
    {
        std::cout << "x" << i << ": " << r1.read(i) << std::endl;
    }
    std::cout << "===============" << std::endl;
    // for (const auto& pair : labelMap) {
    //     std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    // } // labels
    std::cout << "==============" << std::endl;
    for (int i = 0; i < 30; ++i)
    {
        std::cout << "Address " << i << ": " << m.read_memory(i, 1) << std::endl;
    }
    std::cout << "==============" << std::endl;
    std::cout << "String Map:" << std::endl;
    std::cout << "===========" << std::endl;
    for (const auto& pair : m.strmap_1)
    {
        std::cout << "Key: " << pair.first.first << ", Value: " << pair.first.second << ", Address: " << pair.second << std::endl;
    }
    std::cout << "===========" << std::endl;




     std::cout << "Register Table 2 :" << std::endl;
    std::cout << "===============" << std::endl;
    for (int i = 0; i < 32; ++i)
    {
        std::cout << "x" << i << ": " << r2.read(i) << std::endl;
    }
    std::cout << "===============" << std::endl;
    // for (const auto& pair : labelMap) {
    //     std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    // } // labels
    std::cout << "==============" << std::endl;
    for (int i = 0; i < 30; ++i)
    {
        std::cout << "Address " << i << ": " << m.read_memory(i, 2) << std::endl;
    }
    std::cout << "==============" << std::endl;
    std::cout << "String Map:" << std::endl;
    std::cout << "===========" << std::endl;
    for (const auto& pair : m.strmap_2)
    {
        std::cout << "Key: " << pair.first.first << ", Value: " << pair.first.second << ", Address: " << pair.second << std::endl;
    }
    std::cout << "===========" << std::endl;

    for(int i=0;i<30;i++)
    {
        cout<<m.instructions_1[i][0]<<" "<<m.instructions_1[i][1]<<" "<<m.instructions_1[i][2]<<" "<<m.instructions_1[i][3]<<" "<<endl;
    }


    return 0;
}