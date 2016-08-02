#ifndef RMEM_H
#define RMEM_H
#include <stdint.h>

typedef struct region region_t;
typedef struct region
{
    uint8_t used;
    uint32_t pages;
    region_t *prev;
    region_t *next;
} region_t;


void rmem_init(uint8_t *heap_start, uint32_t heap_size);
void *ralloc(uint32_t size);
void rfree(void *p);

#endif /* RMEM_H */
