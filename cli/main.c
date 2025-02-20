#include "idt/idt.h"

#include "keyboard/keyboard.h"
#include "screen/screen.h"
#include "console/console.h"

#include "kernel/types.h"

#include "memory/frame.h"

void init_interrupt() {
    interrupt_descriptor_table_init();
    load_idt_entry(0x21, (unsigned long) asm_keyboard_handler_interrupt, 0x08, 0x8e);
    keyboard_init(console_input);
}

int kmain(void) {
    screen_clear();
    write_string("Initializing kernel...\n");
    write_string("Initializing interrupts...\n");
    init_interrupt();

    write_format("[kernel] [%x-%x] %d bytes\n", (uint32_t) &start_text, (uint32_t) &end_data, &end_data - &start_text);
    write_format("[stack] [%x-%x] %d bytes\n", (uint32_t) &stack_begin, (uint32_t) &stack_end, &stack_end - &stack_begin);
    write_format("[bss] [%x-%x] %d bytes\n", (uint32_t) &start_bss, (uint32_t) &end_bss, &end_bss - &start_bss);
    
    write_string("Initializing memory frame allocator...\n");

    init_frame_allocator();

    write_format("[frame] [%x-%x] %d bytes\n", (uint32_t) &frame_map, (uint32_t) &frame_map + sizeof(frame_map), sizeof(frame_map));

    write_string("First frame address: 0x");
    write_hex((uint32_t) frame_map[0].address);
    write_string("\n");
    
    int rc = kalloc(5100);
    if (rc == -1) {
        write_string("Failed to allocate frame\n");
    } else {
        write_string("Allocated 2 frames at address: 0x");
        write_hex(rc);
        write_string("\n");
    }

    write_string("Writing to allocated memory...\n");
    char *ptr = (char *) rc;
    for (int i = 0; i < 5100; i++) {
        ptr[i] = 'W';
    }

    write_string("First 10 bytes of allocated memory: ");
    for (int i = 0; i < 10; i++) {
        write_char(ptr[i]);
    }
    write_string("\n");

    write_string("Initializing console...\n");
    write_string("Welcome to my keyboard-interruptable system.\n");
    console_prompt();
   
    while(1) __asm__("hlt\n\t");
}
