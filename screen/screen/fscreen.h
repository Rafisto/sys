#ifndef BASE_FSCREEN_H
#define BASE_FSCREEN_H

#include "../kernel/types.h"

#define write_format(fmt, ...) \
    do { \
        uint32_t args[] = { __VA_ARGS__ }; \
        write_format_impl(fmt, args, sizeof(args) / sizeof(args[0])); \
    } while (0)

void screen_clear();
void screen_write_at(int row, int column, char character, int color);
void set_color(int R, int G, int B);
void write_char(char character);
void write_string(const char *str);
void write_format_impl(const char *fmt, uint32_t *args, int arg_count);
void write_int(int num);
void write_hex(int num);
void write_blahaj();
void remove_char();

#endif // BASE_FSCREEN_H