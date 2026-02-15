cmake_minimum_required(VERSION 3.22.1)

project("WorkerQueueAndroid")

# Sets LIB_SRC_DIR to the path of the target CMake project.
set(LIB_SRC_DIR ../../../../../../WorkerQueue)
# Sets LIB_BUILD_DIR to the path of the desired output directory.
set(LIB_BUILD_DIR ${LIB_SRC_DIR}/outputs)

# Adds the CMakeLists.txt file located in the specified directory
# as a build dependency.
add_subdirectory( # Specifies the directory of the CMakeLists.txt file.
        ${LIB_SRC_DIR}
        # Specifies the directory for the build outputs.
        ${LIB_BUILD_DIR})

add_library(${CMAKE_PROJECT_NAME} SHARED
WorkerQueueAndroid.cpp
)

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        android
        log
        WorkerQueue::WorkerQueue)
