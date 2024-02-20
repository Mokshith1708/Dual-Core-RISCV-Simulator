void executeInstruction(const int *instruction, memory &m, int core)
{
    RISCV::Inst opcode = static_cast<RISCV::Inst>(instruction[0]);
    switch (opcode)
    {
        case RISCV::add:
        {
            int rd = instruction[1];
            int rs1 = instruction[2];
            int rs2 = instruction[3];
            m.write_memory(rd, m.read_memory(rs1, core) + m.read_memory(rs2, core), core);
            break;
        }
        case RISCV::sub:
        {
            int rd = instruction[1];
            int rs1 = instruction[2];
            int rs2 = instruction[3];
            m.write_memory(rd, m.read_memory(rs1, core) - m.read_memory(rs2, core), core);
            break;
        }
        default:
            break;
        }
}