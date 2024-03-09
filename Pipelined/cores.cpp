#include "cores.hpp"
#include "simulator.hpp"
#include "memory.hpp"
#include "registers.hpp"
using namespace std;
ALU::ALU(std::pair<int, int> &p1, std::pair<int, int> &p2, int no_inst_11, int no_inst_22, memory &m, registers &r1, registers &r2, int core1, int core2)
{
    pc1 = p1.second + 1;
    pc2 = p2.second + 1;
    // labelMap_1 = labelMap_11;
    // labelMap_2 = labelMap_22;
    std::vector<int> tempReg1(32);
    std::vector<int> tempReg2(32);
    no_inst_1 = no_inst_11;
    no_inst_2 = no_inst_22;
    // prevpc1[0]=pc1;
    // prevpc2[0]=pc2;
    // prevpc1[1]=pc1;
    // prevpc2[1]=pc2;
    // cout<<no_inst_2<<endl;
    int maxim = std::max(no_inst_1, no_inst_2);
    std::vector<int> v1, k1, k2, v2;
    //  cout<<no_inst_11<<endl;
    while (pc1 < maxim + 4 && pc2 < maxim + 4)
    {
        // std::cout << no_inst << std::endl;
        if (pc1 < no_inst_1 + 4)
        {
            // executeInstruction(m.read_instruction(pc1, core1), m, r1, core1, pc1);
            // for(auto& p:k1)
            // {
            // cout<<p<<" ";
            // }
            // cout<<endl;
            // cout<<"k1++"<<endl;
            writeBack(k1, m, core1, pc1, r1);
            // for(auto& p:write1)
            // {
            // cout<<p<<" ";
            // }
            // cout<<endl;
            // for(auto& p:k1)
            // {
            // cout<<p<<" ";
            // }
            // cout<<endl;
            // cout<<"k1"<<endl;
            memoryAccess(k1, m, core1, pc1);
            //  for(auto& p:mem1)
            // {
            // cout<<p<<" ";
            // }
            // cout<<endl;
            // cout<<"$$$$"<<endl;
            // for(auto& p:v1)
            // {
            // cout<<p<<" ";
            // }
            // cout<<endl;
            // k1.clear();
            k1 = instructionExecute(v1, m, r1, core1, pc1, tempReg1);
            //  for(auto& p:execute1)
            // {
            // cout<<p<<" ";
            // }
            // cout<<endl;
            // v1.clear();
            v1 = instructionDecode(m, core1, r1, pc1, tempReg1);
            //  for(auto& p:decode1)
            // {
            // cout<<p<<" ";
            // }
            // cout<<endl;
            instructionFetch(m, core1, pc1, r1, tempReg1);
            //  for(auto& p:fetch1)
            // {
            // cout<<p<<" ";
            // }
            // cout<<endl;
            clockCycles1++;
            // std::cout<<clockCycles1<<std::endl;
        }
        if (pc2 < no_inst_2 + 4)
        { // executeInstruction(m.read_instruction(pc1, core1), m, r1, core1, pc1);
            cout << "------------start-----------" << endl;
            for (auto &p : k2)
            {
                cout << p << " ";
            }
            cout << endl;
            cout << "k2++" << endl;

            writeBack(k2, m, core2, pc2, r2);

            for (auto &p : write2)
            {
                cout << p << " ";
            }
            cout << endl;
            for (auto &p : k2)
            {
                cout << p << " ";
            }
            cout << endl;
            cout << "k2" << endl;

            memoryAccess(k2, m, core2, pc2);

            for (auto &p : mem2)
            {
                cout << p << " ";
            }
            cout << endl;
            cout << "$$$$" << endl;
            for (auto &p : v2)
            {
                cout << p << " ";
            }
            cout << endl;
            k2.clear();

            k2 = instructionExecute(v2, m, r2, core2, pc2, tempReg2);

            for (auto &p : execute2)
            {
                cout << p << " ";
            }
            cout << endl;
            v2.clear();

            v2 = instructionDecode(m, core2, r2, pc2, tempReg2);

            for (auto &p : decode2)
            {
                cout << p << " ";
            }
            cout << endl;

            instructionFetch(m, core2, pc2, r2, tempReg2);

            for (auto &p : fetch2)
            {
                cout << p << " ";
            }
            cout << endl;

            clockCycles2++;
            std::cout << clockCycles2 << std::endl;
        } //  pc++;
    }
}

