#include "../../include/core/parent.h"
#include "../../include/core/backends/sdl2/sdl2.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>


// Returns DPI scale relative to standard 96 DPI
float get_display_dpi(int display_index) {
	return return_display_dpi(display_index);
}

Parent new_window(char* title, int w, int h) {
	Parent parent;
    // Compute DPI scale using the same helper
    parent.base.dpi_scale = get_display_dpi(0);
    parent.is_window = 1;
    parent.w = w;
    parent.h = h;
    parent.color = COLOR_GRAY;
    parent.is_open = true;
    parent.title_height=0;

	create_window(&parent.base,title, w,h);
	
    return parent;
}


void free_parent(Parent* parent) {
    if (!parent) return;

    if (parent->is_window) {
        destroy_window(&parent->base);
    }

}

//=============== children helper func ====================
SDL_Rect  get_parent_rect(Parent *p){
	SDL_Rect rect ={p->x, p->y,p->w,p->h};
	return rect;
}
