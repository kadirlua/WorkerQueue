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
    target_compile_options(${LIBRARY_NAME} PUBLIC "/Zc:__cplusplus")
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

# Custom target to copy the CHANGELOG.md file to the build directory
add_custom_target(copy_changelog ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/CHANGELOG.md ${CMAKE_BINARY_DIR}/CHANGELOG.md
    DEPENDS ${PROJECT_SOURCE_DIR}/CHANGELOG.md
    COMMENT "Copying CHANGELOG.md to the build directory"
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
install(FILES ${PROJECT_SOURCE_DIR}/CHANGELOG.md DESTINATION .)
install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION .)

# Include other necessary files and targets
install(TARGETS ${LIBRARY_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)