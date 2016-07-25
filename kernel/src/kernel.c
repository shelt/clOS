/* GCC stdlibs */
#include <stdbool.h>
#include <stdint.h>

#include "gdt.h"
#include "idt.h"
#include "kernel.h"
#include "common.h"
#include "term.h"
#include "mboot.h"
#include "mem.h"

void kernel_main(register uint32_t magic, multiboot_info_t *mbi)
{
    term_init();
    term_puts(KERNEL_BANNER);
    
    mboot_verify(magic, mbi);
    
    gdt_init();
    idt_init();
    
    term_setcolor(make_color(COLOR_WHITE, COLOR_BLACK));
    term_puts("\n** kernel_main complete **\n");
    asm("sti");
    while (1)
        asm("hlt");
}

void kernel_panic(const char *data)
{
    term_setcolor(make_color(COLOR_RED, COLOR_BLACK));
    term_puts("\nkernel panic: ");
    term_puts(data);
    while (1)
        asm("hlt");
}
