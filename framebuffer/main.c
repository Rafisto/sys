// https://github.com/klange/toaruos/blob/master/kernel/arch/x86_64/main.c#L92
// https://f.osdev.org/viewtopic.php?t=33144

#include "kernel/types.h"

#define NULL 0

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef struct {
    uint16_t type;
    uint16_t flags;
    uint32_t size;
} multiboot_tag_t;

typedef struct {
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
} multiboot_tag_framebuffer_t;

void kmain(uint32_t multiboot_info_address) {
    multiboot_tag_t *tag;
    for (tag = (multiboot_tag_t *)(multiboot_info_address + 8);
         tag->type != 0; 
         tag = (multiboot_tag_t *)((uint8_t *)tag + ((tag->size + 7) & ~7))) {
        if (tag->type == 8) {
            multiboot_tag_framebuffer_t *fb = (multiboot_tag_framebuffer_t *)tag;
            uint32_t *framebuffer = (uint32_t *)(uint64_t)fb->framebuffer_addr;
            uint32_t x = 100;
            uint32_t y = 100;
            uint32_t color = 0x00FF00; // Green
            framebuffer[y * fb->framebuffer_pitch / 4 + x] = color;
            break;
        }
    }
}