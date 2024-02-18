#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstdarg>
namespace RISCV
{
    enum reg
    {
        x0 = 0,
        x1 = 1,
        x2 = 2,
        x3 = 3,
        x4 = 4,
        x5 = 5,
        x6 = 6,
        x7 = 7,
        x8 = 8,
        x9 = 9,
        x10 = 10,
        x11 = 11,
        x12 = 12,
        x13 = 13,
        x14 = 14,
        x15 = 15,
        x16 = 16,
        x17 = 17,
        x18 = 18,
        x19 = 19,
        x20 = 20,
        x21 = 21,
        x22 = 22,
        x23 = 23,
        x24 = 24,
        x25 = 25,
        x26 = 26,
        x27 = 27,
        x28 = 28,
        x29 = 29,
        x30 = 30,
        x31 = 31,
    };

    enum Inst
    {
        jal = 1,
        jalr = 2,
        beq = 3,
        bne = 4,
        blt = 5,
        bge = 6,
        bltu = 7,
        begu = 8,
        lb = 10,
        lw = 11,
        sb = 12,
        sw = 13,
        addi = 14,
        add = 15,
        sub = 16,
        ecall = 17,
        lbu = 18,
        lwu = 19,

    };
}
std::string InstToString(RISCV::Inst inst)
{
    switch (inst)
    {
    case RISCV::jal:
        return "jal";
    case RISCV::jalr:
        return "jalr";
    case RISCV::beq:
        return "beq";
    case RISCV::bne:
        return "bne";
    case RISCV::blt:
        return "blt";
    case RISCV::bge:
        return "bge";
    case RISCV::bltu:
        return "bltu";
    case RISCV::begu:
        return "begu";
    case RISCV::lb:
        return "lb";
    case RISCV::lw:
        return "lw";
    case RISCV::sb:
        return "sb";
    case RISCV::sw:
        return "sw";
    case RISCV::addi:
        return "addi";
    case RISCV::add:
        return "add";
    case RISCV::sub:
        return "sub";
    case RISCV::ecall:
        return "ecall";
    case RISCV::lbu:
        return "lbu";
    case RISCV::lwu:
        return "lwu";
    default:
        return "Unknown";
    }
}
std::string regToString(RISCV::reg r)
{
    switch (r)
    {
    case RISCV::x0:
        return "x0";
    case RISCV::x1:
        return "x1";
    case RISCV::x2:
        return "x2";
    case RISCV::x3:
        return "x3";
    case RISCV::x4:
        return "x4";
    case RISCV::x5:
        return "x5";
    case RISCV::x6:
        return "x6";
    case RISCV::x7:
        return "x7";
    case RISCV::x8:
        return "x8";
    case RISCV::x9:
        return "x9";
    case RISCV::x10:
        return "x10";
    case RISCV::x11:
        return "x11";
    case RISCV::x12:
        return "x12";
    case RISCV::x13:
        return "x13";
    case RISCV::x14:
        return "x14";
    case RISCV::x15:
        return "x15";
    case RISCV::x16:
        return "x16";
    case RISCV::x17:
        return "x17";
    case RISCV::x18:
        return "x18";
    case RISCV::x19:
        return "x19";
    case RISCV::x20:
        return "x20";
    case RISCV::x21:
        return "x21";
    case RISCV::x22:
        return "x22";
    case RISCV::x23:
        return "x23";
    case RISCV::x24:
        return "x24";
    case RISCV::x25:
        return "x25";
    case RISCV::x26:
        return "x26";
    case RISCV::x27:
        return "x27";
    case RISCV::x28:
        return "x28";
    case RISCV::x29:
        return "x29";
    case RISCV::x30:
        return "x30";
    case RISCV::x31:
        return "x31";
    default:
        return "Unknown";
    }
}
bool isBranch(int i)
{
    if (i == 3 || i == 4 || i == 5 || i == 6 ||
        i == 7 || i == 8)
    {
        return true;
    }
    return false;
}

bool isJump(int i)
{
    if (i == 1 || i == 2)
    {
        return true;
    }
    return false;
}

bool isReadMem(int i)
{
    if (i == 10 || i == 12 || i == 18 || i == 19)
    {
        return true;
    }
    return false;
}