#ifndef SDL2_BACKEND_H
#define SDL2_BACKEND_H

#include <SDL2/SDL.h>

/**
 * Base struct that bundles SDL_Window and SDL_Renderer.
 */
typedef struct {
    SDL_Window*   sdl_window;
    SDL_Renderer* sdl_renderer;
    float         dpi_scale;   // DPI scale factor (e.g. 2.0 on Retina)
} Base;

/* ----- public API ----------------------------------------------------- */
float return_display_dpi(int display_index);

void create_window (Base *base, char *title, int w, int h);
void destroy_window(Base *base);

#endif /* SDL2_BACKEND_H */
