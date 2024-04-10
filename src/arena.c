#include <arena.h>
#include <stdint.h>
#include <string.h>
#include <stdalign.h>

// TODO maybe make size and count to overflow check?
void* ctd_arena_allocate(void* context, ptrdiff_t size, ptrdiff_t align)
{
    ctd_arena_context* arena = context;

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

void* ctd_arena_reallocate(void* context, void* source, ptrdiff_t old_size, ptrdiff_t new_size, ptrdiff_t align)
{
    ctd_arena_context* arena = context;

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

void ctd_arena_free(void* context, void* block, ptrdiff_t size)
{
    ctd_arena_context* arena = (ctd_arena_context* )context;
    memset(block, 0, size);
    if (block == arena->data + arena->length - size)
    {
        arena->length -= size;
    }
}

ctd_arena_allocator ctd_arena_allocator_create(ctd_arena_context* context, ptrdiff_t size, ctd_allocator* alloc)
{
    context->data = alloc->allocate(alloc->context, size, alignof(char));
    context->capacity = size;
    ctd_allocator allocator = {0};
    allocator.allocate = ctd_arena_allocate;
    allocator.reallocate = ctd_arena_reallocate;
    allocator.free = ctd_arena_free;
    allocator.context = context;

    return (ctd_arena_allocator){.allocator = allocator};
}

/**
 * Destroys and frees the memory inside an allocator.
 *
 * @param self The allocator you want to destroy.
 * @param allocator The allocator you created self with.
 */
void ctd_arena_allocator_destroy(ctd_arena_allocator* self, ctd_allocator* allocator)
{
    ctd_arena_context* arena = self->allocator.context;
    allocator->free(allocator->context, arena->data, arena->capacity);
    *arena = (ctd_arena_context){0};
    *self = (ctd_arena_allocator){0};
}