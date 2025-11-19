#ifndef SDL2_BACKEND_H
#define SDL2_BACKEND_H

#include <SDL2/SDL.h>

#define WIN_FULLSCREEN         SDL_WINDOW_FULLSCREEN
#define WIN_FULLSCREEN_DESKTOP SDL_WINDOW_FULLSCREEN_DESKTOP
#define WIN_OPENGL             SDL_WINDOW_OPENGL
#define WIN_VULKAN             SDL_WINDOW_VULKAN
#define WIN_SHOWN              SDL_WINDOW_SHOWN
#define WIN_HIDDEN             SDL_WINDOW_HIDDEN
#define WIN_BORDERLESS         SDL_WINDOW_BORDERLESS
#define WIN_RESIZABLE          SDL_WINDOW_RESIZABLE
#define WIN_MINIMIZED          SDL_WINDOW_MINIMIZED
#define WIN_MAXIMIZED          SDL_WINDOW_MAXIMIZED
#define WIN_INPUT_GRABBED      SDL_WINDOW_INPUT_GRABBED
#define WIN_INPUT_FOCUS        SDL_WINDOW_INPUT_FOCUS
#define WIN_MOUSE_FOCUS        SDL_WINDOW_MOUSE_FOCUS
#define WIN_ALWAYS_ON_TOP      SDL_WINDOW_ALWAYS_ON_TOP
#define WIN_SKIP_TASKBAR       SDL_WINDOW_SKIP_TASKBAR
#define WIN_UTILITY            SDL_WINDOW_UTILITY
#define WIN_TOOLTIP            SDL_WINDOW_TOOLTIP
#define WIN_POPUP_MENU         SDL_WINDOW_POPUP_MENU
#define WIN_ALLOW_HIGHDPI      SDL_WINDOW_ALLOW_HIGHDPI


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

void create_window (Base *base, char *title, int w, int h, Uint32 flags);
void destroy_window(Base *base);

typedef struct { int x, y, w, h; } Rect;

// Save current clip and set a new one
void clip_begin(Base *base, const Rect *rect);

// Restore previous clip (call after drawing)
void clip_end(Base *base);
int rect_intersect(const Rect *a, const Rect *b, Rect *result);

#endif /* SDL2_BACKEND_H */
