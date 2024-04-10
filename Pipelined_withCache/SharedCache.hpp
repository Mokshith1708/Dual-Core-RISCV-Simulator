#pragma once
#include <cstdint>
#include <vector>
#include <stdexcept>

class SharedCache {
public:
    struct CacheEntry {
        bool valid;
        int32_t tag;
        bool isInstruction;
        int coreBit;
        std::vector<int32_t> data_or_instructions; 
        int offset;
        CacheEntry() : valid(false), tag(-1), isInstruction(false), coreBit(0), offset(-1) {}
    };

    int cacheSize;      // in bytes
    int blockSize;      // in bytes
    int associativity;
    int sets;
    int accesses,access1,access2;
    int misses1,misses2,misses;
    std::vector<std::vector<CacheEntry>> cache;

public:
    SharedCache(int cacheSize, int blockSize, int associativity);

    bool read_cache(int32_t address, int core, bool isInstruction);
    void write_cache(int32_t address, int core, bool isInstruction,bool lru_bool);
    double calculate_miss_rate(int core);
    double calculate_miss(int core);
    void print_cache();
};
