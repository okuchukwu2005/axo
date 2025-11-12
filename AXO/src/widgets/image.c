/* image.c --------------------------------------------------------------- */
#include "../../include/widgets/image.h"
#include "../../include/core/graphics.h"
#include "../../include/core/parent.h"   // for Rect
#include "../../include/core/backends/sdl2/clip.h"     // for clip_begin/end
#include <SDL2/SDL_image.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Image definition (opaque) */
/* --------------------------------------------------------------------- */
struct Image {
    Parent*       parent;
    int           x, y, w, h;     // logical coordinates
    const char*   file_path;
    SDL_Texture*  texture;
};

/* --------------------------------------------------------------------- */
Image new_image(Parent *parent, int x, int y, const char *file_path, int w, int h)
{
    if (!parent || !parent->base.sdl_renderer) {
        printf("Invalid parent or renderer for image widget\n");
        Image img = {0};
        return img;
    }

    Image img = {0};
    img.parent    = parent;
    img.x         = x;
    img.y         = y;
    img.w         = w;
    img.h         = h;
    img.file_path = file_path;

    img.texture = IMG_LoadTexture(parent->base.sdl_renderer, file_path);
    if (!img.texture) {
        printf("Failed to load image %s: %s\n", file_path, IMG_GetError());
    }

    return img;
}

/* --------------------------------------------------------------------- */
void render_image(Image *image)
{
    if (!image || !image->parent || !image->parent->base.sdl_renderer || !image->parent->is_open) {
        printf("Invalid image widget, renderer, or parent is not open\n");
        return;
    }
    if (!image->texture) return;

    Base* base = &image->parent->base;
    float dpi  = base->dpi_scale;

    /* ---------- DPI-SCALED POSITION & SIZE ---------- */
    int abs_x = image->x + image->parent->x;
    int abs_y = image->y + image->parent->y + image->parent->title_height;

    int sx = (int)roundf(abs_x * dpi);
    int sy = (int)roundf(abs_y * dpi);
    int sw = (int)roundf(image->w * dpi);
    int sh = (int)roundf(image->h * dpi);

    /* ---------- PARENT CLIPPING (for containers) ---------- */
    if (!image->parent->is_window) {
        Rect pr = get_parent_rect(image->parent);           // our own Rect
        pr.x = (int)roundf(pr.x * dpi);
        pr.y = (int)roundf(pr.y * dpi);
        pr.w = (int)roundf(pr.w * dpi);
        pr.h = (int)roundf(pr.h * dpi);
        clip_begin(base, &pr);
    } else {
        clip_begin(base, NULL);  // no clipping for root window
    }

    /* ---------- DRAW IMAGE ---------- */
    draw_image_from_texture(base, image->texture, sx, sy, sw, sh);

    /* ---------- RESTORE CLIP ---------- */
    clip_end(base);
}

/* --------------------------------------------------------------------- */
void update_image(Image *image, Event *event)
{
    // Placeholder for future interactive features (resize, drag, etc.)
    (void)image;
    (void)event;
}

/* --------------------------------------------------------------------- */
void free_image(Image *image)
{
    if (image && image->texture) {
        SDL_DestroyTexture(image->texture);
        image->texture = NULL;
    }
}

/* --------------------------------------------------------------------- */
/* Registration of widgets for rendering */
Image* image_widgets[MAX_IMAGES];
int    images_count = 0;

void register_image(Image* image)
{
    if (images_count < MAX_IMAGES) {
        image_widgets[images_count++] = image;
    }
}

void render_all_registered_images(void)
{
    for (int i = 0; i < images_count; ++i) {
        if (image_widgets[i]) {
            render_image(image_widgets[i]);
        }
    }
}

void update_all_registered_images(Event* event)
{
    for (int i = 0; i < images_count; ++i) {
        if (image_widgets[i]) {
            update_image(image_widgets[i], event);
        }
    }
}

void free_all_registered_images(void)
{
    for (int i = 0; i < images_count; ++i) {
        if (image_widgets[i]) {
            free_image(image_widgets[i]);
            image_widgets[i] = NULL;
        }
    }
    images_count = 0;
}
