#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "memory.hpp"

uint32_t memory::read(uint32_t address) const
{
    if (address < MEMORY_SIZE)
    {
        return memory[address];
    }
    else
    {
        throw std::out_of_range("Error: Attempted to read from out of bounds memory address.");
    }
}

void memory::write(uint32_t address, uint32_t data)
{
    if (address < MEMORY_SIZE)
    {
        memory[address] = data;
    }
    else
    {
        throw std::out_of_range("Error: Attempted to write out of bounds of memory address.");
    }
}