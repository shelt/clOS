#include "paging.h"




void page_map(
{
    
}

void paging_init()
{
    // Enable paging
    asm volatile ("               \
        movl %%cr0, %%eax       \n\
        orl $0x80000000, %%eax  \n\
        movl %%eax, %%cr0"
        :
        :
        : "eax"
        );
}
