#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include "memory.hpp"
#include "SharedCache.hpp"

int32_t memory::read_memory_1(int32_t address, int core, bool lru_bool)
{
    if (cache.read_cache(address, core, false))
    {
        if (core == 1)
        {
            return memory_1[address];
        }
        else if (core == 2)
        {
            return memory_2[address];
        }
    }
    else
    {
        if (core == 1)
        {
            if (address < MEMORY_SIZE)
            {
                int32_t data = memory_1[address];
                cache.write_cache(address, core, false, lru_bool);
                cache.accesses--;
                ck1 = true;
                return data;
            }
            else
            {
                throw std::out_of_range("Error: Attempted to read from out of bounds memory address.11");
            }
        }
        else if (core == 2)
        {
            if (address < MEMORY_SIZE)
            {
                int32_t data = memory_2[address];
                cache.write_cache(address, core, false, lru_bool);
                cache.accesses--;
                ck2 = true;
                ;
                return data;
            }
            else
            {
                throw std::out_of_range("Error: Attempted to read from out of bounds memory address.12");
            }
        }
    }
}

void memory::write_memory_1(int32_t address, int32_t data, int core, bool lru_bool)
{
    // Write data to memory
    if (core == 1)
    {
        if (address < MEMORY_SIZE)
        {
            memory_1[address] = data;
            //  cache.write_cache(address, core, false,lru_bool);
            // cache.accesses--;
        }
        else
        {
            throw std::out_of_range("Error: Attempted to write from out of bounds memory address. 13");
        }
    }
    else if (core == 2)
    {
        if (address < MEMORY_SIZE)
        {
            memory_2[address] = data;
            // cache.write_cache(address, core, false,lru_bool);
            // cache.accesses--;
        }
        else
        {
            throw std::out_of_range("Error: Attempted to write from out of bounds memory address. 14");
        }
    }
}

std::vector<int> memory::read_instruction_1(int32_t address, int core, bool lru_bool)
{
    // Check cache for instruction
    // if(instructions_1[address][1]==-101)
    // {
    //     std::vector<int> instruction(4, 0);
    //     if(core==1)
    //     {
    //      instruction[1] = instructions_1[address][1];
    //     }
    //     return instruction;
    // }
    //     if(instructions_2[address][1]==-101)
    // {
    //     std::vector<int> instruction(4, 0);
    //     if(core==1)
    //     {
    //      instruction[1] = instructions_2[address][1];
    //     }
    //     return instruction;
    // }

    if (cache.read_cache(address, core, true))
    {
        std::vector<int> instruction(4, 0);
        if (core == 1)
        {
            for (int i = 0; i < 4; i++)
            {
                instruction[i] = instructions_1[address][i];
            }
        }
        else if (core == 2)
        {
            for (int i = 0; i < 4; i++)
            {
                instruction[i] = instructions_2[address][i];
            }
        }
        return instruction;
    }
    else
    {
        std::vector<int> instruction(4, 0);
        if (core == 1)
        {
            if (address < MEMORY_SIZE / 4)
            {
                for (int i = 0; i < 4; i++)
                {
                    instruction[i] = instructions_1[address][i];
                }
                cache.write_cache(address, core, true, lru_bool);
                ck3 = true;
                cache.accesses--;
            }
            else
            {
                throw std::out_of_range("Error: Attempted to read inst from out of bounds memory address. 15");
            }
        }
        else if (core == 2)
        {
            if (address < MEMORY_SIZE / 4)
            {
                for (int i = 0; i < 4; i++)
                {
                    instruction[i] = instructions_2[address][i];
                }
                cache.write_cache(address, core, true, lru_bool);
                ck4 = true;
                cache.accesses--;
            }
            else
            {
                throw std::out_of_range("Error: Attempted to read inst from out of bounds memory address. 16");
            }
        }
        return instruction;
    }
}

void memory::write_instruction_1(int32_t address, int encode[], int core, bool lru_bool)
{
    if (core == 1)
    {
        if (address < MEMORY_SIZE / 4)
        {
            for (int i = 0; i < 4; i++)
            {
                instructions_1[address][i] = encode[i];
            }
            // cache.write_cache(address, core, true,lru_bool);
        }
        else
        {
            throw std::out_of_range("Error: Attempted to write from out of bounds memory address. 17");
        }
    }
    else if (core == 2)
    {
        if (address < MEMORY_SIZE / 4)
        {
            for (int i = 0; i < 4; i++)
            {
                instructions_2[address][i] = encode[i];
            }
            //  cache.write_cache(address, core, true,lru_bool);
        }
        else
        {
            throw std::out_of_range("Error: Attempted to write from out of bounds memory address. 18");
        }
    }
}

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
            throw std::out_of_range("Error: Attempted to read from out of bounds memory address.11");
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
            throw std::out_of_range("Error: Attempted to read from out of bounds memory address.12");
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
            throw std::out_of_range("Error: Attempted to write from out of bounds memory address. 13");
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
            throw std::out_of_range("Error: Attempted to write from out of bounds memory address. 14");
        }
    }
}
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
            throw std::out_of_range("Error: Attempted to read inst from out of bounds memory address. 15");
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
            throw std::out_of_range("Error: Attempted to read inst from out of bounds memory address. 16");
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
            throw std::out_of_range("Error: Attempted to write from out of bounds memory address. 17");
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
            throw std::out_of_range("Error: Attempted to write from out of bounds memory address. 18");
        }
    }
}
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
std::string memory::read_str(int32_t address, int core)
{
    std::pair<std::string, std::string> p;
    std::string result = "";

    if (core == 1)
    {
        for (auto &entry : strmap_1)
        {
            if (entry.second == address)
            {
                result = entry.first.second;
                break;
            }
        }
    }
    else if (core == 2)
    {
        for (auto &entry : strmap_2)
        {
            if (entry.second == address)
            {
                result = entry.first.second;
                break;
            }
        }
    }

    return result;
}

double memory::missrate_count(int core)
{
    return cache.calculate_miss_rate(core);
}

double memory::miss_count(int core)
{
    return cache.calculate_miss(core);
}

void memory::print_needed()
{
    cache.print_cache();
}