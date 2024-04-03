#ifndef ALLOCATOR_H
#define ALLOCATOR_H
#include <stddef.h>

typedef struct allocator
{
    void* (*allocate)(void*, ptrdiff_t, ptrdiff_t);
    void* (*reallocate)(void*, void*, ptrdiff_t, ptrdiff_t, ptrdiff_t);
    void (*free)(void*, void*, ptrdiff_t);
    void* context;
} allocator;

typedef struct default_allocator
{
    allocator internal_allocator;
} default_allocator;

typedef struct arena_allocator
{
    allocator internal_allocator;
} arena_allocator;

typedef struct expandable_arena_allocator
{
    allocator internal_allocator;
} expandable_arena_allocator;

typedef struct arena_context
{
    ptrdiff_t length;
    ptrdiff_t capacity;
    char* data;
} arena_context;

typedef struct expandable_arena_context
{
    ptrdiff_t length;
    ptrdiff_t capacity;
    char* data;
    allocator* alloc;
} expandable_arena_context;

default_allocator default_allocator_create();
arena_allocator arena_allocator_create(arena_context* context, ptrdiff_t size, allocator* alloc);
expandable_arena_allocator expandable_arena_allocator_create(expandable_arena_context* context, ptrdiff_t size,
                                                             allocator* alloc);

void arena_allocator_destroy(arena_allocator* self, allocator* alloc);
void expandable_arena_allocator_destroy(expandable_arena_allocator* self);

#endif
