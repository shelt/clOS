#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

// DIRECTORIES

typedef struct page_dir_entry
{
    unsigned int present: 1;
    unsigned int rw: 1;
    unsigned int user: 1;
    unsigned int wthrough: 1;
    unsigned int cacheoff: 1;
    unsigned int accessed: 1;
    unsigned int reserved: 1;  // set to 0!
    unsigned int size: 1;
    unsigned int global: 1;    // ignored
    unsigned int unused: 3;
    unsigned int table_a: 20;
} __attribute__((packed)) page_dir_entry_t;

typedef struct page_dir
{
    page_dir_entry_t entries[1024];
} __attribute__((packed)) page_dir_t;

// TABLES

typedef struct page_table_entry
{
    unsigned int present: 1;
    unsigned int rw: 1;
    unsigned int user: 1;
    unsigned int wthrough: 1;
    unsigned int cacheoff: 1;
    unsigned int accessed: 1;
    unsigned int dirty: 1;
    unsigned int ptai: 1;
    unsigned int global: 1;
    unsigned int unused: 3;
    unsigned int frame: 20;
} __attribute__((packed)) page_table_entry_t;

typedef struct page_table
{
    page_table_entry_t entries[1024];
} __attribute__((packed)) page_table_t;

#endif /* PAGING_H */

