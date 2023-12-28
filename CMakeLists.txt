cmake_minimum_required(VERSION 3.28)

project(waterwheel_monitor
    VERSION 1.0.9
    DESCRIPTION "Program for monitoring performance of the waterwheel"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Set generator
set(CMAKE_GENERATOR "Visual Studio 17 2022")

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib)