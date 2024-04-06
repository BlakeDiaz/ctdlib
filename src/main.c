#include "error_message.h"
#include <stdio.h>
#include <ctd_string.h>
#include <allocator.h>
#include <arena.h>
#include <stdalign.h>
#include <string.h>

int main()
{
    Error error = {0};
    CTD_String str = ctd_string_create_from_c_string("Hi \tthe\vre John!\n", &error);

    CTD_String copy = ctd_string_remove_whitespace(str, &error);
    ERROR_ABORT(error);
    printf("Original: %.*s", (int)str.length, str.data);
    printf("Copy: %.*s\n", (int)copy.length, copy.data);

    ctd_string_destroy(&str);
    ctd_string_destroy(&copy);

    default_allocator lib_alloc = default_allocator_create();
    arena_context context = {0};
    arena_allocator arena = arena_allocator_create(&context, 1024, &lib_alloc.allocator);
    char* stuff = arena.allocator.allocate(arena.allocator.context, 17 * sizeof(char), alignof(char));
    memcpy(stuff, "Hi there John!!\n", 17 * sizeof(char));
    uint64_t* numbers = arena.allocator.allocate(arena.allocator.context, 3 * sizeof(uint64_t), alignof(uint64_t));
    numbers[0] = 3;
    numbers[1] = 18993;
    numbers[2] = 98;
    printf("%s", stuff);
    printf("%p\n", (void*)numbers);
    arena.allocator.reallocate(arena.allocator.context, numbers, 3 * sizeof(uint64_t), 6 * sizeof(uint64_t), alignof(uint64_t));
    printf("%p\n", (void*)numbers);

    for (ptrdiff_t i = 0; i < 3; i++)
    {
        printf("%llu\n", numbers[i]);
    }

    arena_allocator_destroy(&arena, &lib_alloc.allocator);
    return 0;
}
