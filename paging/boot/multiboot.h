#ifndef BOOT_MULTIBOOT_H
#define BOOT_MULTIBOOT_H

#include "../kernel/types.h"
#include "../serial/uart.h"

typedef struct
{
    uint16_t type;
    uint16_t flags;
    uint32_t size;
} multiboot2_tag_t;

typedef struct
{
    uint16_t type;
    uint16_t flags;
    uint32_t size;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
    uint8_t reserved;
} framebuffer_tag_t;

typedef struct
{
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
} multiboot2_mmap_tag_t;

typedef struct
{
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} multiboot2_mmap_entry_t;

framebuffer_tag_t *framebuffer = NULL;
uint32_t *framebuffer_addr = NULL;

void multiboot_info(uint32_t multiboot_info_address)
{
    slog("Multiboot Info Address: %x", multiboot_info_address);

    multiboot2_tag_t *tag;
    for (tag = (multiboot2_tag_t *)(multiboot_info_address + 8);
         tag->type != 0;
         tag = (multiboot2_tag_t *)((uint8_t *)tag + ((tag->size + 7) & ~7)))
    {
        if (tag->type == 8)
        {
            framebuffer = (framebuffer_tag_t *)tag;
            framebuffer_addr = (uint32_t *)(uint64_t)framebuffer->framebuffer_addr;
            slog("Found Framebuffer at %x", framebuffer->framebuffer_addr);
            slog("Framebuffer size: %dx%d(%dbpp)",
                 framebuffer->framebuffer_width,
                 framebuffer->framebuffer_height,
                 framebuffer->framebuffer_bpp);
        }
    }
}

#endif // BOOT_MULTIBOOT_H