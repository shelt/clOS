/* GCC stdlibs */
#include <stdbool.h>
#include <stdint.h>

#include "gdt.h"
#include "idt.h"
#include "kernel.h"
#include "vga.h"
#include "vgaf.h"
#include "mboot.h"
#include "rmem.h"
#include "paging.h"

#define KVERS "0.2"
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

extern int kernel_end;
extern int bss_end;
void kernel_main(register uint32_t magic, multiboot_info_t *mbi)
{
    vga_init();
    vga_puts("Starting kernel "KVERS"\n");
    vgaf_put_mmap( (multiboot_mmap_t *)mbi->mmap_addr, mbi->mmap_length);//TODO temp for debug
    
    if (magic != MB_BL_MAGIC)
        kernel_panic("bootloader not multiboot-compliant");
    if (!CHECK_FLAG(mbi->flags, 6))
        kernel_panic("bootloader didn't give memory map");
    if (CHECK_FLAG(mbi->flags, 4) && CHECK_FLAG(mbi->flags, 5))
        kernel_panic("bootloader info bits 4 and 5 are both set");
    
    gdt_init();
    idt_init();
    asm("sti"); // Enable interrupts
    
    rmem_init((uint8_t *)&kernel_end, 1048576);  //TODO temp number obviously
    paging_init((uint8_t *)&kernel_end);
    set_process(0);
    paging_enable();
    
    
    //vga_puts("Kernel ready.\n"); TODO
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
