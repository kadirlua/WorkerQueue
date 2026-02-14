set(PROJECT_NAME WorkerQueueTest)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/printDir.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_TEST_SOURCES})

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "/Zc:__cplusplus")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE WorkerQueue::WorkerQueue)

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
