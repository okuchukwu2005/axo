/**
* @author Okuchukwu2005
* @date August 7, 2025
*/

#ifndef PARENT_H
#define PARENT_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "color.h"


/**
 * @brief Base struct that bundles SDL_Window and SDL_Renderer.
 */
typedef struct {
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;
    float dpi_scale;  // DPI scale factor (e.g., 2.0 on Retina; computed after renderer creation)
} Base;

/**
 * @brief This struct represents both root windows and containers.
 *        For root windows, is_window is 1 and SDL fields in Base are initialized.
 *        For containers, is_window is 0 and Base fields are NULL.
 */
typedef struct {
    Base base;                 // Holds SDL_Window + SDL_Renderer
    int is_window;             // 1 for root window, 0 for container
    int x, y, w, h;            // Position and size (x,y=0 for root window)
    Color color;               // Background color (transparent for root window)
    int moveable;              // Can be moved (0 for root window)
    const char* title_bar;     // Title bar text (NULL for root window)
    bool has_title_bar;        // Has title bar (false for root window)
    bool is_dragging;          // Is being dragged
    int drag_offset_x;         // Drag offset X
    int drag_offset_y;         // Drag offset Y
    bool closeable;            // Can be closed (false for root window)
    bool resizeable;           // Can be resized (false for root window)
    bool is_resizing;          // Is being resized
    int resize_zone;           // Size of edge zone for resizing
    bool is_open;              // Is open (true for root window)
    int title_height;          // Height of title bar (0 for root window)
} Parent;

/**
 * @brief Creates a new root window as a Parent struct.
 * @param title The title of the window.
 * @param w The width of the window (logical size).
 * @param h The height of the window (logical size).
 * @return Pointer to the created Parent, or NULL on failure.
 */
 
// Returns DPI scale relative to standard 96 DPI
float get_display_dpi(int display_index);

 Parent new_window(char* title, int w, int h);

/**
 * @brief Destroys the Parent struct, cleaning up SDL resources if it's a root window.
 * @param parent Pointer to the Parent to destroy.
 */
void destroy_parent(Parent* parent);

//=============== children helper func ====================
SDL_Rect  get_parent_rect(Parent *p);
#endif /* PARENT_H */
