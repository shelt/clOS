/* GCC stdlibs */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "mem.h"
#include "str.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

/* Hardware text mode color constants. */
enum vga_color
{
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
};
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

/* MAKERS */

uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

uint16_t make_vgaentry(char c, uint8_t color)
{
    uint16_t c16 = c;
    uint16_t color16 = color;
    return c16 | color16 << 8;
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_setc(char c, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = make_vgaentry(c, terminal_color);
}
 
void terminal_putc(char c)
{
    if (c == '\n')
    {
        ++terminal_row;
        terminal_column = 0;
    }
    else
    {
        terminal_setc(c, terminal_column, terminal_row);
        ++terminal_column;
    }
    
    // Prevent overflow
    if (terminal_column == VGA_WIDTH)
    {
        terminal_column = 0;
        ++terminal_row;
    }
    if (terminal_row == VGA_HEIGHT)
    {
        --terminal_row;
        for (size_t y=1; y<VGA_HEIGHT; y++)
            memcpy(&terminal_buffer[(y-1)*VGA_WIDTH],
                   &terminal_buffer[y*VGA_WIDTH], VGA_WIDTH);
        for (size_t x=0; x<VGA_WIDTH; x++)
            terminal_setc(' ', x, terminal_row);
    }
}
 
void terminal_puts(const char* data)
{
    size_t datalen = strlen(data);
    for (size_t i = 0; i < datalen; i++)
        terminal_putc(data[i]);
}

void terminal_initialize()
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            terminal_setc(' ', x, y);
}