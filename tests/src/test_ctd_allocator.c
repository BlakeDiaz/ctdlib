#include <test.h>
#include <ctd_allocator.h>
#include <stdint.h>
#include <stdalign.h>
#include <stdlib.h>

int test_ctd_heap_allocator_create()
{
    const ctd_heap_allocator heap_allocator = ctd_heap_allocator_create();
    const ctd_allocator allocator = heap_allocator.allocator;

    if (allocator.allocate == NULL) return 1;
    if (allocator.reallocate == NULL) return 1;
    if (allocator.deallocate == NULL) return 1;
    if (allocator.context != NULL) return 1;

    return 0;
}

int test_ctd_heap_allocator_allocate()
{
    ctd_heap_allocator heap_allocator = ctd_heap_allocator_create();
    ctd_allocator allocator = heap_allocator.allocator;

    ptrdiff_t size = 100 * sizeof(uint32_t);
    char* buffer = allocator.allocate(allocator.context, size, alignof(uint32_t));
    if (buffer == NULL) return 1;
    free(buffer);
    return 0;
}


int test_ctd_heap_allocator_reallocate()
{
    ctd_heap_allocator heap_allocator = ctd_heap_allocator_create();
    ctd_allocator allocator = heap_allocator.allocator;

    const ptrdiff_t size = 100 * sizeof(uint32_t);
    char* buffer = allocator.allocate(allocator.context, size, alignof(uint32_t));
    if (buffer == NULL) return 1;
    const ptrdiff_t new_size = size * 2;
    char* new_buffer = allocator.reallocate(allocator.context, buffer,  size, new_size, alignof(uint32_t));
    if (new_buffer == NULL) goto cleanup;

    free(new_buffer);
    return 0;
cleanup:
    free(buffer);
    return 1;
}

void test_ctd_allocator_functions()
{
    int status;
    uint32_t number_of_tests_failed = 0;
    printf("---------- Begin ctd_allocator Test ----------\n");

    RUN_TEST(ctd_heap_allocator_create, status, number_of_tests_failed)
    RUN_TEST(ctd_heap_allocator_allocate, status, number_of_tests_failed)
    RUN_TEST(ctd_heap_allocator_reallocate, status, number_of_tests_failed)

    if (number_of_tests_failed == 0)
    {
        printf("\x1b[32mAll tests passed!\x1b[0m\n");
    }
    else
    {
        printf("\x1b[31m%u tests failed.\x1b[0m\n", number_of_tests_failed);
    }
    printf("---------- End ctd_allocator Test ----------\n\n");
}
