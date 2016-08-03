#ifndef VGAF_H
#define VGAF_H

#include <stdint.h>
#include "mboot.h"
#include "rmem.h"

void vgaf_put_mmap(multiboot_mmap_t *mmap, uint32_t len);
void vgaf_put_rmem(region_t *regions_head, region_t *regions_foot);


#endif // VGAF_H
