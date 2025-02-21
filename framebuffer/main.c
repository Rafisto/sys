#include "framebuffer.h"
#include "cube.h"
#include "time.h"

void kmain(uint32_t multiboot_info_address) {
    multiboot2_tag_t *tag;
    for (tag = (multiboot2_tag_t *)(multiboot_info_address + 8);
         tag->type != 0; 
         tag = (multiboot2_tag_t *)((uint8_t *)tag + ((tag->size + 7) & ~7))) {
        if (tag->type == 8) {
            framebuffer = (framebuffer_tag_t *)tag;
            framebuffer_addr = (uint32_t *)(uint64_t)framebuffer->framebuffer_addr;
            break;
        }
    }

    if (framebuffer == NULL) {
        while(1) __asm__("hlt\n\t");
    }

    while (1) {
        rotate_cube(0.01f, 0.01f, 0.01f);
        draw_cube(0xFF00FF);

        sleep(100);
    }

    while(1) __asm__("hlt\n\t");
}
