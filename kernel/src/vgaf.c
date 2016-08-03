#include "vgaf.h"
#include "vga.h"

void vgaf_put_mmap(multiboot_mmap_t *mmap, uint32_t len)
{
    multiboot_mmap_t *mmap_i = mmap;
    vga_printf("------------ memory map -------------\n");
    vga_printf("  start       end       length   type\n");
    while ((uint32_t)mmap_i < ((uint32_t)mmap + len))
    {
        
        vga_printf("0x%08x 0x%08x 0x%08x 0x%02x\n",
                                            (unsigned) mmap_i->base_addr,
                                            (unsigned) (mmap_i->base_addr + mmap_i->length - 1),
                                            (unsigned) mmap_i->length,
                                            (unsigned) mmap_i->type);
        mmap_i = (multiboot_mmap_t *) ((uint32_t)mmap_i + mmap_i->size + sizeof(mmap_i->size));
    }
    vga_printf("-------------------------------------\n");
}

void vgaf_put_rmem(region_t *regions_head, region_t *regions_foot)
{
    region_t *curr = regions_head;
    while ((curr = curr->next) != regions_foot)
    {
        vga_printf("[0x%08x]-[0x%08x] (%d 4KB pages) (used:%d)\n", 
                                            ((uint8_t *)curr)+KB(4),
                                            ((uint8_t *)curr)+KB(4) + (curr->pages*KB(4)),
                                            curr->pages,
                                            curr->used);
    }
}
