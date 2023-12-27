cmake_minimum_required(VERSION 3.28)

project(waterwheel_monitor
    VERSION 1.0.6
    DESCRIPTION "Program for monitoring performance of the waterwheel"
    LANGUAGES CXX)

string(TIMESTAMP BUILD_DATE "%Y-%m-%d %H:%M:%S")

# Add include directory
include_directories(include)

# Set generator
set(CMAKE_GENERATOR "Visual Studio 17 2022")

# Add all source files in the 'src' directory
file(GLOB SOURCES "src/*.cpp" "include/*.hpp")

# Generate version header file
configure_file(
    "${PROJECT_SOURCE_DIR}/include/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

# Add the executable and include the binary directory for the version header
add_executable(monitor ${SOURCES})
target_include_directories(monitor PRIVATE "${PROJECT_BINARY_DIR}")