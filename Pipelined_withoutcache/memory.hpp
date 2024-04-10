#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <map>

const int32_t MEMORY_SIZE = 512;

class memory
{
public:
    int32_t memory_1[MEMORY_SIZE]={0};
    std::map<std::pair<std::string, std::string>, int> strmap_1;
    int16_t instructions_1[MEMORY_SIZE/4][4]={0};
    int32_t memory_2[MEMORY_SIZE]={0};
    std::map<std::pair<std::string, std::string>, int> strmap_2;
    int16_t instructions_2[MEMORY_SIZE/4][4]={0};

public:
    int32_t read_memory(int32_t address, int core);
    void write_memory(int32_t address, int32_t data, int core);
    std::vector<int> read_instruction(int32_t address, int core);
    void write_instruction(int32_t address, int encode[], int core);
    void write_str(std::string s1, std::string s2, int address_str, int core);
    std::string read_str(int32_t address, int core);
};
