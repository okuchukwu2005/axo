/* image.c --------------------------------------------------------------- */
#include "../../include/widgets/image.h"
#include "../../include/core/parent.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
axImage axCreateImage(axParent *parent, int x, int y, const char *file_path, int w, int h)
{
    if (!parent || !parent->base.sdl_renderer) {
        printf("Invalid parent or renderer for image widget\n");
        axImage img = {0};
        return img;
    }

    axImage img = {0};
    img.parent    = parent;
    img.x         = x;
    img.y         = y;
    img.w         = w;
    img.h         = h;
    img.file_path = file_path;

    img.handle = image_backend_load(parent->base.sdl_renderer, file_path);
    if (!img.handle) {
        printf("Failed to load image %s\n", file_path);
    }
    return img;
}

/* --------------------------------------------------------------------- */
void axRenderImage(axImage *image)
{
    if (!image || !image->parent || !image->parent->base.sdl_renderer ||
        !image->parent->is_open || !image->handle) {
        return;
    }

    Base *base = &image->parent->base;
    float dpi  = base->dpi_scale;

    /* ----- DPI-scaled destination rectangle ----- */
    int abs_x = image->x + image->parent->x;
    int abs_y = image->y + image->parent->y + image->parent->title_height;
    int sx = (int)roundf(abs_x * dpi);
    int sy = (int)roundf(abs_y * dpi);
    int sw = (int)roundf(image->w * dpi);
    int sh = (int)roundf(image->h * dpi);

    /* ----- clipping (containers) ----- */
    if (!image->parent->is_window) {
        Rect pr = get_parent_rect(image->parent);
        pr.x = (int)roundf(pr.x * dpi);
        pr.y = (int)roundf(pr.y * dpi);
        pr.w = (int)roundf(pr.w * dpi);
        pr.h = (int)roundf(pr.h * dpi);
        clip_begin(base, &pr);
    } else {
        clip_begin(base, NULL);
    }

    /* ----- delegate drawing to backend ----- */
    image_backend_draw(base->sdl_renderer, image->handle, sx, sy, sw, sh);

    clip_end(base);
}

/* --------------------------------------------------------------------- */
void axUpdateImage(axImage *image, axEvent *event)
{
    (void)image; (void)event;   /* placeholder */
}

/* --------------------------------------------------------------------- */
void axFreeImage(axImage *image)
{
    if (image && image->handle) {
        image_backend_free(image->handle);
        image->handle = NULL;
    }
}

/* --------------------------------------------------------------------- */
/* Global registration (unchanged) */
axImage* image_widgets[MAX_IMAGES];
int    images_count = 0;

void axRegisterImage(axImage* img)
{
    if (images_count < MAX_IMAGES) image_widgets[images_count++] = img;
}
void axRenderAllRegisteredImages(void)
{
    for (int i = 0; i < images_count; ++i)
        if (image_widgets[i]) axRenderImage(image_widgets[i]);
}
void axUpdateAllRegisteredImages(axEvent* ev)
{
    for (int i = 0; i < images_count; ++i)
        if (image_widgets[i]) axUpdateImage(image_widgets[i], ev);
}
void axFreeAllRegisteredImages(void)
{
    for (int i = 0; i < images_count; ++i) {
        if (image_widgets[i]) {
            axFreeImage(image_widgets[i]);
            image_widgets[i] = NULL;
        }
    }
    images_count = 0;
}
