#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "memory.hpp"

uint32_t memory::read(uint32_t address, uint32_t arr[]) const
{
    if (address < MEMORY_SIZE)
    {
        return arr[address];
    }
    else
    {
        throw std::out_of_range("Error: Attempted to read from out of bounds memory address.");
    }
}

void memory::write(uint32_t address, uint32_t data, uint32_t arr[])
{
    if (address < MEMORY_SIZE)
    {
        arr[address] = data;
    }
    else
    {
        throw std::out_of_range("Error: Attempted to write out of bounds of memory address.");
    }
}