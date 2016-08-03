#ifndef IO_H
#define IO_H
#include "common.h"

uint8_t inportb(uint32_t port);
void outportb(uint32_t port,uint8_t value);

#endif /* IO_H */
