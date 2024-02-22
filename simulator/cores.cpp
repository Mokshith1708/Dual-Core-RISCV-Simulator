#include "cores.hpp"
#include "simulator.hpp"
// #include "memory.h"
// #include "memory.cpp"
 #include "registers.hpp"
ALU::ALU(pair<int, int> &p, int no_inst, memory &m, registers &r1, int core)
{
    pc = p.second;
    while (pc < no_inst)
    {
        executeInstruction(m.read_instruction(pc, core), m, r1);
        pc++;
    };
}
// int ALU::registerfetch(int regValue,registers &r1)
// {
//     switch(regValue){
//     case 0:

//     }
//}
// int ALU::countLines(const string& filename) {
//     ifstream file(filename);
//     if (!file.is_open()) {
//         cerr << "Error opening file: " << filename << endl;
//         return -1;
//     }
//     int lineCount = count(istreambuf_iterator<char>(file), istreambuf_iterator<char>(), '\n');
//     file.close();
//     return lineCount;
// }
void ALU::executeInstruction(vector<int> instruction, memory &m, registers &r1)
{
    RISCV::Inst opcode = static_cast<RISCV::Inst>(instruction[0]);
    switch (opcode)
    {
    case RISCV::add:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int rs2 = instruction[3];
        int ans1 = r1.read(rs1);
        int ans2 = r1.read(rs2);
        r1.write(rd, ans1+ans2);
        break;
    }
    case RISCV::sub:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int rs2 = instruction[3];
        r1.write(rd, r1.read(rs1) - r1.read(rs2));
        break;
    }
    case RISCV::addi:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int temp2 = instruction[3];
       // cout<<rd<<" "<<r1.read(rs1)<<" "<<temp2<<endl;
        r1.write(rd, r1.read(rs1) + temp2);
    }

    default:
        break;
    }
}