#ifndef STR_H
#define STR_H

#include <stddef.h>

void hstrtob(unsigned char *bytearr, char *string, size_t bytes);
void btohstr(char *string, unsigned char *bytearr, size_t bytes);
size_t strlen(const char* str);

#endif /* STR_H */

