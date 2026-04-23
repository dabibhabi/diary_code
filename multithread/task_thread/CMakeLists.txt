cmake_minimum_required(VERSION 3.15)
project(task_thread CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(Threads REQUIRED)

add_executable(task_thread
    main.cpp
    src/Task/task.cpp
    src/Worker/worker.cpp
    src/Pool/pool.cpp
    src/Runner/runner.cpp
)

target_include_directories(task_thread PRIVATE
    src
    src/Task
    src/Worker
    src/Pool
    src/Runner
)
target_link_libraries(task_thread PRIVATE Threads::Threads)

add_custom_target(distclean
    COMMAND ${CMAKE_COMMAND} -E rm -rf
        ${CMAKE_BINARY_DIR}/CMakeFiles
        ${CMAKE_BINARY_DIR}/CMakeCache.txt
        ${CMAKE_BINARY_DIR}/cmake_install.cmake
        ${CMAKE_BINARY_DIR}/Makefile
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_BINARY_DIR}/task_thread
    COMMENT "Wiping CMake-generated files from ${CMAKE_BINARY_DIR}"
)
