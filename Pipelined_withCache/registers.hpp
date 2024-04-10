#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
const int32_t MAX_REG = 32;
class registers
{
public:
    int32_t registers1[MAX_REG] = {0};

public:
    int32_t read(int32_t address);
    void write(int32_t address, int32_t data);
};