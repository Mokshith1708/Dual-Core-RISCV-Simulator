#include "SharedCache.hpp"
#include <iostream>

SharedCache::SharedCache(int cacheSize, int blockSize, int associativity)
    : cacheSize(cacheSize), blockSize(blockSize), associativity(associativity),
      accesses(0), misses1(0), misses2(0),misses(0)
{
    sets = cacheSize / (blockSize * associativity);
    cache.resize(sets, std::vector<CacheEntry>(associativity));
}

bool SharedCache::read_cache(int32_t address, int core, bool isInstruction)
{
    accesses++;
 //std::cout<< 1<<" | "<< "its a access "<<core<<" "<< address<<" "<<accesses<<" "<<isInstruction<<std::endl;
    int setIndex = (address / blockSize) % sets;
    int tag = (address * blockSize) / cacheSize;
    int offset = address % blockSize;

    for (auto &entry : cache[setIndex])
    {
        if (entry.valid && entry.tag == tag && entry.isInstruction == isInstruction && entry.coreBit == core && entry.offset == offset )
        {
          //   std::cout<< 1<<" | "<< "its a hit "<<core<<" "<< address<<std::endl;
            return true; // hit
        }
    }

    // miss
    misses++;
    // std::cout<< 1<<" | "<< "its a miss "<<core<<" "<< address<<std::endl;
    if(core==1)
    {
     misses1++;
    }
    else if(core==2)
    {
        misses2++;
    }
    return false;
}

void SharedCache::write_cache(int32_t address, int core, bool isInstruction,bool lru_bool)
{
    accesses++;
    // if(core==1)
    // {
    //     access1++;
    // }
    //  if(core==2)
    // {
    //     access2++;
    // }
    int setIndex = (address / blockSize) % sets;
    int tag = (address * blockSize) / cacheSize;

    for (auto &entry : cache[setIndex])
    {
        if (!entry.valid)
        {
            // Found an empty slot
            entry.valid = true;
            entry.tag = tag;
            entry.isInstruction = isInstruction;
            entry.coreBit = core;
            entry.offset = address % blockSize;
            entry.data_or_instructions.resize(blockSize, -1);
            entry.data_or_instructions[entry.offset]=address;
            return;
        }
    }

    if (lru_bool)
    {//LRU
        cache[setIndex].front().tag = tag;
        cache[setIndex].front().isInstruction = isInstruction;
        cache[setIndex].front().coreBit = core;
        cache[setIndex].front().offset = address % blockSize;
        cache[setIndex].front().data_or_instructions[cache[setIndex].front().offset]=address;
    }
    else
    {
        // Random
        int randomIndex = rand() % associativity;
        cache[setIndex][randomIndex].tag = tag;
        cache[setIndex][randomIndex].isInstruction = isInstruction;
        cache[setIndex][randomIndex].coreBit = core;
        cache[setIndex][randomIndex].offset = address % blockSize;
        cache[setIndex][randomIndex].data_or_instructions[cache[setIndex][randomIndex].offset]=address;
    }
}
double SharedCache::calculate_miss_rate(int core)
{
    if (accesses == 0)
    {
        return 0.0;
    }
    else
    {
        // if(core==1)
        // {
        // return static_cast<double>(misses1);
        // }
        // else if(core==2)
        // {
        // return static_cast<double>(misses2);
        // }
        // else
        // {
        //     return static_cast<double>(misses);
        // }
        return static_cast<double>(misses1+misses2)/accesses;
    }
}

double SharedCache::calculate_miss(int core)
{
    if (accesses == 0)
    {
        return 0.0;
    }
    else
    {
        if(core==1)
        {
        return static_cast<double>(misses1);
        }
        else if(core==2)
        {
        return static_cast<double>(misses2);
        }
        else
        {
            return static_cast<double>(misses);
        }
        
    }
}

void SharedCache::print_cache()
{
    for (int setIndex = 0; setIndex < sets; ++setIndex)
    {
        std::cout << "Set " << setIndex << ":\n";
        for (int way = 0; way < associativity; ++way)
        {
            CacheEntry &entry = cache[setIndex][way];
            std::cout << "  Way " << way << ": ";
            if (entry.valid)
            {
                std::cout << "Valid, Tag: " << entry.tag << ", Offset: " << entry.offset << ", CoreBit: " << entry.coreBit << ", ";
                if (entry.isInstruction)
                {
                    std::cout << "Instructions: ";
                    for (auto &inst : entry.data_or_instructions)
                    {
                        std::cout << inst << " ";
                    }
                }
                else
                {
                    std::cout << "Data: ";
                    for (auto &d : entry.data_or_instructions)
                    {
                        std::cout << d << " ";
                    }
                }
            }
            else
            {
                std::cout << "Invalid";
            }
            std::cout << std::endl;
        }
    }
}