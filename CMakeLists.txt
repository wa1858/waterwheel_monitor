cmake_minimum_required(VERSION 3.28)

project(waterwheel_monitor
    VERSION 1.3
    DESCRIPTION "Program for monitoring performance of the waterwheel"
    LANGUAGES CXX)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(src/config.h.in config.h)

# include_directories(${CMAKE_BINARY_DIR})

add_executable(monitor
    src/monitor.cpp)

# target_link_libraries(monitor)