#include "cores.hpp"
#include "simulator.hpp"
#include "memory.hpp"
// #include "memory.cpp"
#include "registers.hpp"
using namespace std;
ALU::ALU(pair<int, int> &p, int no_inst, memory &m, registers &r1, int core)
{
    pc = p.second;
    //cout << pc << endl;
    while (pc < no_inst)
    {
        // cout << no_inst << endl;
        executeInstruction(m.read_instruction(pc, core), m, r1, core);
        //  pc++;
    };
}
void ALU::executeInstruction(vector<int> instruction, memory &m, registers &r1, int core)
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
        r1.write(rd, ans1 + ans2);
        pc++;
        break;
    }
    case RISCV::sub:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int rs2 = instruction[3];
        r1.write(rd, r1.read(rs1) - r1.read(rs2));
        pc++;
        break;
    }
    case RISCV::addi:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int temp2 = instruction[3];
        // cout<<rd<<" "<<r1.read(rs1)<<" "<<temp2<<endl;
        r1.write(rd, r1.read(rs1) + temp2);
        pc++;
        break;
    }
    case RISCV::jal:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        r1.write(rd, pc + 1);
        pc = rs1;
        break;
    }
    case RISCV::jalr:
    {
        int rd = instruction[1];
        int rs1 = r1.read(instruction[2]);
        int offset = instruction[3];
        r1.write(rd, pc + 1);
      //  cout << rs1 << endl;
        pc = rs1 + offset-1;
      //  cout << pc << endl;
        break;
    }
    case RISCV::beq:
    {
        int rs1 = instruction[1];
        int rs2 = instruction[2];
        int rd = instruction[3];
        if (r1.read(rs1) == r1.read(rs2))
        {
            pc = rd;
            break;
        }
        else
        {
            pc++;
            break;
        }
    }
    case RISCV::bne:
    {
        int rs1 = instruction[1];
        int rs2 = instruction[2];
        int rd = instruction[3];
        if (r1.read(rs1) != r1.read(rs2))
        {
            pc = rd;
            break;
        }
        else
        {
            pc++;
            break;
        }
    }
    case RISCV::blt:
    {
        int rs1 = instruction[1];
        int rs2 = instruction[2];
        int rd = instruction[3];
        if (r1.read(rs1) < r1.read(rs2))
        {
            pc = rd;
            break;
        }
        else
        {
            pc++;
            break;
        }
    }
    case RISCV::bge:
    {
        int rs1 = instruction[1];
        int rs2 = instruction[2];
        int rd = instruction[3];
        if (r1.read(rs1) >= r1.read(rs2))
        {
            pc = rd;
            break;
        }
        else
        {
            pc++;
            break;
        }
    }
    case RISCV::j:
    {
        int rd = instruction[1];
        cout<<rd<<endl;
        pc = rd;
        break;
    }
    case RISCV::sw:
    {
        int rd = instruction[1];
        int rs1 = r1.read(instruction[3]);
        int offset = instruction[2];
        r1.write(rd, m.read_memory(rs1 + offset, core));
        pc++;
        break;
    }
    case RISCV::lw:
    {
        int rs1 = instruction[1];
        int rd = r1.read(instruction[3]);
        int offset = instruction[2];
        m.write_memory(rd + offset, rs1, core);
        pc++;
        break;
    }
    default:
        pc++;
        break;
    }
}