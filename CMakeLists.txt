cmake_minimum_required(VERSION 3.0)

project(cls2json)

set(CMAKE_CXX_FLAGS "-Wall -O2 -std=c++17 -pthread")

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
add_subdirectory(src)
