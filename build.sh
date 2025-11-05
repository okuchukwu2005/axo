#!/bin/bash

# Root and build directories
ROOT_DIR=$(pwd)
BUILD_DIR="$ROOT_DIR/build"
SRC_DIR="$ROOT_DIR/forms/src"
EXAMPLE_DIR="$ROOT_DIR"
EXECUTABLE="AAA"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

echo "Compiling .c files from core and widgets..."

# Compile all .c files from core and widgets into .o in build
for dir in "$SRC_DIR/core" "$SRC_DIR/widgets"; do
    for file in "$dir"/*.c; do
        [ -e "$file" ] || continue
        base=$(basename "$file" .c)
        echo "Compiling $file -> $BUILD_DIR/$base.o"
        gcc -c "$file" -o "$BUILD_DIR/$base.o"
    done
done

echo "Compiling example/main.c..."

# Compile main.c in example directory
MAIN_C="$EXAMPLE_DIR/main.c"
if [ -f "$MAIN_C" ]; then
    gcc -c "$MAIN_C" -o "$BUILD_DIR/main.o"
else
    echo "Warning: $MAIN_C not found!"
fi

echo "Linking all object files into executable $EXECUTABLE..."

# Link all .o files in build to a single executable with SDL2, SDL2_image, SDL2_ttf, and math library
gcc "$BUILD_DIR"/*.o -o "$ROOT_DIR/$EXECUTABLE" \
    $(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -lm

echo "Build complete. Executable: $ROOT_DIR/$EXECUTABLE"
