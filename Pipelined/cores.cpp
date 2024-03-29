#include "cores.hpp"
#include "simulator.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include <vector>
#include <map>
using std::cout;
using std::endl;
using std::string;
void print_array(int core, std::vector<int> k, std::vector<int> v, std::vector<int> fetch, std::vector<int> decode, std::vector<int> execute, std::vector<int> mem, std::vector<int> write)
{
    cout << core << " |"
         << " k       : ";
    for (auto p : k)
        cout << p << " ";
    cout << endl;
    cout << core << " |"
         << " v       : ";
    for (auto p : v)
        cout << p << " ";
    cout << endl;
    cout << core << " |"
         << " fetch   : ";
    for (auto p : fetch)
        cout << p << " ";
    cout << endl;
    cout << core << " |"
         << " decode  : ";
    for (auto p : decode)
        cout << p << " ";
    cout << endl;
    cout << core << " |"
         << " execute : ";
    for (auto p : execute)
        cout << p << " ";
    cout << endl;
    cout << core << " |"
         << " mem     : ";
    for (auto p : mem)
        cout << p << " ";
    cout << endl;
    cout << core << " |"
         << " write   : ";
    for (auto p : write)
        cout << p << " ";
    cout << endl;
    cout << core << " |------------------------ " << endl;
}
ALU::ALU(std::map<string, int> &latency_map, std::pair<int, int> &p1, std::pair<int, int> &p2, int no_inst_11, int no_inst_22, memory &m, registers &r1, registers &r2, int core1, int core2, bool dataforwarding_on)
{
    pc1 = p1.second;
    pc2 = p2.second;
    add_lat = 1;
    addi_lat = 1;
    sub_lat = 1;
    mul_lat = 1;
    muli_lat = 1;
    t_f_1=0;
    t_f_2=0;
    if (!latency_map.empty())
    {
        if (latency_map["add"] > 0)
        {
            add_lat = latency_map["add"];
        }
        if (latency_map["addi"] > 0)
        {
            addi_lat = latency_map["addi"];
        }
        if (latency_map["sub"] > 0)
        {
            sub_lat = latency_map["sub"];
        }
        if (latency_map["mul"] > 0)
        {
            mul_lat = latency_map["mul"];
        }
        if (latency_map["muli"] > 0)
        {
            muli_lat = latency_map["muli"];
        }
    }
    // add_lat=1;
    // addi_lat=1;
    // sub_lat=1;
    // mul_lat=1;
    // muli_lat=1;
    std::vector<int> tempReg1(32);
    std::vector<int> tempReg2(32);
    no_inst_1 = no_inst_11;
    no_inst_2 = no_inst_22;
    int maxim = std::max(no_inst_1, no_inst_2);
    std::vector<int> v1, k1, k2, v2;
    if (dataforwarding_on)
        dataforwarding2 = true;
    else
        dataforwarding2 = false;
    while (pc1 < maxim + 4 && pc2 < maxim + 4)
    {
        if (pc1 < no_inst_1 + 4)
        {
            print_array(1, k1, v1, fetch1, decode1, execute1, mem1, write1);
            writeBack(k1, m, core1, pc1, r1);
            memoryAccess(k1, m, core1, pc1);
            k1.clear();
            k1 = instructionExecute(v1, m, r1, core1, pc1, tempReg1);
            v1.clear();
            v1 = instructionDecode(m, core1, r1, pc1, tempReg1);
            instructionFetch(m, core1, pc1, r1, tempReg1);
            clockCycles1++;
            std::cout << "1 | clockCycles1 : " << clockCycles1 << std::endl;
            count1++;
            std::cout << std::endl;
        }
        if (pc2 < no_inst_2 + 4)
        {
            print_array(2, k2, v2, fetch2, decode2, execute2, mem2, write2);
            writeBack(k2, m, core2, pc2, r2);
            memoryAccess(k2, m, core2, pc2);
            k2.clear();
            k2 = instructionExecute(v2, m, r2, core2, pc2, tempReg2);
            v2.clear();
            v2 = instructionDecode(m, core2, r2, pc2, tempReg2);
            instructionFetch(m, core2, pc2, r2, tempReg2);
            clockCycles2++;
            std::cout << "2 | clockCycles2 : " << clockCycles2 << std::endl;
            count2++;
            std::cout << std::endl;
        }
    }
    print_array(1, k1, v1, fetch1, decode1, execute1, mem1, write1);

    print_array(2, k2, v2, fetch2, decode2, execute2, mem2, write2);

    cout << 3 << " | "
         << "No of instructions for core1 : ";
    cout << count1 << endl;
    cout << 3 << " | "
         << "No of Clockcycles for core1 : ";
    cout << clockCycles1 << endl;
    cout << 3 << " | "
         << "IPC_1 : ";
    cout << (double)count1 / clockCycles1 << endl;
    cout << 3 << " | "
         << "CPI_1 : ";
    cout << (double)clockCycles1 / count1 << endl;
    cout << 3 << " | "
         << "     *****       " << endl;
    cout << 3 << " | "
         << "No of instructions for core2:";
    cout << count2 << endl;
    cout << 3 << " | "
         << "No of Clockcycles for core2:";
    cout << clockCycles2 << endl;
    cout << 3 << " | "
         << "IPC_2 : ";
    cout << (double)count2 / clockCycles2 << endl;
    cout << 3 << " | "
         << "CPI_2 : ";
    cout << (double)clockCycles2 / count2 << endl;
}

