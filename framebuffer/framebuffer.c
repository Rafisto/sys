#include "framebuffer.h"

framebuffer_tag_t *framebuffer = NULL;
uint32_t *framebuffer_addr = NULL;

void clear_framebuffer() {
    if (framebuffer == NULL) {
        return;
    }

    uint32_t color = 0x000000;
    for (int i = 0; i < framebuffer->framebuffer_width * framebuffer->framebuffer_height; i++) {
        framebuffer_addr[i] = color;
    }
}