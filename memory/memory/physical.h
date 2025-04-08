#ifndef MEMORY_PHYSICAL_H
#define MEMORY_PHYSICAL_H

#include "../kernel/types.h"
#include "../serial/serial.h"
#include "../screen/framebuffer.h"

#include "memory.h"

void init_physical_memory();
void* allocate_frame();
void free_frame(void *frame);
int is_frame_free(void *frame);

#endif // MEMORY_PHYSICAL_H