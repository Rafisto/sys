#ifndef BASE_SCREEN_H
#define BASE_SCREEN_H

#include "../kernel/types.h"

#define VIDEO_MEMORY 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT * 2)

#define write_format(fmt, ...) \
    do { \
        uint32_t args[] = { __VA_ARGS__ }; \
        write_format_impl(fmt, args, sizeof(args) / sizeof(args[0])); \
    } while (0)

static int current_color = 0x0F; 
static int cursor_row = 0;
static int cursor_column = 0;

void screen_clear();
void screen_write_at(int row, int column, char character, int color);
void set_color(int fg, int bg);
void write_char(char character);
void write_string(const char *str);
void write_format_impl(const char *fmt, uint32_t *args, int arg_count);
void write_int(int num);
void write_hex(int num);
void write_blahaj();
void remove_char();

#endif // BASE_SCREEN_H
