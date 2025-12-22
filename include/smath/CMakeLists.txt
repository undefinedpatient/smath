# When refering to this library, you could just add add_subdirectory(path/to/smath)
# and link library via target_link_libraries(NAME PRIVATE smath)
# Basic Setting
cmake_minimum_required(VERSION 4.0)
project(smath)

# C++ Standard
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQURIED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Header only Library
add_library(smath INTERFACE)
target_include_directories(smath INTERFACE .)