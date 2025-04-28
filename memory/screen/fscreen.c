#include "fscreen.h"
#include "framebuffer.h"

#define MAX_COLUMNS 160
#define MAX_ROWS 80

static int current_row = 0;
static int current_column = 0;
static int current_color = 0xFFFFFF;

int get_current_row() {
    return current_row;
}

int get_current_column() {
    return current_column;
}

void screen_clear() {
    clear_framebuffer();
    current_row = 0;
    current_column = 0;
}

void screen_write_at(int row, int column, char character, int color) {
    print_character(column * 8, row * 12, character, current_color);
}

void set_color(int r, int g, int b) {
    current_color = (r << 16) | (g << 8) | b;
}

void write_char(char character) {
    if (character == '\n') {
        current_row++;
        current_column = 0;
        if (current_row >= MAX_ROWS) {
            current_row = 0;
        }
        return;
    }

    screen_write_at(current_row, current_column, character, current_color);
    current_column++;
    if (current_column >= MAX_COLUMNS) {
        current_column = 0;
        current_row++;
    }
}

void write_string(const char *str) {
    while (*str) {
        write_char(*str++);
    }
}

void remove_char() {
    if (current_column == 0) {
        current_column = MAX_COLUMNS - 1;
        current_row--;
        if (current_row < 0) {
            current_row = MAX_ROWS - 1;
        }
    } else {
        current_column--;
    }

    print_background(current_column * 8, current_row * 12, 8, 12, 0x000000);
}

void write_int(int num) {
    if (num == 0) {
        write_char('0');
        return;
    }

    char buffer[50];
    int i = 0;
    while (num > 0) {
        buffer[i++] = num % 10 + '0';
        num /= 10;
    }

    while (i > 0) {
        write_char(buffer[--i]);
    }
}

void write_hex(int num) {
    if (num == 0) {
        write_char('0');
        return;
    }

    char buffer[50];
    int i = 0;
    while (num > 0) {
        int remainder = num % 16;
        if (remainder < 10) {
            buffer[i++] = remainder + '0';
        } else {
            buffer[i++] = remainder - 10 + 'A';
        }
        num /= 16;
    }

    while (i > 0) {
        write_char(buffer[--i]);
    }
}

void write_blahaj() {
    screen_clear();

    set_color(0x0F, 0x00, 0x00);
    write_string("[!] It is blahaj -");
    set_color(0x0D, 0x00, 0x00);
    write_string(" a cute little ");
    set_color(0x0F, 0x00, 0x00);
    write_string("shark");
    write_string("\n");

    const char *blahaj_art = 
"                                        ,(((/                                   "\
"                                      /(((((                                    "\
"                                    ((((#((                              (//    "\
"                                   (((((((.                           *(((/     "\
"                                 /(######/                          *((((/      "\
"                              *//\\%#####((/                         ((#((/       "\
"            ,*/********/////////////////(//*           (\\%*      ,((##((         "\
"      ,*/((///(//////////((/(///////(/////(////*,(*#((/(/((//////###(###(/(      "\
"  /(((((((//((///((////((((((/(((((((((((((((((/(((##((#\\%(##(/((///*(&#(##/    "\
"  /#((\\%(#(((((//#((((((((((((((((((((((((#(((((((((((/##(((((//((//*    ####(/  "\
"  (((###(###(#(#####(###############((#((((((((/((//(((#/(/////            ,,   "\
"      ,(###\\%####\\%&\\%#############(#(#(####(((((((/(((/////*//,                   "\
"          . .....*#(#######(((###(#(##(##(((/(/(/////,                          "\
"          .. ....,..........,..*#\\%#######/(                                     "\
"              ..  .............,*\\%\\%\\%#\\%((((/                                    "\
"                      **,,,****//*(##((###(#(((                                 "\
"                                      &#(#/#((((((((#                           ";

    int row = 2;
    int column = 0;

    for (const char *c = blahaj_art; *c; c++) {
        if (*c == '\n') {
            row++;
            column = 0;
        } else {
            screen_write_at(row, column++, *c, 0x0D00);
            if (column >= MAX_COLUMNS) {
                column = 0;
                row++;
            }
        }
    }

    set_color(0x0F, 0x00, 0x00);
}

void write_format_impl(const char *fmt, uint32_t *args, int arg_count) {
    int arg_index = 0;
    while (*fmt) {
        if (*fmt == '%' && *(fmt + 1)) {
            fmt++; // Move past '%'
            switch (*fmt) {
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
        } else {
            write_char(*fmt);
        }
        fmt++;
    }
}