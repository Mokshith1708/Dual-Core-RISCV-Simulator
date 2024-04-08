#pragma once
#include <cstdint>
#include <vector>

class SharedCache {
public:
    struct CacheEntry {
        bool valid;
        int32_t tag;
        int offset;  // Offset within the cache block
        int coreBit; // Indicates which core the data belongs to (0 for core 1, 1 for core 2)
        bool isInstruction; // Indicates whether the cached entry is for an instruction or data
        CacheEntry() : valid(false), tag(-1), offset(-1), coreBit(0), isInstruction(false) {}
    };

    int cacheSize; // in bytes
    int blockSize; // in bytes
    int associativity;
    int sets;
    int accesses; // Total cache accesses
    int misses;   // Cache misses
    std::vector<std::vector<CacheEntry>> cache;

public:
    SharedCache(int cacheSize, int blockSize, int associativity);

    bool read_cache(int32_t address, int core, bool isInstruction);
    void write_cache(int32_t address, int core, bool isInstruction);
    double calculate_miss_rate();
    void print_cache();
};
