#include "paging.h"
#include "kernel.h"
#include "rmem.h"

// Array of page directories. Each process gets one of these.
static page_dir_entry_t page_dir_data[MAX_PROCESSES * PAGE_ENTRY_COUNT] __attribute__((aligned(KB(4))));

static page_dir_entry_t *get_page_dir(uint8_t proc_i)
{
    return &page_dir_data[PAGE_ENTRY_COUNT * proc_i];
}

/*
 * ENTRY INITIALIZATION
 */

void init_page_dir_entry(page_dir_entry_t *e, uint8_t present, uint8_t supervisor, uint8_t size, uint32_t addr)
{
    e->present  = present;
    e->rw       = 1;
    e->user     = !supervisor;
    e->wthrough = 1;
    e->cacheoff = 1;
    e->accessed = 0;
    e->reserved = 0;
    e->size     = size;
    e->addr_shifted = addr >> 12;
}

void init_page_tbl_entry(page_tbl_entry_t *e, uint8_t present, uint8_t supervisor, uint32_t addr)
{
    e->present  = present;
    e->rw       = 1;
    e->user     = !supervisor;
    e->wthrough = 1;
    e->cacheoff = 1;
    e->accessed = 0;
    e->dirty    = 0;
    e->ptai     = 0;
    e->addr_shifted = addr >> 12;
}

/*
 * DIR/TABLE INITIALIZATION
 */

void init_page_dir(uint32_t proc_i, uint8_t supervisor)
{
    for (int dir_i=0; dir_i<MAX_PROCESSES; dir_i++)
        init_page_dir_entry(&(get_page_dir(proc_i)[dir_i]), 0, supervisor, 0, 0x0);
}

void init_page_tbl(page_tbl_entry_t* tbl, uint8_t supervisor)
{
    for (int tbl_i=0; tbl_i<PAGE_ENTRY_COUNT; tbl_i++)
        init_page_tbl_entry(&tbl[tbl_i], 0, supervisor, 0x0);
}

void page_map_4kb(uint32_t proc_i, uint32_t phys_a, uint32_t virt_a, uint8_t supervisor)
{
    uint32_t dir_i = virt_a / MB(4);
    uint32_t tbl_i = (virt_a % MB(4)) / KB(4);
    
    page_tbl_entry_t *tbl;
    page_dir_entry_t *dir_entry = &(get_page_dir(proc_i)[dir_i]);
    if (!dir_entry->present)
    {
        tbl = ralloc_bypage(1);
        init_page_dir_entry(dir_entry, 1, supervisor, 0, (uint32_t)tbl);
        init_page_tbl(tbl, supervisor);
    }
    else
        tbl = (page_tbl_entry_t *)(dir_entry->addr_shifted << 12);

    init_page_tbl_entry(&tbl[tbl_i], 1, supervisor, phys_a);
}

void page_map_4mb(uint32_t proc_i, uint32_t phys_a, uint32_t virt_a, uint8_t supervisor)
{
    uint32_t dir_i = virt_a / MB(4);
    page_dir_entry_t *dir_entry = &(get_page_dir(proc_i)[dir_i]);
    
    // Ensure we won't forget about an allocated page table
    if (dir_entry->present && dir_entry->size)
        kernel_panic("page_map_4mb: phys_a already mapped to 4kb table (remap would hide existing page table)");

    init_page_dir_entry(dir_entry, 1, supervisor, 1, phys_a);
}

void page_unmap(uint32_t proc_i, uint32_t virt_a)
{
    uint32_t dir_i = virt_a / MB(4);
    page_dir_entry_t *dir_entry = &(get_page_dir(proc_i)[dir_i]);
    
    if (!dir_entry->present)
        return;
    else if (dir_entry->size)
        dir_entry->present = 0;
    else
    {
        uint32_t tbl_i = (virt_a % MB(4)) / KB(4);
        page_tbl_entry_t *tbl = (page_tbl_entry_t *)(dir_entry->addr_shifted << 12);
        tbl[tbl_i].present = 0;
    }
    
    
}

// Note: not the same as doing page_unmap for all mapped pages;
//       This (rather importantly) frees the process's page tables.
//       In other words, page_unmap() is for runtime freeing whereas
//       page_unmap_all() is for process cleanup.
void page_unmap_all(uint32_t proc_i)
{
    page_dir_entry_t *dir_entry;
    for (int dir_i=0; dir_i<PAGE_ENTRY_COUNT; dir_i++)
    {
        dir_entry = &(get_page_dir(proc_i)[dir_i]);
        if (!dir_entry->present)
            continue;
        else if (dir_entry->size)
            dir_entry->present = 0;
        else
            rfree((void *)(dir_entry->addr_shifted << 12));
    }
}

void set_process(uint32_t proc_i)
{
    asm volatile ("           \
        addl $0x18, %%eax   \n\
        movl %%eax, %%cr3"
        : /* no outputs */
        : "a" ((uint32_t) get_page_dir(proc_i))
        );
}

/**
 * @brief initialize system for paging
 * @param heap_start Where the heap starts. Everything before it needs to be paged to the kernel process.
 */
void paging_init(uint8_t *heap_start)
{
    heap_start = NEXT_PAGE_BOUNDARY(heap_start);
    asm volatile ("                     \n\
        # Enable write-protect (bit 16) \n\
        movl %%cr0, %%eax               \n\
        orl $0x1000, %%eax              \n\
        movl %%eax, %%cr0               \n\
        # Enable pse and pge (4 and 7)  \n\
        movl %%cr4, %%eax               \n\
        orl $0x90, %%eax                \n\
        movl %%eax, %%cr4"
        :
        :
        : "eax"
        );
    
    // Initialize directories
    for (int proc_i=0; proc_i<MAX_PROCESSES; proc_i++)
        init_page_dir(proc_i, 1);
    
    // Page entire memory to kernel (because why not)
    uint32_t i,pos;
    for (pos=0, i=0; i<1024; i++, pos+=MB(4))
        page_map_4mb(0, pos, pos, 1);

/*    
    // Map kernel memory for kernel process
    uint32_t needed_4mb = PAGES_NEEDED_4MB((uint32_t)heap_start);
    uint32_t remain_4kb = PAGES_REMAIN_4KB((uint32_t)heap_start);
    uint32_t pos=0;
    for (uint32_t i=0; i<needed_4mb; i++, pos+=MB(4))
        page_map_4mb(0, pos, pos, 1);
    for (uint32_t i=0; i<remain_4kb; i++, pos+=KB(4))
        page_map_4kb(0, pos, pos, 1);
*/
}


void paging_enable()
{
    asm volatile ("                     \n\
        # Enable paging                 \n\
        movl %%cr0, %%eax               \n\
        orl $0x80000000, %%eax          \n\
        movl %%eax, %%cr0"
        :
        :
        : "eax"
        );
}
