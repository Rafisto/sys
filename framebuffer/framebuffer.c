#include "framebuffer.h"

framebuffer_tag_t *framebuffer = NULL;
uint32_t *framebuffer_addr = NULL;

void clear_framebuffer() {
    if (framebuffer == NULL) {
        return;
    }

    uint32_t color = 0x000000;
    uint32_t total_pixels = framebuffer->framebuffer_width * framebuffer->framebuffer_height;
    uint32_t i = 0;

    for (; i + 4 <= total_pixels; i += 4) {
        framebuffer_addr[i] = color;
        framebuffer_addr[i + 1] = color;
        framebuffer_addr[i + 2] = color;
        framebuffer_addr[i + 3] = color;
    }

    for (; i < total_pixels; i++) {
        framebuffer_addr[i] = color;
    }
}


typedef long int __m256i __attribute__((__vector_size__(32)));

static inline __m256i _mm256_loadu_si256(const void *mem_addr) {
    return *(__m256i *)mem_addr;
}

static inline void _mm256_storeu_si256(void *mem_addr, __m256i a) {
    *(__m256i *)mem_addr = a;
}


void memcpy_avx2(uint32_t *dst, const uint32_t *src, uint32_t pixel_count) {
    uint32_t i = 0;
    uint32_t simd_end = pixel_count & ~7;

    for (; i < simd_end; i += 8) {
        __m256i data = _mm256_loadu_si256((const __m256i *)&src[i]);
        _mm256_storeu_si256((__m256i *)&dst[i], data);
    }

    for (; i < pixel_count; i++) {
        dst[i] = src[i];
    }
}

swap_frame_t swap_frames[8];

void set_framebuffer(swap_frame_t *swap_frame) {
    if (framebuffer == NULL) {
        return;
    }

    memcpy_avx2(framebuffer_addr, swap_frame->pixels, framebuffer->framebuffer_width * framebuffer->framebuffer_height);
}

void clear_swap_frame(swap_frame_t *swap_frame) {
    for (int i = 0; i < framebuffer->framebuffer_width * framebuffer->framebuffer_height; i++) {
        swap_frame->pixels[i] = 0x000000;
    }
}