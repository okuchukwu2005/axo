cmake_minimum_required(VERSION 3.10)

# Project name
project(example C)

# Output directory for object files + executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_OBJECT_PATH_MAX 2000)

# Collect all source files from the axo engine
file(GLOB_RECURSE AXO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../axo/src/*.c
)

# Collect source files in this examples/ directory
file(GLOB_RECURSE EXAMPLE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

# Merge both sets of sources
set(ALL_SOURCES
    ${AXO_SOURCES}
    ${EXAMPLE_SOURCES}
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../axo/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../axo
)

# Create the executable
add_executable(example ${ALL_SOURCES})
