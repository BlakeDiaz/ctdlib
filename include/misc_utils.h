#ifndef MISC_UTILS_H
#define MISC_UTILS_H
#include <stddef.h>
extern ptrdiff_t max(ptrdiff_t a, ptrdiff_t b);
extern ptrdiff_t min(ptrdiff_t a, ptrdiff_t b);
#define sizeof(x) (ptrdiff_t) sizeof(x)
#define countof(x) (ptrdiff_t) (sizeof(x)/sizeof(*x))
#define lengthof(x) (countof(x) - 1)

#endif // MISC_UTILS_H
