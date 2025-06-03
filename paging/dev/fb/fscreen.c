#include "fscreen.h"
#include "framebuffer.h"

#define MAX_COLUMNS 160
#define MAX_ROWS 80

static int current_row = 0;
static int current_column = 0;
static int current_color = 0xFFFFFF;

void fb_clear() {
    clear_framebuffer();
    current_row = 0;
    current_column = 0;
}

void fb_screen_write_at(int row, int column, char character, int color) {
    print_character(column * 8, row * 12, character, current_color);
}

void fb_set_color(int r, int g, int b) {
    current_color = (r << 16) | (g << 8) | b;
}

void fb_sprintc(char character) {
    if (character == '\n') {
        current_row++;
        current_column = 0;
        if (current_row >= MAX_ROWS) {
            current_row = 0;
        }
        return;
    }

    fb_screen_write_at(current_row, current_column, character, current_color);
    current_column++;
    if (current_column >= MAX_COLUMNS) {
        current_column = 0;
        current_row++;
    }
}

void fb_sprint(const char *str) {
    while (*str) {
        fb_sprintc(*str++);
    }
}

void fb_rmchar() {
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

void fb_sprinti(int num) {
    if (num == 0) {
        fb_sprintc('0');
        return;
    }

    char buffer[50];
    int i = 0;
    while (num > 0) {
        buffer[i++] = num % 10 + '0';
        num /= 10;
    }

    while (i > 0) {
        fb_sprintc(buffer[--i]);
    }
}

void fb_sprinth(int num) {
    if (num == 0) {
        fb_sprintc('0');
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
        fb_sprintc(buffer[--i]);
    }
}

void fb_sprintf_impl(const char *fmt, uint32_t *args, int arg_count) {
    int arg_index = 0;
    while (*fmt) {
        if (*fmt == '%' && *(fmt + 1)) {
            fmt++; // Move past '%'
            switch (*fmt) {
                case 'd':
                    if (arg_index < arg_count) fb_sprinti(args[arg_index++]);
                    break;
                case 'x':
                    fb_sprintc('0');
                    fb_sprintc('x');
                    if (arg_index < arg_count) fb_sprinth(args[arg_index++]);
                    break;
                default:
                    fb_sprintc('%');
                    fb_sprintc(*fmt);
                    break;
            }
        } else {
            fb_sprintc(*fmt);
        }
        fmt++;
    }
}