#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/* 
    Refer to the Multiboot2 specification for more information on the tags.
    https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html
*/

#define NULL 0

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

/*
    3.1.3 General tag structure
*/
typedef struct {
    uint16_t type;
    uint16_t flags;
    uint32_t size;
} multiboot2_tag_t;


/*
    3.1.10 The framebuffer tag of Multiboot2 header
*/
typedef struct {
    uint16_t type;
    uint16_t flags;
    uint32_t size;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
    uint8_t reserved;
} framebuffer_tag_t;

extern framebuffer_tag_t *framebuffer;
extern uint32_t *framebuffer_addr;

/*
    Create a swap frame struct (list of pixels) to store the pixels of the framebuffer
*/
typedef struct {
    uint32_t pixels[1024* 768];
} swap_frame_t;

// create 2 swap frames
extern swap_frame_t swap_frames[2];

void set_framebuffer(swap_frame_t *swap_frame);
void clear_swap_frame(swap_frame_t *swap_frame);
void print_pixel(uint32_t x, uint32_t y, uint32_t color);
void print_character(uint32_t x, uint32_t y, char character, uint32_t color);
void print_background(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void print_os_logo();
void clear_framebuffer();

#endif // FRAMEBUFFER_H