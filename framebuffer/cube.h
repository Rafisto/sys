#ifndef FRAMEBUFFER_CUBE_H
#define FRAMEBUFFER_CUBE_H

#include "framebuffer.h"

typedef struct {
    float x;
    float y;
    float z;
} Point;

typedef struct {
    Point points[8];
} Cube;

void draw_cube();
void draw_point(float x, float y, uint32_t color);
void draw_line(float x0, float y0, float x1, float y1, uint32_t color);
void rotate_cube(float angle_x, float angle_y, float angle_z);

#endif // FRAMEBUFFER_CUBE_H