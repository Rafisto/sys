#ifndef MEMORY_PAGING_H
#define MEMORY_PAGING_H

#include "../kernel/types.h"
#include "../asm/asm.h"

extern uint32_t page_directory[1024] __attribute__((aligned(4096)));
extern uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void init_paging();

#endif // MEMORY_PAGING_H