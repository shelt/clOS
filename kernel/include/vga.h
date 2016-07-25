#ifndef VGA_H
#define VGA_H
#include <stddef.h>

/* GCC stdlibs */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

#define STATUS_FAIL 0
#define STATUS_OKAY 1

uint8_t make_color(enum vga_color fg, enum vga_color bg);

inline uint8_t make_default_color()
{
    return make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
}

void vga_setcolor(uint8_t color);

void vga_setc(char c, size_t x, size_t y);

void vga_putc(char c);

void vga_puts(const char *data);

void vga_status(int status, const char *data, ...);

void vga_printf(const char *format, ...);

void vga_statusnewline(int status);

void vga_cursor(int x, int y);

void vga_init();

#endif /* VGA_H */

