#include "physical.h"
#include "memory.h"

static inline void mark_frame(uint32_t frame, int bit)
{
    if (bit) bitmap[frame / 8] |= (1 << (frame % 8));
    else bitmap[frame / 8] &= ~(1 << (frame % 8));
}

static inline int frame_free(uint32_t frame)
{
    return (bitmap[frame / 8] & (1 << (frame % 8))) == 0;    
}

void *allocate_frame()
{
    for (uint32_t i = 0; i < MAX_FRAMES ; i++)
    {
        if (frame_free(i))
        {
            mark_frame(i, 1);
            return (void *)(i * FRAME_SIZE);
        }
    }
    return NULL;
}

void free_frame(void *addr)
{
    uint32_t frame = (uint32_t)addr / FRAME_SIZE;
    mark_frame(frame, 0);
}

int is_frame_free(void *addr)
{
    uint32_t frame = (uint32_t)addr / FRAME_SIZE;
    return frame_free(frame);
}

void init_physical_memory()
{
    memset(bitmap, 0, sizeof(bitmap));

    uint32_t kernel_start = (uint32_t)&start_text;
    uint32_t kernel_end = (uintptr_t)&end_bss;
    serial_write_format("[pmem] kernel memory: %x - %x\n", kernel_start, kernel_end);

    uint32_t frame = 0;
    while(frame * FRAME_SIZE < kernel_end) {
        mark_frame(frame, 1);
        frame++;
    }

    dump_bitmap();
    
    serial_write_format("[pmem] kernel memory marked as used\n");

    uint32_t total_memory = 0;
    for (size_t i = 0; i < memory_region_count; i++)
    {
        total_memory += memory_regions[i].length;
    }
    serial_write_format("[pmem] available memory: %d KB\n", total_memory / 1000);

    uint32_t free_memory = 0;
    for (size_t i = 0; i < MAX_FRAMES; i++)
    {
        if (!frame_free(i))
        {
            free_memory += FRAME_SIZE;
        }
    }

    serial_write_format("[pmem] free memory: %d KB\n", free_memory / 1000);
}