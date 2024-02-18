#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
using namespace std;
const uint32_t MEMORY_SIZE = 512;
class memory
{
private:
    uint32_t memory_1[MEMORY_SIZE];
    uint32_t instructions_1[MEMORY_SIZE][4];
    uint32_t memory_2[MEMORY_SIZE];
    uint32_t instructions_2[MEMORY_SIZE][4];

public:
    uint32_t read_memory(uint32_t address, int core) const;
    void write_memory(uint32_t address, uint32_t data, int core);
    vector<int> read_instruction(uint32_t address, int core);
    void write_instruction(uint32_t address, int encode[], int core);
};