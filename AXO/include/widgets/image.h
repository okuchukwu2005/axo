#ifndef IMAGE_H
#define IMAGE_H

#include "../core/parent.h"
#include "../core/backends/sdl2/sdl2_events.h"
#include "../core/backends/sdl2/sdl2_image.h"


typedef struct{
    Parent*      parent;
    int          x, y, w, h;          /* logical coordinates */
    const char*  file_path;
    ImageHandle* handle;              /* opaque backend handle */
}Image;

Image new_image(Parent * parent, int x, int y, const char * file_path, int w, int h  );

void render_image(Image * image);

void update_image(Image *image, Event *event);
void free_image(Image *image);


// Registration of widgets for rendering
#define MAX_IMAGES 100
extern Image * image_widgets[MAX_IMAGES];

extern int images_count;

void register_image(Image* image);

void render_all_registered_images(void);

void update_all_registered_images(Event* event);

void free_all_registered_images(void);

#endif // IMAGE_H
