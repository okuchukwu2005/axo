#include "../../../../include/core/backends/sdl2/clip.h"

#include <SDL2/SDL.h>

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
