/* GCC stdlibs */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * Functions that manipulate memory contents 
 * in a basic way.
 */

// Very basic implementation; TODO improve
void *memcpy(void *_dst, void *_src, size_t len)
{
    register char *dst = _dst;
    register char *src = _src;
    for (size_t i=0; i<len; i++)
        dst[i] = src[i];
    
    return dst;
}

void *memset(void *_dst, unsigned char data, size_t len)
{
    register char *dst = _dst;
    for (size_t i=0; i<len; i++)
        dst[i] = data;
    
    return dst;
}