#include <ctd_expandable_arena_allocator.h>
#include <test_ctd_expandable_arena_allocator.h>
#include <test.h>
#include <stdint.h>
#include <stdalign.h>


int test_ctd_expandable_arena_allocate()
{
    return 0;
}
int test_ctd_expandable_arena_reallocate()
{
    return 0;
}
int test_ctd_expandable_arena_deallocate()
{
    return 0;
}

void test_ctd_expandable_arena_allocator_functions()
{

    int status;
    uint32_t number_of_tests_failed = 0;
    printf("---------- Begin ctd_expandable_arena Test ----------\n");

    RUN_TEST(ctd_expandable_arena_allocate, status, number_of_tests_failed)
    RUN_TEST(ctd_expandable_arena_reallocate, status, number_of_tests_failed)
    RUN_TEST(ctd_expandable_arena_deallocate, status, number_of_tests_failed)

    if (number_of_tests_failed == 0)
    {
        printf("\x1b[32mAll tests passed!\x1b[0m\n");
    }
    else
    {
        printf("\x1b[31m%u tests failed.\x1b[0m\n", number_of_tests_failed);
    }
    printf("---------- End ctd_expandable_arena Test ----------\n\n");
}