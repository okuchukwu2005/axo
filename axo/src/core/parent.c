#include "../../include/core/parent.h"
#include <stdio.h>


// Returns DPI scale relative to standard 96 DPI
float get_display_dpi(int display_index) {
	return return_display_dpi(display_index);
}

axParent axCreateWindow(char* title, int w, int h) {
	axParent parent;
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


void free_parent(axParent* parent) {
    if (!parent) return;

    if (parent->is_window) {
        destroy_window(&parent->base);
    }

}

Rect get_parent_rect(const axParent *p)
{
    return (Rect){ .x = p->x, .y = p->y, .w = p->w, .h = p->h };
}
