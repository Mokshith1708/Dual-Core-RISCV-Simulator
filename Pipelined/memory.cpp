#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include "memory.hpp"

// Read data from memory based on the address and core
int32_t memory::read_memory(int32_t address, int core)
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

// Write data to memory based on the address, data, and core
void memory::write_memory(int32_t address, int32_t data, int core)
{
    if (core == 1)
    {
        if (address < MEMORY_SIZE)
        {
            memory_1[address] = data;
        }
        else
        {
            throw std::out_of_range("Error: Attempted to write from out of bounds memory address.");
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
            throw std::out_of_range("Error: Attempted to write from out of bounds memory address.");
        }
    }
}
// Read instruction from instruction memory based on the address and core
std::vector<int> memory::read_instruction(int32_t address, int core)
{
    if (core == 1)
    {
        if (address < MEMORY_SIZE)
        {
            std::vector<int> v(4, 0);
            for (int i = 0; i < 4; i++)
            {
                v[i] = instructions_1[address][i];
            }
            return v;
        }
        else
        {
            throw std::out_of_range("Error: Attempted to read inst from out of bounds memory address.");
        }
    }
    if (core == 2)
    {
        if (address < MEMORY_SIZE)
        {
            std::vector<int> v(4, 0);
            for (int i = 0; i < 4; i++)
            {
                v[i] = instructions_2[address][i];
            }
            return v;
        }
        else
        {
            throw std::out_of_range("Error: Attempted to read inst from out of bounds memory address.");
        }
    }
}

// write instruction to instruction memory based on the address and core
void memory::write_instruction(int32_t address, int encode[], int core)
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
            throw std::out_of_range("Error: Attempted to write from out of bounds memory address.");
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
            throw std::out_of_range("Error: Attempted to write from out of bounds memory address.");
        }
    }
}
// Write strings to str memory based on the addresses and core
void memory::write_str(std::string s1, std::string s2, int address_str, int core)
{
    std::pair<std::string, std::string> p;
    p.first = s1;
    p.second = s2;
    if (core == 1)
    {
        strmap_1[p] = address_str;
    }
    else if (core == 2)
    {
        strmap_2[p] = address_str;
    }
}
// read strings from memory based on the addresses and core
std::string memory::read_str(int32_t address, int core)
{
    std::pair<std::string, std::string> p;
    std::string result = ""; // Initialize an empty string to store the result

    if (core == 1)
    {
        // Search for the string in strmap_1 using the given address
        for (auto &entry : strmap_1)
        {
            if (entry.second == address)
            {
                // Found the string associated with the address
                result = entry.first.second;
                break;
            }
        }
    }
    else if (core == 2)
    {
        // Search for the string in strmap_2 using the given address
        for (auto &entry : strmap_2)
        {
            if (entry.second == address)
            {
                // Found the string associated with the address
                result = entry.first.second;
                break;
            }
        }
    }

    return result;
}
