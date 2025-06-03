#include "memory/paging.h"
#include "interrupt/idt.h"
#include "proc/tasks.h"

#include "../boot/multiboot.h"
#include "../serial/uart.h"

#include "../dev/rtc/rtc.h"
#include "../dev/ps2/mouse.h"
#include "../dev/fb/fscreen.h"


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


static void task_entry(uint32_t id) {
    // do a software interrupt
	asm("int $0x80" :: "a"(id));
	// there's no memory protection so we can write directly to vga buffer
	// (just to show that it's still running)
	uint8_t a = 0;
    char t = '1';
	while (1) {
        t++;
        if (t > '9') t = '1';
        fb_screen_write_at(0, id * 2, t, 0x0F);
        
	}

	// IMPORTANT: all tasks need to end in an infinite loop, otherwise
	// the cpu will just continue executing garbage code from here
}

void kmain()
{
    slog("Kernel Booting...");
    print_entrypgdir();
    slog("Kernel framebuffer address %x", (unsigned int)framebuffer_addr);

    slog("Dynamically identity-mapping framebuffer at %x", framebuffer->framebuffer_addr);
    dynamic_identity_map(framebuffer->framebuffer_addr, PAGE_PRESENT | PAGE_RW);

    print_entrypgdir();

    setup_gdt();

    idt_init();
	pit_init(1000);
    setup_tasks();

	create_task(1, (uint32_t) task_entry, 0xC80000, 0xC00000, 0);
	create_task(2, (uint32_t) task_entry, 0xD80000, 0xD00000, 0);
	create_task(3, (uint32_t) task_entry, 0xE80000, 0xE00000, 0);
	create_task(4, (uint32_t) task_entry, 0xF80000, 0xF00000, 0);

    enable_interrupts();

    slog("Kernel loaded");

    rtc_time time;
    rtc_get_time(&time);

    slog("RTC Time: h=%d m=%d s=%d day=%d/%d/%d",
         time.hours, time.minutes, time.seconds,
         time.day, time.month, time.year);

    while (1)
    {
        asm volatile("hlt");
    }
}