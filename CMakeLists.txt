cmake_minimum_required(VERSION 3.25)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(test C)

set(CMAKE_C_STANDARD 11)

add_compile_options("-Wall" "-Wextra")

add_library(ctdlib
    src/ctd_string.c
    src/ctd_define.c
    src/ctd_allocator.c
    src/ctd_arena_allocator.c
    src/ctd_expandable_arena_allocator.c
    src/ctd_page_allocator.c
)

target_include_directories(ctdlib PUBLIC include)

add_executable(test_ctdlib
    tests/src/test.c
    tests/src/test_ctd_string.c
    tests/src/test_ctd_allocator.c
    tests/src/test_ctd_arena_allocator.c
    tests/src/test_ctd_expandable_arena_allocator.c
    tests/src/test_ctd_page_allocator.c
)
target_include_directories(test_ctdlib PUBLIC tests/include)

target_link_libraries(test_ctdlib ctdlib)

target_compile_options(test_ctdlib PRIVATE "-fsanitize=address,undefined")
target_link_options(test_ctdlib PRIVATE "-fsanitize=address,undefined")
