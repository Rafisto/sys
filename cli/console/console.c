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

// Custom string tokenization function
void tokenize_input(char *input, char **argv, int *argc)
{
    *argc = 0;
    while (*input)
    {
        // Skip leading spaces
        while (*input == ' ')
        {
            input++;
        }
        if (*input == '\0')
        {
            break; // End of input
        }
        // Start of a new token
        argv[(*argc)++] = input;
        // Find the end of the token
        while (*input && *input != ' ')
        {
            input++;
        }
        // Null-terminate the token
        if (*input)
        {
            *input++ = '\0';
        }
    }
}

void analyze_command(char *input)
{
    char *argv[16]; // Max 16 arguments, TODO: use kalloc
    int argc = 0;

    tokenize_input(input, argv, &argc);

    for (int i = 0; i < get_commands_count(); i++)
    {
        if (strcmp(argv[0], get_commands()[i].name))
        {
            COMMANDS[i].handler(argc, argv);
            return;
        }
    }

    write_string("Command not found: ");
    write_string(argv[0]);
    write_string("\n");
}