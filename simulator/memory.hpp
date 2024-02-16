#pragma once
#include<iostream>
#include <cstdint>
#include <vector>
const uint32_t MEMORY_SIZE = 1024;
class memory
{
private:
    uint32_t memory[MEMORY_SIZE];

public:
    uint32_t read(uint32_t address) const;
    void write(uint32_t address, uint32_t data);
};