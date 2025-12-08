cmake_minimum_required(VERSION 3.10)
project(axo_example C)

# ------------------------------------------------------
# Use Clang
# ------------------------------------------------------
set(CMAKE_C_COMPILER clang)

# Output binaries in cmake_build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# ------------------------------------------------------
# Project structure
# ------------------------------------------------------
set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(AXO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

# ------------------------------------------------------
# Collect all source files
# ------------------------------------------------------
file(GLOB_RECURSE AXO_SOURCES
    ${AXO_ROOT}/axo/*.c
    ${AXO_ROOT}/axo/src/**/*.c
)

file(GLOB_RECURSE EXAMPLE_SOURCES
    ${EXAMPLES_DIR}/*.c
)

#  Fix: remove CMake internal compiler test files (they contain a main!)
list(FILTER EXAMPLE_SOURCES EXCLUDE REGEX ".*CMakeFiles/.*")

set(ALL_SOURCES
    ${EXAMPLE_SOURCES}
    ${AXO_SOURCES}
)

# ------------------------------------------------------
# Include paths
# ------------------------------------------------------
include_directories(
    ${AXO_ROOT}/axo/include
    ${AXO_ROOT}/axo
)

# ------------------------------------------------------
# Create executable
# ------------------------------------------------------
add_executable(example ${ALL_SOURCES})

# ------------------------------------------------------
# SDL Linking (robust fallback system)
# ------------------------------------------------------
find_package(SDL2 QUIET)
find_package(SDL2_ttf QUIET)
find_package(SDL2_image QUIET)

if (SDL2_FOUND AND SDL2_TTF_FOUND AND SDL2_IMAGE_FOUND)
    message(STATUS "Using CMake SDL2 packages")

    target_include_directories(example PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
    )

    target_link_libraries(example
        ${SDL2_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        m
    )

else()
    message(WARNING "CMake SDL2 packages NOT found -> using manual linker flags!")

    target_link_libraries(example
        -lSDL2
        -lSDL2_ttf
        -lSDL2_image
        -lm
    )
endif()