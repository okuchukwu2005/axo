#ifndef SDL2_IMAGE_H
#define SDL2_IMAGE_H

#include <SDL2/SDL.h>

/* Opaque handle that the backend uses */
typedef struct {
    SDL_Texture *texture;   /* owned by the backend */
} ImageHandle;

/* Backend entry points */
ImageHandle *image_backend_load(SDL_Renderer *ren, const char *path);
void image_backend_draw(SDL_Renderer *ren, ImageHandle *h,
                                int sx, int sy, int sw, int sh);
void image_backend_free(ImageHandle *h);

#endif /* SDL2_IMAGE_H */
