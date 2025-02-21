#include "cube.h"

Cube cube = {
    .points = {
        {-0.8f, -0.8f, -0.8f}, 
        {0.8f, -0.8f, -0.8f}, 
        {0.8f, 0.8f, -0.8f}, 
        {-0.8f, 0.8f, -0.8f}, 
        {-0.8f, -0.8f, 0.8f}, 
        {0.8f, -0.8f, 0.8f}, 
        {0.8f, 0.8f, 0.8f}, 
        {-0.8f, 0.8f, 0.8f}  
    }
};

float absf(float x) {
    return x < 0 ? -x : x;
}

float sin(float x) {
    float term = x;
    float sum = x;
    float x_squared = x * x;
    int n = 1;

    for (int i = 1; i <= 10; i++) {
        term *= -x_squared / ((2 * n) * (2 * n + 1));
        sum += term;
        n++;
    }

    return sum;
}

float cos(float x) {
    float term = 1.0f;
    float sum = 1.0f;
    float x_squared = x * x;
    int n = 1;

    for (int i = 1; i <= 10; i++) {
        term *= -x_squared / ((2 * n - 1) * (2 * n));
        sum += term;
        n++;
    }

    return sum;
}

void rotate_cube(float angle_x, float angle_y, float angle_z) {
    float cos_x = cos(angle_x);
    float sin_x = sin(angle_x);
    float cos_y = cos(angle_y);
    float sin_y = sin(angle_y);
    float cos_z = cos(angle_z);
    float sin_z = sin(angle_z);

    for (int i = 0; i < 8; i++) {
        float x = cube.points[i].x;
        float y = cube.points[i].y;
        float z = cube.points[i].z;

        float y1 = y * cos_x - z * sin_x;
        float z1 = y * sin_x + z * cos_x;
        y = y1;
        z = z1;

        float x1 = x * cos_y + z * sin_y;
        float z2 = -x * sin_y + z * cos_y;
        x = x1;
        z = z2;

        float x2 = x * cos_z - y * sin_z;
        float y2 = x * sin_z + y * cos_z;
        x = x2;
        y = y2;

        cube.points[i].x = x;
        cube.points[i].y = y;
        cube.points[i].z = z;
    }
}

void draw_cube(uint32_t color) {
    if (framebuffer == NULL) {
        return; 
    }

    static const int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, 
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    static Point projected_points[8];
    for (int i = 0; i < 8; i++) {
        float z = cube.points[i].z + 2.0f; // avoid div by zero
        projected_points[i].x = (cube.points[i].x / z) * framebuffer->framebuffer_width / 2.0f + framebuffer->framebuffer_width / 2.0f;
        projected_points[i].y = (cube.points[i].y / z) * framebuffer->framebuffer_height / 2.0f + framebuffer->framebuffer_height / 2.0f;
    }

    clear_framebuffer();

    for (int i = 0; i < 12; i++) {
        int start_idx = edges[i][0];
        int end_idx = edges[i][1];

        float x0 = projected_points[start_idx].x;
        float y0 = projected_points[start_idx].y;
        float x1 = projected_points[end_idx].x;
        float y1 = projected_points[end_idx].y;

        draw_point(x0, y0, color);
        draw_point(x1, y1, color);

        draw_line(x0, y0, x1, y1, color);
    }
}

void draw_point(float x, float y, uint32_t color) {
    int ix = (int)x;
    int iy = (int)y;
    if (ix >= 0 && ix < framebuffer->framebuffer_width && iy >= 0 && iy < framebuffer->framebuffer_height) {
        framebuffer_addr[iy * framebuffer->framebuffer_pitch / 4 + ix] = color;
    }
}

void draw_line(float x0, float y0, float x1, float y1, uint32_t color) {
    int dx = absf((int)x1 - (int)x0);
    int dy = absf((int)y1 - (int)y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        draw_point(x0, y0, color);
        if ((int)x0 == (int)x1 && (int)y0 == (int)y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}