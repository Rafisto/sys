#include "framebuffer.h"
#include "font.h"

void clear_framebuffer()
{
    if (framebuffer == NULL)
    {
        return;
    }

    uint32_t color = 0x000000;
    uint32_t total_pixels = framebuffer->framebuffer_width * framebuffer->framebuffer_height;
    uint32_t i = 0;

    for (; i + 4 <= total_pixels; i += 4)
    {
        framebuffer_addr[i] = color;
        framebuffer_addr[i + 1] = color;
        framebuffer_addr[i + 2] = color;
        framebuffer_addr[i + 3] = color;
    }

    for (; i < total_pixels; i++)
    {
        framebuffer_addr[i] = color;
    }
}

void print_pixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (framebuffer == NULL)
    {
        return;
    }

    if (x >= framebuffer->framebuffer_width || y >= framebuffer->framebuffer_height)
    {
        return;
    }

    framebuffer_addr[y * framebuffer->framebuffer_width + x] = color;
}

void print_character(uint32_t x, uint32_t y, char character, uint32_t color)
{
    if (framebuffer == NULL)
    {
        return;
    }

    if (x >= framebuffer->framebuffer_width || y >= framebuffer->framebuffer_height)
    {
        return;
    }

    uint8_t *font = font_data[(uint8_t)character];
    for (uint32_t j = 0; j < 12; j++)
    {
        for (uint32_t i = 0; i < 8; i++)
        {
            if (font[j] & (1 << (7 - i)))
            {
                print_pixel(x + i, y + j, color);
            }
            else
            {
                print_pixel(x + i, y + j, 0x000000);
            }
        }
    }
}

void print_background(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color)
{
    if (framebuffer == NULL)
    {
        return;
    }

    for (uint32_t j = 0; j < height; j++)
    {
        for (uint32_t i = 0; i < width; i++)
        {
            print_pixel(x + i, y + j, color);
        }
    }
}

void print_os_logo(uint32_t x, uint32_t y) {
    const uint8_t logo[8][12] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // Column 1
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // Column 2
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // Column 3
        {0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1},  // Column 4
        {1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0},  // Column 5
        {0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1},  // Column 6
        {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1},  // Column 7
        {1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1}   // Column 8
    };

    // Scale factor: 8x8 pixels -> 64x64, meaning each pixel becomes a block of 8x8 pixels.
    const uint32_t scale_factor = 8;

    // Define light blue and dark blue colors
    uint32_t light_blue = 0xADD8E6;  // Light blue color
    uint32_t dark_blue = 0x00008B;   // Dark blue color

    // Print the scaled logo on the screen
    for (uint32_t j = 0; j < 8; j++) {            // Iterate over each row of the logo
        for (uint32_t i = 0; i < 12; i++) {        // Iterate over each column of the logo
            // Loop through the scaled block size
            for (uint32_t block_y = 0; block_y < scale_factor; block_y++) {
                for (uint32_t block_x = 0; block_x < scale_factor; block_x++) {
                    // Calculate the actual position on the screen
                    uint32_t screen_x = x + (i * scale_factor) + block_x;
                    uint32_t screen_y = y + (j * scale_factor) + block_y;

                    // If logo pixel is 0, print light blue, otherwise dark blue
                    if (logo[j][i] == 1) {
                        print_pixel(screen_x, screen_y, light_blue);  // Light blue color for logo
                    } else {
                        print_pixel(screen_x, screen_y, dark_blue);  // Dark blue background
                    }
                }
            }
        }
    }
}
