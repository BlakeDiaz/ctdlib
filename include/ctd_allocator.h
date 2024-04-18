#ifndef CTD_ALLOCATOR_H
#define CTD_ALLOCATOR_H
#include <stddef.h>

typedef struct ctd_allocator
{
    void* (*allocate)(void*, ptrdiff_t, ptrdiff_t); // context, size, align
    void* (*reallocate)(void*, void*, ptrdiff_t, ptrdiff_t, ptrdiff_t); // context, pointer, old_size, new_size, align
    void (*deallocate)(void*, void*, ptrdiff_t); // context, pointer, size
    void* context;
} ctd_allocator;

typedef struct ctd_default_allocator
{
    ctd_allocator allocator;
} ctd_default_allocator;

extern ctd_default_allocator ctd_default_allocator_instance;
ctd_default_allocator ctd_default_allocator_create();
#endif // CTD_ALLOCATOR_H
