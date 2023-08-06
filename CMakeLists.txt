cmake_minimum_required(VERSION 3.16)

project(WorkerQueue)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_DIR ${CMAKE_SOURCE_DIR})

#enable_testing()

add_subdirectory(concurrency)
add_subdirectory(examples)