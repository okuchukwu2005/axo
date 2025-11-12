#ifndef CLIP_H
#define CLIP_H

#include "sdl2.h"  // Only here we know Base

typedef struct { int x, y, w, h; } Rect;

// Save current clip and set a new one
void clip_begin(Base *base, const Rect *rect);

// Restore previous clip (call after drawing)
void clip_end(Base *base);

#endif
