#ifndef KERNEL_GDT
#define KERNEL_GDT

#define NUM_GDT_ENTRIES 6
#define GDT_KERNEL_CODE 0x08
#define GDT_KERNEL_DATA 0x10
#define GDT_USER_CODE   0x18
#define GDT_USER_DATA   0x20
#define GDT_TSS         0x28

#define RPL_USER 3

#include "../types.h"
#include "../memory/memory.h"

typedef struct {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_mid;
	uint8_t  access;
	uint8_t  granularity;
	uint8_t  base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) gdt_pointer_t;

typedef struct {
	uint16_t previous_task, __previous_task_unused;
	uint32_t esp0;
	uint16_t ss0, __ss0_unused;
	uint32_t esp1;
	uint16_t ss1, __ss1_unused;
	uint32_t esp2;
	uint16_t ss2, __ss2_unused;
	uint32_t cr3;
	uint32_t eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	uint16_t es, __es_unused;
	uint16_t cs, __cs_unused;
	uint16_t ss, __ss_unused;
	uint16_t ds, __ds_unused;
	uint16_t fs, __fs_unused;
	uint16_t gs, __gs_unused;
	uint16_t ldt_selector, __ldt_sel_unused;
	uint16_t debug_flag, io_map;
} __attribute__((packed)) tss_t; 

extern void load_gdt(uint32_t ptr);
void setup_gdt();

#endif