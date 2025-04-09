#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

#define NULL 0

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;
typedef unsigned long size_t;
typedef unsigned long uintptr_t;

// stack

extern uint32_t stack_begin;
extern uint32_t stack_end;

// linker symbols

// text - code
extern uint32_t start_text;
extern uint32_t end_text;

// data - initialized data
extern uint32_t start_data;
extern uint32_t end_data;

// bss - block started by symbol - uninitialized data
extern uint32_t start_bss;
extern uint32_t end_bss;

/* 
    MULTIBOOT2 HEADERS
    Refer to the Multiboot2 specification for more information on the tags.
    https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html
*/

// 3.1.3 General tag structure
typedef struct {
    uint16_t type;
    uint16_t flags;
    uint32_t size;
} multiboot2_tag_t;

// 3.1.10 The framebuffer tag
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

// 3.6.8 Memory map tag
typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
} multiboot2_mmap_tag_t;

typedef struct {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} multiboot2_mmap_entry_t;

#endif // KERNEL_TYPES_H