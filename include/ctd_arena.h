#ifndef ARENA_H
#define ARENA_H
#include <ctd_allocator.h>

typedef struct ctd_arena_allocator
{
    ctd_allocator allocator;
} ctd_arena_allocator;

typedef struct ctd_expandable_arena_allocator
{
    ctd_allocator allocator;
} ctd_expandable_arena_allocator;

typedef struct ctd_arena_context
{
    ptrdiff_t length;
    ptrdiff_t capacity;
    char* data;
} ctd_arena_context;

typedef struct ctd_expandable_arena_context
{
    ptrdiff_t length;
    ptrdiff_t capacity;
    char* data;
    ctd_allocator* allocator;
} ctd_expandable_arena_context;

ctd_arena_allocator ctd_arena_allocator_create(ptrdiff_t size, ctd_allocator* alloc);
ctd_expandable_arena_allocator ctd_expandable_arena_allocator_create(ctd_expandable_arena_context* context, ptrdiff_t size,
                                                               ctd_allocator* allocator);

void ctd_arena_allocator_destroy(ctd_arena_allocator* self, ctd_allocator* allocator);
void ctd_expandable_arena_allocator_destroy(ctd_expandable_arena_allocator* self);

#endif // ARENA_H
