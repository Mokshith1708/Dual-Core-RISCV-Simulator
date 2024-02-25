#include "cores.hpp"
#include "simulator.hpp"
#include "memory.hpp"
#include "registers.hpp"

// void ALU::logs(const std::string &message, std::ofstream &outputFile)
// {
//     // Implementation for logging strings
//     // Print to terminal
//     std::cout << message << std::endl;
//     // Print to output file
//     outputFile << message << std::endl;
// }

// void ALU::logi(int number, std::ofstream &outputFile)
// {
//     // Implementation for logging integers
//     // Print to terminal
//     std::cout << number << std::endl;
//     // Print to output file
//     outputFile << number << std::endl;
// }

// template <typename T>
// void ALU::logVariable(const std::string &variableName, T variable, std::ofstream &outputFile)
// {
//     // Implementation for logging variables
//     // Print to terminal
//     std::cout << variableName << ": " << variable << std::endl;
//     // Print to output file
//     outputFile << variableName << ": " << variable << std::endl;
// }

// // Explicit instantiation for supported types
// template void ALU::logVariable<int>(const std::string &, int, std::ofstream &);

// Constructor for ALU class
ALU::ALU(std::pair<int, int> &p1, std::pair<int, int> &p2, int no_inst_1, int no_inst_2, memory &m, registers &r1, registers &r2, int core1, int core2)
{
    pc1 = p1.second;
    pc2 = p2.second;
    // std::cout << pc << std::endl;
    // std::ofstream outputFile1("..\\data_files\\output\\console1.txt", std::ios::app);
    // if (!outputFile1.is_open())
    // {
    //     std::cerr << "Error: Unable to open output file!" << std::endl;
    //     // Optionally, you can return or handle the error in another way
    //     return;
    // }
    // std::ofstream outputFile1("..\\data_files\\output\\console1.txt", std::ios::trunc);
    int maxim = std::max(no_inst_1, no_inst_2);
    while (pc1 < maxim && pc2 < maxim)
    {
        // std::cout << no_inst << std::endl;
        if (pc1 < no_inst_1)
        {
            executeInstruction(m.read_instruction(pc1, core1), m, r1, core1, pc1);
        }
        if (pc2 < no_inst_2)
        {
            executeInstruction(m.read_instruction(pc2, core2), m, r2, core2, pc2);
        } //  pc++;
    }
}

// Function to execute individual instructions
void ALU::executeInstruction(std::vector<int> instruction, memory &m, registers &r, int core, int &pc)
{ // Check for no-operation instruction
    if (instruction[1] == 0 && instruction[2] == 0 && instruction[3] == 0)
    {
        pc++;
        return;
    }
    if (instruction[0] == -1 || instruction[0] == -2)
    {
        pc++;
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
    case RISCV::mul:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int rs2 = instruction[3];
        r.write(rd, r.read(rs1) * r.read(rs2));
        pc++;
        break;
    }
    case RISCV::addi:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int temp2 = instruction[3];
        // std::cout<<rd<<" "<<r.read(rs1)<<" "<<temp2<<std::endl;
        r.write(rd, r.read(rs1) + temp2);
        pc++;
        break;
    }
    case RISCV::muli:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        int temp2 = instruction[3];
        // std::cout<<rd<<" "<<r.read(rs1)<<" "<<temp2<<std::endl;
        r.write(rd, r.read(rs1) * temp2);
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
        // std::cout << rs1 << std::endl;
        pc = rs1 + offset - 1;
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
        // std::cout<<rd<<std::endl;
        pc = rd;
        break;
    }
    case RISCV::lw:
    {
        int rd = instruction[1];
        int rs1 = r.read(instruction[3]);
        int offset = instruction[2];
        r.write(rd, m.read_memory((rs1 + offset) / 4, core));
        pc++;
        break;
    }
    case RISCV::sw:
    {
        int rs1 = instruction[1];
        int rd = r.read(instruction[3]);
        int offset = instruction[2];
        m.write_memory((rd + offset) / 4, r.read(rs1), core);
        pc++;
        break;
    }
    case RISCV::la:
    {
        int rd = instruction[1];
        std::vector<int> v = m.read_instruction(instruction[2], core);
        int rs1 = v[1];
        r.write(rd, rs1);
        // std::cout << rs1 << std::endl;
        pc++;
        break;
    }
    case RISCV::li:
    {
        int rd = instruction[1];
        int rs1 = instruction[2];
        // std::cout << rs1 << " " << rd << std::endl;
        r.write(rd, rs1);
        pc++;
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
            std::cout << m.read_str(k, core);
        }
        pc++;
        break;
    }

    default:
        pc++;
        break;
    }
}
