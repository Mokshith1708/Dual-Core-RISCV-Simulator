#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "memory.hpp"

uint32_t memory::read_memory(uint32_t address, int core) const
{
    if (core == 1)
    {
        if (address < MEMORY_SIZE)
        {
            return memory_1[address];
        }
        else
        {
            throw std::out_of_range("Error: Attempted to read from out of bounds memory address.");
        }
    }
    if (core == 2)
    {
        if (address < MEMORY_SIZE)
        {
            return memory_2[address];
        }
        else
        {
            throw std::out_of_range("Error: Attempted to read from out of bounds memory address.");
        }
    }
}

void memory::write_memory(uint32_t address, uint32_t data, int core)
{
    if (core == 1)
    {
        if (address < MEMORY_SIZE)
        {
            memory_1[address] = data;
        }
        else
        {
            throw std::out_of_range("Error: Attempted to read from out of bounds memory address.");
        }
    }
    if (core == 2)
    {
        if (address < MEMORY_SIZE)
        {
            memory_2[address] = data;
        }
        else
        {
            throw std::out_of_range("Error: Attempted to read from out of bounds memory address.");
        }
    }
}
vector<int> memory::read_instruction(uint32_t address, int core)
{
    if (core == 1)
    {
        if (address < MEMORY_SIZE)
        {
            vector<int> v(4, 0);
            for (int i = 0; i < 4; i++)
            {
                v[i] = instructions_1[address][i];
            }
            return v;
        }
        else
        {
            throw std::out_of_range("Error: Attempted to read from out of bounds memory address.");
        }
    }
    if (core == 2)
    {
        if (address < MEMORY_SIZE)
        {
            vector<int> v(4, 0);
            for (int i = 0; i < 4; i++)
            {
                v[i] = instructions_2[address][i];
            }
            return v;
        }
        else
        {
            throw std::out_of_range("Error: Attempted to read from out of bounds memory address.");
        }
    }
}
void memory::write_instruction(uint32_t address, int encode[], int core)
{
    if (core == 1)
    {
        if (address < MEMORY_SIZE)
        {
            for (int i = 0; i < 4; i++)
            {
                instructions_1[address][i] = encode[i];
            }
        }
        else
        {
            throw std::out_of_range("Error: Attempted to read from out of bounds memory address.");
        }
    }
    if (core == 2)
    {
        if (address < MEMORY_SIZE)
        {
            for (int i = 0; i < 4; i++)
            {
                instructions_2[address][i] = encode[i];
            }
        }
        else
        {
            throw std::out_of_range("Error: Attempted to read from out of bounds memory address.");
        }
    }
}