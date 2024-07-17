#include <ctd_page_allocator.h>
#include <ctd_arena_allocator.h>
#include <ctd_internal_dynamic_array.h>
#include <stdalign.h>
#include <ctd_define.h>
#include <string.h>
#include <ctd_error.h>

typedef struct ctd_page_context
{
    ctd_internal_dynamic_array(ctd_arena_allocator) arenas;
    ptrdiff_t default_page_size;
    ctd_allocator* allocator;
} ctd_page_context;

/**
 * Adds a new arena to the end of a page allocator
 *
 * @param page_context Context of page allocator
 * @param size Size of the new arena in bytes
 * @param error Pointer to error struct
 */
static void add_new_arena(ctd_page_context* page_context, const ptrdiff_t size, ctd_error* error)
{
    const ptrdiff_t new_arena_size = ctd_max(size, page_context->default_page_size);
    ctd_arena_allocator new_arena = ctd_arena_allocator_create(new_arena_size, page_context->allocator);
    // We shouldn't deallocate the arena list's data because that could have catastrophic consequences to the user
    if (new_arena.allocator.context == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Failed to allocate new arena";
    }

    ctd_internal_dynamic_array_append_with_allocator(page_context->arenas, ctd_arena_allocator, new_arena, *page_context->allocator, error);
}

/**
 * Allocates memory from a page allocator
 *
 * @param context Context of page allocator
 * @param size Size of memory to be allocated in bytes
 * @param align Alignment of memory to be allocated
 * @return Pointer to allocated memory if allocation is successful, otherwise returns NULL.
 */
static void* ctd_page_allocator_allocate(void* context, const ptrdiff_t size, const ptrdiff_t align)
{
    ctd_error error = {0};
    ctd_page_context* page_context = context;
    ctd_allocator current_arena = page_context->arenas.data[page_context->arenas.length - 1].allocator;

    void* data = current_arena.allocate(current_arena.context, size, align);
    if (data != NULL) return data;

    // Otherwise, the arena is full, and we need to make a new one
    add_new_arena(page_context, size, &error);
    if (error.error_type != NO_ERROR) return NULL;

    ctd_allocator new_arena = page_context->arenas.data[page_context->arenas.length - 1].allocator;
    return new_arena.allocate(new_arena.context, size, align);
}

/**
 * Reallocates a region of memory.
 * Note - since it is undefined behavior to compare pointers not in the same region of memory, and the page allocator
 * operates with a bunch of distinct memory regions, reallocation will never resize a region of memory in-place - it
 * will always allocate a new chunk of memory. However, reallocation will copy the old data into the new chunk of
 * memory.
 *
 * @param context Page allocator's context
 * @param source Pointer to the memory to be reallocated
 * @param old_size The size of the memory to be reallocated
 * @param new_size The size the memory will be reallocated to
 * @param align The alignment of the region of memory
 * @return Pointer to the reallocated memory if reallocation succeeds, otherwise returns NULL pointer.
 */
static void* ctd_page_allocator_reallocate(void* context, void* source, const ptrdiff_t old_size, const ptrdiff_t new_size, const ptrdiff_t align)
{
    void* new_data = ctd_page_allocator_allocate(context, new_size, align);
    if (new_data == NULL) return NULL;

    memcpy(new_data, source, old_size);

    return new_data;
}

/**
 * Deallocates a region of memory.
 * Note - it is too expensive to individually go through each arena and run the deallocate function, since each
 * arena.deallocate call sets the memory to zero, so this function doesn't try and deallocate the memory - it simply
 * sets the memory to zero.
 * TODO add flag to allow this to occur.
 *
 * @param context Page allocator's context
 * @param block Pointer to memory to be deallocated
 * @param size Size of memory to be deallocated
 */
static void ctd_page_allocator_deallocate(void* context, void* block, ptrdiff_t size)
{
    memset(block, 0, size);
}

ctd_page_allocator ctd_page_allocator_create(ptrdiff_t default_page_size, ctd_allocator* allocator)
{
    ctd_page_allocator page_allocator = {0};
    ctd_page_context* context = allocator->allocate(allocator->context, sizeof(ctd_page_context), alignof(ctd_page_context));
    if (context == NULL) goto context_alloc_failed_cleanup;

    context->arenas.data = allocator->allocate(allocator->context, sizeof(ctd_arena_allocator), alignof(ctd_arena_allocator));
    if (context->arenas.data == NULL) goto arena_array_alloc_failed_cleanup;
    context->default_page_size = default_page_size;
    context->allocator = allocator;
    context->arenas.length = 1;
    context->arenas.capacity = 1;

    context->arenas.data[0] = ctd_arena_allocator_create(default_page_size, allocator);
    if (context->arenas.data[0].allocator.context == NULL) goto individual_arena_alloc_failed_cleanup;

    page_allocator.allocator.allocate = ctd_page_allocator_allocate;
    page_allocator.allocator.reallocate = ctd_page_allocator_reallocate;
    page_allocator.allocator.deallocate = ctd_page_allocator_deallocate;
    page_allocator.allocator.context = context;

    return page_allocator;

individual_arena_alloc_failed_cleanup:
    allocator->deallocate(allocator->context, context->arenas.data, sizeof(ctd_arena_allocator));
arena_array_alloc_failed_cleanup:
    allocator->deallocate(allocator->context, context, sizeof(ctd_page_context));
context_alloc_failed_cleanup:
    return (ctd_page_allocator) {0};
}

void ctd_page_allocator_destroy(ctd_page_allocator* self)
{
    ctd_page_context* context = self->allocator.context;
    ctd_allocator* underlying_allocator = context->allocator;
    ctd_arena_allocator current_arena;

    for (ptrdiff_t i = context->arenas.length - 1; i >= 0; i--)
    {
        current_arena = context->arenas.data[i];
        ctd_arena_allocator_destroy(&current_arena, underlying_allocator);
    }
    underlying_allocator->deallocate(underlying_allocator->context, context->arenas.data, context->arenas.capacity * sizeof(ctd_arena_allocator));
    underlying_allocator->deallocate(underlying_allocator->context, context, sizeof(ctd_page_context));

    *self = (ctd_page_allocator) {0};
}
