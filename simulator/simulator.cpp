#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstdarg>
#include "simulator.hpp"
//#include "D:\IITTP\Risc_V_Dual_core_simulator\Dual-Core-RISCV-Simulator\data_files\test.s"
using namespace std;
using vs = vector<string>;
using vi = vector<int>;

 
int main ()
{
   // const string file_name = "test.s";
    const string file_path = "D:\IITTP\Risc_V_Dual_core_simulator\Dual-Core-RISCV-Simulator\data_files\test.s" ;
    cout<<file_path<<endl;
    ifstream instructions(file_path);
    if(!instructions.is_open())
    {
        cout<<"Error in opening the file"<<endl;
        return 0;
    }
    vs lines;
    string line;
    while(getline(instructions,line))
    {
        lines.push_back(line);
    }
    instructions.close();
    for(string s :lines) 
    {
      cout<<s<<endl;
    } 
}