#ifndef PAGING_H
#define PAGING_H
#include "common.h"

#define MAX_PROCESSES 1024

typedef struct page_dir_entry
{
    uint32_t present: 1;
    uint32_t rw: 1;
    uint32_t user: 1;
    uint32_t wthrough: 1;
    uint32_t cacheoff: 1;
    uint32_t accessed: 1;
    uint32_t reserved: 1;  // set to 0!
    uint32_t size: 1;
    uint32_t global: 1;    // ignored
    uint32_t unused: 3;
    uint32_t addr_shifted: 20;
} __attribute__((packed)) page_dir_entry_t;

typedef struct page_tbl_entry
{
    uint32_t present: 1;
    uint32_t rw: 1;
    uint32_t user: 1;
    uint32_t wthrough: 1;
    uint32_t cacheoff: 1;
    uint32_t accessed: 1;
    uint32_t dirty: 1;
    uint32_t ptai: 1;
    uint32_t global: 1;
    uint32_t unused: 3;
    uint32_t addr_shifted: 20;
} __attribute__((packed)) page_tbl_entry_t;

void page_map_4kb(uint32_t proc_i, uint32_t phys_a, uint32_t virt_a, uint8_t supervisor);
void page_map_4mb(uint32_t proc_i, uint32_t phys_a, uint32_t virt_a, uint8_t supervisor);
void page_unmap(uint32_t proc_i, uint32_t virt_a);
void page_unmap_all(uint32_t proc_i);
void set_process(uint32_t proc_i);
void paging_init();
void paging_enable();

#endif /* PAGING_H */
