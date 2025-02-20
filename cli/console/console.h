#ifndef BASE_CONSOLE_H
#define BASE_CONSOLE_H

#include "../screen/screen.h"
#include "../commands/commands.h"

static char buffer[64];
static int buffer_index = 0;

void analyze_command(char *command);
void clear_buffer();
void console_prompt();
void console_input(char character);
int strcmp(const char *a, const char *b);

#endif // BASE_CONSOLE_H
