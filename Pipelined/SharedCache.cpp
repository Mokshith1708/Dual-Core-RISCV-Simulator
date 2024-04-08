#include "SharedCache.hpp"

SharedCache::SharedCache(int cacheSize, int blockSize, int associativity)
    : cacheSize(cacheSize), blockSize(blockSize), associativity(associativity),
      accesses(0), misses(0)
{
    // Calculate the number of sets in the cache
    sets = cacheSize / (blockSize * associativity);
    // Initialize cache entries
    cache.resize(sets, std::vector<CacheEntry>(associativity));
}

bool SharedCache::read_cache(int32_t address, int core, bool isInstruction)
{
    // Increment total cache accesses
    accesses++;

    int setIndex = (address / blockSize) % sets;
    int tag = (address*blockSize)/cacheSize;

    // Search for the cache block in the set
    for (auto& entry : cache[setIndex]) {
        if (entry.valid && entry.tag == tag && entry.isInstruction == isInstruction) {
            // Cache hit
            return true;
        }
    }

    // Cache miss
    misses++;
    return false;
}

void SharedCache::write_cache(int32_t address, int core, bool isInstruction)
{
    // Increment total cache accesses
    accesses++;

    int setIndex = (address / blockSize) % sets;
    int tag = (address*blockSize)/cacheSize;

    // Search for an empty or invalid entry in the set
    for (auto& entry : cache[setIndex]) {
        if (!entry.valid) {
            // Found an empty slot
            entry.valid = true;
            entry.tag = tag;
            entry.isInstruction = isInstruction;
            entry.coreBit = core;                // Set the coreBit
            entry.offset = address % blockSize;  // Set the offset within the cache block
            // Resize the data_or_instructions vector based on the block size
            entry.data_or_instructions.resize(blockSize, 0);
            return;
        }
    }

    // If no empty slot found, use a replacement policy (e.g., LRU)
    // Here, we assume LRU and evict the first entry in the set
    cache[setIndex].front().tag = tag;
    cache[setIndex].front().isInstruction = isInstruction;
    cache[setIndex].front().coreBit = core;                // Set the coreBit
    cache[setIndex].front().offset = address % blockSize;  // Set the offset within the cache block
}

double SharedCache::calculate_miss_rate() 
{
    // Calculate miss rate as misses divided by total accesses
    if (accesses == 0) {
        return 0.0;
    } else {
        return static_cast<double>(misses) / accesses;
    }
}

void SharedCache::print_cache() {
    // Loop through each set and each way in the cache
    for (int setIndex = 0; setIndex < sets; ++setIndex) {
        std::cout << "Set " << setIndex << ":\n";
        for (int way = 0; way < associativity; ++way) {
            CacheEntry& entry = cache[setIndex][way];
            std::cout << "  Way " << way << ": ";
            if (entry.valid) {
                std::cout << "Valid, Tag: " << entry.tag << ", Offset: " << entry.offset << ", CoreBit: " << entry.coreBit << ", ";
                if (entry.isInstruction) {
                    // Print instructions stored in the cache block
                    std::cout << "Instructions: ";
                    for (auto& inst : entry.data_or_instructions) {
                        std::cout << inst << " ";
                    }
                } else {
                    // Print data stored in the cache block
                    std::cout << "Data: ";
                    for (auto& d : entry.data_or_instructions) {
                        std::cout << d << " ";
                    }
                }
            } else {
                std::cout << "Invalid";
            }
            std::cout << std::endl;
        }
    }
}