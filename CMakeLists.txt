cmake_minimum_required(VERSION 3.14)
project(memory_leak_detector C)

set(CMAKE_C_STANDARD 11)

add_executable(memory_leak_detector main.c tests/structure_offset/structure_offset.h tests/structure_offset/structure_offset.c tests/array_of_structures/array_of_structures.h tests/array_of_structures/array_of_structures.c mld.h mld.c css.h)