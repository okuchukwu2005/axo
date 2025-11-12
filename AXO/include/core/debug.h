// debug.h
#pragma once
#include <stdio.h>

/*
 * DEBUG_PRINT macro
 * -------------------------------------------------
 * • Enabled only when -DDEBUG is passed to the compiler
 * • Prints:  [DEBUG:file.c:line] <your message>
 * • Uses fprintf(stderr, ...) – always visible even if stdout is redirected
 * • Zero-cost when DEBUG is not defined
 *
 * Example output:
 *   [DEBUG:button.c:20] Error: NULL parent passed to new_button()
 *
 * Compile with debug:
 *   gcc -DDEBUG your_files.c
 */

#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) \
        do { \
            fprintf(stderr, "[DEBUG:%s:%d] " fmt "\n", \
                    __FILE__, __LINE__, ##__VA_ARGS__); \
        } while (0)
#else
    #define DEBUG_PRINT(fmt, ...) do { } while (0)
#endif
