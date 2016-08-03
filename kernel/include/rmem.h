#ifndef RMEM_H
#define RMEM_H
#include <stdint.h>

#define MAX_REGIONS 512

typedef struct region region_t;
typedef struct region
{
    uint8_t active:4;
    uint8_t used  :4;
    uint32_t addr;
    uint32_t pages;
    region_t *prev;
    region_t *next;
} region_t;

void page_map_4kb(uint32_t proc_i, uint32_t phys_a, uint32_t virt_a, uint8_t supervisor);
void rmem_init(uint8_t *heap_start, uint8_t *heap_end);
void *ralloc_bypage(uint32_t page_c);
void *ralloc(uint32_t size);
void rfree(void *p);

#endif /* RMEM_H */
