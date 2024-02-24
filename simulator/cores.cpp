#include "cores.hpp"
#include "simulator.hpp"
#include "memory.hpp"
#include "registers.hpp"
using namespace std;
ALU::ALU(pair<int, int> &p1,pair<int, int> &p2, int no_inst_1,int no_inst_2,memory &m1,memory &m2, registers &r1,registers &r2, int core1, int core2)
{
    pc1 = p1.second;
    pc2 = p2.second;
    //cout << pc << endl;
    int maxim = max(no_inst_1,no_inst_2);
    while (pc1<maxim || pc2<maxim)
    {
        // cout << no_inst << endl;
        if(pc1<no_inst_1)
        {
            executeInstruction(m1.read_instruction(pc1, core1), m1, r1, core1,pc1);
        }
        if(pc2<no_inst_2)
        {
        executeInstruction(m2.read_instruction(pc2, core2), m2, r2, core2,pc2);
        }//  pc++;
    };
}
void ALU::executeInstruction(vector<int> instruction, memory &m, registers &r, int core,int &pc)
{
    RISCV::Inst opcode = static_cast<RISCV::Inst>(instruction[0]);
    switch (opcode)
    {
    case RISCV::add:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int rs2 = instruction[3];
        int ans1 = r.read(rs1);
        int ans2 = r.read(rs2);
        r.write(rd, ans1 + ans2);
        pc++;
        break;
    }
    case RISCV::sub:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int rs2 = instruction[3];
        r.write(rd, r.read(rs1) - r.read(rs2));
        pc++;
        break;
    }
    case RISCV::addi:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int temp2 = instruction[3];
        // cout<<rd<<" "<<r.read(rs1)<<" "<<temp2<<endl;
        r.write(rd, r.read(rs1) + temp2);
        pc++;
        break;
    }
    case RISCV::jal:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        r.write(rd, pc + 1);
        pc = rs1;
        break;
    }
    case RISCV::jalr:
    {
        int rd = instruction[1];
        int rs1 = r.read(instruction[2]);
        int offset = instruction[3];
        r.write(rd, pc + 1);
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
        if (r.read(rs1) == r.read(rs2))
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
        if (r.read(rs1) != r.read(rs2))
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
        if (r.read(rs1) < r.read(rs2))
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
        if (r.read(rs1) >= r.read(rs2))
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
    case RISCV::lw:
    {
        int rd = instruction[1];
        int rs1 = r.read(instruction[3]);
        int offset = instruction[2];
        r.write(rd, m.read_memory(rs1 + offset, core));
        pc++;
        break;
    }
    case RISCV::sw:
    {
        int rs1 = instruction[1];
        int rd = r.read(instruction[3]);
        int offset = instruction[2];
        m.write_memory(rd + offset, r.read(rs1), core);
        pc++;
        break;
    }
    default:
        pc++;
        break;
    }
}