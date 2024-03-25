cmake_minimum_required(VERSION 3.25)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(ctdlib C)

set(CMAKE_C_STANDARD 11)

add_executable(ctdlib src/main.c src/ctd_string.c src/misc_utils.c)

target_include_directories(ctdlib PUBLIC include)
