#include "cores.hpp"
#include "simulator.hpp"
#include "memory.hpp"
#include "registers.hpp"
using namespace std;
// Constructor for ALU class
// ALU::ALU()
//     : fetch1(std::vector<int>()),
//       fetch2(std::vector<int>()),
//       decode1(std::vector<int>()),
//       decode2(std::vector<int>()),
//       execute1(std::vector<int>()),
//       execute2(std::vector<int>()),
//       mem1(std::vector<int>()),
//       mem2(std::vector<int>()),
//       write1(std::vector<int>()),
//       write2(std::vector<int>());
// {
// }
ALU::ALU(std::pair<int, int> &p1, std::pair<int, int> &p2, int no_inst_1, int no_inst_2, memory &m, registers &r1, registers &r2, int core1, int core2)
{
    pc1 = p1.second;
    pc2 = p2.second;

    int maxim = std::max(no_inst_1, no_inst_2);
    std::vector<int>v1,k1,k2,v2;
    while (pc1 < maxim+4 && pc2 < maxim+4)
    {
        // std::cout << no_inst << std::endl;
        if (pc1 < no_inst_1+4)
        {
            // executeInstruction(m.read_instruction(pc1, core1), m, r1, core1, pc1);
            for(auto& p:k1)
            {
            cout<<p<<" ";
            }
            cout<<endl;
            cout<<"k1++"<<endl;
            writeBack(k1,m,core1, pc1,r1);
            for(auto& p:write1)
            {
            cout<<p<<" ";
            }
            cout<<endl;
            for(auto& p:k1)
            {
            cout<<p<<" ";
            }
            cout<<endl;
            cout<<"k1"<<endl;
            memoryAccess(k1,m,core1,pc1);
             for(auto& p:mem1)
            {
            cout<<p<<" ";
            }
            cout<<endl;
            cout<<"$$$$"<<endl;
            for(auto& p:v1)
            {
            cout<<p<<" ";
            }
            cout<<endl;
            k1.clear();
            k1=instructionExecute(v1,m,r1,core1,pc1);
             for(auto& p:execute1)
            {
            cout<<p<<" ";
            }
            cout<<endl;
            v1.clear();
            v1=instructionDecode(m,core1,r1,pc1);
             for(auto& p:decode1)
            {
            cout<<p<<" ";
            }
            cout<<endl;
            instructionFetch(m,core1,pc1);
             for(auto& p:fetch1)
            {
            cout<<p<<" ";
            }
            cout<<endl;
            clockCycles1++;
            std::cout<<clockCycles1<<std::endl;
        }
        if (pc2 < no_inst_2+4)
        {
            writeBack(k2,m,core2, pc2,r2);
            memoryAccess(k2,m,core2,pc2);
            k2=instructionExecute(v2,m,r2,core2,pc2);
            v2=instructionDecode(m,core2,r2,pc2);
            instructionFetch(m,core2,pc2);
            clockCycles2++;
          //  std::cout<<clockCycles2<<std::endl;
        } //  pc++;
    }
}

void ALU::instructionFetch(memory &m, int core, int& pc)
{   
    // fetch.push_back(m.read_instruction(pc, core));
    // pc++;
    std::vector<int>& fetch = (core == 1) ? fetch1 : fetch2;
    // std::vector<int>& decode = (core == 1) ? decode1 : decode2;
    fetch.clear();
    std::vector<int> instruction = m.read_instruction(pc, core);
    fetch.insert(fetch.end(), instruction.begin(), instruction.end());
    pc++;
}

