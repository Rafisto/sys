#ifndef MMAP_H
#define MMAP_H

#include "../kernel/types.h"
#include "../serial/serial.h"

#include "memory.h"

int init_mmap(uint32_t multiboot_info_address);

#endif // MMAP_H