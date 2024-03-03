#include "cores.hpp"
#include "simulator.hpp"
#include "memory.hpp"
#include "registers.hpp"
// Constructor for ALU class
ALU::ALU(std::pair<int, int> &p1, std::pair<int, int> &p2, int no_inst_1, int no_inst_2, memory &m, registers &r1, registers &r2, int core1, int core2)
{
    pc1 = p1.second;
    pc2 = p2.second;

    int maxim = std::max(no_inst_1, no_inst_2);
    while (pc1 < maxim && pc2 < maxim)
    {
        // std::cout << no_inst << std::endl;
        if (pc1 < no_inst_1)
        {
            //executeInstruction(m.read_instruction(pc1, core1), m, r1, core1, pc1);
            writeBack(m,core1, pc1);
        }
        if (pc2 < no_inst_2)
        {
            executeInstruction(m.read_instruction(pc2, core2), m, r2, core2, pc2);
        } //  pc++;
    }
}

void ALU::instructionFetch(memory &m, int core, int &pc)
{    
    fetch[core-1].push_back(m.read_instruction(pc, core));
    pc++;
}

void ALU::instructionDecode(memory &m, int core)
{
    if (fetch[core-1].empty())
    {
        return;
    }
    for (const auto &temp : fetch[core - 1])
    {
        decode[core - 1].push_back(temp);
    }
    fetch[core - 1].clear();
}
void ALU::instructionExecute(memory &m, registers &r, int core, int &pc)
{
    if (execute[core-1].empty())
    {
        return;
    }
    if (execute[core-1][1] == 0 && execute[core-1][2] == 0 && execute[3] == 0)
    {
        return;
    }
    if (execute[0] == -1 || execute[0] == -2)
    {
        return;
    }
    // Convert the execute opcode to enum type
    RISCV::Inst opcode = static_cast<RISCV::Inst>(execute[core-1][0]);
    switch (opcode)
    {
    case RISCV::add:
    {
        int rd = execute[core-1][1];
        int rs1 = execute[core-1][2];
        int rs2 = execute[core-1][3];
        int ans1 = r.read(rs1);
        int ans2 = r.read(rs2);
        r.write(rd, ans1 + ans2);
        break;
    }
    case RISCV::sub:
    {
        int rd = execute[1];
        int rs1 = execute[2];
        int rs2 = execute[3];
        r.write(rd, r.read(rs1) - r.read(rs2));
        break;
    }
    case RISCV::mul:
    {
        int rd = execute[1];
        int rs1 = execute[2];
        int rs2 = execute[3];
        r.write(rd, r.read(rs1) * r.read(rs2));
        break;
    }
    case RISCV::addi:
    {
        int rd = execute[1];
        int rs1 = execute[2];
        int temp2 = execute[3];
        // std::cout<<rd<<" "<<r.read(rs1)<<" "<<temp2<<std::endl;
        r.write(rd, r.read(rs1) + temp2);
        break;
    }
    case RISCV::muli:
    {
        int rd = execute[1];
        int rs1 = execute[2];
        int temp2 = execute[3];
        // std::cout<<rd<<" "<<r.read(rs1)<<" "<<temp2<<std::endl;
        r.write(rd, r.read(rs1) * temp2);
        break;
    }
    case RISCV::jal:
    {
        int rd = execute[1];
        int rs1 = execute[2];
        r.write(rd, pc + 1);
        pc = rs1;
        break;
    }
    case RISCV::jalr:
    {
        int rd = execute[1];
        int rs1 = r.read(execute[2]);
        int offset = execute[3];
        r.write(rd, pc + 1);
        // std::cout << rs1 << std::endl;
        pc = rs1 + offset - 1;
        // std::cout << pc << std::endl;
        break;
    }
    case RISCV::beq:
    {
        int rs1 = execute[1];
        int rs2 = execute[2];
        int rd = execute[3];
        if (r.read(rs1) == r.read(rs2))
        {
            pc = rd;
            break;
        }
        else
        {
            break;
        }
    }
    case RISCV::bne:
    {
        int rs1 = execute[1];
        int rs2 = execute[2];
        int rd = execute[3];
        if (r.read(rs1) != r.read(rs2))
        {
            pc = rd;
            break;
        }
        else
        {
            break;
        }
    }
    case RISCV::blt:
    {
        int rs1 = execute[1];
        int rs2 = execute[2];
        int rd = execute[3];
        if (r.read(rs1) < r.read(rs2))
        {
            pc = rd;
            break;
        }
        else
        {
            break;
        }
    }
    case RISCV::bge:
    {
        int rs1 = execute[1];
        int rs2 = execute[2];
        int rd = execute[3];
        if (r.read(rs1) >= r.read(rs2))
        {
            pc = rd;
            break;
        }
        else
        {
            break;
        }
    }
    case RISCV::j:
    {
        int rd = execute[1];
        // std::cout<<rd<<std::endl;
        pc = rd;
        break;
    }
    case RISCV::lw:
    {
        int rd = execute[1];
        int rs1 = r.read(execute[3]);
        int offset = execute[2];
        r.write(rd, m.read_memory((rs1 + offset) / 4, core));
        break;
    }
    case RISCV::sw:
    {
        int rs1 = execute[1];
        int rd = r.read(execute[3]);
        int offset = execute[2];
        m.write_memory((rd + offset) / 4, r.read(rs1), core);
        break;
    }
    case RISCV::la:
    {
        int rd = execute[1];
        std::vector<int> v = m.read_instruction(execute[2], core);
        int rs1 = v[1];
        r.write(rd, rs1);
        // std::cout << rs1 << std::endl;
        break;
    }
    case RISCV::li:
    {
        int rd = execute[1];
        int rs1 = execute[2];
        // std::cout << rs1 << " " << rd << std::endl;
        r.write(rd, rs1);
        break;
    }
    case RISCV::ecall:
    {
        int n = r.read(17);
        int k = r.read(10);
        if (n == 1)
        {
            std::cout << k;
        }
        if (n == 4)
        {
            std::string st_temp = m.read_str(k, core);
            std::cout << st_temp.substr(1, st_temp.size() - 2);
        }
        break;
    }
    default:
        break;
    }
    execute = decode;
    decode.clear();
}

void ALU::memoryAccess(memory& m, int core, int& pc)
{
    if (mem.empty())
    {
        return;
    }
    mem=execute;
    execute.clear();
}

void ALU::writeBack(memory& m, int core, int& pc)
{
    if(write.empty())
    {
        return;
    }
    write = mem;
    mem.clear();
}