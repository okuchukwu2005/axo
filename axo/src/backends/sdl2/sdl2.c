#include "../../../include/backends/sdl2/sdl2.h"
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
// Returns DPI scale relative to standard 96 DPI
float return_display_dpi(int display_index) {
    // Make sure SDL video subsystem is initialized
    if (SDL_WasInit(SDL_INIT_VIDEO) == 0) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            printf("SDL_Init failed: %s\n", SDL_GetError());
            return 1.0f; // fallback
        }
    }

    float ddpi, hdpi, vdpi;
    if (SDL_GetDisplayDPI(display_index, &ddpi, &hdpi, &vdpi) != 0) {
        printf("Failed to get display DPI: %s\n", SDL_GetError());
        return 1.0f; // fallback
    }

    // Normalize to standard 96 DPI
    float dpi_scale = ddpi / 96.0f;
    if (dpi_scale < 1.0f) dpi_scale = 1.0f; // minimum 1.0

    return dpi_scale;
}

void create_window(Base *base, char* title, int w, int h) {
    // Enable DPI scaling hint for Windows (set before any SDL_Init calls if possible)
    SDL_SetHint(SDL_HINT_WINDOWS_DPI_SCALING, "1");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return;  // Early exit on failure
    }

    if (TTF_Init() == -1) {
        printf("TTF initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return;
    }

    // Init SDL_image
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP;
    int initted = IMG_Init(imgFlags);
    if ((initted & imgFlags) != imgFlags) {
        printf("Warning: Not all image formats were initialized! IMG_Error: %s\n", IMG_GetError());
    }

    base->sdl_window = SDL_CreateWindow(title,
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         w, h,
                                         SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);  // Enable high DPI
    if (!base->sdl_window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return;
    }

    base->sdl_renderer = SDL_CreateRenderer(base->sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (!base->sdl_renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(base->sdl_window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return;
    }

    // Optionally recompute DPI scale here if needed (it's already set in new_window, but you could override)
    // base->dpi_scale = return_display_dpi(0);
}

void destroy_window(Base *base) {
    if (base->sdl_renderer) {
        SDL_DestroyRenderer(base->sdl_renderer);
        base->sdl_renderer = NULL;  // Null out to avoid double-free
    }
    if (base->sdl_window) {
        SDL_DestroyWindow(base->sdl_window);
        base->sdl_window = NULL;
    }
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

static struct {
    SDL_bool enabled;
    SDL_Rect rect;
} saved_clip = {0};

void clip_begin(Base *base, const Rect *rect)
{
    if (!base || !base->sdl_renderer) return;

    SDL_Renderer *ren = base->sdl_renderer;

    // Save current
    saved_clip.enabled = SDL_RenderIsClipEnabled(ren);
    if (saved_clip.enabled) {
        SDL_RenderGetClipRect(ren, &saved_clip.rect);
    }

    // Set new
    if (rect) {
        SDL_Rect r = { rect->x, rect->y, rect->w, rect->h };
        SDL_RenderSetClipRect(ren, &r);
    } else {
        SDL_RenderSetClipRect(ren, NULL);
    }
}

void clip_end(Base *base)
{
    if (!base || !base->sdl_renderer) return;

    SDL_Renderer *ren = base->sdl_renderer;

    if (saved_clip.enabled) {
        SDL_RenderSetClipRect(ren, &saved_clip.rect);
    } else {
        SDL_RenderSetClipRect(ren, NULL);
    }
}
int rect_intersect(const Rect *a, const Rect *b, Rect *result)
{
    if (!a || !b || !result) return 0;

    SDL_Rect sa = { a->x, a->y, a->w, a->h };
    SDL_Rect sb = { b->x, b->y, b->w, b->h };
    SDL_Rect sr;

    if (SDL_IntersectRect(&sa, &sb, &sr)) {
        result->x = sr.x;
        result->y = sr.y;
        result->w = sr.w;
        result->h = sr.h;
        return 1;
    }
    return 0;
}
