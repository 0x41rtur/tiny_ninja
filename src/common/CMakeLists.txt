cmake_minimum_required(VERSION 3.15)

project(common C CXX)

# Источники common
file(GLOB COMMON_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

# Создаем статическую библиотеку common
add_library(common STATIC ${COMMON_SOURCES})

# Включаем заголовки для всех, кто линковался с common
target_include_directories(common
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Подключаем unity (библиотека для тестов)
add_library(unitycommon STATIC ${CMAKE_SOURCE_DIR}/src/lib/unity/unity.c)
target_include_directories(unitycommon SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/src/lib/unity)

# Тесты для common
enable_testing()

file(GLOB COMMON_TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c
)

add_executable(test_common ${COMMON_TEST_SOURCES})

# unitycommon для тестов, а также common для линковки
target_link_libraries(test_common PRIVATE common unitycommon)

# Для теста указываем путь к unity заголовкам (включаем как system)
target_include_directories(test_common SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/src/lib/unity)

add_test(NAME test_common COMMAND test_common)
