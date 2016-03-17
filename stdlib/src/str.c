/* GCC stdlibs */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * Functions that manipulate strings in a
 * basic way.
 */

size_t strlen(const char* str)
{
    size_t ret = 0;
    while ( str[ret] != 0 )
        ret++;
    return ret;
}
