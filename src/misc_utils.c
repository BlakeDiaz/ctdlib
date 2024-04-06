#include "../include/misc_utils.h"

inline ptrdiff_t max(ptrdiff_t a, ptrdiff_t b)
{
    return a >= b ? a : b;
}

inline ptrdiff_t min(ptrdiff_t a, ptrdiff_t b)
{
    return a <= b ? a : b;
}
