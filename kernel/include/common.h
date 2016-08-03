#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>
//#include <stdbool.h>

// Constants
#define PAGE_ENTRY_COUNT 1024
#define PAGE_ENTRY_SIZE 4

// Quantifiers
#define MB(num) ((num) << 20)
#define KB(num) ((num) << 10)

// Page boundary alignment
#define NEXT_PAGE_BOUNDARY(b) (void *) ((((int)b+(KB(4)-1)) / KB(4)) * KB(4))
#define PREV_PAGE_BOUNDARY(b) (void *) (int)b - ((int)b % KB(4))

// Number of 4mb pages we can use to represent b bytes
#define PAGES_NEEDED_4MB(b) b / MB(4)
// Remainder of above computation (in 4kb pages)
#define PAGES_REMAIN_4KB(b) ((b % MB(4)) + KB(4) - 1) / KB(4)

#endif // COMMON_H
