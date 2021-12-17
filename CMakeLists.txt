cmake_minimum_required(VERSION 3.16)

project(WorkerQueue)

set(CMAKE_CXX_STANDARD 14)
set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_DIR})
set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/examples)

set(PROJECT_SOURCES 
        ${PROJECT_SOURCE_DIR}/main.cpp 
    )

set(PROJECT_HEADERS
        ${PROJECT_INCLUDE_DIR}/concurrency/WorkerQueue.h
        ${PROJECT_INCLUDE_DIR}/version.h
        )


include_directories(${PROJECT_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} )
target_link_libraries(${PROJECT_NAME} -lpthread  )

if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/Zc:__cplusplus")
endif()
