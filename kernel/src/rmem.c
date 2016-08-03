#include "rmem.h"
#include "common.h"
#include "kernel.h"
#include "vga.h" //TODO dbg
#include "vgaf.h" //TODO debug
#include <stddef.h>

/**
 * Note: In this file, a "page" refers *only* to 4KB pages.
 * only paging.c is concerned with using PSE.
 */

#define PAGE_SIZE KB(4)
#define PAGES_NEEDED(b) (b + PAGE_SIZE - 1) / PAGE_SIZE


static region_t regions_head = {0};
static region_t regions_foot = {0};
static region_t regions[MAX_REGIONS];

static region_t *find_new_region()
{
    for (int i=0; i<MAX_REGIONS; i++)
        if (!regions[i].active)
        {
            regions[i].active = 1;
            return &regions[i];
        }
    return NULL;
}

static region_t *find_region(void *addr)
{
    for (int i=0; i<MAX_REGIONS; i++)
        if (!regions[i].addr == (uint32_t)addr)
            return &regions[i];
    return NULL;
}

/*******************
 * HEAP MANAGEMENT *
 *******************/

static void shrink(region_t *r, uint32_t pages)
{
    if (r->pages <= pages)
        return;
    
    region_t *new = find_new_region();
    new->used = 0;
    new->addr = r->addr+(PAGE_SIZE*pages);
    new->pages = r->pages - pages;
    new->prev = r;
    new->next = r->next;
    
    r->pages = pages;
    r->next = new;
}

static void merge(region_t *r1, region_t *r2)
{
    r1->pages += r2->pages;
    r1->next = r2->next;
    r2->next->prev = r1;
    r2->active = 0;
}


// Note: invalidates r!
static void expand(region_t *r)
{
    if (r->used)
        kernel_panic("Attempt to expand used region");
    if (r->next != &regions_foot && !r->next->used)
        merge(r, r->next);
    if (r->prev != &regions_head && !r->prev->used)
        merge(r->prev, r);
}

void *ralloc_bypage(uint32_t page_c)
{
    region_t *curr = &regions_head;
    while ((curr=curr->next) != &regions_foot)
    {
        if (!curr->used && curr->pages >= page_c)
        {
            curr->used = 1;
            shrink(curr, page_c);
            return (void *)curr->addr;
        }
    }
    return NULL;
}

void *ralloc(uint32_t size)
{
    return ralloc_bypage(PAGES_NEEDED(size));
}

void rfree(void *p)
{
    region_t *r = find_region(p);
    if (r == NULL)
        return;
    r->used = 0;
    expand(r);
}


/**
 * @brief Initialize memory regioning structures.
 *
 */
void rmem_init(uint8_t *heap_start, uint8_t *heap_end)
{
    heap_start = NEXT_PAGE_BOUNDARY(heap_start);
    heap_end   = PREV_PAGE_BOUNDARY(heap_end);
    region_t *initial = find_new_region();
    initial->used = 0;
    initial->addr = (uint32_t)heap_start;
    initial->pages = (((uint32_t)heap_end) - ((uint32_t)heap_start))/PAGE_SIZE;
    initial->prev = &regions_head;
    initial->next = &regions_foot;
    
    regions_head.next = initial;
    regions_foot.prev = initial; 
    
    vgaf_put_rmem(&regions_head, &regions_foot);//TODO temp for debug
}

