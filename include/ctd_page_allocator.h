#ifndef CTD_PAGE_ALLOCATOR_H
#define CTD_PAGE_ALLOCATOR_H
#include <ctd_allocator.h>

/**
 * This allocator aims to solve the problem of using arena allocators that need to grow. Specifically, uses where having
 * a large chunk of memory to put stuff in is useful, but you don't know how much data you're going to use. This
 * allocator works by storing a dynamic array of arenas that are continually allocated, so that if you run out of space
 * in one arena, instead of reallocing and potentially changing the memory adresses of your data, you simply make a new
 * arena somewhere while keeping the old to free later.
 *
 * This should primarily be used in conjunction with a heap allocator to avoid running out of memory for new arenas.
 */
typedef struct ctd_page_allocator
{
    ctd_allocator allocator;
} ctd_page_allocator;

/**
 * Creates a page allocator.
 *
 * @param default_page_size Default size of each arena in bytes.
 * @param allocator Allocator used to allocate the context of and each arena in the page allocator.
 * @return Page allocator if creation is successful, otherwise returns an empty object. This can be checked by seeing if
 * the allocator's context pointer is NULL or not with page_allocator_name.allocator.context == NULL.
 */
ctd_page_allocator ctd_page_allocator_create(ptrdiff_t default_page_size, ctd_allocator* allocator);
/**
 * Destroys a page allocator.
 *
 * @param self Page allocator to be destroyed
 */
void ctd_page_allocator_destroy(ctd_page_allocator* self);

#endif //CTD_PAGE_ALLOCATOR_H
