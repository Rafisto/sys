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

void clear_buffer()
{
    for (int i = 0; i < buffer_index; i++)
    {
        buffer[i] = 0;
    }
    buffer_index = 0;
}

void analyze_command(char *command)
{
    if (strcmp(command, "blahaj"))
    {
        write_blahaj();
    }
    else if (strcmp(command, "clear"))
    {
        screen_clear();
    }
    else if (strcmp(command, "help"))
    {
        write_string("Available commands:\n");
        write_string("blahaj - draw a picture of Blahaj\n");
        write_string("clear - clear the screen\n");
        write_string("help - show this help message\n");
    }
    else
    {
        if (buffer_index != 0)
        {
            write_string("unknown ");
            write_string(command);
            write_string("\n");
        }
    }
}
