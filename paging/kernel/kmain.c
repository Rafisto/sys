#include "memory/paging.h"
#include "interrupt/idt.h"

#include "../boot/multiboot.h"
#include "../serial/uart.h"

#include "../dev/rtc/rtc.h"
#include "../dev/ps2/mouse.h"

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

    idt_init();
	pit_init(1000);

    enable_interrupts();

    slog("Kernel loaded");

    rtc_time time;
    rtc_get_time(&time);

    slog("RTC Time: h=%d m=%d s=%d day=%d/%d/%d",
         time.hours, time.minutes, time.seconds,
         time.day, time.month, time.year);

    uint8_t *fb = (uint8_t *)framebuffer->framebuffer_addr;
    uint32_t color = 0xFF00FF;
    uint32_t black = 0x000000;
    mouse_packet packet;
    mouse_init();

    int32_t mouse_x = framebuffer->framebuffer_width / 2;
    int32_t mouse_y = framebuffer->framebuffer_height / 2;

    while (1)
    {
        if (mouse_poll(&packet))
        {
            // Erase previous square
            for (uint32_t dy = 0; dy < 10; dy++)
            {
                for (uint32_t dx = 0; dx < 10; dx++)
                {
                    uint32_t offset = (mouse_y + dy) * framebuffer->framebuffer_pitch + (mouse_x + dx) * 4;
                    fb[offset + 0] = (black >> 0) & 0xFF;
                    fb[offset + 1] = (black >> 8) & 0xFF;
                    fb[offset + 2] = (black >> 16) & 0xFF;
                    fb[offset + 3] = 0x00;
                }
            }

            // Update position
            mouse_x += packet.dx;
            mouse_y -= packet.dy;

            // Clamp to bounds
            if (mouse_x < 0)
                mouse_x = 0;
            if (mouse_y < 0)
                mouse_y = 0;
            if (mouse_x > (int)(framebuffer->framebuffer_width - 10))
                mouse_x = framebuffer->framebuffer_width - 10;
            if (mouse_y > (int)(framebuffer->framebuffer_height - 10))
                mouse_y = framebuffer->framebuffer_height - 10;

            // Draw new square
            for (uint32_t dy = 0; dy < 10; dy++)
            {
                for (uint32_t dx = 0; dx < 10; dx++)
                {
                    uint32_t offset = (mouse_y + dy) * framebuffer->framebuffer_pitch + (mouse_x + dx) * 4;
                    fb[offset + 0] = (color >> 0) & 0xFF;
                    fb[offset + 1] = (color >> 8) & 0xFF;
                    fb[offset + 2] = (color >> 16) & 0xFF;
                    fb[offset + 3] = 0x00;
                }
            }
        }
    }
}