void ALU::instructionFetch(memory &m, int core, int &pc, registers &r, std::vector<int> &tempReg)
{
    // fetch.push_back(m.read_instruction(pc, core));
    // pc++;
    std::vector<int> &fetch = (core == 1) ? fetch1 : fetch2;
    int no_inst = (core == 1) ? no_inst_1 : no_inst_2;
    std::vector<int> &prevpc = (core == 1) ? prevpc1 : prevpc2;
    // std::vector<int>& decode = (core == 1) ? decode1 : decode2;
    fetch.clear();
    std::vector<int> instruction(4, 0);
    // prevpc[1]=prevpc[0];
    // prevpc[0]=pc-1;

    cout << ggg << "opp" << ggg1 << endl;
    if (pc < no_inst)
    {
        instruction.clear();
        instruction = m.read_instruction(pc, core);
        while (instruction[1] == -101)
        {
            if (pc + 1 > no_inst)
            {
                break;
            }
            instruction.clear();
            pc++;
            instruction = m.read_instruction(pc, core);
        }
    }
    int tempry = pc;
    pc++;

    if (dataforwarding1)
    {
        if (instruction[1] != 0 || instruction[2] != 0 || instruction[3] != 0)
        {
            if (instruction[0] == 20)
            {
                pc = instruction[1];
                cout << pc << "~~~~~~~~~~~~20" << endl;
            }
            else if (instruction[0] == 3)
            {
                int rs1 = instruction[1];
                int rs2 = instruction[2];
                int rd = instruction[3];
                //    if(m.read_instruction(ggg1, core)[0]==11 || m.read_instruction(ggg1, core)[0]==13 ||m.read_instruction(ggg1, core)[0]==14 ||m.read_instruction(ggg1, core)[0]==15||m.read_instruction(ggg1, core)[0]==16||m.read_instruction(ggg1, core)[0]==21 ||m.read_instruction(ggg1, core)[0]==22 ||m.read_instruction(ggg1, core)[0]==23||m.read_instruction(ggg1, core)[0]==24)

                // if(m.read_instruction(ggg, core)[0]==11 || m.read_instruction(ggg, core)[0]==13 ||m.read_instruction(ggg, core)[0]==14 ||m.read_instruction(ggg, core)[0]==15||m.read_instruction(ggg, core)[0]==16||m.read_instruction(ggg, core)[0]==21 ||m.read_instruction(ggg, core)[0]==22 ||m.read_instruction(ggg, core)[0]==23||m.read_instruction(ggg, core)[0]==24)
                executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg4);
                executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg3);
                executeInstructiondummy(m.read_instruction(ggg2, core), m, r, core, ggg2);
                executeInstructiondummy(m.read_instruction(ggg1, core), m, r, core, ggg1);
                executeInstructiondummy(m.read_instruction(ggg, core), m, r, core, ggg);
                if (r.read(rs1) == r.read(rs2))
                {
                    pc = rd;
                    cout << pc << "~~~~~~~~~~~~3" << endl;
                }
            }
            else if (instruction[0] == 4)
            {
                int rs1 = instruction[1];
                int rs2 = instruction[2];
                int rd = instruction[3];
                cout << rd << "kokokoko" << (r.read(rs1) < r.read(rs2)) << endl;
                cout << r.read(rs1) << r.read(rs2) << endl;
                executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg4);
                executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg3);
                executeInstructiondummy(m.read_instruction(ggg2, core), m, r, core, ggg2);
                executeInstructiondummy(m.read_instruction(ggg1, core), m, r, core, ggg1);
                executeInstructiondummy(m.read_instruction(ggg, core), m, r, core, ggg);
                if (r.read(rs1) != r.read(rs2))
                {
                    pc = rd;
                    cout << pc << "~~~~~~~~~~~~4" << endl;
                }
            }
            else if (instruction[0] == 5)
            {
                int rs1 = instruction[1];
                int rs2 = instruction[2];
                int rd = instruction[3];
                executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg4);
                executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg3);
                executeInstructiondummy(m.read_instruction(ggg2, core), m, r, core, ggg2);
                executeInstructiondummy(m.read_instruction(ggg1, core), m, r, core, ggg1);
                executeInstructiondummy(m.read_instruction(ggg, core), m, r, core, ggg);
                cout << "HI" << r.read(rs1) << " " << r.read(rs2) << endl;
                if (r.read(rs1) < r.read(rs2))
                {
                    pc = rd;
                    cout << pc << "~~~~~~~~~~~~5" << endl;
                }
            }
            else if (instruction[0] == 6)
            {
                int rs1 = instruction[1];
                int rs2 = instruction[2];
                int rd = instruction[3];
                executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg4);
                executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg3);
                executeInstructiondummy(m.read_instruction(ggg2, core), m, r, core, ggg2);
                executeInstructiondummy(m.read_instruction(ggg1, core), m, r, core, ggg1);
                executeInstructiondummy(m.read_instruction(ggg, core), m, r, core, ggg);
                if (r.read(rs1) >= r.read(rs2))
                {
                    pc = rd;
                    cout << pc << "~~~~~~~~~~~~6" << endl;
                }
            }
        }
    }
    ggg4 = ggg3;
    ggg3 = ggg2;
    ggg2 = ggg1;
    ggg1 = ggg;
    ggg = tempry;
    fetch.insert(fetch.end(), instruction.begin(), instruction.end());
}
std::vector<int> ALU::instructionDecode(memory &m, int core, registers &r, int &pc, std::vector<int> &tempReg)
{
    std::vector<int> &fetch = (core == 1) ? fetch1 : fetch2;
    std::vector<int> &decode = (core == 1) ? decode1 : decode2;
    std::vector<int> &execute = (core == 1) ? execute1 : execute2;
    std::vector<int> v;
    // cout<<"decode"<<decode.empty()<<endl;
    // cout<<"fetch"<<fetch.empty()<<endl;
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
    if (decode[1] == 0 && decode[2] == 0 && decode[3] == 0)
    {
        decode.clear();
        decode = fetch;
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
        if (dataforwarding1)
        {
            ans1 = tempReg[rs1];
            ans2 = tempReg[rs2];
        }
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
        int ans1 = r.read(rs1);
        int ans2 = r.read(rs2);
        if (dataforwarding1)
        {
            ans1 = tempReg[rs1];
            ans2 = tempReg[rs2];
        }
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(ans1);
        v.push_back(ans2);
        //   tempReg[rd]=r.read(rs1)-r.read(rs2);
        break;
    }
    case RISCV::mul:
    {
        int rd = decode[1];
        int rs1 = decode[2];
        int rs2 = decode[3];
        int ans1 = r.read(rs1);
        int ans2 = r.read(rs2);
        if (dataforwarding1)
        {
            ans1 = tempReg[rs1];
            ans2 = tempReg[rs2];
        }
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(ans1);
        v.push_back(ans2);
        //    tempReg[rd]=r.read(rs1)*r.read(rs2);
        // r.write(rd, r.read(rs1) * r.read(rs2));
        break;
    }
    case RISCV::addi:
    {
        int rd = decode[1];
        int rs1 = decode[2];
        int temp2 = decode[3];
        // std::cout<<rd<<" "<<r.read(rs1)<<" "<<temp2<<std::endl;
        int ans = r.read(rs1);
        if (dataforwarding1)
        {
            ans = tempReg[rs1];
        }
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(ans);
        v.push_back(temp2);
        //   tempReg[rd]=r.read(rs1)+temp2;
        //  r.write(rd, r.read(rs1) + temp2);
        break;
    }
    case RISCV::muli:
    {
        int rd = decode[1];
        int rs1 = decode[2];
        int temp2 = decode[3];
        int ans = r.read(rs1);
        if (dataforwarding1)
        {
            ans = tempReg[rs1];
        }
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(ans);
        v.push_back(temp2);
        //   tempReg[rd]=r.read(rs1)*temp2;
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
        // tempReg[rd]=pc+1;
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
        //  tempReg[rd]=pc+1;
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
        int ans1 = r.read(rs1);
        int ans2 = r.read(rs2);
        if (dataforwarding1)
        {
            ans1 = tempReg[rs1];
            ans2 = tempReg[rs2];
        }
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(ans1);
        v.push_back(ans2);

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
        int ans1 = r.read(rs1);
        int ans2 = r.read(rs2);
        if (dataforwarding1)
        {
            ans1 = tempReg[rs1];
            ans2 = tempReg[rs2];
        }
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(ans1);
        v.push_back(ans2);
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
        int ans1 = r.read(rs1);
        int ans2 = r.read(rs2);
        if (dataforwarding1)
        {
            ans1 = tempReg[rs1];
            ans2 = tempReg[rs2];
        }
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(ans1);
        v.push_back(ans2);
        break;
        // if (r.read(rs1) < r.read(rs2))
        // {
        //     pc = rd;
        //     break;
        // }
        // else
        // {
        //     break;
        // }
        // break;
    }
    case RISCV::bge:
    {
        int rs1 = decode[1];
        int rs2 = decode[2];
        int rd = decode[3];
        int ans1 = r.read(rs1);
        int ans2 = r.read(rs2);
        if (dataforwarding1)
        {
            ans1 = tempReg[rs1];
            ans2 = tempReg[rs2];
        }
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(ans1);
        v.push_back(ans2);
        break;
        // v.push_back(decode[0]);
        // v.push_back(rd);
        // v.push_back(r.read(rs1));
        // v.push_back(r.read(rs2));
        // if (r.read(rs1) >= r.read(rs2))
        // {
        //     pc = rd;
        //     break;
        // }
        // else
        // {
        //     break;
        // }
        // break;
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
        if (dataforwarding1)
        {
            rs1 = tempReg[decode[3]];
        }
        int offset = decode[2];
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(rs1);
        v.push_back(offset);
        //   tempReg[rd]=m.read_memory((rs1 + offset) / 4,core);
        // r.write(rd, m.read_memory((rs1 + offset) / 4, core));
        break;
    }
    case RISCV::sw:
    {
        int rs1 = decode[1];
        int rd = r.read(decode[3]);
        int offset = decode[2];
        int ans = r.read(rs1);
        if (dataforwarding1)
        {
            rd = tempReg[decode[3]];
            ans = tempReg[rs1];
        }
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(ans);
        v.push_back(offset);
        // m.write_memory((rd + offset) / 4, r.read(rs1), core);
        break;
    }
    case RISCV::la:
    {
        int rd = decode[1];
        std::vector<int> v11 = m.read_instruction(decode[2], core);
        int rs1 = v11[1];
        // if(dataforwarding1)
        // {

        // }
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(rs1);
        //  tempReg[rd]=rs1;
        // r.write(rd, rs1);
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
        //  tempReg[rd]=rs1;
        // r.write(rd, rs1);
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
std::vector<int> ALU::instructionExecute(std::vector<int> v, memory &m, registers &r, int core, int &pc, std::vector<int> &tempReg)
{
    std::vector<int> &execute = (core == 1) ? execute1 : execute2;
    std::vector<int> &decode = (core == 1) ? decode1 : decode2;
    std::vector<int> k;
    if (execute.empty())
    {
        execute = decode;
        // decode.clear();
        return k;
    }
    if (v.empty())
    {
        execute.clear();
        execute = decode;
        return k;
    }
    if (execute[1] == 0 && execute[2] == 0 && execute[3] == 0)
    {
        execute.clear();
        execute = decode;
        return k;
    }
    if (execute[0] == -1 || execute[0] == -2)
    {
        execute.clear();
        execute = decode;
        return k;
    }

    // Convert the execute opcode to enum type
    cout << v[0] << endl;
    RISCV::Inst opcode = static_cast<RISCV::Inst>(v[0]);
    switch (opcode)
    {
    case RISCV::add:
    {
        int ans = v[2] + v[3];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(ans);
        tempReg[v[1]] = ans;
        // r.write(rd, ans1 + ans2);
        break;
    }
    case RISCV::sub:
    {
        int ans = v[2] - v[3];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(ans);
        tempReg[v[1]] = ans;
        break;
    }
    case RISCV::mul:
    {
        int ans = v[2] * v[3];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(ans);
        tempReg[v[1]] = ans;
        break;
    }
    case RISCV::addi:
    {
        int ans = v[2] + v[3];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(ans);
        tempReg[v[1]] = ans;
        break;
    }
    case RISCV::muli:
    {
        int ans = v[2] * v[3];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(ans);
        tempReg[v[1]] = ans;
        break;
    }
    case RISCV::jal:
    {
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(pc + 1);
        // r.write(rd, pc + 1);
        pc = v[2];
        tempReg[v[1]] = pc + 1;
        break;
    }
    case RISCV::jalr:
    {
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(pc + 1);
        pc = v[2] + v[3] - 1;
        tempReg[v[1]] = pc + 1;
        break;
    }
    case RISCV::beq:
    {
        int rs1 = v[2];
        int rs2 = v[3];
        int rd = v[1];
        // cout<<v[1]<<" "<<v[2]<<" "<<v[3]<<endl;
        // if (rs1 == rs2)
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
    case RISCV::bne:
    {
        int rs1 = v[2];
        int rs2 = v[3];
        int rd = v[1];
        // if (rs1 != rs2)
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
    case RISCV::blt:
    {
        int rs1 = v[2];
        int rs2 = v[3];
        int rd = v[1];
        // if (rs1 < rs2)
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
        int rs1 = v[2];
        int rs2 = v[3];
        int rd = v[1];
        // if (rs1 >=rs2)
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
        int rd = v[1];
        // std::cout<<rd<<std::endl;
        //  pc = rd;
        break;
    }
    case RISCV::lw:
    { // cout<<"namate"<<std::endl;
        k.push_back(0);
        k.push_back(v[1]);

        cout << m.read_memory((v[2] + v[3]) / 4, core) << "blacksheep" << endl;
        k.push_back(m.read_memory((v[2] + v[3]) / 4, core));
        tempReg[v[1]] = m.read_memory((v[2] + v[3]) / 4, core);
        // r.write(rd, m.read_memory((rs1 + offset) / 4, core));
        break;
    }
    case RISCV::sw:
    {
        k.push_back(1);
        k.push_back((v[1] + v[3]) / 4);
        k.push_back(v[2]);
        break;
    }
    case RISCV::la:
    {
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(v[2]);
        tempReg[v[1]] = v[2];
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
        tempReg[v[1]] = v[2];
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
void ALU::memoryAccess(std::vector<int> k, memory &m, int core, int &pc)
{
    std::vector<int> &execute = (core == 1) ? execute1 : execute2;
    std::vector<int> &mem = (core == 1) ? mem1 : mem2;
    if (mem.empty())
    {
        mem = execute;
        //   execute.clear();
        return;
    }
    if (k.empty())
    {
        mem.clear();
        mem = execute;
        return;
    }
    if (k[0] == 1)
    {
        m.write_memory(k[1], k[2], core);
    }
    mem.clear();
    mem = execute;
    // execute.clear();
}
void ALU::writeBack(std::vector<int> k, memory &m, int core, int &pc, registers &r)
{
    std::vector<int> &write = (core == 1) ? write1 : write2;
    std::vector<int> &mem = (core == 1) ? mem1 : mem2;
    if (write.empty())
    {
        write = mem;
        // mem.clear();
        return;
    }
    if (k.empty())
    {
        write.clear();
        write = mem;
        return;
    }
    if (k[0] == 0)
    {
        r.write(k[1], k[2]);
    }
    write.clear();
    write = mem;
    // mem.clear();
}

////
void ALU::executeInstructiondummy(std::vector<int> instruction, memory &m, registers &r, int core, int pc)
{ // Check for no-operation instruction
    if (instruction[1] == 0 && instruction[2] == 0 && instruction[3] == 0)
    {
        //  pc++;
        return;
    }
    if (instruction[0] == -1 || instruction[0] == -2)
    {
        //  pc++;
        return;
    }

    // Convert the instruction opcode to enum type
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
        //  pc++;
        break;
    }
    case RISCV::sub:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int rs2 = instruction[3];
        r.write(rd, r.read(rs1) - r.read(rs2));
        // pc++;
        break;
    }
    case RISCV::mul:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int rs2 = instruction[3];
        r.write(rd, r.read(rs1) * r.read(rs2));
        //    pc++;
        break;
    }
    case RISCV::addi:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int temp2 = instruction[3];
        // std::cout<<rd<<" "<<r.read(rs1)<<" "<<temp2<<std::endl;
        r.write(rd, r.read(rs1) + temp2);
        //  pc++;
        break;
    }
    case RISCV::muli:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int temp2 = instruction[3];
        // std::cout<<rd<<" "<<r.read(rs1)<<" "<<temp2<<std::endl;
        r.write(rd, r.read(rs1) * temp2);
        //   pc++;
        break;
    }
    case RISCV::jal:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        r.write(rd, pc + 1);
        // pc = rs1;
        break;
    }
    case RISCV::jalr:
    {
        int rd = instruction[1];
        int rs1 = r.read(instruction[2]);
        int offset = instruction[3];
        r.write(rd, pc + 1);
        // std::cout << rs1 << std::endl;
        //   pc = rs1 + offset - 1;
        // std::cout << pc << std::endl;
        break;
    }
    case RISCV::beq:
    {
        int rs1 = instruction[1];
        int rs2 = instruction[2];
        int rd = instruction[3];
        if (r.read(rs1) == r.read(rs2))
        {
            //   pc = rd;
            break;
        }
        else
        {
            //    pc++;
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
            //      pc = rd;
            break;
        }
        else
        {
            //    pc++;
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
            //     pc = rd;
            break;
        }
        else
        {
            //    pc++;
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
            //     pc = rd;
            break;
        }
        else
        {
            //   pc++;
            break;
        }
    }
    case RISCV::j:
    {
        int rd = instruction[1];
        // std::cout<<rd<<std::endl;
        //      pc = rd;
        break;
    }
    case RISCV::lw:
    {
        int rd = instruction[1];
        int rs1 = r.read(instruction[3]);
        int offset = instruction[2];
        r.write(rd, m.read_memory((rs1 + offset) / 4, core));
        //    pc++;
        break;
    }
    case RISCV::sw:
    {
        int rs1 = instruction[1];
        int rd = r.read(instruction[3]);
        int offset = instruction[2];
        m.write_memory((rd + offset) / 4, r.read(rs1), core);
        //    pc++;
        break;
    }
    case RISCV::la:
    {
        int rd = instruction[1];
        std::vector<int> v = m.read_instruction(instruction[2], core);
        int rs1 = v[1];
        r.write(rd, rs1);
        // std::cout << rs1 << std::endl;
        //  pc++;
        break;
    }
    case RISCV::li:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        // std::cout << rs1 << " " << rd << std::endl;
        r.write(rd, rs1);
        //   pc++;
        break;
    }
    case RISCV::ecall:
    {
        int n = r.read(17);
        int k = r.read(10);
        if (n == 1)
        {
            // logVariable("k", k, outputFile1);
            // std::cout<<core<<" "<<k;
            std::cout << k;
        }
        if (n == 4)
        {
            // std::string temstr = m.read_str(k, core);
            // logs(temstr, outputFile1);
            // std::cout<<m.read_str(k,core);
            // std::cout<<core<<" "<<m.read_str(k, core);
            std::string st_temp = m.read_str(k, core);
            std::cout << st_temp.substr(1, st_temp.size() - 2);
        }
        //  pc++;
        break;
    }

    default:
        //  pc++;
        break;
    }
}

