#include "memory/paging.h"

#include "../boot/multiboot.h"
#include "../serial/uart.h"

void print_entrypgdir()
{
    sprint("Page Directory:\n");
    for (int i = 0; i < PAGE_DIR_SIZE; i++)
    {
        if (entrypgdir[i] != 0)
        {
            sprint("-> ");
            sprinth(i);
            sprint(": ");
            sprinth(entrypgdir[i]);
            sprint("\n");
        }
    }
}

void kmain()
{
    slog("Kernel Booting...");
    print_entrypgdir();
    slog("Kernel framebuffer address %x", (unsigned int)framebuffer_addr);

    slog("Dynamically identity-mapping framebuffer at %x", framebuffer->framebuffer_addr);
    dynamic_identity_map(framebuffer->framebuffer_addr, PAGE_PRESENT | PAGE_RW);
    print_entrypgdir();

    slog("Printing a pixel");
    uint8_t *fb = (uint8_t *)framebuffer->framebuffer_addr;
    uint32_t x = 100;
    uint32_t y = 100;
    uint32_t color = 0x00FF00; // green in 0x00RRGGBB

    uint32_t offset = y * framebuffer->framebuffer_pitch + x * 4; // 4 bytes per pixel (32bpp)
    fb[offset + 0] = (color >> 0) & 0xFF;                         // Blue
    fb[offset + 1] = (color >> 8) & 0xFF;                         // Green
    fb[offset + 2] = (color >> 16) & 0xFF;                        // Red
    fb[offset + 3] = 0x00;                                        // Alpha or padding

    slog("Kernel loaded");

    while (1)
    {
        asm volatile("hlt");
    }
}
