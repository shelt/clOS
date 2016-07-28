#ifndef MEM_H
#define MEM_H
#include <stdint.h>

typedef struct region region_t;
typedef struct region
{
    uint8_t used;
    uint32_t pages;
    region_t *prev;
    region_t *next;
} region_t;

#endif /* MEM_H */
