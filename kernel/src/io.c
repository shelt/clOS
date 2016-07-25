#include "io.h"

unsigned char inportb(unsigned int port)
{
   unsigned char ret;
   asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
   return ret;
}

void outportb(unsigned int port, unsigned char value)
{
   asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}
