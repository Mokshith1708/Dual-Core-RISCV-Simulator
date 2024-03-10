#include "cores.hpp"
#include "simulator.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include <bits/stdc++.h>
using namespace std;
void print_array(int core, vector<int> k, vector<int> v, vector<int> fetch, vector<int> decode, vector<int> execute, vector<int> mem, vector<int> write)
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
ALU::ALU(std::pair<int, int> &p1, std::pair<int, int> &p2, int no_inst_11, int no_inst_22, memory &m, registers &r1, registers &r2, int core1, int core2)
{
    pc1 = p1.second;
    pc2 = p2.second;
    dataforwarding2 = false;
    add_lat = 3;
    addi_lat = 3;
    sub_lat = 3;
    mul_lat = 3;
    muli_lat = 3;
    std::vector<int> tempReg1(32);
    std::vector<int> tempReg2(32);
    no_inst_1 = no_inst_11;
    no_inst_2 = no_inst_22;
    int maxim = std::max(no_inst_1, no_inst_2);
    std::vector<int> v1, k1, k2, v2;
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
            std::cout << "1 | clockCycles2 : " << clockCycles2 << std::endl;
        } //  pc++;
    }
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
    if (k == 10 || k == 11)
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
    if (typeOf(v1[0]) == 5 && typeOf(v2[0]) == 1)
    {
        if (v1[1] == v2[2] || v1[1] == v2[3])
        {
            return 5;
        }
        else
        {
            return -1;
        }
    }
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
    // fetch.push_back(m.read_instruction(pc, core));
    // pc++;
    std::vector<int> &fetch = (core == 1) ? fetch1 : fetch2;
    int no_inst = (core == 1) ? no_inst_1 : no_inst_2;
    std::vector<int> &prevpc = (core == 1) ? prevpc1 : prevpc2;
    // std::vector<int>& decode = (core == 1) ? decode1 : decode2;
    if (core == 1)
    {
        fetch.clear();
        std::vector<int> instruction(4, 0);
        // prevpc[1]=prevpc[0];
        // prevpc[0]=pc-1;

        // cout <<core<<" | "<< ggg << "opp" << ggg1 << endl;
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
                    // cout << pc << "~~~~~~~~~~~~20" << endl;
                }
                else if (instruction[0] == 3)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    //    if(m.read_instruction(ggg1, core)[0]==11 || m.read_instruction(ggg1, core)[0]==13 ||m.read_instruction(ggg1, core)[0]==14 ||m.read_instruction(ggg1, core)[0]==15||m.read_instruction(ggg1, core)[0]==16||m.read_instruction(ggg1, core)[0]==21 ||m.read_instruction(ggg1, core)[0]==22 ||m.read_instruction(ggg1, core)[0]==23||m.read_instruction(ggg1, core)[0]==24)

                    // if(m.read_instruction(ggg, core)[0]==11 || m.read_instruction(ggg, core)[0]==13 ||m.read_instruction(ggg, core)[0]==14 ||m.read_instruction(ggg, core)[0]==15||m.read_instruction(ggg, core)[0]==16||m.read_instruction(ggg, core)[0]==21 ||m.read_instruction(ggg, core)[0]==22 ||m.read_instruction(ggg, core)[0]==23||m.read_instruction(ggg, core)[0]==24)
                    //    executeInstructiondummy(m.read_instruction(ggg4, core), m, r, core, ggg4);
                    executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg3);
                    executeInstructiondummy(m.read_instruction(ggg2, core), m, r, core, ggg2);
                    executeInstructiondummy(m.read_instruction(ggg1, core), m, r, core, ggg1);
                    executeInstructiondummy(m.read_instruction(ggg, core), m, r, core, ggg);
                    if (r.read(rs1) == r.read(rs2))
                    {
                        pc = rd;
                        // cout << pc << "~~~~~~~~~~~~3" << endl;
                    }
                }
                else if (instruction[0] == 4)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    //  cout << rd << "kokokoko" << (r.read(rs1) < r.read(rs2)) << endl;
                    // cout << r.read(rs1) << r.read(rs2) << endl;
                    //    executeInstructiondummy(m.read_instruction(ggg4, core), m, r, core, ggg4);
                    executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg3);
                    executeInstructiondummy(m.read_instruction(ggg2, core), m, r, core, ggg2);
                    executeInstructiondummy(m.read_instruction(ggg1, core), m, r, core, ggg1);
                    executeInstructiondummy(m.read_instruction(ggg, core), m, r, core, ggg);
                    if (r.read(rs1) != r.read(rs2))
                    {
                        pc = rd;
                        // cout << pc << "~~~~~~~~~~~~4" << endl;
                    }
                }
                else if (instruction[0] == 5)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    //   executeInstructiondummy(m.read_instruction(ggg4, core), m, r, core, ggg4);
                    executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg3);
                    executeInstructiondummy(m.read_instruction(ggg2, core), m, r, core, ggg2);
                    executeInstructiondummy(m.read_instruction(ggg1, core), m, r, core, ggg1);
                    executeInstructiondummy(m.read_instruction(ggg, core), m, r, core, ggg);
                    //   cout << "HI" << r.read(rs1) << " " << r.read(rs2) << endl;
                    if (r.read(rs1) < r.read(rs2))
                    {
                        pc = rd;
                        //   cout << pc << "~~~~~~~~~~~~5" << endl;
                    }
                }
                else if (instruction[0] == 6)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    //    executeInstructiondummy(m.read_instruction(ggg4, core), m, r, core, ggg4);
                    executeInstructiondummy(m.read_instruction(ggg3, core), m, r, core, ggg3);
                    executeInstructiondummy(m.read_instruction(ggg2, core), m, r, core, ggg2);
                    executeInstructiondummy(m.read_instruction(ggg1, core), m, r, core, ggg1);
                    executeInstructiondummy(m.read_instruction(ggg, core), m, r, core, ggg);
                    if (r.read(rs1) >= r.read(rs2))
                    {
                        pc = rd;
                        //   cout << pc << "~~~~~~~~~~~~6" << endl;
                    }
                }
            }
        }

        if (dataforwarding2 == false)
        {
            int p = RAW_Hazard(m.read_instruction(ggg, core), m.read_instruction(tempry, core));
            if (p != -1)
            {
                clockCycles1 += 3;
                cout << core << " |--------Stall-------" << endl;
                cout << core << " | " << tempry + 1 << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(tempry, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(ggg, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " |--------Stall end-------" << endl;
            }
        }
        if (dataforwarding2 == true)
        {
            int p = RAW_Hazard(m.read_instruction(ggg, core), m.read_instruction(tempry, core));
            if (p == 3)
            {
                clockCycles1 += 1;

                cout << core << " |--------Stall-------" << endl;
                cout << core << " | " << tempry + 1 << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(tempry, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(ggg, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " --------Stall end-------" << endl;
            }
            if (p == 6)
            {
                clockCycles1 += 1;

                cout << core << " |--------Stall-------" << endl;
                cout << core << " | " << tempry + 1 << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(tempry, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(ggg, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " |--------Stall end-------" << endl;
            }
            if (p == 7)
            {
                clockCycles1 += 1;

                cout << core << " |--------Stall-------" << endl;
                cout << core << " | " << tempry + 1 << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(tempry, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " | " << ggg << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(ggg, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " |--------Stall end-------" << endl;
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
        // prevpc[1]=prevpc[0];
        // prevpc[0]=pc-1;

        // cout <<core<<" | "<< gg << "opp" << gg1 << endl;
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
                    //  cout << pc << "~~~~~~~~~~~~20" << endl;
                }
                else if (instruction[0] == 3)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    //    if(m.read_instruction(ggg1, core)[0]==11 || m.read_instruction(ggg1, core)[0]==13 ||m.read_instruction(ggg1, core)[0]==14 ||m.read_instruction(ggg1, core)[0]==15||m.read_instruction(ggg1, core)[0]==16||m.read_instruction(ggg1, core)[0]==21 ||m.read_instruction(ggg1, core)[0]==22 ||m.read_instruction(ggg1, core)[0]==23||m.read_instruction(ggg1, core)[0]==24)

                    // if(m.read_instruction(ggg, core)[0]==11 || m.read_instruction(ggg, core)[0]==13 ||m.read_instruction(ggg, core)[0]==14 ||m.read_instruction(ggg, core)[0]==15||m.read_instruction(ggg, core)[0]==16||m.read_instruction(ggg, core)[0]==21 ||m.read_instruction(ggg, core)[0]==22 ||m.read_instruction(ggg, core)[0]==23||m.read_instruction(ggg, core)[0]==24)
                    // executeInstructiondummy(m.read_instruction(gg4, core), m, r, core, gg4);
                    executeInstructiondummy(m.read_instruction(gg3, core), m, r, core, gg3);
                    executeInstructiondummy(m.read_instruction(gg2, core), m, r, core, gg2);
                    executeInstructiondummy(m.read_instruction(gg1, core), m, r, core, gg1);
                    executeInstructiondummy(m.read_instruction(gg, core), m, r, core, gg);
                    if (r.read(rs1) == r.read(rs2))
                    {
                        pc = rd;
                        //   cout << pc << "~~~~~~~~~~~~3" << endl;
                    }
                }
                else if (instruction[0] == 4)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    //    cout << rd << "kokokoko" << (r.read(rs1) < r.read(rs2)) << endl;
                    //    cout << r.read(rs1) << r.read(rs2) << endl;
                    // executeInstructiondummy(m.read_instruction(gg4, core), m, r, core, gg4);
                    executeInstructiondummy(m.read_instruction(gg3, core), m, r, core, gg3);
                    executeInstructiondummy(m.read_instruction(gg2, core), m, r, core, gg2);
                    executeInstructiondummy(m.read_instruction(gg1, core), m, r, core, gg1);
                    executeInstructiondummy(m.read_instruction(gg, core), m, r, core, gg);
                    if (r.read(rs1) != r.read(rs2))
                    {
                        pc = rd;
                        //  cout << pc << "~~~~~~~~~~~~4" << endl;
                    }
                }
                else if (instruction[0] == 5)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    //  executeInstructiondummy(m.read_instruction(gg4, core), m, r, core, gg4);
                    executeInstructiondummy(m.read_instruction(gg3, core), m, r, core, gg3);
                    executeInstructiondummy(m.read_instruction(gg2, core), m, r, core, gg2);
                    executeInstructiondummy(m.read_instruction(gg1, core), m, r, core, gg1);
                    executeInstructiondummy(m.read_instruction(gg, core), m, r, core, gg);
                    //   cout << "HI" << r.read(rs1) << " " << r.read(rs2) << endl;
                    if (r.read(rs1) < r.read(rs2))
                    {
                        pc = rd;
                        //  cout << pc << "~~~~~~~~~~~~5" << endl;
                    }
                }
                else if (instruction[0] == 6)
                {
                    int rs1 = instruction[1];
                    int rs2 = instruction[2];
                    int rd = instruction[3];
                    //      executeInstructiondummy(m.read_instruction(gg4, core), m, r, core, gg4);
                    executeInstructiondummy(m.read_instruction(gg3, core), m, r, core, gg3);
                    executeInstructiondummy(m.read_instruction(gg2, core), m, r, core, gg2);
                    executeInstructiondummy(m.read_instruction(gg1, core), m, r, core, gg1);
                    executeInstructiondummy(m.read_instruction(gg, core), m, r, core, gg);
                    if (r.read(rs1) >= r.read(rs2))
                    {
                        pc = rd;
                        //  cout << pc << "~~~~~~~~~~~~6" << endl;
                    }
                }
            }
        }

        if (dataforwarding2 == false)
        {
            bool stall_adj = false;
            int p = RAW_Hazard(m.read_instruction(gg, core), m.read_instruction(tempry, core));
            if (p == 1 || p == 2 || p == 3 || p == 4 || p == 5 || p == 6 || p == 7 || p == 8)
            {
                clockCycles2 += 2;
                stall_adj = true;
                cout << core << " |--------Stall-------" << p << endl;
                cout << core << " | " << tempry + 1 << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(tempry, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " | " << gg + 1 << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(gg, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " |--------Stall end-------" << endl;
            }
            int r = RAW_Hazard(m.read_instruction(gg1, core), m.read_instruction(tempry, core));
            if ((r == 1 || r == 2 || r == 3 || r == 4 || r == 5 || r == 6 || r == 7 || r == 8))
            {
                clockCycles2 += 1;
                cout << core << " |--------Stall-------" << p << endl;
                cout << core << " | " << tempry + 1 << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(tempry, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " | " << gg1 + 1 << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(gg1, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " |--------Stall end-------" << endl;
            }
            stall_adj = false;
        }
        if (dataforwarding2 == true)
        {
            int p = RAW_Hazard(m.read_instruction(gg, core), m.read_instruction(tempry, core));
            if (p == 3)
            {
                clockCycles2 += 1;

                cout << core << " --------Stall-------" << endl;
                cout << core << " | " << tempry + 1 << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(tempry, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " | " << gg + 1 << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(gg, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " |--------Stall end-------" << endl;
            }
            if (p == 6)
            {
                clockCycles2 += 1;

                cout << core << " |--------Stall-------" << endl;
                cout << core << " | " << tempry + 1 << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(tempry, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " | " << gg + 1 << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(gg, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " |--------Stall end-------" << endl;
            }
            if (p == 7)
            {
                clockCycles2 += 1;

                cout << core << " |--------Stall-------" << endl;
                cout << core << " | " << tempry + 1 << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(tempry, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
                cout << core << " | " << gg + 1 << endl;
                cout << core << " | ";
                for (auto &jj : m.read_instruction(gg, core))
                {
                    cout << jj << " ";
                }
                cout << endl;
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
    //  cout << v[0] << endl;
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
        bool i = predictor(pc, m);
        // cout<<v[1]<<" "<<v[2]<<" "<<v[3]<<endl;
        if (rs1 == rs2)
        {
            // bool i = predictor(pc,m);
            break;
        }
        else
        {
            cout <<core<<" | "<< "*** Wrong Prediction ***" << endl;
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
            // pc = rd;
            break;
        }
        else
        {
            cout <<core<<" | "<< "*** Wrong Prediction ***" << endl;
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
            // pc = rd;
            break;
        }
        else
        {
            cout <<core<<" | "<< "*** Wrong Prediction ***" << endl;
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
            // pc = rd;
            break;
        }
        else
        {
            cout <<core<<" | "<< "*** Wrong Prediction ***" << endl;
            break;
        }
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

        // cout << m.read_memory((v[2] + v[3]) / 4, core) << "blacksheep" << endl;
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
bool ALU::predictor(int pc, memory &m)
{
    return true;
}
