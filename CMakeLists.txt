cmake_minimum_required(VERSION 3.25.1 FATAL_ERROR)

project(waterwheel_monitor
    VERSION 1.1.0
    DESCRIPTION "Program for monitoring performance of the waterwheel"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wpedantic -static-libgcc -static-libstdc++")

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib)