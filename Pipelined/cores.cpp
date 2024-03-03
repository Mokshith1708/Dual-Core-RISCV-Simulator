#include "cores.hpp"
#include "simulator.hpp"
#include "memory.hpp"
#include "registers.hpp"
// Constructor for ALU class
ALU::ALU()
    : fetch(2, std::vector<int>()),
      decode(2, std::vector<int>()),
      execute(2, std::vector<int>()),
      mem(2, std::vector<int>()),
      write(2, std::vector<int>())
{
}
ALU::ALU(std::pair<int, int> &p1, std::pair<int, int> &p2, int no_inst_1, int no_inst_2, memory &m, registers &r1, registers &r2, int core1, int core2)
{
    pc1 = p1.second;
    pc2 = p2.second;

    int maxim = std::max(no_inst_1, no_inst_2);
    std::vector<int>v,k;
    while (pc1 < maxim && pc2 < maxim)
    {
        // std::cout << no_inst << std::endl;
        if (pc1 < no_inst_1)
        {
            // executeInstruction(m.read_instruction(pc1, core1), m, r1, core1, pc1);
            writeBack(k,m,core1, pc1,r1);
            memoryAccess(k,m,core1,pc1);
            k=instructionExecute(v,m,r1,core1,pc1);
            v=instructionDecode(m,core1,r1,pc1);
            instructionFetch(m,core1,pc1);
            clockCycles1++;
            std::cout<<clockCycles1<<std::endl;
        }
        if (pc2 < no_inst_2)
        {
            writeBack(k,m,core2, pc2,r2);
            memoryAccess(k,m,core2,pc2);
            k=instructionExecute(v,m,r2,core2,pc2);
            v=instructionDecode(m,core2,r2,pc2);
            instructionFetch(m,core2,pc2);
            clockCycles2++;
        } //  pc++;
    }
}

void ALU::instructionFetch(memory &m, int core, int& pc)
{   
    // fetch[core - 1].push_back(m.read_instruction(pc, core));
    // pc++;
    std::vector<int> instruction = m.read_instruction(pc, core);
    fetch[core - 1].insert(fetch[core - 1].end(), instruction.begin(), instruction.end());
    pc++;
}

