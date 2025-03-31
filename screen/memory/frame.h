#ifndef MEMORY_FRAME_H
#define MEMORY_FRAME_H

#include "../kernel/types.h"

#define NPAGES 1024

typedef struct
{
    uint32_t address;
    uint32_t status;
} pageframe_t;

extern pageframe_t frame_map[NPAGES];

int init_frame_allocator();
int kalloc_frame_int(pageframe_t* frame);
int kalloc_frame();
int kalloc(uint32_t size);
int kfree_frame(uint32_t address);

#endif // MEMORY_FRAME_H
