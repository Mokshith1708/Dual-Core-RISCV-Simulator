#include "SharedCache.hpp"
#include "simulator.hpp"
#include "cores.hpp"
SharedCache::SharedCache(int cacheSize, int blockSize, int associativity)
    : cacheSize(cacheSize), blockSize(blockSize), associativity(associativity)
{
    // Calculate the number of sets in the cache
    sets = cacheSize / (blockSize * associativity);
    // Initialize cache entries
    cache.resize(sets, std::vector<CacheEntry>(associativity));
}

bool SharedCache::read_cache(int32_t address, int core, bool isInstruction)
{
    int setIndex = (address / blockSize) % sets;
    int tag = address * blockSize / cacheSize;
    int offset = address % blockSize;

    for (int i = 0; i < associativity; ++i)
    {
        if (cache[setIndex][i].valid && cache[setIndex][i].tag == tag && cache[setIndex][i].coreBit == core &&
            cache[setIndex][i].isInstruction == isInstruction)
        {
            // Cache hit
            // You can use the offset here if needed
            return true;
        }
    }
    // Cache miss
    return false;
}

void SharedCache::write_cache(int32_t address, int core, bool isInstruction)
{
    int setIndex = (address / blockSize) % sets;
    int tag = address * blockSize / cacheSize;
    int offset = address % blockSize;

    // Find an empty slot or evict one according to replacement policy
    int emptySlot = -1;
    for (int i = 0; i < associativity; ++i)
    {
        if (!cache[setIndex][i].valid)
        {
            emptySlot = i;
            break;
        }
    }
    if (emptySlot != -1)
    {
        // Found an empty slot
        cache[setIndex][emptySlot].valid = true;
        cache[setIndex][emptySlot].tag = tag;
        cache[setIndex][emptySlot].offset = offset;
        cache[setIndex][emptySlot].coreBit = core;
        cache[setIndex][emptySlot].isInstruction = isInstruction;
    }
    else
    {
        // Evict a block (use LRU or other replacement policy)
        // Here, let's assume LRU
        int lruIndex = 0;
        for (int i = 1; i < associativity; ++i)
        {
            if (cache[setIndex][i].valid && cache[setIndex][i].tag < cache[setIndex][lruIndex].tag)
            {
                lruIndex = i;
            }
        }
        // Replace the evicted block with new data
        cache[setIndex][lruIndex].tag = tag;
        cache[setIndex][lruIndex].offset = offset;
        cache[setIndex][lruIndex].coreBit = core;
        cache[setIndex][lruIndex].isInstruction = isInstruction;
    }
}
