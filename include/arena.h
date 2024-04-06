#ifndef ARENA_H
#define ARENA_H
#include <allocator.h>

typedef struct arena_allocator
{
    allocator_t allocator;
} arena_allocator;

typedef struct expandable_arena_allocator
{
    allocator_t allocator;
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
    allocator_t* allocator;
} expandable_arena_context;


arena_allocator arena_allocator_create(arena_context* context, ptrdiff_t size, allocator_t* alloc);
expandable_arena_allocator expandable_arena_allocator_create(expandable_arena_context* context, ptrdiff_t size,
                                                             allocator_t* allocator);

void arena_allocator_destroy(arena_allocator* self, allocator_t* allocator);
void expandable_arena_allocator_destroy(expandable_arena_allocator* self);

#endif // ARENA_H
