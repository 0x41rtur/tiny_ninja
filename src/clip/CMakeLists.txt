cmake_minimum_required(VERSION 3.15)

project(clip_module C CXX)

file(GLOB CLIP_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/internal/*.c
)

add_library(clip STATIC ${CLIP_SOURCES})

target_include_directories(clip
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

target_link_libraries(clip PUBLIC common)

enable_testing()

file(GLOB CLIP_TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c
)

add_executable(test_clip ${CLIP_TEST_SOURCES})

target_include_directories(test_clip SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/src/lib/unity)
add_library(unitylib STATIC ${CMAKE_SOURCE_DIR}/src/lib/unity/unity.c)

target_link_libraries(test_clip clip unitylib)

add_test(NAME test_clip COMMAND test_clip)
