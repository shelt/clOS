#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

// DIRECTORIES


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

typedef struct page_table_entry
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
} __attribute__((packed)) page_table_entry_t;

#endif /* PAGING_H */
