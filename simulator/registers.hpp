#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
const uint32_t MAX_REG = 32;
class registers
{
private:
    uint32_t registers[MAX_REG];

public:
    uint32_t read(uint32_t address) const;
    void write(uint32_t address, uint32_t data);
};