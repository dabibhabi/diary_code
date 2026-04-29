cmake_minimum_required(VERSION 3.15)
project(statistics CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

# Core library — pure stats code, no I/O, no demo logic.
add_library(stats
    src/Dataset.cpp
    src/Mean.cpp
    src/Median.cpp
    src/Variance.cpp
    src/StandardDeviation.cpp
    src/MinMax.cpp
    src/OrderStatistic.cpp
    src/MathUtils.cpp
    src/MLE.cpp
    src/Inference.cpp
    src/Pipeline.cpp
    src/StatisticFactory.cpp
)
target_include_directories(stats PUBLIC include)

# Demo runner — main.cpp is just the entry point; sections live in Demo.cpp.
add_executable(stats_demo
    apps/main.cpp
    apps/Demo.cpp
)
target_include_directories(stats_demo PRIVATE apps)
target_link_libraries(stats_demo PRIVATE stats)
