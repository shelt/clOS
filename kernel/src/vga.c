#include "vga.h"
#include "stdlib/mem.h"
#include "stdlib/str.h"
#include "io.h"

size_t vga_row;
size_t vga_column;
uint8_t vga_color;
uint16_t *vga_buffer;

void prevent_overflow();

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

void vga_setcolor(uint8_t color)
{
    vga_color = color;
}

void vga_setc(char c, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    vga_buffer[index] = make_vgaentry(c, vga_color);
}

void vga_putc(char c)
{
    if (c == '\n')
    {
        ++vga_row;
        vga_column = 0;
    }
    else
    {
        vga_setc(c, vga_column, vga_row);
        ++vga_column;
    }
    prevent_overflow();
}

void vga_puts(const char* data)
{
    size_t datalen = strlen(data);
    for (size_t i = 0; i < datalen; i++)
        vga_putc(data[i]);
}

void prevent_overflow()
{
    if (vga_column == VGA_WIDTH)
    {
        vga_column = 0;
        ++vga_row;
    }
    if (vga_row == VGA_HEIGHT)
    {
        --vga_row;
        
        for (size_t y = 1; y < VGA_HEIGHT; y++)
            memcpy(&vga_buffer[(y - 1) * VGA_WIDTH],
                   &vga_buffer[y * VGA_WIDTH], VGA_WIDTH*2);
        for (size_t x = 0; x < VGA_WIDTH; x++)
            vga_setc(' ', x, vga_row);
    }
}

void vga_cursor(int row, int col)
{
    unsigned short position = (row * 80) + col;

    // cursor LOW port to vga INDEX register
    outportb(0x3D4, 0x0F);
    outportb(0x3D5, (unsigned char) (position & 0xFF));
    // cursor HIGH port to vga INDEX register
    outportb(0x3D4, 0x0E);
    outportb(0x3D5, (unsigned char) ((position >> 8)&0xFF));
}

int numlen(int d)
{
    int len = 0;
    int tmp = 1L;
    while(tmp < d)
    {
        len++;
        tmp = (tmp << 3) + (tmp << 1);
    }
    return len;
}

/* Convert the integer D to a string and save the string in BUF. If
   BASE is equal to 'd', interpret that D is decimal, and if BASE is
   equal to 'x', interpret that D is hexadecimal. */
static void itoa(char *buf, int base, int d, int l)
{
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;
    int numlen = 0;

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
        
        numlen++;
    }
    while (ud /= divisor);
    
    int zeros = l-numlen;
    if (zeros > 0)
        for (int i=0; i<zeros; i++)
            *p++ = '0';

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

#define MAX_FORMAT_SIZE 30
void vga_printf(const char *format, ...)
{
    char **arg = (char **) &format;
    int c;
    char buf[MAX_FORMAT_SIZE];

    arg++;

    while ((c = *format++) != 0)
    {
        if (c != '%')
            vga_putc(c);
        else
        {
            char *p;

            c = *format++;
            switch (c)
            {
            
            case '0':
                c = *format++;
                int leading = c - '0';
                c = *format++;
                itoa(buf, c, *((int *) arg++), leading);
                p = buf;
                goto string;
                break;
            case 'd':
            case 'u':
            case 'x':
                itoa(buf, c, *((int *) arg++), 0);
                p = buf;
                goto string;
                break;

            case 's':
                p = *arg++;
                if (!p)
                    p = "(null)";

            string:
                while (*p)
                    vga_putc(*p++);
                break;

            default:
                vga_putc(*((int *) arg++));
                break;
            }
        }
    }
}

void vga_statusnewline(int status)
{
    vga_setcolor(make_default_color());
    vga_setc('[', VGA_WIDTH-9, vga_row);
    vga_setc(' ', VGA_WIDTH-8, vga_row);
    
    if (status == STATUS_OKAY)
    {
        vga_setcolor(make_color(COLOR_GREEN, COLOR_BLACK));
        vga_setc('O', VGA_WIDTH-7, vga_row);
        vga_setc('K', VGA_WIDTH-6, vga_row);
        vga_setc('A', VGA_WIDTH-5, vga_row);
        vga_setc('Y', VGA_WIDTH-4, vga_row);
    }
    else if (status == STATUS_FAIL)
    {
        vga_setcolor(make_color(COLOR_RED, COLOR_BLACK));
        vga_setc('F', VGA_WIDTH-7, vga_row);
        vga_setc('A', VGA_WIDTH-6, vga_row);
        vga_setc('I', VGA_WIDTH-5, vga_row);
        vga_setc('L', VGA_WIDTH-4, vga_row);
    }
    vga_setcolor(make_default_color());
    vga_setc(' ', VGA_WIDTH-3, vga_row);
    vga_setc(']', VGA_WIDTH-2, vga_row);
    vga_setc(' ', VGA_WIDTH-1, vga_row);
    
    vga_putc('\n');
    
}

void vga_init()
{
    vga_cursor(-1, 0); // hide cursor TODO external cursor manipulation functions
    vga_row = 0;
    vga_column = 0;
    vga_color = make_default_color();
    vga_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            vga_setc(' ', x, y);
    
    
    //uint16_t tmp = make_vgaentry('T', make_color(COLOR_WHITE,COLOR_WHITE));
    
    //method 1
    //memcpy(&vga_buffer[VGA_WIDTH-1], &tmp, 2);
    //memcpy(&vga_buffer[VGA_WIDTH*2-1], &vga_buffer[VGA_WIDTH-1], 2);
    
    //2
    //vga_buffer[VGA_WIDTH-1] = tmp;
    
    //vga_statusnewline(1);
}
