#include "idt/idt.h"

#include "keyboard/keyboard.h"
#include "console/console.h"

#include "kernel/types.h"

#include "memory/memory.h"
#include "screen/framebuffer.h"
#include "screen/fscreen.h"

#include "memory/mmap.h"
#include "memory/physical.h"

#include "serial/serial.h"

void init_interrupt()
{
    interrupt_descriptor_table_init();
    load_idt_entry(0x21, (unsigned long)asm_keyboard_handler_interrupt, 0x08, 0x8e);
    keyboard_init(console_input);
}

void kmain(uint32_t multiboot_info_address)
{
    multiboot2_tag_t *tag;
    for (tag = (multiboot2_tag_t *)(multiboot_info_address + 8);
         tag->type != 0;
         tag = (multiboot2_tag_t *)((uint8_t *)tag + ((tag->size + 7) & ~7)))
    {
        if (tag->type == 6)
        {
            multiboot2_mmap_tag_t *mmap_tag = (multiboot2_mmap_tag_t *)tag;
            multiboot2_mmap_entry_t *entry = (multiboot2_mmap_entry_t *)(mmap_tag + 1);
            serial_write_format("[Multiboot][I] Found Memory Map at %x\n", (uint32_t)mmap_tag);
            for (size_t i = 0; i < mmap_tag->size; i += mmap_tag->entry_size)
            {
                entry = (multiboot2_mmap_entry_t *)((uint8_t *)entry + mmap_tag->entry_size);
                serial_write_format("[Multiboot] Memory region %d: base_addr = %x, length = %x, type = %d\n",
                                    i / mmap_tag->entry_size, entry->base_addr, entry->length, entry->type);
            }
        }
        if (tag->type == 8)
        {
            framebuffer = (framebuffer_tag_t *)tag;
            framebuffer_addr = (uint32_t *)(uint64_t)framebuffer->framebuffer_addr;
            serial_write_format("[Multiboot][I] Found Framebuffer at %x\n", framebuffer->framebuffer_addr);
            serial_write_format("[Multiboot] Framebuffer size: %dx%d(%d bpp)\n",
                                framebuffer->framebuffer_width,
                                framebuffer->framebuffer_height,
                                framebuffer->framebuffer_bpp);
        }
    }

    if (framebuffer == NULL)
    {
        while (1)
            __asm__("hlt\n\t");
    }

    for (int i = 0; i < 6; i++)
    {
        write_string("\n");
    }

    print_os_logo();

    write_string("OS by rafist0\n");
    serial_print("OS by rafist0\n");
    
    init_interrupt();
    init_mmap(multiboot_info_address);

    write_string("Welcome to my tiny system.\n");
    console_prompt();

    // allocate a frame
    void *frame = allocate_frame();
    void *frame2 = allocate_frame();
    if (frame)
    {
        serial_write_format("[pmem] Frame number: %x\n", (uint32_t)frame);
        serial_write_format("[pmem] Frame2 number: %x\n", (uint32_t)frame2);
    }
    else
    {
        serial_print("[pmem] Failed to allocate frame\n");
    }

    // free the frame
    free_frame(frame);
    serial_write_format("[pmem] Freed frame at %x\n", (uint32_t)frame);

    // check if the frame is free
    if (is_frame_free(frame))
    {
        serial_write_format("[pmem] Frame at %x is free\n", (uint32_t)frame);
    }
    else
    {
        serial_print("[pmem] Frame is not free\n");
    }

    // this literally works like arduino serial port
    char buffer[10];
    while (1) serial_read(0, 10);
}
