#include <ctd_expandable_arena_allocator.h>
#include <ctd_define.h>
#include <stdalign.h>
#include <stdint.h>
#include <string.h>

typedef struct ctd_expandable_arena_context
{
    ptrdiff_t length;
    ptrdiff_t capacity;
    char* data;
    ctd_allocator* allocator;
} ctd_expandable_arena_context;

static void ctd_expandable_arena_allocator_expand(ctd_expandable_arena_context* context, const ptrdiff_t expand_by)
{
    const ptrdiff_t new_capacity = ctd_max(context->capacity * 2 + 1, context->capacity + expand_by);
    char* new_data = context->allocator->reallocate(context->allocator->context, context->data, context->capacity, new_capacity, alignof(char));
    if (new_data == NULL)
    {
        // TODO handle error
        return;
    }
    context->data = new_data;
    context->capacity = new_capacity;
}

// TODO maybe make size and count to overflow check?
static void* ctd_expandable_arena_allocator_allocate(void* context, const ptrdiff_t size, const ptrdiff_t align)
{
    ctd_expandable_arena_context* expandable_arena = context;

    const ptrdiff_t padding = -(uintptr_t)(expandable_arena->data + expandable_arena->length) & (align-1);
    const ptrdiff_t available_space = expandable_arena->capacity - expandable_arena->length - padding;
    if (size > available_space)
    {
        ctd_expandable_arena_allocator_expand(expandable_arena, size - available_space);
    }
    void* ptr = expandable_arena->data + expandable_arena->length + padding;
    expandable_arena->length += size + padding;
    return ptr;
}

static void* ctd_expandable_arena_allocator_reallocate(void* context, void* source, const ptrdiff_t old_size, const ptrdiff_t new_size, const ptrdiff_t align)
{
    ctd_expandable_arena_context* expandable_arena = context;

    const ptrdiff_t padding = -(uintptr_t)(expandable_arena->data + expandable_arena->length) & (align-1);
    const ptrdiff_t difference = new_size - old_size;
    const ptrdiff_t abs_difference = difference >= 0 ? difference : -difference;
    const ptrdiff_t available_space = expandable_arena->capacity - expandable_arena->length - padding;

    // If there isn't any change, simply return the source pointer
    if (difference == 0)
    {
        return source;
    }
    // If the reallocated object is at the end of the expandable_arena, we can simply change the position of expandable_arena->beginning by difference
    if (expandable_arena->data + expandable_arena->length - old_size == source)
    {
        if (difference < 0)
        {
            memset(expandable_arena->data + expandable_arena->length + difference, 0, abs_difference);
        }
        else if (available_space < difference)
        {
            ctd_expandable_arena_allocator_expand(expandable_arena, difference - available_space);
        }
        expandable_arena->length += difference;

        return source;
    }
    // Otherwise,
    // If the object is shrinked, we zero out the memory but we don't change the position of expandable_arena->beginning
    if (difference < 0)
    {
        memset(expandable_arena->data + expandable_arena->length + difference, 0, abs_difference);
        return source;
    }

    // If the object is expanded, we move it to the front of the expandable_arena, and we change expandable_arena->beginning by new_size instead of difference
    if (available_space < new_size)
    {
        ctd_expandable_arena_allocator_expand(expandable_arena, new_size - available_space);
    }
    char* destination = expandable_arena->data + expandable_arena->length + padding;
    memmove(destination, source, old_size);
    expandable_arena->length += padding + new_size;
    return destination;
}

static void ctd_expandable_arena_allocator_deallocate(void* context, void* block, const ptrdiff_t size)
{
    ctd_expandable_arena_context* expandable_arena = context;
    memset(block, 0, size);
    if (block == expandable_arena->data + expandable_arena->length - size)
    {
        expandable_arena->length -= size;
    }
}

ctd_expandable_arena_allocator ctd_expandable_arena_allocator_create(const ptrdiff_t size, ctd_allocator* allocator)
{
    ctd_expandable_arena_allocator expandable_arena = {0};
    ctd_expandable_arena_context* context = allocator->allocate(allocator->context, sizeof(ctd_expandable_arena_context), alignof(ctd_expandable_arena_context));
    if (context == NULL) return expandable_arena;
    context->data = allocator->allocate(allocator->context, size, alignof(char));
    if (context->data == NULL) return expandable_arena;
    context->length = 0;
    context->capacity = size;
    context->allocator = allocator;
    expandable_arena.allocator.allocate = ctd_expandable_arena_allocator_allocate;
    expandable_arena.allocator.reallocate = ctd_expandable_arena_allocator_reallocate;
    expandable_arena.allocator.deallocate = ctd_expandable_arena_allocator_deallocate;
    expandable_arena.allocator.context = context;

    return expandable_arena;
}

/**
 * Destroys and frees the memory inside an expandable arena allocator.
 *
 * @param self The expandable arena allocator you want to destroy.
 */
void ctd_expandable_arena_allocator_destroy(ctd_expandable_arena_allocator* self)
{
    ctd_expandable_arena_context* expandable_arena = self->allocator.context;
    ctd_allocator* allocator = expandable_arena->allocator;
    allocator->deallocate(allocator->context, expandable_arena->data, expandable_arena->capacity);
    allocator->deallocate(allocator->context, expandable_arena, sizeof(ctd_expandable_arena_context));
    *expandable_arena = (ctd_expandable_arena_context){0};
    *self = (ctd_expandable_arena_allocator){0};
}