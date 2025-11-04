#ifndef IMAGE_H
#define IMAGE_H

#include "parent.h"
#include<SDL2/SDL.h>
typedef struct{
	Parent *parent;
	int x, y, w, h;
	const char * file_path;
	SDL_Texture *texture;
}Image;


Image new_image(Parent * parent, int x, int y, const char * file_path, int w, int h  );

void render_image(Image * image);

void update_image(Image *image, SDL_Event event);
void free_image(Image *image);


// Registration of widgets for rendering
#define MAX_IMAGES 100
extern Image * image_widgets[MAX_IMAGES];

extern int images_count;

void register_image(Image* image);

void render_all_registered_images(void);

void update_all_registered_images(SDL_Event event);

void free_all_registered_images(void);

#endif // IMAGE_H
