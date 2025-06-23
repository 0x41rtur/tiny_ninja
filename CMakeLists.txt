cmake_minimum_required(VERSION 3.31)

set(NAME tn)
project(${NAME} C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

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
endif ()

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/lib/unity)

# Добавляем все модули
add_subdirectory(src/clip)
add_subdirectory(src/common)
add_subdirectory(src/fs)
add_subdirectory(src/executer)

# Главный исполняемый файл
add_executable(tn src/main.c)

# Линкуем его с нужными модулями
target_link_libraries(tn clip common fs executer)


