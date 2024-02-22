#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
const int32_t MAX_REG = 32;
class registers
{
private:
    int32_t registers[MAX_REG];

public:
    int32_t read(int32_t address) const;
    void write(int32_t address, int32_t data);
};