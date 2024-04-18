#include <stdlib.h>
#include <ctd_allocator.h>

static void* ctd_malloc(void* context, ptrdiff_t size, ptrdiff_t align)
{
    (void)context;
    return malloc(size);
}

static void* ctd_calloc(void* context, ptrdiff_t size, ptrdiff_t count, ptrdiff_t align)
{
    (void)context;
    return calloc(count, size);
}

static void* ctd_realloc(void* context, void* source, ptrdiff_t old_size, ptrdiff_t new_size, ptrdiff_t align)
{
    (void)context;
    (void)old_size;
    return realloc(source, new_size);
}

static void ctd_free(void* context, void* block, ptrdiff_t size)
{
    (void)context;
    (void)size;
    free(block);
}

ctd_default_allocator ctd_default_allocator_instance = {.allocator = {.context = NULL, .allocate = ctd_malloc, .reallocate = ctd_realloc, .deallocate = ctd_free}};

ctd_default_allocator ctd_default_allocator_create()
{
    ctd_allocator allocator = {.allocate = ctd_malloc, .reallocate = ctd_realloc, .deallocate = ctd_free, .context = NULL};
    return (ctd_default_allocator){.allocator = allocator};
}

