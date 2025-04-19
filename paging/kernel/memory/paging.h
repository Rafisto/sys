#ifndef KERNEL_MEMORY_PAGIN_H
#define KERNEL_MEMORY_PAGING_H

#include "../types.h"

#define PAGE_SIZE 4096
#define PAGE_TABLE_SIZE 1024
#define PAGE_DIR_SIZE 1024

#define PAGE_PS 0x80 // Page Size Bit -> 4MB Page

#define PAGE_PRESENT 0x1 // Page Present Bit
#define PAGE_RW 0x2      // Read/Write Bit
#define PAGE_USER 0x4    // User/Supervisor Bit

#define IDENTITY_MAP 0x00000000
#define IDENTITY_MAP_ENTRY (IDENTITY_MAP >> 22)

#define KERNEL_LINK 0x00100000
#define KERNEL_VIRTUAL 0xC0000000
#define KERNEL_MAP_ENTRY (KERNEL_VIRTUAL >> 22)

#define PHYSICAL_TO_VIRTUAL(x) (KERNEL_VIRTUAL + (x))
#define VIRTUAL_TO_PHYSICAL(x) ((x) - KERNEL_VIRTUAL)

__attribute__((aligned(PAGE_SIZE)))
uint32_t entrypgdir[PAGE_DIR_SIZE] = {
    [IDENTITY_MAP_ENTRY] = IDENTITY_MAP | (PAGE_PS | PAGE_PRESENT | PAGE_RW),
    [KERNEL_MAP_ENTRY] = KERNEL_LINK | (PAGE_PS | PAGE_PRESENT | PAGE_RW),
};

static inline void dynamic_identity_map(uint32_t phys_addr, uint32_t flags) {
    uint32_t pd_index = phys_addr >> 22;
    entrypgdir[pd_index] = (phys_addr & 0xFFC00000) | PAGE_PS | flags;
    asm volatile("invlpg (%0)" :: "r"(phys_addr) : "memory");
}

#endif // KERNEL_MEMORY_PAGING_H