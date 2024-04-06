#include <stdlib.h>
#include <allocator.h>

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

default_allocator default_allocator_instance = {.allocator = {.context = NULL, .allocate = lib_malloc, .reallocate = lib_realloc, .free = lib_free}};

default_allocator default_allocator_create()
{
    allocator_t allocator = {.allocate = lib_malloc, .reallocate = lib_realloc, .free = lib_free, .context = NULL};
    return (default_allocator){.allocator = allocator};
}

