cmake_minimum_required(VERSION 3.21)

project(WorkerQueue VERSION "1.1.0")

set(CMAKE_CXX_STANDARD 17)

# build options
option(BUILD_EXAMPLES_SRC "Build examples source files" ON)
option(BUILD_TESTS_SRC "Build test source files" ON)

include(CTest)
enable_testing()

add_subdirectory(concurrency)
if (BUILD_EXAMPLES_SRC)
    add_subdirectory(examples)
endif()

if (BUILD_TESTS_SRC)
    add_subdirectory(test)
endif()