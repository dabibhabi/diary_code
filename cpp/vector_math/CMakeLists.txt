cmake_minimum_required(VERSION 3.10)
project(Vec3Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_executable(vec3
    src/main.cpp
    src/Vec3.cpp
    src/Cartesian/VectorCartesian.cpp
    src/Spherical/VectorSpherical.cpp
    src/Cylindrical/VectorCylindrical.cpp
    src/Frenet/VectorFrenet.cpp
    src/Body/VectorBody.cpp
)
