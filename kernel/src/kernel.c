/* GCC stdlibs */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kernel.h"
#include "common.h"
#include "term.h"
#include "mem.h"

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

void mboot_init();
void panic(const char *data);

void kernel_main()
{
    volatile uint64_t initial_eax;
    volatile uint64_t initial_ebx;
    asm("movl %%eax, %0\n"
        "movl %%ebx, %1\n"
        : "=m"(initial_eax),
          "=m"(initial_ebx));
    
    term_init();
    term_puts(KERNEL_BANNER);
    
    mboot_verify(initial_eax, initial_ebx);
    
    mem_init();
    
    
    // ONLY REACHED FOR DEBUG REASONS
    term_setcolor(make_color(COLOR_WHITE, COLOR_BLACK));
    term_puts("\n** kernel halted normally **\n");
    while (1)
        asm("hlt");
}

void mboot_verify(register uint64_t magic, register uint64_t addr)
{
    multiboot_info_t *mbi;

    if (magic == MB_BL_MAGIC)
    {
        term_printf("Multiboot-compliant bootloader");
        term_statusnewline(STATUS_OKAY);
    }
    else
        panic("Bootloader not found to be Multiboot-compliant");

    mbi = (multiboot_info_t *) addr;

    term_printf("Memory     : %uKB lower, %uKB upper",
                (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);
    term_statusnewline(CHECK_FLAG(mbi->flags, 0));

    term_printf("Boot device: 0x%x",
                (unsigned) mbi->boot_device);
    term_statusnewline(CHECK_FLAG(mbi->flags, 1) > 0);

    term_printf("cmdline    : %s", (char *) mbi->cmdline);
    term_statusnewline(CHECK_FLAG(mbi->flags, 2) > 0);

    term_printf("%d modules provided, starting at 0x%x\n",
                (int) mbi->mods_count, (int) mbi->mods_addr);

    // List available modules
    multiboot_module_t *mod;
    unsigned int i;
    for (i = 0, mod = (multiboot_module_t *) mbi->mods_addr;
            i < mbi->mods_count;
            i++, mod++)
        term_printf(" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
                    (unsigned) mod->mod_start,
                    (unsigned) mod->mod_end,
                    (char *) mod->cmdline);

    // Check for mutex of bits 4 and 5
    if (CHECK_FLAG(mbi->flags, 4) && CHECK_FLAG(mbi->flags, 5))
        panic("Bits 4 and 5 are both set");

    // Symbol table
    if (CHECK_FLAG(mbi->flags, 4))
    {
        multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);

        term_printf("multiboot_aout_symbol_table: tabsize = 0x%0x, "
                    "strsize = 0x%x, addr = 0x%x\n",
                    (unsigned) multiboot_aout_sym->tabsize,
                    (unsigned) multiboot_aout_sym->strsize,
                    (unsigned) multiboot_aout_sym->addr);
    }

    // ELF header table
    if (CHECK_FLAG(mbi->flags, 5))
    {
        multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

        term_printf("multiboot_elf_sec: num = %u, size = 0x%x,"
                    " addr = 0x%x, shndx = 0x%x\n",
                    (unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
                    (unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->shndx);
    }
    if (CHECK_FLAG(mbi->flags, 6))
    {
        multiboot_mmap_t *mmap;

        term_printf("   M E M O R Y   M A P\n");
        term_printf(" baseaddr    length   type\n");
        for (mmap = (multiboot_mmap_t *) mbi->mmap_addr;
            (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
                mmap = (multiboot_mmap_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size)))
            term_printf("0x%08x 0x%08x 0x%02x\n",
                   (unsigned) mmap->base_addr,
                   (unsigned) mmap->length,
                   (unsigned) mmap->type);
    }
}

void panic(const char *data)
{
    term_setcolor(make_color(COLOR_RED, COLOR_BLACK));
    term_puts("\nkernel panic: ");
    term_puts(data);
    while (1)
        asm("hlt");
}