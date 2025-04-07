#include "memory.h"

memory_region_t memory_regions[MAX_USABLE_REGIONS];
size_t memory_region_count = 0;

void* memset(void* dest, int value, size_t len) {
    uint8_t* ptr = (uint8_t*)dest;
    while (len-- > 0) {
        *ptr++ = (uint8_t)value;
    }
    return dest;
}