#ifndef BASE_FSCREEN_H
#define BASE_FSCREEN_H

#include "../../kernel/types.h"

#define fb_sprintf(fmt, ...) \
    do { \
        uint32_t args[] = { __VA_ARGS__ }; \
        fb_sprintf_impl(fmt, args, sizeof(args) / sizeof(args[0])); \
    } while (0)

void fb_clear();
void fb_screen_write_at(int row, int column, char character, int color);
void fb_set_color(int R, int G, int B);
void fb_sprintc(char character);
void fb_sprint(const char *str);
void fb_sprintf_impl(const char *fmt, uint32_t *args, int arg_count);
void fb_sprinti(int num);
void fb_sprinth(int num);
void fb_rmchar();

#endif // BASE_FSCREEN_H