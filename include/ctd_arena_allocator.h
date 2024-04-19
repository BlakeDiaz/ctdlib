#ifndef CTD_ARENA_H
#define CTD_ARENA_H
#include <ctd_allocator.h>

typedef struct ctd_arena_allocator
{
    ctd_allocator allocator;
} ctd_arena_allocator;

ctd_arena_allocator ctd_arena_allocator_create(ptrdiff_t size, ctd_allocator* alloc);
void ctd_arena_allocator_destroy(ctd_arena_allocator* self, ctd_allocator* allocator);

#endif // CTD_ARENA_H
