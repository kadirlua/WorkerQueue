set(LIBRARY_NAME WorkerQueue)

# WorkerQueue library requires C++14 or higher features
if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()

set(PROJECT_INCLUDE_DIR ${PROJECT_DIR})

set(PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/concurrency/WorkerQueue.cpp)

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
    target_compile_options(${LIBRARY_NAME} PRIVATE "/Zc:__cplusplus")
endif()

# Set the library version properties
set_target_properties(${LIBRARY_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# configure header template
configure_file(
    "${PROJECT_SOURCE_DIR}/concurrency/version.h.in"
    "${PROJECT_BINARY_DIR}/concurrency/version.h"
)

# Custom target to copy the Changelog.md file to the build directory
add_custom_target(copy_changelog ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/Changelog.md ${CMAKE_BINARY_DIR}/Changelog.md
    DEPENDS ${PROJECT_SOURCE_DIR}/Changelog.md
    COMMENT "Copying Changelog.md to the build directory"
)

include(CPack)

# Set packaging details
set(CPACK_PACKAGE_NAME "WorkerQueue")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION "Cross platform asynchronous worker queue based on modern C++")
set(CPACK_PACKAGE_MAINTAINER "kadirlua")

# Use file(GLOB ...) to match specific header files
file(GLOB HEADER_FILES "${PROJECT_SOURCE_DIR}/concurrency/*.h")

# Install the matched header files
install(FILES ${HEADER_FILES} DESTINATION include)

install(FILES ${PROJECT_BINARY_DIR}/concurrency/version.h DESTINATION include)

# Include the CHANGELOG.md and LICENSE files in the package
install(FILES ${PROJECT_SOURCE_DIR}/Changelog.md DESTINATION .)
install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION .)

# Include other necessary files and targets
install(TARGETS ${LIBRARY_NAME}
    EXPORT WorkerQueueTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

# Export the targets for other projects to find
install(EXPORT WorkerQueueTargets
        FILE WorkerQueueTargets.cmake
        NAMESPACE WorkerQueue::
        DESTINATION lib/cmake/WorkerQueue)

# Create a config file for find_package
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/WorkerQueueConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/WorkerQueueConfig.cmake"
  INSTALL_DESTINATION lib/cmake/WorkerQueue
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/WorkerQueueConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/WorkerQueueConfigVersion.cmake"
  DESTINATION lib/cmake/WorkerQueue
)