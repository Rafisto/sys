#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../kernel/types.h"

extern framebuffer_tag_t *framebuffer;
extern uint32_t *framebuffer_addr;

void print_pixel(uint32_t x, uint32_t y, uint32_t color);
void print_character(uint32_t x, uint32_t y, char character, uint32_t color);
void print_background(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void print_os_logo();
void clear_framebuffer();

#endif // FRAMEBUFFER_H