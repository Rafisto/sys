#ifndef KERNEL_INTERRUPT_IDT_H
#define KERNEL_INTERRUPT_IDT_H

#include "../x86.h"
#include "../types.h"

#include "../../serial/uart.h"

extern void pit_handler_asm();
extern void page_fault_handler_asm();

#define IDT_SIZE 256
#define IDT_ENTRIES 256

#define PIC_1_CTRL 0x20
#define PIC_2_CTRL 0xA0
#define PIC_1_DATA 0x21
#define PIC_2_DATA 0xA1

struct idt_entry_t
{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short int offset_higherbits;
} __attribute__((packed));

struct idt_pointer_t
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

extern struct idt_entry_t idt_table[IDT_SIZE];
extern struct idt_pointer_t idt_ptr;

void load_idt_entry(int isr_number, unsigned long base, short int selector, unsigned char flags);
void idt_init();

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND 0x43
#define PIT_FREQUENCY 1193182
#define PIT_HZ 1000000

void pit_init();
void pit_handler();
void page_fault_handler();

#endif // KERNEL_INTERRUPT_IDT_H
