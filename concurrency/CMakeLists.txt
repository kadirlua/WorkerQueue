set(LIBRARY_NAME WorkerQueue)

# WorkerQueue library requires C++11 or higher features
if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enforce C++11 standard

set(PROJECT_ROOT_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/WorkerQueue.cpp)

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

add_library(${LIBRARY_NAME} ${PROJECT_SOURCES})
add_library(WorkerQueue::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

if (BUILD_SHARED_LIBS AND WIN32)
    add_compile_definitions(WORKERQUEUE_COMPILE_DLL=1)
	target_compile_definitions(${LIBRARY_NAME} PRIVATE "WORKERQUEUE_DLL_EXPORT")
endif()

if(MSVC)
    target_compile_options(${LIBRARY_NAME} PRIVATE "/Zc:__cplusplus")
endif()

target_include_directories(${LIBRARY_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_ROOT_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>
        $<INSTALL_INTERFACE:include>
)

# Custom target to copy the Changelog.md file to the build directory
add_custom_target(copy_changelog ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_ROOT_DIR}/Changelog.md ${CMAKE_BINARY_DIR}/Changelog.md
    DEPENDS ${PROJECT_ROOT_DIR}/Changelog.md
    COMMENT "Copying Changelog.md to the build directory"
)

include(CPack)

# Set packaging details
set(CPACK_PACKAGE_NAME "WorkerQueue")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION "Cross platform asynchronous worker queue based on modern C++")
set(CPACK_PACKAGE_MAINTAINER "kadirlua")

# =========================================================
# ASAN
# =========================================================

if(ENABLE_ASAN)
    message(STATUS "AddressSanitizer enabled")

    if(MSVC)
        # Apply to the library AND anyone linking to it
        target_compile_options(${LIBRARY_NAME} PUBLIC /fsanitize=address /Zi)

        # /INFERASANLIBS tells the linker to find the correct ASAN runtime (static or dll)
        # /INCREMENTAL:NO is mandatory for ASAN
        target_link_options(${LIBRARY_NAME} PUBLIC /INFERASANLIBS /INCREMENTAL:NO /DEBUG)
        
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${LIBRARY_NAME} PUBLIC -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(${LIBRARY_NAME} PUBLIC -fsanitize=address)
    endif()
endif()

# =========================================================
# Version header generation
# =========================================================

# configure header template
configure_file(
    "${PROJECT_ROOT_DIR}/include/workerqueue/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/workerqueue/version.h"
    @ONLY
)

# =========================================================
# Library version
# =========================================================

# Set the library version properties
set_target_properties(${LIBRARY_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# =========================================================
# Install headers
# =========================================================

# Install the matched header files
install(DIRECTORY "${PROJECT_ROOT_DIR}/include/workerqueue"
        DESTINATION include
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/generated/workerqueue/version.h"
    DESTINATION include/workerqueue
)

# Include the CHANGELOG.md and LICENSE files in the package
install(FILES ${PROJECT_ROOT_DIR}/Changelog.md DESTINATION .)
install(FILES ${PROJECT_ROOT_DIR}/LICENSE DESTINATION .)

# =========================================================
# Install library
# =========================================================

# Include other necessary files and targets
install(TARGETS ${LIBRARY_NAME}
    EXPORT WorkerQueueTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

# =========================================================
# Export package config
# =========================================================

# Export the targets for other projects to find
install(EXPORT WorkerQueueTargets
        FILE WorkerQueueTargets.cmake
        NAMESPACE WorkerQueue::
        DESTINATION lib/cmake/workerqueue)

# Create a config file for find_package
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/WorkerQueueConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/../Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/WorkerQueueConfig.cmake"
  INSTALL_DESTINATION lib/cmake/workerqueue
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/WorkerQueueConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/WorkerQueueConfigVersion.cmake"
  DESTINATION lib/cmake/workerqueue
)
