/* GCC stdlibs */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kernel.h"
#include "common.h"
#include "term.h"
#include "mem.h"

void mboot_init();
void panic(const char *data);

void kernel_main()
{
    volatile uint64_t initial_eax;
    volatile uint64_t initial_ebx;
    asm ("movl %%eax, %0\n"
         "movl %%ebx, %1\n"
         : "=m"(initial_eax),
           "=m"(initial_ebx)
        );
    
    term_init();
    term_cursor(-1,0); // hide cursor
    term_puts(KERNEL_BANNER);
    
    mboot_init(initial_eax, initial_ebx);
    
    mem_init();
    
    term_puts("\nkernel halted normally\n");
}

void mboot_init(register uint64_t magic, register uint64_t addr)
{
    if (magic != MB_BL_MAGIC)
        panic("Bootloader not found to be Multiboot-compliant");
    else
        term_status("Multiboot-compliant bootloader", STATUS_OKAY);
    
    //TODO deal with data at addr
}

void panic(const char *data)
{
    term_setcolor(make_color(COLOR_RED, COLOR_BLACK));
    term_puts("\nkernel panic: ");
    term_puts(data);
    asm("hlt");
}