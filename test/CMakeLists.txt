set(PROJECT_NAME WorkerQueueTest)
set(PROJECT_TEST_DIR ${PROJECT_DIR}/test)

set(PROJECT_TEST_SOURCES
    ${PROJECT_TEST_DIR}/printDir.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_TEST_SOURCES})

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "/Zc:__cplusplus")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_DIR})

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/concurrency)

target_link_libraries(${PROJECT_NAME} PRIVATE WorkerQueue)

if (WIN32 AND BUILD_SHARED_LIBS)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
  COMMAND_EXPAND_LISTS
)
endif()

if (WIN32)
  add_test(NAME WorkerQueuePrintTest COMMAND $<TARGET_FILE:${PROJECT_NAME}> "c:\\")
else()
  add_test(NAME WorkerQueuePrintTest COMMAND $<TARGET_FILE:${PROJECT_NAME}> "/home/")
endif()