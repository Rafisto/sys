#include "physical.h"

static inline void set_frame(uint32_t frame)
{
    bitmap[frame / 8] |= (1 << (frame % 8));
}

static inline void clear_frame(uint32_t frame)
{
    bitmap[frame / 8] &= ~(1 << (frame % 8));
}

static inline int test_frame(uint32_t frame)
{
    return bitmap[frame / 8] & (1 << (frame % 8));
}

void *allocate_frame()
{
    for (uint32_t i = 0; i < MAX_FRAMES; i++)
    {
        if (!test_frame(i))
        {
            set_frame(i);
            return (void *)(i * FRAME_SIZE);
        }
    }
    return NULL;
}

void free_frame(void *addr)
{
    uint32_t frame = (uint32_t)addr / FRAME_SIZE;
    clear_frame(frame);
}

int is_frame_free(void *addr)
{
    uint32_t frame = (uint32_t)addr / FRAME_SIZE;
    return !test_frame(frame);
}

void init_physical_memory()
{
    memset(bitmap, 0, sizeof(bitmap));

    for (size_t i = 0; i < memory_region_count; i++)
    {
        serial_write_format("[pmem] usable region %d: base_addr = %x, length = %x, type = %d\n", i, memory_regions[i].base, memory_regions[i].length, 1);
        uint64_t start = memory_regions[i].base;
        uint64_t end = start + memory_regions[i].length;

        for (uint64_t j = start; j < end; j += FRAME_SIZE)
        {
            uint32_t frame = j / FRAME_SIZE;
            if (frame < MAX_FRAMES)
            {
                clear_frame(frame);
            }
        }
    }
    for (uint32_t i = 0; i < 256; i++)
    {
        set_frame(i);
    }

    uint64_t free_memory = 0;
    for (size_t i = 0; i < MAX_FRAMES; i++)
    {
        if (!test_frame(i))
        {
            free_memory += FRAME_SIZE;
        }
    }

    serial_write_format("[pmem] free memory: %d KB\n", (uint32_t)(free_memory / 1024));

    void *page = allocate_frame();
    if (page)
    {
        serial_write_format("[pmem] allocated page: %x\n", (uint32_t)page);
    }
    else
    {
        serial_write_format("[pmem] failed to allocate page\n");
    }

    free_memory = 0;
    for (size_t i = 0; i < MAX_FRAMES; i++)
    {
        if (!test_frame(i))
        {
            free_memory += FRAME_SIZE;
        }
    }

    serial_write_format("[pmem] free memory after allocation: %d KB\n", (uint32_t)(free_memory / 1024));

    free_frame(page);
    serial_write_format("[pmem] freed page: %x\n", (uint32_t)page);
    if (is_frame_free(page))
    {
        serial_write_format("[pmem] page is free: %x\n", (uint32_t)page);
    }
    else
    {
        serial_write_format("[pmem] page is not free: %x\n", (uint32_t)page);
    }
}