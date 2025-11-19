#!/bin/bash
set -e  # exit if any command fails

# Root directory of examples
ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Build directory
BUILD_DIR="$ROOT_DIR/build"
mkdir -p "$BUILD_DIR"

# Include directories
INCLUDES="-I$ROOT_DIR/../axo/include -I$ROOT_DIR/../axo"

# Find all source files in axo/ and examples/
SRC_FILES=$(find "$ROOT_DIR/../axo" "$ROOT_DIR" -name '*.c')

# Compile each .c to .o in the build dir
OBJ_FILES=""
for SRC in $SRC_FILES; do
    # preserve relative path for object files
    REL_PATH="${SRC#$ROOT_DIR/../}"  # relative to axo/ or examples/
    OBJ="$BUILD_DIR/${REL_PATH//\//_}.o" # replace / with _ to avoid subdirs
    mkdir -p "$(dirname "$OBJ")"
    echo "Compiling $SRC -> $OBJ"
    gcc -c "$SRC" $INCLUDES -o "$OBJ"
    OBJ_FILES="$OBJ_FILES $OBJ"
done

# Link all object files into a single executable
echo "Linking into $BUILD_DIR/example"
gcc $OBJ_FILES -o "example" -lSDL2 -lSDL2_ttf -lSDL2_image -lm

echo "Build complete. Executable: $example"

