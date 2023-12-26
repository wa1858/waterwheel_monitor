cmake_minimum_required(VERSION 3.28)
project(waterwheel_monitor
    VERSION 1.0.3
    DESCRIPTION "Program for monitoring performance of the waterwheel"
    LANGUAGES CXX)
add_executable(monitor
    src/monitor.cpp)