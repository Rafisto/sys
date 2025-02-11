#ifndef BASE_SCREEN_H
#define BASE_SCREEN_H

#define VIDEO_MEMORY 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT * 2)

static int current_color = 0x0F; 
static int cursor_row = 0;
static int cursor_column = 0;

void screen_clear();
void screen_write_at(int row, int column, char character, int color);
void set_color(int fg, int bg);
void write_char(char character);
void write_string(const char *str);
void write_blahaj();
void remove_char();

#endif // BASE_SCREEN_H
