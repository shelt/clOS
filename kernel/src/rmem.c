#include "rmem.h"
#include "common.h"
#include "kernel.h"
#include "vga.h" //temp for debug
#include <stddef.h>

//TODO small pages for headers, larger pages for spaces.
//TODO and when we do that we will need to change every offset into it's own preproc var HEADER_SIZE
#define PAGE_SIZE KB(4)
#define PAGES_NEEDED(b) (b + PAGE_SIZE - 1) / PAGE_SIZE
#define NEXT_PAGE_BOUNDARY(b) (void *) ((((int)b+(PAGE_SIZE-1)) / PAGE_SIZE) * PAGE_SIZE)

static region_t regions_head = {0};
static region_t regions_foot = {0};


/*******************
 * HEAP MANAGEMENT *
 *******************/

static void shrink(region_t *r, uint32_t pages)
{
    if (r->pages <= pages)
        return;
    // regions require an extra page for bookkeeping, so ensure we can fit it
    if ((r->pages - pages) < 2)
        return;
    
    region_t *new = (region_t *)((uint8_t *)r)+(PAGE_SIZE*(1+pages)); // +1 because of BK page
    new->used = 0;
    new->pages = r->pages - pages - 1; // -1 because of BK page
    new->prev = r;
    new->next = r->next;
    
    r->pages = pages;
    r->next = new;
}

static void merge(region_t *r1, region_t *r2)
{
    r1->pages += r2->pages + 1; // +1 because of BK page
    r1->next = r2->next;
    r2->next->prev = r1;
}

static void expand(region_t *r)
{
    if (r->used)
        kernel_panic("Attempt to expand used region");
    if (r->prev != &regions_head && !r->prev->used)
    {
        merge(r->prev, r);
        r = r->prev; // left-most merger's header is preserved
    }
    if (r->next != &regions_foot && !r->next->used)
        merge(r, r->next);
}


void *ralloc(uint32_t size)
{
    region_t *curr = &regions_head;
    while ((curr=curr->next) != &regions_foot)
    {
        if (!curr->used && (curr->pages*PAGE_SIZE) >= size)
        {
            curr->used = 1;
            shrink(curr, PAGES_NEEDED(size));
            return ((uint8_t *)curr) + PAGE_SIZE;
        }
    }
    return NULL;
}

void rfree(void *p)
{
    region_t *r = p - PAGE_SIZE;
    r->used = 0;
    expand(r);
}


/**
 * @brief Initialize memory regioning structures.
 *
 */
void rmem_init(uint8_t *heap_start, uint32_t heap_size)
{
    region_t *initial = NEXT_PAGE_BOUNDARY(heap_start);
    initial->used = 0;
    initial->pages = heap_size/PAGE_SIZE;
    initial->prev = &regions_head;
    initial->next = &regions_foot;
    
    regions_head.next = initial;
    regions_foot.prev = initial;
}

