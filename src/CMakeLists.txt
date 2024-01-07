# TODO - Resolve CMake build error
string(TIMESTAMP BUILD_DATE "%Y-%m-%d %H:%M:%S")

# Generate version header file
configure_file(
    "${PROJECT_SOURCE_DIR}/lib/utils/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

set(target waterwheel_monitor)
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB code "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(${target} ${headers} ${code})
target_include_directories(${target}
    PRIVATE
    "${CMAKE_SOURCE_DIR}/lib"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

# TODO - Good practice? Can this cyclic dependency be removed?
# Cyclic Dependencies of Static Libraries
# See: https://cmake.org/cmake/help/latest/command/target_link_libraries.html#id10
target_link_libraries(${target}
    library_core
    library_hardware
)

target_link_libraries(${target}
    library_hardware
    library_core
)

target_link_libraries(${target}
    library_network
    library_utils
)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Append version number to the output file name
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}_${PROJECT_VERSION}.exe")