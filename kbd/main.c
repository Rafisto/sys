#include "idt/idt.h"

#include "keyboard/keyboard.h"
#include "screen/screen.h"
#include "console/console.h"

void init_interrupt() {
    interrupt_descriptor_table_init();
    load_idt_entry(0x21, (unsigned long) keyboard_handler_interrupt, 0x08, 0x8e);
    keyboard_init(console_input);
}

int kmain(void) {
    init_interrupt();

    screen_clear();

    write_string("Welcome to my keyboard-interruptable system.\n");
    console_prompt();
   
    while(1) __asm__("hlt\n\t");
}
