cmake_minimum_required(VERSION 3.31)

set(NAME tn)
project(${NAME} C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
            -Wall
            -Wextra
            -Wpedantic
            -Werror
            -Wshadow
            -Wconversion
            -Wformat=2
            -Wnull-dereference
            -Wstrict-prototypes
            -Wmissing-prototypes
            -Wcast-align
            -Wundef
    )
endif()

add_executable(${NAME} src/main.c
        src/clip/clip.c
        src/clip/clip.h
        src/sapper/sapper.c
        src/sapper/sapper.h
        src/common/common.c
        src/common/common.h
        src/common/console.c
        src/common/console.h)

target_include_directories(${NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
