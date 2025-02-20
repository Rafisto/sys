#include "paging.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void init_paging() {
    int i;
    for(i = 0; i < 1024; i++)
    {
        page_directory[i] = 0x00000002;
    }

    for(i = 0; i < 1024; i++)
    {
        first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
    }

    page_directory[0] = ((unsigned int)first_page_table) | 3;

    asm_load_page_directory(page_directory);
    asm_enable_paging();
}