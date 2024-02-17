#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
const uint32_t MEMORY_SIZE = 512;
class memory
{
private:
    uint32_t memory_1[MEMORY_SIZE];
    uint32_t instructions_1[MEMORY_SIZE][4];
    uint32_t memory_2[MEMORY_SIZE][4];
    uint32_t instructions_2[MEMORY_SIZE];

public:
    uint32_t read_memory(uint32_t address, uint32_t arr[]) const;
    void write_memory(uint32_t address, uint32_t data, uint32_t arr[]);
    uint32_t read_instruction(uint32_t address, uint32_t arr[]) const;
    void write_instruction(uint32_t address,vector<int>, uint32_t arr[]);
};