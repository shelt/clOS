/* GCC stdlibs */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "term.h"

void kernel_main()
{
    terminal_initialize();
    terminal_puts("The kernel spreads its wings!\n");
}
