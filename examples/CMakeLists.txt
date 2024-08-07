set(PROJECT_NAME WorkerQueueExample)
set(PROJECT_EXAMPLES_DIR ${PROJECT_SOURCE_DIR}/examples)

set(PROJECT_EXAMPLES_SOURCES
    ${PROJECT_EXAMPLES_DIR}/main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_EXAMPLES_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/concurrency)

target_link_libraries(${PROJECT_NAME} PRIVATE WorkerQueue)

if (WIN32 AND BUILD_SHARED_LIBS)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
  COMMAND_EXPAND_LISTS
)
endif()

add_test(WorkerQueueExampleTest ${PROJECT_NAME})