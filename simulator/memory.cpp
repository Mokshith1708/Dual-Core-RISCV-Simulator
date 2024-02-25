#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <bits/stdc++.h>
#include "memory.hpp"

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
            throw out_of_range("Error: Attempted to read from out of bounds memory address.");
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
            throw out_of_range("Error: Attempted to read from out of bounds memory address.");
        }
    }
}

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
            throw out_of_range("Error: Attempted to read from out of bounds memory address.");
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
            throw out_of_range("Error: Attempted to read from out of bounds memory address.");
        }
    }
}
vector<int> memory::read_instruction(int32_t address, int core)
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
            throw out_of_range("Error: Attempted to read from out of bounds memory address.");
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
            throw out_of_range("Error: Attempted to read from out of bounds memory address.");
        }
    }
}
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
            throw out_of_range("Error: Attempted to read from out of bounds memory address.");
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
            throw out_of_range("Error: Attempted to read from out of bounds memory address.");
        }
    }
}
void memory::write_str(string s1, string s2, int address_str, int core)
{
    pair<string, string> p;
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
string memory::read_str(int32_t address, int core)
{
    pair<string, string> p;
    string result = "";  // Initialize an empty string to store the result

    if (core == 1)
    {
        // Search for the string in strmap_1 using the given address
        for ( auto &entry : strmap_1)
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
        for ( auto &entry : strmap_2)
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
