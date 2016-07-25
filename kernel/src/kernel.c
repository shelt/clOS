/* GCC stdlibs */
#include <stdbool.h>
#include <stdint.h>

#include "gdt.h"
#include "idt.h"
#include "kernel.h"
#include "common.h"
#include "vga.h"
#include "mboot.h"
#include "mem.h"

void kernel_main(register uint32_t magic, multiboot_info_t *mbi)
{
    vga_init();
    vga_puts(KERNEL_BANNER);
    
    mboot_verify(magic, mbi);
    
    gdt_init();
    idt_init();
    
    vga_puts("\nEnabling interrupts\n");
    asm("sti");
    while (1)
        asm("hlt");
}

void kernel_panic(const char *data)
{
    vga_setcolor(make_color(COLOR_RED, COLOR_BLACK));
    vga_puts("\nkernel panic: ");
    vga_puts(data);
    while (1)
        asm("hlt");
}
