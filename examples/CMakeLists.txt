set(PROJECT_NAME WorkerQueueExample)
set(PROJECT_EXAMPLES_DIR ${PROJECT_DIR}/examples)

set(PROJECT_EXAMPLES_SOURCES
    ${PROJECT_EXAMPLES_DIR}/main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_EXAMPLES_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_DIR})

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/concurrency)

target_link_libraries(${PROJECT_NAME} PUBLIC WorkerQueue)

add_test(WorkerQueueTest ${PROJECT_NAME})