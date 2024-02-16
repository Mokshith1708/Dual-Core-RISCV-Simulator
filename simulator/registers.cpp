#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "registers.hpp"

uint32_t registers::read(uint32_t address) const
{
    if (address < MAX_REG)
    {
        return registers[address];
    }
    else
    {
        throw std::out_of_range("Error: Attempted to read from out of bounds memory address.");
    }
}

void registers::write(uint32_t address, uint32_t data)
{
    if (address < MAX_REG)
    {
        registers[address] = data;
    }
    else
    {
        throw std::out_of_range("Error: Attempted to write out of bounds of memory address.");
    }
}