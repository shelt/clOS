#include "isrs.h"
#include "io.h"
#include "vga.h" // temp for kb TODO


uint8_t kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',
   '9', '0', '-', '=', '\b','\t','q', 'w', 'e', 'r',
   't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`',   0,'\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/',0,'*',0,' ', 0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,'-',0,0,0,'+',0,0,0,0,0,0,0,0,0,0,0,
};

void isr_handle33(struct isr_regs *r)
{
    uint8_t scancode;

    scancode = inportb(0x60); // read from data buffer

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80)
    {
        /* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
    }
    else
    {
        /* Here, a key was just pressed. Please note that if you
        *  hold a key down, you will get repeated key press
        *  interrupts. */

        /* Just to show you how this works, we simply translate
        *  the keyboard scancode into an ASCII value, and then
        *  display it to the screen. You can get creative and
        *  use some flags to see if a shift is pressed and use a
        *  different layout, or you can add another 128 entries
        *  to the above layout to correspond to 'shift' being
        *  held. If shift is held using the larger lookup table,
        *  you would add 128 to the scancode when you look for it */
        vga_putc(kbdus[scancode]);
    }
}
