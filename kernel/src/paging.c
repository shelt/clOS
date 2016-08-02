
#include "common.h"
#include "paging.h"
#include "rmem.h"

#define PAGE_ENTRY_COUNT 1024
#define PAGE_ENTRY_SIZE 4

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
        tbl = ralloc(sizeof(page_tbl_entry_t) * PAGE_ENTRY_COUNT); //TODO check null
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
    init_page_dir_entry(dir_entry, 1, supervisor, 1, phys_a);
}

void set_process(uint32_t proc_i)
{
    asm volatile ("               \
        addl $0x18, %%eax   \n\
        movl %%eax, %%cr3"
        : /* no outputs */
        : "a" ((uint32_t) get_page_dir(proc_i))
        );
}

void paging_init()
{
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
    
    // Map kernel page
    for (int i=0; i<4096; i++)
        page_map_4kb(0, i*4096, i*4096, 0);
    

//page_map_4mb

//    page_map_4kb(0, 0, 0, 0);
 //   page_map_4mb(0, 4194304, 4194304, 0);
  //  page_map_4mb(0, 8388608, 8388608, 0);
   // page_map_4mb(0, 12582912, 12582912, 0);
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
