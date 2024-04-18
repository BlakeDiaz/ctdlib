#include <ctd_define.h>

inline ptrdiff_t ctd_max(ptrdiff_t a, ptrdiff_t b)
{
    return a >= b ? a : b;
}

inline ptrdiff_t ctd_min(ptrdiff_t a, ptrdiff_t b)
{
    return a <= b ? a : b;
}
