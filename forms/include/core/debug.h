// debug.h
#pragma once
#include <stdio.h>

/*
 * DEBUG_PRINT macro:
 * - Prints messages only if DEBUG is defined at compile time.
 * - Otherwise, it does nothing (no runtime overhead).
 *
 * Usage:
 *   DEBUG_PRINT("Value of x: %d\n", x);
 *
 * Enable by compiling with:
 *   gcc -DDEBUG ...
 */

#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) \
        do { fprintf(stderr, "[DEBUG] %s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__); } while (0)
#else
    #define DEBUG_PRINT(fmt, ...) do {} while (0)
#endif
