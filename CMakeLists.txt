cmake_minimum_required(VERSION 3.21)

project(WorkerQueue VERSION "1.0.0")

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include(CTest)
enable_testing()

add_subdirectory(concurrency)
add_subdirectory(examples)
