# Basic Setting
cmake_minimum_required(VERSION 4.0)
# project(smath)
project(smath_dev)

# C++ Standard
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Header only Library
add_subdirectory(./include/smath)
# add_library(smath INTERFACE)
# target_include_directories(smath INTERFACE include)

# Test
option(SMATH_BUILD_TESTS "Build the smath Test" ON)

if(SMATH_BUILD_TESTS)
    file(GLOB TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "test/*test.cpp")
    foreach(TEST_SOURCE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
        add_executable(${TEST_NAME}
          ${TEST_SOURCE}
          test/test_tool.hpp
        )
        target_link_libraries(${TEST_NAME} PRIVATE smath)
        add_test(NAME ${TEST_NAME}_unit COMMAND ${TEST_NAME})
    endforeach()
    enable_testing()
endif()
