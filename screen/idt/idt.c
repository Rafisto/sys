#include "idt.h"

struct idt_entry idt_table[IDT_SIZE];
struct idt_pointer idt_ptr;

static void initialize_idt_pointer()
{
    idt_ptr.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idt_ptr.base = (unsigned int)&idt_table;
}

static void initialize_pic()
{
    asm_write_port(PIC_1_CTRL, 0x11);
    asm_write_port(PIC_2_CTRL, 0x11);

    asm_write_port(PIC_1_DATA, 0x20);
    asm_write_port(PIC_2_DATA, 0x28);

    asm_write_port(PIC_1_DATA, 0x04);
    asm_write_port(PIC_2_DATA, 0x02);

    asm_write_port(PIC_1_DATA, 0x01);
    asm_write_port(PIC_2_DATA, 0x01);

    asm_write_port(0x21, 0xff);
    asm_write_port(0xA1, 0xff);
}

void load_idt_entry(int isr_number, unsigned long base, short int selector, unsigned char flags)
{
    idt_table[isr_number].offset_lowerbits = base & 0xFFFF;
    idt_table[isr_number].offset_higherbits = (base >> 16) & 0xFFFF;
    idt_table[isr_number].selector = selector;
    idt_table[isr_number].flags = flags;
    idt_table[isr_number].zero = 0;
}

void load_idt_default_entry(unsigned long base, short int selector, unsigned char flags)
{
    for (int i = 0; i < 10; i++)
    {
        idt_table[i].offset_lowerbits = base & 0xFFFF;
        idt_table[i].offset_higherbits = (base >> 16) & 0xFFFF;
        idt_table[i].selector = selector;
        idt_table[i].flags = flags;
        idt_table[i].zero = 0;
    }
}

void interrupt_descriptor_table_init()
{
    initialize_pic();
    initialize_idt_pointer();
    asm_load_idt(&idt_ptr);
}
