#include <stdint.h>

extern char kernel_end; // linker.ld
static void *free_addr = &kernel_end;

void *kalloc(uint32_t size)
{
    void *addr = free_addr;
    free_addr += size;
    return addr;
}

void *kmalloc_al(uint32_t size)
{
    free_addr = ((unsigned int) free_addr & 0xFFFFF000) + 0x1000;
    void *addr = free_addr;
    free_addr += size;
    return addr;
}
