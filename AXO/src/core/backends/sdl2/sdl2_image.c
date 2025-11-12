/* image_backend_sdl.c – the *only* file that knows about SDL_image */
#include "../../../../include/core/backends/sdl2/sdl2_image.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

ImageHandle *image_backend_load(SDL_Renderer *ren, const char *path)
{
    SDL_Texture *tex = IMG_LoadTexture(ren, path);
    if (!tex) {
        printf("IMG_LoadTexture(%s) failed: %s\n", path, IMG_GetError());
        return NULL;
    }

    ImageHandle *h = malloc(sizeof *h);
    if (h) h->texture = tex;
    return h;
}

void image_backend_draw(SDL_Renderer *ren, ImageHandle *h,
                        int sx, int sy, int sw, int sh)
{
	 // If width/height are 0, use the texture's actual size
    if (sw <= 0 || sh <= 0) {
        SDL_QueryTexture(h->texture, NULL, NULL, &sw, &sh);
    }

    SDL_Rect dst = { sx, sy, sw, sh };
    SDL_RenderCopy(ren, h->texture, NULL, &dst);
}

void image_backend_free(ImageHandle *h)
{
    if (h) {
        if (h->texture) SDL_DestroyTexture(h->texture);
        free(h);
    }
}
