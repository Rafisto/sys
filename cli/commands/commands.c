#include "commands.h"
#include "../screen/screen.h"
#include "../asm/asm.h"

void help_command() {
    write_string("Available commands:\n");
    for (int i = 0; i < get_commands_count(); i++) {
        write_string(get_commands()[i].name);
        write_string(", ");
    }
    write_string("\n");
}

void clear_command() {
    screen_clear();
}

void shutdown_command() {
    asm_shutdown();
}

void reboot_command() {
    asm_reboot();
}