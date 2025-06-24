cmake_minimum_required(VERSION 3.15)

project(fs C CXX)

# Источники fs
file(GLOB FS_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

# Создаем статическую библиотеку fs
add_library(fs STATIC ${FS_SOURCES})

# Включаем заголовки для всех, кто линковался с common
target_include_directories(fs
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Подключаем unity (библиотека для тестов)
add_library(unityfs STATIC ${CMAKE_SOURCE_DIR}/src/lib/unity/unity.c)
target_include_directories(unityfs SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/src/lib/unity)

target_link_libraries(fs PUBLIC common clip)

# Тесты для common
enable_testing()

file(GLOB FS_TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c
)

add_executable(test_fs ${FS_TEST_SOURCES})

# unitycommon для тестов, а также common для линковки
target_link_libraries(test_fs PRIVATE fs unityfs)

# Для теста указываем путь к unity заголовкам (включаем как system)
target_include_directories(test_fs SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/src/lib/unity)

add_test(NAME test_fs COMMAND test_fs)
