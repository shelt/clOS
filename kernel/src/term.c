/* GCC stdlibs */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "term.h"
#include "stdlib/mem.h"
#include "str.h"
#include "io.h"


size_t term_row;
size_t term_column;
uint8_t term_color;
uint16_t* term_buffer;

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

void term_setcolor(uint8_t color)
{
    term_color = color;
}

void term_setc(char c, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    term_buffer[index] = make_vgaentry(c, term_color);
}

void term_putc(char c)
{
    if (c == '\n')
    {
        ++term_row;
        term_column = 0;
    }
    else
    {
        term_setc(c, term_column, term_row);
        ++term_column;
    }

    // Prevent overflow
    if (term_column == VGA_WIDTH)
    {
        term_column = 0;
        ++term_row;
    }
    if (term_row == VGA_HEIGHT)
    {
        --term_row;
        for (size_t y = 1; y < VGA_HEIGHT; y++)
            memcpy(&term_buffer[(y - 1) * VGA_WIDTH],
                   &term_buffer[y * VGA_WIDTH], VGA_WIDTH);
        for (size_t x = 0; x < VGA_WIDTH; x++)
            term_setc(' ', x, term_row);
    }
}

void term_puts(const char* data)
{
    size_t datalen = strlen(data);
    for (size_t i = 0; i < datalen; i++)
        term_putc(data[i]);
}

void term_status(const char* data, int status)
{
    term_puts("[ ");
    if (status == STATUS_OKAY)
    {
        term_setcolor(make_color(COLOR_GREEN, COLOR_BLACK));
        term_puts("OKAY");
    }
    else if (status == STATUS_FAIL)
    {
        term_setcolor(make_color(COLOR_RED, COLOR_BLACK));
        term_puts("FAIL");
    }
    term_setcolor(make_default_color());
    term_puts(" ] ");
    term_puts(data);
    term_puts("\n");
}

void term_cursor(int row, int col)
{
    unsigned short position = (row * 80) + col;

    // cursor LOW port to vga INDEX register
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char) (position & 0xFF));
    // cursor HIGH port to vga INDEX register
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char) ((position >> 8)&0xFF));
}

static void itoa(char *buf, int base, int d)
{
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;

    /* If %d is specified and D is minus, put `-' in the head. */
    if (base == 'd' && d < 0)
    {
        *p++ = '-';
        buf++;
        ud = -d;
    }
    else if (base == 'x')
        divisor = 16;

    /* Divide UD by DIVISOR until UD == 0. */
    do
    {
        int remainder = ud % divisor;

        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
    while (ud /= divisor);

    /* Terminate BUF. */
    *p = 0;

    /* Reverse BUF. */
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

void term_printf(const char *format, ...)
{
    char **arg = (char **) &format;
    int c;
    char buf[20];

    arg++;

    while ((c = *format++) != 0)
    {
        if (c != '%')
            term_putc(c);
        else
        {
            char *p;

            c = *format++;
            switch (c)
            {
            case 'd':
            case 'u':
            case 'x':
                itoa(buf, c, *((int *) arg++));
                p = buf;
                goto string;
                break;

            case 's':
                p = *arg++;
                if (!p)
                    p = "(null)";

string:
                while (*p)
                    term_putc(*p++);
                break;

            default:
                term_putc(*((int *) arg++));
                break;
            }
        }
    }
}

void term_init()
{
    term_row = 0;
    term_column = 0;
    term_color = make_default_color();
    term_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            term_setc(' ', x, y);
}