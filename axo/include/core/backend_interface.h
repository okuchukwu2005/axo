#ifndef BACKEND_INTERFACE_H
#define BACKEND_INTERFACE_H

//------------------ Backend Selection Logic --------------------------

// If nothing is defined, default to SDL2
#if !defined(BACKEND_SDL2) && !defined(BACKEND_SDL3)
#define BACKEND_SDL2
#endif

// Prevent selecting multiple backends at once
#if defined(BACKEND_SDL2) && defined(BACKEND_SDL3)
#error "Multiple backends selected! Define only one backend."
#endif

//------------------ SDL2 --------------------------
#ifdef BACKEND_SDL2
#include "../backends/sdl2/sdl2_ttf.h"
#include "../backends/sdl2/sdl2_draw.h"
#include "../backends/sdl2/sdl2.h"
#include "../backends/sdl2/sdl2_events.h"
#include "../backends/sdl2/sdl2_image.h"
#endif

//------------------ SDL3 --------------------------
#ifdef BACKEND_SDL3
#include "../backends/sdl3/sdl3.h"
// include additional SDL3 headers here...
#endif

//--------------------- other includes -----------------
#include "debug.h"

#endif // BACKEND_INTERFACE_H