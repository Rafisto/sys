#ifndef MEMORY_PAGING_H
#define MEMORY_PAGING_H

#include "../kernel/types.h"
#include "../serial/serial.h"
#include "physical.h"

#define PAGE_SIZE     4096
#define PAGE_TABLE_ENTRIES 512

#define PTE_PRESENT    (1 << 0)
#define PTE_WRITABLE   (1 << 1)
#define PTE_USER       (1 << 2)
#define PTE_HUGE_PAGE  (1 << 7)

// Allocate and initialize page tables
uint32_t* init_paging() {
    // Allocate page directory (must be page-aligned)
    uint32_t* page_directory = (uint32_t*)allocate_frame();

    memset(page_directory, 0, PAGE_SIZE);
    
    // Identity map first 4MB (for transition)
    // First entry points to a page table that identity maps 0-4MB
    uint32_t* first_page_table = (uint32_t*)allocate_frame();
    page_directory[0] = (uintptr_t)first_page_table | PTE_PRESENT | PTE_WRITABLE;
    
    // Fill the first page table with identity mappings
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | PTE_PRESENT | PTE_WRITABLE;
    }
    
    // Optional: Map kernel to higher half (e.g., 0xC0000000)
    // This requires additional page tables
    
    return page_directory;
}

void enable_paging(uint32_t* page_directory_phys) {
    // Set page directory base address (CR3)
    asm volatile("mov %0, %%cr3" :: "r"(page_directory_phys));
    
    // Enable PSE (Page Size Extension) for 4MB pages if desired
    uint32_t cr4;
    asm volatile("mov %%cr4, %0" : "=r"(cr4));
    cr4 |= (1 << 4); // Set PSE bit
    asm volatile("mov %0, %%cr4" :: "r"(cr4));
    
    // Enable paging in CR0
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= (1 << 31); // Set PG bit
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
    
    // Flush the TLB by reloading CR3
    asm volatile("mov %0, %%cr3" :: "r"(page_directory_phys));
}

#endif // MEMORY_PAGING_H