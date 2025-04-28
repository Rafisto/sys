#ifndef BASE_COMMANDS_H
#define BASE_COMMANDS_H

#include "../screen/framebuffer.h"

void help_command(int argc, char **argv);
void clear_command(int argc, char **argv);
void uptime_command(int argc, char **argv);
void shutdown_command(int argc, char **argv);
void reboot_command(int argc, char **argv);
void echo_command(int argc, char **argv);
void sum_command(int argc, char **argv);
void meminfo_command(int argc, char **argv);
void oslogo_command(int argc, char **argv);

typedef void (*cmdhandler)(int argc, char **argv);

typedef struct
{
    const char *name;
    cmdhandler handler;
} Command;

static Command COMMANDS[] = {
    {"help", help_command},
    {"clear", clear_command},
    {"shutdown", shutdown_command},
    {"reboot", reboot_command},
    {"echo", echo_command},
    {"sum", sum_command},
    {"meminfo", meminfo_command},
    {"oslogo", oslogo_command}
};

static int get_commands_count() {
    return sizeof(COMMANDS) / sizeof(Command);
}

static Command* get_commands() {
    return &COMMANDS[0];
}

#endif // BASE_COMMANDS_H