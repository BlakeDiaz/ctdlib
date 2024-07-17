#include <test_ctd_page_allocator.h>
#include <ctd_arena_allocator.h>
#include <ctd_page_allocator.h>
#include <ctd_internal_dynamic_array.h>
#include <test.h>
#include <stdint.h>
#include <stdalign.h>

typedef struct ctd_page_context
{
    ctd_internal_dynamic_array(ctd_arena_allocator) arenas;
    ptrdiff_t default_page_size;
    ctd_allocator* allocator;
} ctd_page_context;

int test_ctd_page_allocator_create()
{
    ctd_allocator heap_allocator = ctd_default_allocator_create().allocator;
    const ctd_allocator page_allocator = ctd_page_allocator_create(100 * sizeof(char), &heap_allocator).allocator;
    ctd_page_allocator wrapped_page_allocator = {.allocator = page_allocator};
    ctd_page_context* context = page_allocator.context;
    if (page_allocator.context == NULL) return 1;
    if (context->arenas.data == NULL) goto cleanup;

    ctd_page_allocator_destroy(&wrapped_page_allocator);
    return 0;
cleanup:;
    ctd_page_allocator_destroy(&wrapped_page_allocator);
    return 1;
}
int test_ctd_page_allocator_allocate()
{
    ctd_allocator heap_allocator = ctd_default_allocator_create().allocator;
    ctd_allocator page_allocator = ctd_page_allocator_create(100 * sizeof(char), &heap_allocator).allocator;
    ctd_page_allocator wrapped_page_allocator = {.allocator = page_allocator};
    ctd_page_context* context = page_allocator.context;
    void* first_alloc = page_allocator.allocate(page_allocator.context, 98 * sizeof(char), alignof(char));
    if (context->arenas.length != 1) goto cleanup;
    if (context->arenas.capacity != 1) goto cleanup;
    if (first_alloc == NULL) goto cleanup;
    void* second_alloc = page_allocator.allocate(page_allocator.context, 98 * sizeof(char), alignof(char));
    if (context->arenas.length <= 1) goto cleanup;
    if (context->arenas.capacity <= 1) goto cleanup;
    if (second_alloc == NULL) goto cleanup;
    void* third_alloc = page_allocator.allocate(page_allocator.context, 103 * sizeof(char), alignof(char));
    if (context->arenas.length <= 2) goto cleanup;
    if (context->arenas.capacity <= 2) goto cleanup;
    if (third_alloc == NULL) goto cleanup;

    ctd_page_allocator_destroy(&wrapped_page_allocator);
    return 0;
cleanup:;
    ctd_page_allocator_destroy(&wrapped_page_allocator);
    return 1;
}
int test_ctd_page_allocator_reallocate()
{
    ctd_allocator heap_allocator = ctd_default_allocator_create().allocator;
    ctd_allocator page_allocator = ctd_page_allocator_create(100 * sizeof(char), &heap_allocator).allocator;
    ctd_page_allocator wrapped_page_allocator = {.allocator = page_allocator};
    ctd_page_context* context = page_allocator.context;
    void* first_alloc = page_allocator.allocate(page_allocator.context, 98 * sizeof(char), alignof(char));
    if (context->arenas.length != 1) goto cleanup;
    if (context->arenas.capacity != 1) goto cleanup;
    if (first_alloc == NULL) goto cleanup;
    uint32_t* first_alloc_data = first_alloc;
    first_alloc_data[0] = 4;
    first_alloc_data[1] = 5;
    first_alloc_data[2] = 6;
    first_alloc_data[3] = 7;
    void* second_alloc = page_allocator.reallocate(page_allocator.context, first_alloc, 98 * sizeof(char), 104 * sizeof(char), alignof(char));
    if (context->arenas.length <= 1) goto cleanup;
    if (context->arenas.capacity <= 1) goto cleanup;
    if (second_alloc == NULL) goto cleanup;
    uint32_t* second_alloc_data = second_alloc;
    if (second_alloc_data[0] != 4) goto cleanup;
    if (second_alloc_data[1] != 5) goto cleanup;
    if (second_alloc_data[2] != 6) goto cleanup;
    if (second_alloc_data[3] != 7) goto cleanup;

    ctd_page_allocator_destroy(&wrapped_page_allocator);
    return 0;
cleanup:
    ctd_page_allocator_destroy(&wrapped_page_allocator);
    return 1;
}
int test_ctd_page_allocator_deallocate()
{
    ctd_allocator heap_allocator = ctd_default_allocator_create().allocator;
    ctd_allocator page_allocator = ctd_page_allocator_create(100 * sizeof(char), &heap_allocator).allocator;
    ctd_page_allocator wrapped_page_allocator = {.allocator = page_allocator};
    char* data = page_allocator.allocate(page_allocator.context, 5 * sizeof(char), alignof(char));
    if (data == NULL) goto cleanup;
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    data[3] = 4;
    data[4] = 5;

    page_allocator.deallocate(page_allocator.context, data, 5 * sizeof(char));
    if (data[0] != 0) goto cleanup;
    if (data[1] != 0) goto cleanup;
    if (data[2] != 0) goto cleanup;
    if (data[3] != 0) goto cleanup;
    if (data[4] != 0) goto cleanup;

    ctd_page_allocator_destroy(&wrapped_page_allocator);
    return 0;
cleanup:
    ctd_page_allocator_destroy(&wrapped_page_allocator);
    return 1;
}
void test_ctd_page_allocator_functions()
{
    int status;
    uint32_t number_of_tests_failed = 0;
    printf("---------- Begin ctd_page_allocator Test ----------\n");

    RUN_TEST(ctd_page_allocator_create, status, number_of_tests_failed)
    RUN_TEST(ctd_page_allocator_allocate, status, number_of_tests_failed)
    RUN_TEST(ctd_page_allocator_reallocate, status, number_of_tests_failed)
    RUN_TEST(ctd_page_allocator_deallocate, status, number_of_tests_failed)

    if (number_of_tests_failed == 0)
    {
        printf("\x1b[32mAll tests passed!\x1b[0m\n");
    }
    else
    {
        printf("\x1b[31m%u tests failed.\x1b[0m\n", number_of_tests_failed);
    }
    printf("---------- End ctd_page_allocator Test ----------\n\n");
}