std::vector<int> ALU::instructionDecode(memory &m, int core,registers& r,int& pc)
{   std::vector<int>v;
    if (decode[core - 1].empty())
    { for (auto &temp : fetch[core - 1])
    {
        decode[core - 1].push_back(temp);
    }
    fetch[core - 1].clear();
        return v;
    }
    

   // std::vector<int>v;
    RISCV::Inst opcode = static_cast<RISCV::Inst>(decode[core - 1][0]);
    switch (opcode)
    {
    case RISCV::add:
    {
        int rd = decode[core - 1][1];
        int rs1 = decode[core - 1][2];
        int rs2 = decode[core - 1][3];
        int ans1 = r.read(rs1);
        int ans2 = r.read(rs2);
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(ans1);
        v.push_back(ans2);
       // r.write(rd, ans1 + ans2);
        break;
    }
    case RISCV::sub:
    {
        int rd = decode[core - 1][1];
        int rs1 = decode[core - 1][2];
        int rs2 = decode[core - 1][3];
       // r.write(rd, r.read(rs1) - r.read(rs2));
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(r.read(rs1));
        v.push_back(r.read(rs2));
        break;
    }
    case RISCV::mul:
    {
        int rd = decode[core - 1][1];
        int rs1 = decode[core - 1][2];
        int rs2 = decode[core - 1][3];
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(r.read(rs1));
        v.push_back(r.read(rs2));
       // r.write(rd, r.read(rs1) * r.read(rs2));
        break;
    }
    case RISCV::addi:
    {
        int rd = decode[core - 1][1];
        int rs1 = decode[core - 1][2];
        int temp2 = decode[core - 1][3];
        // std::cout<<rd<<" "<<r.read(rs1)<<" "<<temp2<<std::endl;
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(r.read(rs1));
        v.push_back(temp2);
      //  r.write(rd, r.read(rs1) + temp2);
        break;
    }
    case RISCV::muli:
    {
        int rd = decode[core - 1][1];
        int rs1 = decode[core - 1][2];
        int temp2 = decode[core - 1][3];
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(r.read(rs1));
        v.push_back(temp2);
        // std::cout<<rd<<" "<<r.read(rs1)<<" "<<temp2<<std::endl;
       // r.write(rd, r.read(rs1) * temp2);
        break;
    }
    case RISCV::jal:
    {
        int rd = decode[core - 1][1];
        int rs1 = decode[core - 1][2];
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(rs1);
       // v.push_back();
       // r.write(rd, pc + 1);
       // pc = rs1;
        break;
    }
    case RISCV::jalr:
    {
        int rd = decode[core - 1][1];
        int rs1 = r.read(decode[core - 1][2]);
        int offset = decode[core - 1][3];
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(rs1);
        v.push_back(offset);
      //  r.write(rd, pc + 1);
        // std::cout << rs1 << std::endl;
      //  pc = rs1 + offset - 1;
        // std::cout << pc << std::endl;
        break;
    }
    case RISCV::beq:
    {
        int rs1 = decode[core - 1][1];
        int rs2 = decode[core - 1][2];
        int rd = decode[core - 1][3];
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(r.read(rs1));
        v.push_back(r.read(rs2));
        break;
        // if (r.read(rs1) == r.read(rs2))
        // {
        //     pc = rd;
        //     break;
        // }
        // else
        // {
        //     break;
        // }
    }
    case RISCV::bne:
    {
        int rs1 = decode[core - 1][1];
        int rs2 = decode[core - 1][2];
        int rd = decode[core - 1][3];
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(r.read(rs1));
        v.push_back(r.read(rs2));
        break;
        // if (r.read(rs1) != r.read(rs2))
        // {
        //     pc = rd;
        //     break;
        // }
        // else
        // {
        //     break;
        // }
    }
    case RISCV::blt:
    {
        int rs1 = decode[core - 1][1];
        int rs2 = decode[core - 1][2];
        int rd = decode[core - 1][3];
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(r.read(rs1));
        v.push_back(r.read(rs2));
        // if (r.read(rs1) < r.read(rs2))
        // {
        //     pc = rd;
        //     break;
        // }
        // else
        // {
        //     break;
        // }
        break;
    }
    case RISCV::bge:
    {
        int rs1 = decode[core - 1][1];
        int rs2 = decode[core - 1][2];
        int rd = decode[core - 1][3];
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(r.read(rs1));
        v.push_back(r.read(rs2));
        // if (r.read(rs1) >= r.read(rs2))
        // {
        //     pc = rd;
        //     break;
        // }
        // else
        // {
        //     break;
        // }
        break;
    }
    case RISCV::j:
    {
        int rd = decode[core - 1][1];
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        // std::cout<<rd<<std::endl;
       // pc = rd;
        break;
    }
    case RISCV::lw:
    {
        int rd = decode[core - 1][1];
        int rs1 = r.read(decode[core - 1][3]);
        int offset = decode[core - 1][2];
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(rs1);
        v.push_back(offset);
       // r.write(rd, m.read_memory((rs1 + offset) / 4, core));
        break;
    }
    case RISCV::sw:
    {
        int rs1 = decode[core - 1][1];
        int rd = r.read(decode[core - 1][3]);
        int offset = decode[core - 1][2];
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(r.read(rs1));
        v.push_back(offset);
        //m.write_memory((rd + offset) / 4, r.read(rs1), core);
        break;
    }
    case RISCV::la:
    {
        int rd = decode[core - 1][1];
        std::vector<int> v11 = m.read_instruction(decode[core - 1][2], core);
        int rs1 = v11[1];
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(rs1);
        //r.write(rd, rs1);
        // std::cout << rs1 << std::endl;
        break;
    }
    case RISCV::li:
    {
        int rd = decode[core - 1][1];
        int rs1 = decode[core - 1][2];
        // std::cout << rs1 << " " << rd << std::endl;
        v.push_back(decode[core - 1][0]);
        v.push_back(rd);
        v.push_back(rs1);
        r.write(rd, rs1);
        break;
    }
    case RISCV::ecall:
    {
        int n = r.read(17);
        int k = r.read(10);
        v.push_back(decode[core - 1][0]);
        v.push_back(n);
        v.push_back(k);
        // if (n == 1)
        // {
        //     std::cout << k;
        // }
        // if (n == 4)
        // {
        //     std::string st_temp = m.read_str(k, core);
        //     std::cout << st_temp.substr(1, st_temp.size() - 2);
        // }
        break;
    }
    default:
        break;
    }
    for (auto &temp : fetch[core - 1])
    {
        decode[core - 1].push_back(temp);
    }
    fetch[core - 1].clear();
     return v;
}
std::vector<int> ALU::instructionExecute(std::vector<int>v,memory &m, registers &r, int core, int &pc)
{

    std::vector<int>k;
    if (execute[core - 1].empty())
    {
    execute[core - 1] = decode[core - 1];
    decode[core - 1].clear();
    return k;
    }
    if (execute[core - 1][1] == 0 && execute[core - 1][2] == 0 && execute[core-1][3] == 0)
    {
        return k;
    }
    if (execute[core - 1][0] == -1 || execute[core - 1][0] == -2)
    {
        return k;
    }
    
    // Convert the execute opcode to enum type
    RISCV::Inst opcode = static_cast<RISCV::Inst>(v[0]);
    switch (opcode)
    {
    case RISCV::add:
    {
        int ans = v[2]+v[3];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(ans);
        //r.write(rd, ans1 + ans2);
        break;
    }
    case RISCV::sub:
    {
        int ans = v[2]-v[3];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(ans);
        break;
    }
    case RISCV::mul:
    {
       int ans = v[2]*v[3];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(ans);
        break;
    }
    case RISCV::addi:
    {
        int ans = v[2]+v[3];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(ans);
        break;
    }
    case RISCV::muli:
    {
        int ans = v[2]*v[3];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(ans);
        break;
    }
    case RISCV::jal:
    { 
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(pc+1);
       // r.write(rd, pc + 1);
        pc = v[2];
        break;
    }
    case RISCV::jalr:
    {
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(pc+1);
        pc = v[2] + v[3] - 1;
        break;
    }
    case RISCV::beq:
    {
        int rs1 = v[2];
        int rs2 = v[3];
        int rd = v[1];
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
        int rs1 = v[2];
        int rs2 = v[3];
        int rd = v[1];
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
        int rs1 = v[2];
        int rs2 = v[3];
        int rd = v[1];
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
        int rs1 = v[2];
        int rs2 = v[3];
        int rd = v[1];
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
        int rd = v[1];
        // std::cout<<rd<<std::endl;
        pc = rd;
        break;
    }
    case RISCV::lw:
    {
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(m.read_memory((v[2] + v[3])/4,core));
       // r.write(rd, m.read_memory((rs1 + offset) / 4, core));
        break;
    }
    case RISCV::sw:
    {
        k.push_back(1);
        k.push_back(v[1]+v[3]/4);
        k.push_back(v[2]);
        break;
    }
    case RISCV::la:
    {
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(v[2]);
        // r.write(rd, rs1);
        // std::cout << rs1 << std::endl;
        break;
    }
    case RISCV::li:
    {
        // int rd = execute[core - 1][1];
        // int rs1 = execute[core - 1][2];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(v[2]);
        // std::cout << rs1 << " " << rd << std::endl;
       // r.write(rd, rs1);
        break;
    }
    case RISCV::ecall:
    {
        int n = v[1];
        int k = v[2];
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
    execute[core - 1] = decode[core - 1];
    decode[core - 1].clear();
    return k;
}

void ALU::memoryAccess(std::vector<int>k, memory &m, int core, int &pc)
{
    if (mem[core - 1].empty())
    {   
        mem[core - 1] = execute[core - 1];
        execute[core - 1].clear();
        return;
    }
    if(k[0]==1)
    {
      m.write_memory(k[1],k[2],core);
    }

    mem[core - 1] = execute[core - 1];
    execute[core - 1].clear();
}

void ALU::writeBack(std::vector<int>k,memory &m, int core, int &pc, registers &r)
{
    if (write[core - 1].empty())
    {
       write[core - 1] = mem[core - 1];
       mem[core - 1].clear();
       return;
    }
    if(k[0]==0)
    {
        r.write(k[1],k[2]);
    }
    write[core - 1] = mem[core - 1];
    mem[core - 1].clear();
}