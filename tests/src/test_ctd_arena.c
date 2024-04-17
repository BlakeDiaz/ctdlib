#include <ctd_arena.h>
#include <test_ctd_arena.h>
#include <test.h>
#include <stdint.h>
#include <stdalign.h>

int test_ctd_arena_allocator_create()
{
    ctd_allocator heap_allocator = ctd_default_allocator_create().allocator;
    const ctd_allocator arena = ctd_arena_allocator_create(100 * sizeof(char), &heap_allocator).allocator;
    ctd_arena_context* context = arena.context;
    if (arena.allocate == NULL && arena.reallocate == NULL && arena.deallocate == NULL && arena.context == NULL) return 1;
    if (arena.allocate == NULL) goto cleanup;
    if (arena.reallocate == NULL) goto cleanup;
    if (arena.deallocate == NULL) goto cleanup;
    if (context == NULL) goto cleanup;
    if (context->data == NULL) goto cleanup;
    if (context->length != 0) goto cleanup;
    if (context->capacity != 100 * sizeof(char)) goto cleanup;

    heap_allocator.deallocate(heap_allocator.context, context->data, 100 * sizeof(char));
    heap_allocator.deallocate(heap_allocator.context, context, sizeof(ctd_arena_context));
    return 0;
cleanup:
    // This is safe b.c. heap_allocator's deallocatoe is a wrapper around free
    heap_allocator.deallocate(heap_allocator.context, context->data, 100 * sizeof(char));
    heap_allocator.deallocate(heap_allocator.context, context, sizeof(ctd_arena_context));
    return 1;
}
int test_ctd_arena_allocate()
{
    ctd_allocator heap_allocator = ctd_default_allocator_create().allocator;
    const ctd_allocator arena = ctd_arena_allocator_create(100 * sizeof(char), &heap_allocator).allocator;
    ctd_arena_context* context = arena.context;

    char* data = arena.allocate(context, 4 * sizeof(uint32_t), alignof(uint32_t));
    if (data == NULL) goto cleanup;
    // < is to account for padding
    if (context->length < 4 * sizeof(uint32_t)) goto cleanup;

    heap_allocator.deallocate(heap_allocator.context, context->data, 100 * sizeof(char));
    heap_allocator.deallocate(heap_allocator.context, context, sizeof(ctd_arena_context));
    return 0;
cleanup:
    heap_allocator.deallocate(heap_allocator.context, context->data, 100 * sizeof(char));
    heap_allocator.deallocate(heap_allocator.context, context, sizeof(ctd_arena_context));
    return 1;
}

int test_ctd_arena_reallocate()
{
    ctd_allocator heap_allocator = ctd_default_allocator_create().allocator;
    const ctd_allocator arena = ctd_arena_allocator_create(100 * sizeof(char), &heap_allocator).allocator;
    ctd_arena_context* context = arena.context;

    char* data = arena.allocate(context, 4 * sizeof(uint32_t), alignof(uint32_t));
    ptrdiff_t initial_length = context->length;
    char* new_data = arena.reallocate(context, data, 4 * sizeof(uint32_t), 8 * sizeof(uint32_t), alignof(uint32_t));
    if (new_data == NULL) goto cleanup;
    if (new_data != data) goto cleanup;
    if (context->length != initial_length + 4 * sizeof(uint32_t)) goto cleanup;

    heap_allocator.deallocate(heap_allocator.context, context->data, 100 * sizeof(char));
    heap_allocator.deallocate(heap_allocator.context, context, sizeof(ctd_arena_context));
    return 0;
cleanup:
    heap_allocator.deallocate(heap_allocator.context, context->data, 100 * sizeof(char));
    heap_allocator.deallocate(heap_allocator.context, context, sizeof(ctd_arena_context));
    return 1;

    return 0;
}

int test_ctd_arena_deallocate()
{
    ctd_allocator heap_allocator = ctd_default_allocator_create().allocator;
    const ctd_allocator arena = ctd_arena_allocator_create(100 * sizeof(char), &heap_allocator).allocator;
    ctd_arena_context* context = arena.context;

    char* data_1 = arena.allocate(context, 4 * sizeof(uint32_t), alignof(uint32_t));
    char* data_2 = arena.allocate(context, 4 * sizeof(uint64_t), alignof(uint64_t));
    arena.deallocate(context, data_2, 4 * sizeof(uint64_t));
    if (*data_2 != 0) goto cleanup;
    char* data_3 = arena.allocate(context, 4 * sizeof(uint64_t), alignof(uint64_t));
    if (data_2 != data_3) goto cleanup;
    arena.deallocate(context, data_1, 4 * sizeof(uint32_t));
    if (*data_1 != 0) goto cleanup;

    heap_allocator.deallocate(heap_allocator.context, context->data, 0);
    heap_allocator.deallocate(heap_allocator.context, context, 0);
    return 0;
cleanup:
    heap_allocator.deallocate(heap_allocator.context, context->data, 0);
    heap_allocator.deallocate(heap_allocator.context, context, 0);
    return 1;
}

void test_ctd_arena_functions()
{
    int status;
    uint32_t number_of_tests_failed = 0;
    printf("---------- Begin ctd_arena Test ----------\n");

    RUN_TEST(ctd_arena_allocator_create, status, number_of_tests_failed)
    RUN_TEST(ctd_arena_allocate, status, number_of_tests_failed)
    RUN_TEST(ctd_arena_reallocate, status, number_of_tests_failed)
    RUN_TEST(ctd_arena_deallocate, status, number_of_tests_failed)

    if (number_of_tests_failed == 0)
    {
        printf("\x1b[32mAll tests passed!\x1b[0m\n");
    }
    else
    {
        printf("\x1b[31m%u tests failed.\x1b[0m\n", number_of_tests_failed);
    }
    printf("---------- End ctd_arena Test ----------\n\n");
}
