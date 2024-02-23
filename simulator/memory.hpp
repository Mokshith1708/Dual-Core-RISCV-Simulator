#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <map>
using namespace std;
const int32_t MEMORY_SIZE = 512;
class memory
{
public:
    int32_t memory_1[MEMORY_SIZE];
    map<pair<string, string>, int> strmap_1;
    int32_t instructions_1[MEMORY_SIZE][4];
    int32_t memory_2[MEMORY_SIZE];
    map<pair<string, string>, int> strmap_2;
    int32_t instructions_2[MEMORY_SIZE][4];

public:
    int32_t read_memory(int32_t address, int core);
    void write_memory(int32_t address, int32_t data, int core);
    vector<int> read_instruction(int32_t address, int core);
    void write_instruction(int32_t address, int encode[], int core);
    void write_str(string s1, string s2, int address_str, int core);
};