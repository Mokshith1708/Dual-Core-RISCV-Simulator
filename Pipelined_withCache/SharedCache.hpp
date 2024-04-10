#pragma once
#include <cstdint>
#include <vector>
#include <stdexcept> // For std::out_of_range

class SharedCache {
public:
    struct CacheEntry {
        bool valid;
        int32_t tag;
        bool isInstruction; // Indicates whether the cache block stores instructions or data
        int coreBit;        // Indicates which core the data belongs to (0 for core 1, 1 for core 2)
        std::vector<int32_t> data_or_instructions; // Data or instructions stored in the cache block
        int offset;         // Offset within the cache block
        CacheEntry() : valid(false), tag(-1), isInstruction(false), coreBit(0), offset(-1) {}
    };

    int cacheSize;      // in bytes
    int blockSize;      // in bytes
    int associativity;
    int sets;
    int accesses,access1,access2;       // Total cache accesses
    int misses1,misses2,misses;         // Cache misses
    std::vector<std::vector<CacheEntry>> cache;

public:
    SharedCache(int cacheSize, int blockSize, int associativity);

    bool read_cache(int32_t address, int core, bool isInstruction);
    void write_cache(int32_t address, int core, bool isInstruction,bool lru_bool);
    double calculate_miss_rate(int core);
    double calculate_miss(int core);
    void print_cache();
};
