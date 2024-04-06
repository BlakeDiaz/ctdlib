#ifndef ALLOCATOR_H
#define ALLOCATOR_H
#include <stddef.h>

typedef struct allocator_t
{
    void* (*allocate)(void*, ptrdiff_t, ptrdiff_t); // context, size, align
    void* (*reallocate)(void*, void*, ptrdiff_t, ptrdiff_t, ptrdiff_t); // context, pointer, old_size, new_size, align
    void (*free)(void*, void*, ptrdiff_t); // context, pointer, size
    void* context;
} allocator_t;

typedef struct default_allocator
{
    allocator_t allocator;
} default_allocator;

extern default_allocator default_allocator_instance;
default_allocator default_allocator_create();
#endif
