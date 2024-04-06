cmake_minimum_required(VERSION 3.25)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(ctdlib C)

set(CMAKE_C_STANDARD 11)

add_library(ctdlib
    src/ctd_string.c
    src/misc_utils.c
    src/allocator.c
    src/arena.c
)

target_include_directories(ctdlib PUBLIC include)
