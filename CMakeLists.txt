cmake_minimum_required(VERSION 3.25)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(test C)

set(CMAKE_C_STANDARD 11)

add_library(ctdlib
    src/ctd_string.c
    src/ctd_define.c
    src/ctd_allocator.c
    src/ctd_arena_allocator.c
    src/ctd_expandable_arena_allocator.c
)

target_include_directories(ctdlib PUBLIC include)

add_executable(test
    tests/src/test.c
    tests/src/test_ctd_string.c
    tests/src/test_ctd_allocator.c
    tests/src/test_ctd_arena.c
)
target_include_directories(test PUBLIC tests/include)

target_link_libraries(test ctdlib)
