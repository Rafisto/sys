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
    for (uint32_t i = 0; i < MAX_FRAMES ; i++)
    {
        serial_write_format("[max frames] %d\n", MAX_FRAMES);
        serial_write_format("[pmem] checking frame %d\n", i);
        serial_write_format("[pmem] bitmap[%d] = %x\n", i / 8, bitmap[i / 8]);
        serial_write_format("[pmem] bitmap[%d] & %x = %x\n", i / 8, (1 << (i % 8)), bitmap[i / 8] & (1 << (i % 8)));
        serial_write_format("[pmem] test_frame(%d) = %d\n", i, test_frame(i));
        serial_write_format("[pmem] frame %d = %x\n", i, (i * FRAME_SIZE));
        if (test_frame(i))
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

    // mark the kernel memory as used
    uint32_t kernel_start = (uint32_t)&start_text;
    uint32_t kernel_end = (uint32_t)&end_bss;
    serial_write_format("[pmem] kernel memory: %x - %x\n", kernel_start, kernel_end);
    for (uint32_t j = (kernel_start / FRAME_SIZE) * FRAME_SIZE; j < ((kernel_end + FRAME_SIZE - 1) / FRAME_SIZE) * FRAME_SIZE; j += FRAME_SIZE)
    {
        uint32_t frame = j / FRAME_SIZE;
        if (frame < MAX_FRAMES)
        {
            set_frame(frame);
        }
    }
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
        if (!test_frame(i))
        {
            free_memory += FRAME_SIZE;
        }
    }

    serial_write_format("[pmem] free memory: %d KB\n", free_memory / 1000);
}