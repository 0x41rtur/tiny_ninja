cmake_minimum_required(VERSION 3.15)

project(executer C CXX)

# Источники executer
file(GLOB EXECUTOR_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

# Создаем статическую библиотеку executer
add_library(executer STATIC ${EXECUTOR_SOURCES})

# Включаем заголовки для всех, кто линковался с common
target_include_directories(executer
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Подключаем unity (библиотека для тестов)
add_library(unityexecuter STATIC ${CMAKE_SOURCE_DIR}/src/lib/unity/unity.c)
target_include_directories(unityexecuter SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/src/lib/unity)

target_link_libraries(executer PUBLIC common fs clip)

# Тесты для common
enable_testing()

file(GLOB EXECUTOR_TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c
)

add_executable(test_executer ${EXECUTOR_TEST_SOURCES})

# unitycommon для тестов, а также common для линковки
target_link_libraries(test_executer PRIVATE executer unityexecuter)

# Для теста указываем путь к unity заголовкам (включаем как system)
target_include_directories(test_executer SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/src/lib/unity)

add_test(NAME test_executer COMMAND test_executer)
