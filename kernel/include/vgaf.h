#ifndef VGAF_H
#define VGAF_H

#include <stdint.h>
#include "mboot.h"

void vgaf_put_mmap(multiboot_mmap_t *mmap, uint32_t len);


#endif // VGAF_H
