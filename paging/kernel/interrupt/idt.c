#include "idt.h"

struct idt_entry_t idt_table[IDT_SIZE];
struct idt_pointer_t idt_ptr;

static void initialize_idt_pointer()
{
    idt_ptr.limit = (sizeof(struct idt_entry_t) * IDT_SIZE) - 1;
    idt_ptr.base = (unsigned int)&idt_table;
}

static void initialize_pic()
{
    outb(PIC_1_CTRL, 0x11);
    outb(PIC_2_CTRL, 0x11);

    outb(PIC_1_DATA, 0x20);
    outb(PIC_2_DATA, 0x28);

    outb(PIC_1_DATA, 0x04);
    outb(PIC_2_DATA, 0x02);

    outb(PIC_1_DATA, 0x01);
    outb(PIC_2_DATA, 0x01);

    // Mask all interrupts on master PIC (PIC1)
    outb(PIC_1_DATA, 0xFF);
    outb(PIC_2_DATA, 0xFF); 

    outb(PIC_1_DATA, 0xFE);
    outb(PIC_2_DATA, 0xFE); 
}

void load_idt_entry(int isr_number, unsigned long base, short int selector, unsigned char flags)
{
    idt_table[isr_number].offset_lowerbits = base & 0xFFFF;
    idt_table[isr_number].offset_higherbits = (base >> 16) & 0xFFFF;
    idt_table[isr_number].selector = selector;
    idt_table[isr_number].flags = flags;
    idt_table[isr_number].zero = 0;
}

void idt_init()
{
    initialize_pic();
    initialize_idt_pointer();

    load_idt_entry(32, (unsigned long)pit_handler_asm, 0x08, 0x8E); // IRQ0
    load_idt_entry(14, (unsigned long)page_fault_handler_asm, 0x08, 0x8E); // page fault
    load_idt(&idt_ptr);
}

void pit_init()
{
    int divisor = PIT_FREQUENCY / PIT_HZ;

    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
}

void pit_handler()
{
    outb(0x20, 0x20);
}

void page_fault_handler() {
    unsigned long fault_address;
    asm volatile("movl %%cr2, %0" : "=r"(fault_address));
    sprintf("Page fault at address: %x\n", fault_address);
    outb(0x20, 0x20);
}

