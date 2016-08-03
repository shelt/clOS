#include "pmem.h"
#include "paging.h"
#include "rmem.h"

void *palloc(uint32_t size, uint32_t proc_i, uint8_t supervisor)
{
    void *r = ralloc(size);
    if (r == NULL)
        return NULL;
    
    uint32_t needed_4mb = PAGES_NEEDED_4MB(size);
    uint32_t remain_4kb = PAGES_REMAIN_4KB(size);
    uint32_t pos = 0;
    for (uint32_t i=0; i<needed_4mb; i++, pos+=MB(4))
        page_map_4mb(proc_i, pos, pos, supervisor);
    for (uint32_t i=0; i<remain_4kb; i++, pos+=KB(4))
        page_map_4kb(proc_i, pos, pos, supervisor);

    return r;
}

void pfree(void *p, uint32_t proc_i)
{
    page_unmap(proc_i, (uint32_t)p);
    rfree(p);
}

void pdie(uint32_t proc_i)
{
    page_unmap_all(proc_i);
}
