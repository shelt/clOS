#ifndef STR_H
#define STR_H

#include <stddef.h>

void hstrtob(unsigned char *bytearr, char *string, size_t bytes);
void btohstr(char *string, unsigned char *bytearr, size_t bytes);
size_t strlen(const char* str);

// primitive memory
void *memcpy(void *_dst, void *_src, size_t len);
void *memset(void *_dst, unsigned char data, size_t len);

#endif /* STR_H */

