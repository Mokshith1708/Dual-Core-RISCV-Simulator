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
using namespace std;
using vs = vector<string>;
using vss = vector<vector<string>>;
using vi = vector<int>;

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
    cout << file_path << endl;
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
        vss program_1;
        program_1.push_back(split_string(lines_prog_1[0]));
        int encode[4] = {0, 0, 0, 0}; // used to store intructions in numerical form.
        for (int i = 0; i < program_1[0].size(); i++)
        {
            if (i == 0)
            {
                RISCV::Inst inst = static_cast<RISCV::Inst>(i);
                encode[0] = inst;
                cout << encode[i] << endl;
            }
            else
            {
                RISCV::reg r = static_cast<RISCV::reg>(i);
                //  std::cout << "Register " << regToString(r) << " value: " << (sizeof(r)) << std::endl;
                encode[i] = r;
                cout << encode[i] << endl;
            }
            m.write_instruction(address_1, encode, 1);
        }
    }
}