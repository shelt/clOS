#include "io.h"

/**
 * @file io.c
 * @brief I/O port helpers and wrappers
 */

uint8_t inportb(uint32_t port)
{
   uint8_t ret;
   asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
   return ret;
}

void outportb(uint32_t port, uint8_t value)
{
   asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}
