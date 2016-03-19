#ifndef TERM_H
#define TERM_H

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

#define STATUS_OKAY 0
#define STATUS_FAIL 1

uint8_t make_color(enum vga_color fg, enum vga_color bg);

inline uint8_t make_default_color()
{
    return make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
}

void term_setcolor(uint8_t color);

void term_putc(char c);

void term_puts(const char* data);

void term_status(const char* data, int status);

void term_printf(const char *format, ...);

void term_cursor(int x, int y);

void term_init();

#endif /* TERM_H */

