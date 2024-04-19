#include <ctd_arena.h>
#include <stdint.h>
#include <string.h>
#include <stdalign.h>
#include <ctd_define.h>

typedef struct ctd_arena_context
{
    ptrdiff_t length;
    ptrdiff_t capacity;
    char* data;
} ctd_arena_context;

// TODO maybe make size and count to overflow check?
static void* ctd_arena_allocator_allocate(void* context, ptrdiff_t size, ptrdiff_t align)
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

static void* ctd_arena_allocator_reallocate(void* context, void* source, const ptrdiff_t old_size, const ptrdiff_t new_size, const ptrdiff_t align)
{
    ctd_arena_context* arena = context;

    const ptrdiff_t padding = -(uintptr_t)(arena->data + arena->length) & (align-1);
    const ptrdiff_t difference = new_size - old_size;
    const ptrdiff_t abs_difference = difference >= 0 ? difference : -difference;
    const ptrdiff_t available_space = arena->capacity - arena->length - padding;

    // If there isn't any change, simply return the source pointer
    if (difference == 0)
    {
        return source;
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
    // Otherwise,
    // If the object is shrinked, we zero out the memory but we don't change the position of arena->beginning
    if (difference < 0)
    {
        memset(arena->data + arena->length + difference, 0, abs_difference);
        return source;
    }

    // If the object is expanded, we move it to the front of the arena, and we change arena->beginning by new_size instead of difference
    if (available_space < new_size)
    {
        return NULL;
    }
    char* destination = arena->data + arena->length + padding;
    memmove(destination, source, old_size);
    arena->length += padding + new_size;
    return destination;
}

static void ctd_arena_allocator_deallocate(void* context, void* block, ptrdiff_t size)
{
    ctd_arena_context* arena = (ctd_arena_context* )context;
    memset(block, 0, size);
    if (block == arena->data + arena->length - size)
    {
        arena->length -= size;
    }
}

ctd_arena_allocator ctd_arena_allocator_create(ptrdiff_t size, ctd_allocator* alloc)
{
    ctd_arena_allocator arena = {0};
    ctd_arena_context* context = alloc->allocate(alloc->context, sizeof(ctd_arena_context), alignof(ctd_arena_context));
    if (context == NULL) return arena;
    context->data = alloc->allocate(alloc->context, size, alignof(char));
    if (context->data == NULL) return arena;
    context->capacity = size;
    arena.allocator.allocate = ctd_arena_allocator_allocate;
    arena.allocator.reallocate = ctd_arena_allocator_reallocate;
    arena.allocator.deallocate = ctd_arena_allocator_deallocate;
    arena.allocator.context = context;

    return arena;
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
    allocator->deallocate(allocator->context, arena->data, arena->capacity);
    allocator->deallocate(allocator->context, arena, sizeof(ctd_arena_context));
    *arena = (ctd_arena_context){0};
    *self = (ctd_arena_allocator){0};
}