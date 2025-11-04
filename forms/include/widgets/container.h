#ifndef CONTAINER_H
#define CONTAINER_H

#include "parent.h"
#include<SDL2/SDL.h>
#include<stdbool.h>

Parent new_container(Parent* root, int x, int y, int w, int h);
void set_container_properties(Parent* container, bool moveable, const char* title, bool has_title_bar, bool closeable/*,bool resizeable*/);

void draw_title_bar_(Parent* container);
void render_container(Parent* container);

void update_container(Parent* container, SDL_Event event);
// registering stuffs

#define MAX_CONTAINERS 100

extern Parent* container_widgets[MAX_CONTAINERS];
extern int containers_count;

void register_container(Parent* container);

void render_all_registered_containers(void);
void update_all_registered_containers(SDL_Event event);

void free_con_(Parent* parent);

void free_all_registered_containers(void);

#endif /* CONTAINER_H */
