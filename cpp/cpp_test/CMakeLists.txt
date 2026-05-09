cmake_minimum_required(VERSION 3.20)
project(cuny_sms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(CUNY_BUILD_TESTS "Build unit tests" ON)
option(CUNY_BUILD_PYTHON "Build the nanobind Python module" OFF)
option(CUNY_ENABLE_SANITIZERS "Enable ASan + UBSan" OFF)

# ---------------------------------------------------------------------------
# Core library (Phase 0: empty, populated phase by phase per docs/25-todo-roadmap.md)
# ---------------------------------------------------------------------------
add_library(cuny_sms_core INTERFACE)
target_include_directories(cuny_sms_core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(cuny_sms_core INTERFACE cxx_std_20)

# Once Phase 1 begins, switch INTERFACE -> STATIC and add source files like:
#
#   add_library(cuny_sms_core STATIC
#       src/identity/email_generator.cpp
#       src/identity/cuny_member.cpp
#       ...)
#   target_include_directories(cuny_sms_core PUBLIC
#       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
#   target_compile_features(cuny_sms_core PUBLIC cxx_std_20)

if(CUNY_ENABLE_SANITIZERS)
    target_compile_options(cuny_sms_core INTERFACE
        -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(cuny_sms_core INTERFACE
        -fsanitize=address,undefined)
endif()

# Warnings — applied to every consumer that asks
add_library(cuny_sms_warnings INTERFACE)
target_compile_options(cuny_sms_warnings INTERFACE
    -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion
    $<$<CONFIG:Debug>:-O0 -g>
    $<$<CONFIG:Release>:-O2>
    $<$<CONFIG:RelWithDebInfo>:-O2 -g>
)

# ---------------------------------------------------------------------------
if(CUNY_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(CUNY_BUILD_PYTHON)
    add_subdirectory(bindings)
endif()

# Phase 1 demo — runnable smoke test from C++. Will be superseded by the
# Python demo once the bindings ship (Phase 7+); kept for now as a sanity check.
add_executable(cuny_sms_demo main.cpp)
target_link_libraries(cuny_sms_demo PRIVATE cuny_sms_core cuny_sms_warnings)
