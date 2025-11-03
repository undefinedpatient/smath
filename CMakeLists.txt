# Basic Setting
cmake_minimum_required(VERSION 4.0)
project(smath)

# C++ Standard
set(CMAKE_CXX_STANDARD 26)

# Header only Library
add_library(smath INTERFACE)
target_include_directories(smath INTERFACE include)
add_library(smath::smath ALIAS smath)

# Test
option(SMATH_BUILD_TESTS "Build the smath Test" ON)

if(SMATH_BUILD_TESTS)
    add_executable(smath_test
            test/main.cpp
            test/vec3_test.cpp
            include/message.h
    )
    target_link_libraries(smath_test PRIVATE smath::smath)
    enable_testing()
    add_test(NAME smath_basic_test COMMAND smath_test)
endif()