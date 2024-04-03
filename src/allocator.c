#include <stdlib.h>
#include <string.h>
#include <allocator.h>
#include <stdint.h>
#include <stdalign.h>

void* lib_malloc(void* context, ptrdiff_t size, ptrdiff_t align)
{
    (void)context;
    return malloc(size);
}

void* lib_realloc(void* context, void* source, ptrdiff_t old_size, ptrdiff_t new_size, ptrdiff_t align)
{
    (void)context;
    (void)old_size;
    return realloc(source, new_size);
}

void lib_free(void* context, void* block, ptrdiff_t size)
{
    (void)context;
    (void)size;
    free(block);
}

void* arena_allocate(void* context, ptrdiff_t size, ptrdiff_t align)
{
    arena_context* arena = context;

    ptrdiff_t padding = -(uintptr_t)(arena->data + arena->length) & (align-1);
    ptrdiff_t available_space = arena->capacity - arena->length - padding;
    if (size > available_space)
    {
        return NULL;
    }
    void* ptr = arena->data + arena->length + padding;
    arena->length += size + padding;
    return ptr;
}

void* arena_reallocate(void* context, void* source, ptrdiff_t old_size, ptrdiff_t new_size, ptrdiff_t align)
{
    arena_context* arena = context;

    ptrdiff_t padding = -(uintptr_t)(arena->data + arena->length) & (align-1);
    ptrdiff_t difference = new_size - old_size;
    ptrdiff_t abs_difference = difference >= 0 ? difference : -difference;
    ptrdiff_t available_space = arena->capacity - arena->length - padding;

    if (difference == 0)
    {
        return NULL;
    }
    // If the reallocated object is at the end of the arena, we can simply change the position of arena->beginning by difference
    if (arena->data + arena->length - old_size == source)
    {
        if (difference < 0)
        {
            memset(arena->data + arena->length + difference, 0, abs_difference);
        }
        if (difference > 0 && available_space < difference)
        {
            return NULL;
        }
        arena->length += difference;

        return source;
    }
    /*
        Otherwise,
       If the object is shrinked, we zero out the memory but we don't change the position of arena->beginning
       If the object is expanded, we move it to the front of the arena, and we change arena->beginning by new_size instead of difference
    */
    else
    {
        if (difference < 0)
        {
            memset(arena->data + arena->length + difference, 0, abs_difference);
            return source;
        }
        else // difference > 0
        {
            if (available_space < new_size)
            {
                return NULL;
            }
            char* destination = arena->data + arena->length + padding;
            memmove(destination, source, old_size);
            arena->length += padding + new_size;
            return destination;
        }
    }
}

void arena_free(void* context, void* block, ptrdiff_t size)
{
    arena_context* arena = (arena_context* )context;
    memset(block, 0, size);
    if (block == arena->data + arena->length - size)
    {
        arena->length -= size;
    }
}

default_allocator default_allocator_create()
{
    allocator internal_allocator = {.allocate = lib_malloc, .reallocate = lib_realloc, .free = lib_free, .context = NULL};
    return (default_allocator){.internal_allocator = internal_allocator};
}

arena_allocator arena_allocator_create(arena_context* context, ptrdiff_t size, allocator* alloc)
{
    context->data = alloc->allocate(alloc->context, size, alignof(char));
    context->capacity = size;
    allocator internal_allocator = {0};
    internal_allocator.allocate = arena_allocate;
    internal_allocator.reallocate = arena_reallocate;
    internal_allocator.free = arena_free;
    internal_allocator.context = context;

    return (arena_allocator){.internal_allocator = internal_allocator};
}

void arena_allocator_destroy(arena_allocator* self, allocator* alloc)
{
    arena_context* arena = self->internal_allocator.context;
    alloc->free(alloc->context, arena->data, arena->capacity);
    *arena = (arena_context){0};
    *self = (arena_allocator){0};
}
