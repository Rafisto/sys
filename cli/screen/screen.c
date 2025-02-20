#include "screen.h"

void screen_clear()
{
    char *video_memory = (char *)VIDEO_MEMORY;
    for (int i = 0; i < SCREEN_SIZE; i += 2)
    {
        video_memory[i] = ' ';
        video_memory[i + 1] = 0x00;
    }
    cursor_column=0;
    cursor_row=0;
}

void screen_write_at(int row, int column, char character, int color)
{
    char *video_memory = (char *)VIDEO_MEMORY;

    if (row >= 0 && row < SCREEN_HEIGHT && column >= 0 && column < SCREEN_WIDTH)
    {
        int position = (row * SCREEN_WIDTH + column) * 2;
        video_memory[position] = character;
        video_memory[position + 1] = color;
    }
}

void set_color(int fg, int bg)
{
    current_color = (bg << 4) | (fg & 0x0F);
}

void write_char(char character)
{
    char *video_memory = (char *)VIDEO_MEMORY;
    int position = cursor_row * SCREEN_WIDTH + cursor_column;

    if (character == '\n') {
        cursor_row++;
        cursor_column = 0;
        if (cursor_row > SCREEN_WIDTH) {
            cursor_row = 0;
        }
        if (cursor_column > SCREEN_HEIGHT) {
            cursor_column = 0;
        }
        return;
    }

    video_memory[position * 2] = character;
    video_memory[position * 2 + 1] = current_color;

    cursor_column++;
    if (cursor_column >= SCREEN_WIDTH)
    {
        cursor_column = 0;
        cursor_row++;
        if (cursor_row >= SCREEN_HEIGHT)
        {
            cursor_row = 0;
        }
    }
}

void remove_char()
{
    char *video_memory = (char *)VIDEO_MEMORY;
    int position = cursor_row * SCREEN_WIDTH + cursor_column;

    video_memory[position * 2] = ' ';
    video_memory[position * 2 + 1] = current_color;

    cursor_column--;
    if (cursor_column < 0)
    {
        cursor_column = SCREEN_WIDTH - 1;
        cursor_row--;
        if (cursor_row < 0)
        {
            cursor_row = SCREEN_HEIGHT - 1;
        }
    }
}

void write_string(const char *str)
{
    while (*str)
    {
        write_char(*str++);
    }
}

void write_int(int num)
{
    if (num == 0)
    {
        write_char('0');
        return;
    }

    char buffer[50];
    int i = 0;
    while (num > 0)
    {
        buffer[i++] = num % 10 + '0';
        num /= 10;
    }

    while (i > 0)
    {
        write_char(buffer[--i]);
    }
}

void write_hex(int num) {
    if (num == 0)
    {
        write_char('0');
        return;
    }

    char buffer[50];
    int i = 0;
    while (num > 0)
    {
        int remainder = num % 16;
        if (remainder < 10) {
            buffer[i++] = remainder + '0';
        } else {
            buffer[i++] = remainder - 10 + 'A';
        }
        num /= 16;
    }

    while (i > 0)
    {
        write_char(buffer[--i]);
    }
}

void write_blahaj() {
    screen_clear();

    set_color(0x0F, 0x00);
    write_string("[!] It is blahaj -");
    set_color(0x0D, 0x00);
    write_string(" a cute little ");
    set_color(0x0F, 0x00);
    write_string("shark");
    write_string("\n");

    char blahaj_art[] = 
"                                        ,(((/                                   "\
"                                      /(((((                                    "\
"                                    ((((#((                              (//    "\
"                                   (((((((.                           *(((/     "\
"                                 /(######/                          *((((/      "\
"                              *//\%#####((/                         ((#((/       "\
"            ,*/********/////////////////(//*           (\%*      ,((##((         "\
"      ,*/((///(//////////((/(///////(/////(////*,(*#((/(/((//////###(###(/(      "\ 
"  /(((((((//((///((////((((((/(((((((((((((((((/(((##((#\%(##(/((///*(&#(##/    "\
"  /#((\%(#(((((//#((((((((((((((((((((((((#(((((((((((/##(((((//((//*    ####(/  "\
"  (((###(###(#(#####(###############((#((((((((/((//(((#/(/////            ,,   "\
"      ,(###\%####\%&\%#############(#(#(####(((((((/(((/////*//,                   "\
"          . .....*#(#######(((###(#(##(##(((/(/(/////,                          "\
"          .. ....,..........,..*#\%#######/(                                     "\
"              ..  .............,*\%\%\%\%#\%((((/                                    "\
"                      **,,,****//*(##((###(#(((                                 "\
"                                      &#(#/#((((((((#                           ";

    char *video_memory = (char *)VIDEO_MEMORY;
    int offset = 2*2*80;
    set_color(0x0D, 0x00);
    for (int i = 80; i < 80*17; i++)
    {
        video_memory[2 * i + offset] = blahaj_art[i];
        video_memory[2 * i + 1 + offset] = current_color;
    }
    set_color(0x0F, 0x00);

    cursor_row = 24;
}

void write_format_impl(const char *fmt, uint32_t *args, int arg_count)
{
    int arg_index = 0;
    while (*fmt)
    {
        if (*fmt == '%' && *(fmt + 1))
        {
            fmt++; // Move past '%' 
            switch (*fmt)
            {
                case 'd':
                    if (arg_index < arg_count) write_int(args[arg_index++]);
                    break;
                case 'x':
                    write_char('0');
                    write_char('x');
                    if (arg_index < arg_count) write_hex(args[arg_index++]);
                    break;
                default:
                    write_char('%');
                    write_char(*fmt);
                    break;
            }
        }
        else
        {
            write_char(*fmt);
        }
        fmt++;
    }
}