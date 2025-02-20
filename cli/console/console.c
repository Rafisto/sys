#include "console.h"


void console_prompt()
{
    write_string("> ");
}

void console_input(char character)
{
    if (character == '\n')
    {
        write_string("\n");
        analyze_command(buffer);
        clear_buffer();
        console_prompt();
    }
    else if (character == '\b')
    {
        if (buffer_index > 0)
        {
            remove_char();
            buffer_index--;
            buffer[buffer_index] = 0;
        }
    }
    else
    {
        if (buffer_index >= 64)
        {
            return;
        }
        buffer[buffer_index++] = character;
        write_char(character);
    }
}

void clear_buffer()
{
    for (int i = 0; i < buffer_index; i++)
    {
        buffer[i] = 0;
    }
    buffer_index = 0;
}

int strcmp(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (*a != *b)
        {
            return 0;
        }
        a++;
        b++;
    }
    return *a == *b;
}

void analyze_command(char *command)
{
    for (int i = 0; i < get_commands_count(); i++)
    {
        if (strcmp(command, get_commands()[i].name))
        {
            COMMANDS[i].handler();
            return;
        }
    }
    
    write_string("Command not found: ");
    write_string(command);
    write_string("\n");
}