std::vector<int> ALU::instructionDecode(memory &m, int core,registers& r,int& pc)
{   std::vector<int>& fetch = (core == 1) ? fetch1 : fetch2;
    std::vector<int>& decode = (core == 1) ? decode1 : decode2;
    std::vector<int>& execute = (core == 1) ? execute1 : execute2;
    std::vector<int>v;
   // cout<<"decode"<<decode.empty()<<endl;
    //cout<<"fetch"<<fetch.empty()<<endl;
    if (decode.empty())
    {
         for (auto &temp : fetch)
    {
        decode.push_back(temp);
    }
    // cout<<"decode1"<<decode.empty()<<endl;
    // cout<<"fetch1"<<fetch.empty()<<endl;
      // fetch.clear();
        
        return v;
    }
    
   // std::vector<int>v;
    RISCV::Inst opcode = static_cast<RISCV::Inst>(decode[0]);
    switch (opcode)
    {
    case RISCV::add:
    {
        int rd = decode[1];
        int rs1 = decode[2];
        int rs2 = decode[3];
        int ans1 = r.read(rs1);
        int ans2 = r.read(rs2);
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(ans1);
        v.push_back(ans2);
       // r.write(rd, ans1 + ans2);
        break;
    }
    case RISCV::sub:
    {
        int rd = decode[1];
        int rs1 = decode[2];
        int rs2 = decode[3];
       // r.write(rd, r.read(rs1) - r.read(rs2));
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(r.read(rs1));
        v.push_back(r.read(rs2));
        break;
    }
    case RISCV::mul:
    {
        int rd = decode[1];
        int rs1 = decode[2];
        int rs2 = decode[3];
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(r.read(rs1));
        v.push_back(r.read(rs2));
       // r.write(rd, r.read(rs1) * r.read(rs2));
        break;
    }
    case RISCV::addi:
    {
        int rd = decode[1];
        int rs1 = decode[2];
        int temp2 = decode[3];
        // std::cout<<rd<<" "<<r.read(rs1)<<" "<<temp2<<std::endl;
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(r.read(rs1));
        v.push_back(temp2);
      //  r.write(rd, r.read(rs1) + temp2);
        break;
    }
    case RISCV::muli:
    {
        int rd = decode[1];
        int rs1 = decode[2];
        int temp2 = decode[3];
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(r.read(rs1));
        v.push_back(temp2);
        // std::cout<<rd<<" "<<r.read(rs1)<<" "<<temp2<<std::endl;
       // r.write(rd, r.read(rs1) * temp2);
        break;
    }
    case RISCV::jal:
    {
        int rd = decode[1];
        int rs1 = decode[2];
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(rs1);
       // v.push_back();
       // r.write(rd, pc + 1);
       // pc = rs1;
        break;
    }
    case RISCV::jalr:
    {
        int rd = decode[1];
        int rs1 = r.read(decode[2]);
        int offset = decode[3];
        v.push_back(decode[0]);
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
        int rs1 = decode[1];
        int rs2 = decode[2];
        int rd = decode[3];
        v.push_back(decode[0]);
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
        int rs1 = decode[1];
        int rs2 = decode[2];
        int rd = decode[3];
        v.push_back(decode[0]);
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
        int rs1 = decode[1];
        int rs2 = decode[2];
        int rd = decode[3];
        v.push_back(decode[0]);
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
        int rs1 = decode[1];
        int rs2 = decode[2];
        int rd = decode[3];
        v.push_back(decode[0]);
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
        int rd = decode[1];
        v.push_back(decode[0]);
        v.push_back(rd);
        // std::cout<<rd<<std::endl;
       // pc = rd;
        break;
    }
    case RISCV::lw:
    {
        int rd = decode[1];
        int rs1 = r.read(decode[3]);
        int offset = decode[2];
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(rs1);
        v.push_back(offset);
       // r.write(rd, m.read_memory((rs1 + offset) / 4, core));
        break;
    }
    case RISCV::sw:
    {
        int rs1 = decode[1];
        int rd = r.read(decode[3]);
        int offset = decode[2];
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(r.read(rs1));
        v.push_back(offset);
        //m.write_memory((rd + offset) / 4, r.read(rs1), core);
        break;
    }
    case RISCV::la:
    {
        int rd = decode[1];
        std::vector<int> v11 = m.read_instruction(decode[2], core);
        int rs1 = v11[1];
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(rs1);
        //r.write(rd, rs1);
        // std::cout << rs1 << std::endl;
        break;
    }
    case RISCV::li:
    {
        int rd = decode[1];
        int rs1 = decode[2];
        // std::cout << rs1 << " " << rd << std::endl;
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(rs1);
        r.write(rd, rs1);
        break;
    }
    case RISCV::ecall:
    {
        int n = r.read(17);
        int k = r.read(10);
        v.push_back(decode[0]);
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
    decode.clear();
    for (auto &temp : fetch)
    {
        decode.push_back(temp);
    }
   // fetch.clear();
    // for(int i=0;i<v.size();i++)
    // {
    //     std::cout<<v[i]<<" ";
    // }
     return v;
}
std::vector<int> ALU::instructionExecute(std::vector<int>v,memory &m, registers &r, int core, int &pc)
{
    std::vector<int>& execute = (core == 1) ? execute1 : execute2;
    std::vector<int>& decode = (core == 1) ? decode1 : decode2;
    std::vector<int> k;
    if (execute.empty())
    {
    execute = decode;
   // decode.clear();
    return k;
    }
    if(v.empty())
    {
        execute.clear();
        execute = decode;
        return k;
    }
    if (execute[1] == 0 && execute[2] == 0 && execute[3] == 0)
    {
        return k;
    }
    if (execute[0] == -1 || execute[0] == -2)
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
        // int rd = execute[1];
        // int rs1 = execute[2];
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
    execute.clear();
    execute = decode;
   // decode.clear();
    return k;
}

void ALU::memoryAccess(std::vector<int>k, memory &m, int core, int &pc)
{
    std::vector<int>& execute = (core == 1) ? execute1 : execute2;
    std::vector<int>& mem = (core == 1) ? mem1 : mem2;
    if (mem.empty())
    {   
        mem = execute;
     //   execute.clear();
        return;
    }
    if(k.empty())
    {
        mem.clear();
    mem = execute;
    return;
    }
    if(k[0]==1)
    {
      m.write_memory(k[1],k[2],core);
    }
    mem.clear();
    mem = execute;
   // execute.clear();
}

void ALU::writeBack(std::vector<int>k,memory &m, int core, int &pc, registers &r)
{
    std::vector<int>& write = (core == 1) ? write1 : write2;
    std::vector<int>& mem = (core == 1) ? mem1 : mem2;
    if (write.empty())
    {
       write = mem;
      // mem.clear();
       return;
    }
    if(k.empty())
    {
    write.clear();
    write = mem;
    return;
    }
    if(k[0]==0)
    {
        r.write(k[1],k[2]);
    }
    write.clear();
    write = mem;
   // mem.clear();
}