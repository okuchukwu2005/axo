#include "../../include/core/graphics.h"
#include "../../include/core/backends/sdl2/sdl2_draw.h"
#include <stdio.h>
#include <math.h>

void clear_screen(Base* base, Color color) {
	backend_clear_screen(base, color);
}

void present_screen(Base* base) {
	backend_present(base);
}

// ______________DRAW FUNCTIONS_____________

void draw_rect(Base* base, int x, int y, int w, int h, Color color) {
  backend_draw_rect(base, x, y, w, h, color);
}

void draw_circle(Base* base, int x, int y, int radius, Color color) {
 backend_draw_circle(base, x, y, radius, color);
}

void draw_triangle(Base* base, int x1, int y1, int x2, int y2, int x3, int y3, Color color) {
 backend_draw_triangle(base, x1, y1, x2, y2, x3,y3,color);
}

void draw_rounded_rect(Base* base, int x, int y, int w, int h, float roundness, Color color) {
   backend_draw_rounded_rect(base,x,y,w,h,roundness,color);
}

void draw_text_from_font(Base* base, Font_ttf * font, const char* text, int x, int y, Color color, TextAlign align) {
   backend_draw_text_from_font(base,font,text,x,y,color,align);
}


static char *FONT_FILE = "forms/core/FreeMono.ttf";

void draw_text(Base* base, const char* text, int font_size, int x, int y, Color color) {
   backend_draw_text(base,text,font_size,x,y,color);
}


void draw_image(Base * base, const char * file, int x, int y, int w, int h){
 backend_draw_image(base,file,x,y,w,h);
}

void draw_image_from_texture(Base *base, void * texture, int x, int y, int w, int h){
	backend_draw_image_from_texture(base,texture,x,y,w,h);
}
