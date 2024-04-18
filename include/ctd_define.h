#ifndef CTD_DEFINE_H
#define CTD_DEFINE_H
#include <stddef.h>
extern ptrdiff_t ctd_max(ptrdiff_t a, ptrdiff_t b);
extern ptrdiff_t ctd_min(ptrdiff_t a, ptrdiff_t b);
#define sizeof(x) (ptrdiff_t) sizeof(x)
#define countof(x) (ptrdiff_t) (sizeof(x)/sizeof(*x))
#define lengthof(x) (countof(x) - 1)

#endif // CTD_DEFINE_H