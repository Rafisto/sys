#ifndef BASE_COMMANDS_H
#define BASE_COMMANDS_H

void help_command();
void clear_command();
void uptime_command();
void shutdown_command();
void reboot_command();
void echo_command();
void sum_command();
void meminfo_command();
void blahaj_command();

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
    {"blahaj", blahaj_command}
};

static int get_commands_count() {
    return sizeof(COMMANDS) / sizeof(Command);
}

static Command* get_commands() {
    return &COMMANDS[0];
}

#endif // BASE_COMMANDS_H