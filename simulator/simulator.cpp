#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstdarg>
#include "simulator.hpp"
#include <sstream>

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
    const string file_path = "..\\data_files\\test.s";
    // cout << file_path << endl;
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
    vss program_1;
    program_1.push_back(split_string(lines_prog_1[0]));
    int encode[4]={0,0,0,0}; // used to store intructions in numerical form.
    for (int i = 0; i < program_1[0].size(); i++)
    {
        RISCV::reg r = static_cast<RISCV::reg>(i);
        //  std::cout << "Register " << regToString(r) << " value: " << (sizeof(r)) << std::endl;
        encode[i]=r;
        cout<<encode[i]<<endl;
    }
}