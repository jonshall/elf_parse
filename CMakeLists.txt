cmake_minimum_required(VERSION 3.16)
project(elf_parse)

set(GCC_COVERAGE_COMPILE_FLAGS "-m32")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

include_directories(include)

set(EXECUTABLE_OUTPUT_PATH ../bin)

add_executable(elf_parse src/elf_parse.c)
