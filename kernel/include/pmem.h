#include "common.h"

void *palloc(uint32_t size, uint32_t proc_i, uint8_t supervisor);
void pfree(void *p, uint32_t proc_i);
void pdie(uint32_t proc_i);