int ALU::typeOf(int k)
{
    if (k == 14 || k == 15 || k == 16 || k == 17 || k == 23 || k == 24)
    {
        return 1;
    }
    if (k == 3 || k == 4 || k == 5 || k == 6 || k == 7 || k == 8)
    {
        return 2;
    }
    if (k == 20)
    {
        return 3;
    }
    if (k == 10 || k == 11 || k == 21 || k == 22)
    {
        return 4;
    }
    if (k == 12 || k == 13)
    {
        return 5;
    }
}
int ALU::RAW_Hazard(std::vector<int> v1, std::vector<int> v2)
{
    if (typeOf(v1[0]) == 1 && typeOf(v2[0]) == 1)
    {
        if (v2[1] == v1[1] || v2[2] == v1[1])
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
    if (typeOf(v1[0]) == 1 && typeOf(v2[0]) == 2)
    {
        if (v2[1] == v1[1] || v2[2] == v1[1])
        {
            return 2;
        }
        else
        {
            return -1;
        }
    }
    if (typeOf(v1[0]) == 4 && typeOf(v2[0]) == 5)
    {
        if (v1[1] == v2[1] || v1[1] == v2[3])
        {
            return 3;
        }
        else
        {
            return -1;
        }
    }
    if (typeOf(v1[0]) == 5 && typeOf(v2[0]) == 4)
    {
        if (v1[1] == v2[1] || v1[1] == v2[3] || v1[3] == v2[3])
        {
            return 3;
        }
        else
        {
            return -1;
        }
    }
    if (typeOf(v1[0]) == 1 && typeOf(v2[0]) == 5)
    {
        if (v1[1] == v2[1] || v1[1] == v2[3])
        {
            return 4;
        }
        else
        {
            return -1;
        }
    }
    // if (typeOf(v1[0]) == 5 && typeOf(v2[0]) == 1)
    // {
    //     if (v1[1] == v2[2] || v1[1] == v2[3])
    //     {
    //         return 5;
    //     }
    //     else
    //     {
    //         return -1;
    //     }
    // }
    if (typeOf(v1[0]) == 1 && typeOf(v2[0]) == 4)
    {
        if (v1[1] == v2[3])
        {
            return 6;
        }
        else
        {
            return -1;
        }
    }
    if (typeOf(v1[0]) == 4 && typeOf(v2[0]) == 1)
    {
        if (v1[1] == v2[3] || v1[1] == v2[2])
        {
            // cout<<"/////////////core"<<endl;
            return 7;
        }
        else
        {
            return -1;
        }
    }
    if (typeOf(v1[0]) == 4 && typeOf(v2[0]) == 2)
    {
        if (v1[1] == v2[1] || v1[1] == v2[2])
        {
            return 8;
        }
        else
        {
            return -1;
        }
    }
}
void ALU::instructionFetch(memory &m, int core, int &pc, registers &r, std::vector<int> &tempReg)
{
    std::vector<int> &fetch = (core == 1) ? fetch1 : fetch2;
    int no_inst = (core == 1) ? no_inst_1 : no_inst_2;
    std::vector<int> &prevpc = (core == 1) ? prevpc1 : prevpc2;
    if (core == 1)
    {
        fetch.clear();
        std::vector<int> instruction(4, 0);
        if (pc < no_inst)
        {
            instruction.clear();
            if(t_f_1!=0)
            {
                instruction[0]=0;
                instruction[1]=0;
                instruction[2]=0;
                instruction[3]=0;
                t_f_1--;
                fetch.insert(fetch.end(), instruction.begin(), instruction.end());
                return;
            }
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
                }
                else if (instruction[0] == 3)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg3,tempReg);
                    executeInstructiondummy(m.read_instruction(ggg2, core), m, r, core, ggg2,tempReg);
                    executeInstructiondummy(m.read_instruction(ggg1, core), m, r, core, ggg1,tempReg);
                    executeInstructiondummy(m.read_instruction(ggg, core), m, r, core, ggg,tempReg);
                    if (r.read(rs1) == r.read(rs2))
                    {
                        pc = rd;
                        t_f_1=2;
                        
                    }
                }
                else if (instruction[0] == 4)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg3,tempReg);
                    executeInstructiondummy(m.read_instruction(ggg2, core), m, r, core, ggg2,tempReg);
                    executeInstructiondummy(m.read_instruction(ggg1, core), m, r, core, ggg1,tempReg);
                    executeInstructiondummy(m.read_instruction(ggg, core), m, r, core, ggg,tempReg);
                    if (r.read(rs1) != r.read(rs2))
                    {
                        pc = rd;
                         t_f_1=2;
                    }
                }
                else if (instruction[0] == 5)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg3,tempReg);
                    executeInstructiondummy(m.read_instruction(ggg2, core), m, r, core, ggg2,tempReg);
                    executeInstructiondummy(m.read_instruction(ggg1, core), m, r, core, ggg1,tempReg);
                    executeInstructiondummy(m.read_instruction(ggg, core), m, r, core, ggg,tempReg);
                    if (r.read(rs1) < r.read(rs2))
                    {
                        pc = rd;
                        t_f_1=2;
                    }
                }
                else if (instruction[0] == 6)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg3,tempReg);
                    executeInstructiondummy(m.read_instruction(ggg2, core), m, r, core, ggg2,tempReg);
                    executeInstructiondummy(m.read_instruction(ggg1, core), m, r, core, ggg1,tempReg);
                    executeInstructiondummy(m.read_instruction(ggg, core), m, r, core, ggg,tempReg);
                    if (r.read(rs1) >= r.read(rs2))
                    {
                        pc = rd;
                        t_f_1=2;
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

    else
    {
        fetch.clear();
        std::vector<int> instruction(4, 0);
        if (pc < no_inst)
        {
            instruction.clear();
             if(t_f_2!=0)
            {
                instruction[0]=0;
                instruction[1]=0;
                instruction[2]=0;
                instruction[3]=0;
                t_f_2--;
                fetch.insert(fetch.end(), instruction.begin(), instruction.end());
                return;
            }
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
                }
                else if (instruction[0] == 3)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    executeInstructiondummy(m.read_instruction(gg3, core), m, r, core, gg3,tempReg);
                    executeInstructiondummy(m.read_instruction(gg2, core), m, r, core, gg2,tempReg);
                    executeInstructiondummy(m.read_instruction(gg1, core), m, r, core, gg1,tempReg);
                    executeInstructiondummy(m.read_instruction(gg, core), m, r, core, gg,tempReg);
                    if (r.read(rs1) == r.read(rs2))
                    {
                        pc = rd;
                        t_f_2=2;
                    }
                }
                else if (instruction[0] == 4)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    executeInstructiondummy(m.read_instruction(gg3, core), m, r, core, gg3,tempReg);
                    executeInstructiondummy(m.read_instruction(gg2, core), m, r, core, gg2,tempReg);
                    executeInstructiondummy(m.read_instruction(gg1, core), m, r, core, gg1,tempReg);
                    executeInstructiondummy(m.read_instruction(gg, core), m, r, core, gg,tempReg);
                    if (r.read(rs1) != r.read(rs2))
                    {
                        pc = rd;
                        t_f_2=2;
                    }
                }
                else if (instruction[0] == 5)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    executeInstructiondummy(m.read_instruction(gg3, core), m, r, core, gg3,tempReg);
                    executeInstructiondummy(m.read_instruction(gg2, core), m, r, core, gg2,tempReg);
                    executeInstructiondummy(m.read_instruction(gg1, core), m, r, core, gg1,tempReg);
                    executeInstructiondummy(m.read_instruction(gg, core), m, r, core, gg,tempReg);
                    if (r.read(rs1) < r.read(rs2))
                    {
                        pc = rd;
                        t_f_2=2;
                    }
                }
                else if (instruction[0] == 6)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    executeInstructiondummy(m.read_instruction(gg3, core), m, r, core, gg3,tempReg);
                    executeInstructiondummy(m.read_instruction(gg2, core), m, r, core, gg2,tempReg);
                    executeInstructiondummy(m.read_instruction(gg1, core), m, r, core, gg1,tempReg);
                    executeInstructiondummy(m.read_instruction(gg, core), m, r, core, gg,tempReg);
                    if (r.read(rs1) >= r.read(rs2))
                    {
                        pc = rd;
                        t_f_2=2;
                    }
                }
            }
        }

        gg4 = gg3;
        gg3 = gg2;
        gg2 = gg1;
        gg1 = gg;
        gg = tempry;

        fetch.insert(fetch.end(), instruction.begin(), instruction.end());
    }
}
std::vector<int> ALU::instructionDecode(memory &m, int core, registers &r, int &pc, std::vector<int> &tempReg)
{
    std::vector<int> &fetch = (core == 1) ? fetch1 : fetch2;
    std::vector<int> &decode = (core == 1) ? decode1 : decode2;
    std::vector<int> &execute = (core == 1) ? execute1 : execute2;
    std::vector<int> v;
    if (decode.empty())
    {
        for (auto &temp : fetch)
        {
            decode.push_back(temp);
        }
        return v;
    }
    if (decode[1] == 0 && decode[2] == 0 && decode[3] == 0)
    {
        decode.clear();
        decode = fetch;
        return v;
    }
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
        break;
    }
    case RISCV::addi:
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
        break;
    }
    case RISCV::jal:
    {
        int rd = decode[1];
        int rs1 = decode[2];
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(rs1);
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
    }
    case RISCV::j:
    {
        int rd = decode[1];
        v.push_back(decode[0]);
        v.push_back(rd);
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
        break;
    }
    case RISCV::li:
    {
        int rd = decode[1];
        int rs1 = decode[2];
        v.push_back(decode[0]);
        v.push_back(rd);
        v.push_back(rs1);
        break;
    }
    case RISCV::ecall:
    {
        int n = r.read(17);
        int k = r.read(10);
        v.push_back(decode[0]);
        v.push_back(n);
        v.push_back(k);
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
        if (core == 1)
        {
            clockCycles1 += add_lat - 1;
        }
        else
        {
            clockCycles2 += add_lat - 1;
        }
        break;
    }
    case RISCV::sub:
    {
        int ans = v[2] - v[3];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(ans);
        tempReg[v[1]] = ans;
        if (core == 1)
        {
            clockCycles1 += sub_lat - 1;
        }
        else
        {
            clockCycles2 += sub_lat - 1;
        }
        break;
    }
    case RISCV::mul:
    {
        int ans = v[2] * v[3];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(ans);
        tempReg[v[1]] = ans;
        if (core == 1)
        {
            clockCycles1 += mul_lat - 1;
        }
        else
        {
            clockCycles2 += mul_lat - 1;
        }
        break;
    }
    case RISCV::addi:
    {
        int ans = v[2] + v[3];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(ans);
        tempReg[v[1]] = ans;
        if (core == 1)
        {
            clockCycles1 += addi_lat - 1;
        }
        else
        {
            clockCycles2 += addi_lat - 1;
        }
        break;
    }
    case RISCV::muli:
    {
        int ans = v[2] * v[3];
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(ans);
        tempReg[v[1]] = ans;
        if (core == 1)
        {
            clockCycles1 += muli_lat - 1;
        }
        else
        {
            clockCycles2 += muli_lat - 1;
        }
        break;
    }
    case RISCV::jal:
    {
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(pc + 1);
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
        bool i = predictor(pc, m);
        if (rs1 == rs2)
        {
            break;
        }
        else
        {
            cout << core << " | "
                 << "*** Wrong Prediction ***" << endl;
            break;
        }
        break;
    }
    case RISCV::bne:
    {
        int rs1 = v[2];
        int rs2 = v[3];
        int rd = v[1];
        bool i = predictor(pc, m);
        if (rs1 != rs2)
        {
            break;
        }
        else
        {
            cout << core << " | "
                 << "*** Wrong Prediction ***" << endl;
            break;
        }
        break;
    }
    case RISCV::blt:
    {
        int rs1 = v[2];
        int rs2 = v[3];
        int rd = v[1];
        bool i = predictor(pc, m);
        if (rs1 < rs2)
        {
            break;
        }
        else
        {
            cout << core << " | "
                 << "*** Wrong Prediction ***" << endl;
            break;
        }
        break;
    }
    case RISCV::bge:
    {
        int rs1 = v[2];
        int rs2 = v[3];
        int rd = v[1];
        bool i = predictor(pc, m);
        if (rs1 >= rs2)
        {
            break;
        }
        else
        {
            cout << core << " | "
                 << "*** Wrong Prediction ***" << endl;
            break;
        }
        break;
    }

    case RISCV::j:
    {
        int rd = v[1];
        break;
    }
    case RISCV::lw:
    {
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(m.read_memory((v[2] + v[3]) / 4, core));
        tempReg[v[1]] = m.read_memory((v[2] + v[3]) / 4, core);
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
        break;
    }
    case RISCV::li:
    {
        k.push_back(0);
        k.push_back(v[1]);
        k.push_back(v[2]);
        tempReg[v[1]] = v[2];
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
    return k;
}
void ALU::memoryAccess(std::vector<int> k, memory &m, int core, int &pc)
{
    std::vector<int> &execute = (core == 1) ? execute1 : execute2;
    std::vector<int> &mem = (core == 1) ? mem1 : mem2;
    if (mem.empty())
    {
        mem = execute;
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
}
void ALU::writeBack(std::vector<int> k, memory &m, int core, int &pc, registers &r)
{
    std::vector<int> &write = (core == 1) ? write1 : write2;
    std::vector<int> &mem = (core == 1) ? mem1 : mem2;
    if (write.empty())
    {
        write = mem;
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
}
void ALU::executeInstructiondummy(std::vector<int> instruction, memory &m, registers &r, int core, int pc,std::vector<int> &tempReg)
{
    if (instruction[1] == 0 && instruction[2] == 0 && instruction[3] == 0)
    {
        return;
    }
    if (instruction[0] == -1 || instruction[0] == -2)
    {
        return;
    }
    RISCV::Inst opcode = static_cast<RISCV::Inst>(instruction[0]);
    switch (opcode)
    {
    case RISCV::add:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int rs2 = instruction[3];
        int ans1 = tempReg[rs1];
        int ans2 = tempReg[rs2];
        tempReg[rd]= ans1 + ans2;
        break;
    }
    case RISCV::sub:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int rs2 = instruction[3];
        int ans1 = tempReg[rs1];
        int ans2 = tempReg[rs2];
        tempReg[rd]= ans1 - ans2;
        break;
    }
    case RISCV::mul:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int rs2 = instruction[3];
        int ans1 = tempReg[rs1];
        int ans2 = tempReg[rs2];
        tempReg[rd]= ans1 * ans2;
        break;
    }
    case RISCV::addi:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int temp2 = instruction[3];
        tempReg[rd]= tempReg[rs1] + temp2;
        break;
    }
    case RISCV::muli:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int temp2 = instruction[3];
        tempReg[rd]= tempReg[rs1] * temp2;
        break;
    }
    case RISCV::jal:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        tempReg[rd]= pc + 1;
        break;
    }
    case RISCV::jalr:
    {
        int rd = instruction[1];
        int rs1 =tempReg[instruction[2]];
        int offset = instruction[3];
        tempReg[rd]= pc + 1;
        break;
    }
    case RISCV::beq:
    {
        int rs1 = instruction[1];
        int rs2 = instruction[2];
        int rd = instruction[3];
        if (r.read(rs1) == r.read(rs2))
        {
            break;
        }
        else
        {
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
            break;
        }
        else
        {
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
            break;
        }
        else
        {
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
            break;
        }
        else
        {
            break;
        }
    }
    case RISCV::j:
    {
        int rd = instruction[1];
        break;
    }
    case RISCV::lw:
    {
        int rd = instruction[1];
        int rs1 = tempReg[instruction[3]];
        int offset = instruction[2];
        tempReg[rd, m.read_memory((rs1 + offset) / 4, core)];
        break;
    }
    case RISCV::sw:
    {
        int rs1 = instruction[1];
        int rd = tempReg[instruction[3]];
        int offset = instruction[2];
        m.write_memory((rd + offset) / 4, tempReg[rs1], core);
        break;
    }
    case RISCV::la:
    {
        int rd = instruction[1];
        std::vector<int> v = m.read_instruction(instruction[2], core);
        int rs1 = v[1];
        tempReg[rd] = rs1;
        break;
    }
    case RISCV::li:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
       tempReg[rd] = rs1;
        break;
    }
    case RISCV::ecall:
    {
        int n = r.read(17);
        int k = r.read(10);
        if (n == 1)
        {
          //  std::cout << k;
        }
        if (n == 4)
        {
            //std::string st_temp = m.read_str(k, core);
           // std::cout << st_temp.substr(1, st_temp.size() - 2);
        }
        break;
    }

    default:
        break;
    }
}
bool ALU::predictor(int pc, memory &m)
{
    return true;
}
void ALU::stallManagement(memory &m, int core, registers &r, int &pc, std::vector<int> &tempReg)
{

}