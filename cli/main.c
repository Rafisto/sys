#include "idt/idt.h"

#include "keyboard/keyboard.h"
#include "screen/screen.h"
#include "console/console.h"

#include "kernel/types.h"

#include "memory/frame.h"
#include "memory/paging.h"

void init_interrupt() {
    interrupt_descriptor_table_init();
    load_idt_entry(0x21, (unsigned long) asm_keyboard_handler_interrupt, 0x08, 0x8e);
    keyboard_init(console_input);
}

int kmain(void) {
    volatile unsigned int *addr = (unsigned int *)0x10000;
    *addr = 0xDEADBEEF;

    write_string("Initializing kernel...\n");
    screen_clear();

    write_string("Initializing interrupts...\n");
    init_interrupt();

    write_string("Initializing frame allocator...\n");
    init_frame_allocator();

    write_string("Initializing paging...\n");
    init_paging();

    write_string("Welcome to my tiny system.\n");
    console_prompt();
   
    while(1) __asm__("hlt\n\t");
}
