#include "commands.h"
#include "../screen/fscreen.h"
#include "../asm/asm.h"

#include "../utils/atoi.h"

extern int atoi(const char *str);

void help_command(int argc, char **argv) {
    write_string("Available commands:\n");
    for (int i = 0; i < get_commands_count(); i++) {
        write_string(get_commands()[i].name);
        write_string(", ");
    }
    write_string("\n");
}

void clear_command(int argc, char **argv) {
    screen_clear();
}

void shutdown_command(int argc, char **argv) {
    asm_shutdown();
}

void reboot_command(int argc, char **argv) {
    asm_reboot();
}

void echo_command(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        write_string(argv[i]);
        write_string(" ");
    }
    write_string("\n");
}

void sum_command(int argc, char **argv) {
    int sum = 0;
    for (int i = 1; i < argc; i++) {
        sum += atoi(argv[i]);
    }
    write_format("%d\n", sum);
}

void meminfo_command(int argc, char **argv) {
    write_format("[kernel] [%x-%x] %d bytes\n", (uint32_t) &start_text, (uint32_t) &end_data, &end_data - &start_text);
    write_format("[stack] [%x-%x] %d bytes\n", (uint32_t) &stack_begin, (uint32_t) &stack_end, &stack_end - &stack_begin);
    write_format("[bss] [%x-%x] %d bytes\n", (uint32_t) &start_bss, (uint32_t) &end_bss, &end_bss - &start_bss);
}

void blahaj_command(int argc, char **argv) {
    write_blahaj();
}
