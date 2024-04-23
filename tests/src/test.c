#include <test_ctd_allocator.h>
#include <test_ctd_arena_allocator.h>
#include <test_ctd_expandable_arena_allocator.h>
#include <test_ctd_string.h>

int main()
{
    // Command to check for memory leaks: leaks --atExit -- ./cmake-build-debug/test
    test_ctd_string_functions();
    test_ctd_allocator_functions();
    test_ctd_arena_allocator_functions();
    test_ctd_expandable_arena_functions();
    ctd_memory_debug_terminate();

    return 0;
}