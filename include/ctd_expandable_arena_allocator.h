#ifndef CTD_EXPANDABLE_ARENA_ALLOCATOR_H
#define CTD_EXPANDABLE_ARENA_ALLOCATOR_H
#include <ctd_allocator.h>

typedef struct ctd_expandable_arena_allocator
{
    ctd_allocator allocator;
} ctd_expandable_arena_allocator;

typedef struct ctd_expandable_arena_context
{
    ptrdiff_t length;
    ptrdiff_t capacity;
    char* data;
    ctd_allocator* allocator;
} ctd_expandable_arena_context;

ctd_expandable_arena_allocator ctd_expandable_arena_allocator_create(ptrdiff_t size, ctd_allocator* allocator);
void ctd_expandable_arena_allocator_destroy(ctd_expandable_arena_allocator* self);

#endif // CTD_EXPANDABLE_ARENA_ALLOCATOR_H
