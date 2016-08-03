#include "mboot.h"

/**
 * @file mboot.c
 * @brief Multiboot data parsing
 * This file contains 1 function that's invoked one time
 * (by kernel_main()) - how boring.
 */
 
/**
 * @brief Finds the last nonreserved address in the mmap
 * We know where the heap starts: at the end of the kernel.
 * However, we need this function to tell us where the heap
 * ends.
 */
uint32_t mboot_mmap_end(multiboot_mmap_t *mmap, uint32_t len)
{
    uint32_t curr_end = 0;
    multiboot_mmap_t *mmap_i = mmap;
    while ((uint32_t)mmap_i < ((uint32_t)mmap + len))
    {
        if (mmap_i->type == 1)
            curr_end = (mmap_i->base_addr + mmap_i->length - 1);
        mmap_i = (multiboot_mmap_t *) ((uint32_t)mmap_i + mmap_i->size + sizeof(mmap_i->size));
    }
    return curr_end;
}
