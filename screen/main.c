#include "idt/idt.h"

#include "keyboard/keyboard.h"
#include "console/console.h"

#include "kernel/types.h"

#include "memory/frame.h"
#include "memory/paging.h"

#include "screen/framebuffer.h"
#include "screen/fscreen.h"

void init_interrupt() {
    interrupt_descriptor_table_init();
    load_idt_entry(0x21, (unsigned long) asm_keyboard_handler_interrupt, 0x08, 0x8e);
    keyboard_init(console_input);
}

void kmain(uint32_t multiboot_info_address) {
    multiboot2_tag_t *tag;
    for (tag = (multiboot2_tag_t *)(multiboot_info_address + 8);
         tag->type != 0; 
         tag = (multiboot2_tag_t *)((uint8_t *)tag + ((tag->size + 7) & ~7))) {
        if (tag->type == 8) {
            framebuffer = (framebuffer_tag_t *)tag;
            framebuffer_addr = (uint32_t *)(uint64_t)framebuffer->framebuffer_addr;
            break;
        }
    }

    if (framebuffer == NULL) {
        while(1) __asm__("hlt\n\t");
    }

    print_os_logo();

    for (int i = 0; i < 6; i++) {
        write_string("\n");
    }

    write_string("OS by Rafal Wlodarczyk\n");

    write_string("Initializing kernel...\n");

    write_string("Initializing interrupts...\n");
    init_interrupt();

    write_string("Initializing frame allocator...\n");
    init_frame_allocator();

    // write_string("Initializing paging...\n");
    // init_paging();

    write_string("Welcome to my tiny system.\n");
    console_prompt();
   
    while(1) __asm__("hlt\n\t");
}
