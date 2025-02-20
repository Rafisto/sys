#include "frame.h"

pageframe_t frame_map[NPAGES];

int init_frame_allocator() {
    for (int i = 0; i < NPAGES; i++) {
        frame_map[i].address = (uint32_t) (&stack_end) + i * 0x1000;
        frame_map[i].status = 0;
    }
    return 0;
}

int kalloc_frame_int(pageframe_t* frame) {
    for (int i = 0; i < NPAGES; i++) {
        if (frame_map[i].status == 0) {
            frame_map[i].status = 1;
            *frame = frame_map[i];
            return 0;
        }
    }
    return -1;
}

int kalloc_frame() {
    pageframe_t* frame;
    if (kalloc_frame_int(frame) == 0) {
        return frame->address;
    } else {
        return -1;
    }
}

int kalloc(uint32_t size) {
    int nframes = size / 0x1000;
    if (size % 0x1000 != 0) {
        nframes++;
    }
    for (int i = 0; i < NPAGES; i++) {
        if (frame_map[i].status == 0) {
            int j = 0;
            for (j = 0; j < nframes; j++) {
                if (frame_map[i + j].status != 0) {
                    break;
                }
            }
            if (j == nframes) {
                for (int k = 0; k < nframes; k++) {
                    frame_map[i + k].status = 1;
                }
                return frame_map[i].address;
            }
        }
    }
    return -1;
}

int kfree_frame(uint32_t address) {
    for (int i = 0; i < NPAGES; i++) {
        if (frame_map[i].address == address) {
            frame_map[i].status = 0;
            return 0;
        }
    }
    return -1;
}