// if (dataforwarding1)
//     {
//         if (instruction[1] != 0 || instruction[2] != 0 || instruction[3] != 0)
//         {
//             if (instruction[0] == 20)
//             {
//                 pc = instruction[1];
//                 cout << pc << "~~~~~~~~~~~~20" << endl;
//             }
//             else if (instruction[0] == 3)
//             {
//                 int rs1 = instruction[1];
//                 int rs2 = instruction[2];
//                 int rd = instruction[3];
//                 executeInstructiondummy(m.read_instruction(prevpc[1], core),m,r,core,prevpc[1]);
//                 executeInstructiondummy(m.read_instruction(prevpc[0], core),m,r,core,prevpc[0]);
//                 if (r.read(rs1) == r.read(rs2))
//                 {
//                     pc = rd;
//                     cout << pc << "~~~~~~~~~~~~3" << endl;
//                 }
//             }
//             else if (instruction[0] == 4)
//             {
//                 int rs1 = instruction[1];
//                 int rs2 = instruction[2];
//                 int rd = instruction[3];
//                 cout << rd << "kokokoko" << (r.read(rs1) < r.read(rs2)) << endl;
//                 cout << r.read(rs1) << r.read(rs2) << endl;
//                 executeInstructiondummy(m.read_instruction(prevpc[1], core),m,r,core,prevpc[1]);
//                 executeInstructiondummy(m.read_instruction(prevpc[0], core),m,r,core,prevpc[0]);
//                 if (r.read(rs1) != r.read(rs2))
//                 {
//                     pc = rd;
//                     cout << pc << "~~~~~~~~~~~~4" << endl;
//                 }
//             }
//             else if (instruction[0] == 5)
//             {
//                 int rs1 = instruction[1];
//                 int rs2 = instruction[2];
//                 int rd = instruction[3];

//                 executeInstructiondummy(m.read_instruction(prevpc[1], core),m,r,core,prevpc[1]);
//                 executeInstructiondummy(m.read_instruction(prevpc[0], core),m,r,core,prevpc[0]);
//                  cout << "HI" << r.read(rs1) << " " <<r.read(rs2) << endl;
//                 if (r.read(rs1) < r.read(rs2))
//                 {
//                     pc = rd;
//                     cout << pc << "~~~~~~~~~~~~5" << endl;
//                 }
//             }
//             else if (instruction[0] == 6)
//             {
//                 int rs1 = instruction[1];
//                 int rs2 = instruction[2];
//                 int rd = instruction[3];
//                executeInstructiondummy(m.read_instruction(prevpc[1], core),m,r,core,prevpc[1]);
//                 executeInstructiondummy(m.read_instruction(prevpc[0], core),m,r,core,prevpc[0]);
//                 if (r.read(rs1) >= r.read(rs2))
//                 {
//                     pc = rd;
//                     cout << pc << "~~~~~~~~~~~~6" << endl;
//                 }
//             }
//         }
//     }