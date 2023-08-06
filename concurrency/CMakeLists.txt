set(LIBRARY_NAME WorkerQueue)
set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/concurrency)

# WorkerQueue library requires C++14 or higher features
if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()

set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_DIR})

set(PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/WorkerQueue.cpp)

include_directories(${PROJECT_INCLUDE_DIR})

# build options
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

if (BUILD_SHARED_LIBS)
    add_library(${LIBRARY_NAME} SHARED ${PROJECT_SOURCES})
    if (WIN32)
		add_compile_definitions(WORKERQUEUE_COMPILE_DLL=1)
		target_compile_definitions(${LIBRARY_NAME} PRIVATE "WORKERQUEUE_DLL_EXPORT")
	endif()
else()
    add_library(${LIBRARY_NAME} STATIC ${PROJECT_SOURCES})
endif()

if(MSVC)
    target_compile_options(${LIBRARY_NAME} PUBLIC "/Zc:__cplusplus")
endif()