#ifndef MEMORY_MEMORY_H
#define MEMORY_MEMORY_H

#include "../kernel/types.h"
#include "../serial/serial.h"

typedef struct {
    uint64_t base;
    uint64_t length;
} memory_region_t;

#define MAX_USABLE_REGIONS 64

extern memory_region_t memory_regions[MAX_USABLE_REGIONS];
extern size_t memory_region_count;

#define FRAME_SIZE 4096
#define MAX_FRAMES 512

extern uint8_t bitmap[MAX_FRAMES / 8];
void dump_bitmap();

void* memset(void* dest, int val, size_t len);

#endif // MEMORY_MEMORY_H