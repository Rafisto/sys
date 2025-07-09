#include "paging.h"

__attribute__((aligned(PAGE_SIZE))) uint32_t entrypgdir[PAGE_DIR_SIZE] = {
    [IDENTITY_MAP_ENTRY] = IDENTITY_MAP | (PAGE_PS | PAGE_PRESENT | PAGE_RW),
    [KERNEL_MAP_ENTRY] = KERNEL_LINK | (PAGE_PS | PAGE_PRESENT | PAGE_RW),
};

__attribute__((aligned(PAGE_SIZE))) uint32_t procpgdir[PAGE_DIR_SIZE] = {
    [IDENTITY_MAP_ENTRY] = IDENTITY_MAP | (PAGE_PS | PAGE_PRESENT | PAGE_RW),
    [KERNEL_MAP_ENTRY] = KERNEL_LINK | (PAGE_PS | PAGE_PRESENT | PAGE_RW),
};

void dynamic_identity_map(uint32_t phys_addr, uint32_t flags) {
  uint32_t pd_index = phys_addr >> 22;
  entrypgdir[pd_index] = (phys_addr & 0xFFC00000) | PAGE_PS | flags;
  asm volatile("invlpg (%0)" ::"r"(phys_addr) : "memory");
}
