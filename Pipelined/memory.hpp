#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include "SharedCache.hpp"

const int32_t MEMORY_SIZE = 512;

class memory
{
public:
    SharedCache cache;
    int32_t memory_1[MEMORY_SIZE]={0};
    std::map<std::pair<std::string, std::string>, int> strmap_1;
    int16_t instructions_1[MEMORY_SIZE/4][4]={0};
    int32_t memory_2[MEMORY_SIZE]={0};
    std::map<std::pair<std::string, std::string>, int> strmap_2;
    int16_t instructions_2[MEMORY_SIZE/4][4]={0};

public:
    memory(int cacheSize, int blockSize, int associativity)
        : cache(cacheSize, blockSize, associativity) {} // Initialize cache in the constructor
    SharedCache& getCache() {
        // Return a reference to the SharedCache object
        return cache;
    }
    int32_t read_memory_1(int32_t address, int core);
    void write_memory_1(int32_t address, int32_t data, int core);
    std::vector<int> read_instruction_1(int32_t address, int core);
    void write_instruction_1(int32_t address, int encode[], int core);
     int32_t read_memory(int32_t address, int core);
    void write_memory(int32_t address, int32_t data, int core);
    std::vector<int> read_instruction(int32_t address, int core);
    void write_instruction(int32_t address, int encode[], int core);
    void write_str(std::string s1, std::string s2, int address_str, int core);
    std::string read_str(int32_t address, int core);
    double missrate_count();
    void print_needed();

};
