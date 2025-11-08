#include "../../include/core/parent.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>


// Returns DPI scale relative to standard 96 DPI
float get_display_dpi(int display_index) {
    // Make sure SDL video subsystem is initialized
    if (SDL_WasInit(SDL_INIT_VIDEO) == 0) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            printf("SDL_Init failed: %s\n", SDL_GetError());
            return 1.0f; // fallback
        }
    }

    float ddpi, hdpi, vdpi;
    if (SDL_GetDisplayDPI(display_index, &ddpi, &hdpi, &vdpi) != 0) {
        printf("Failed to get display DPI: %s\n", SDL_GetError());
        return 1.0f; // fallback
    }

    // Normalize to standard 96 DPI
    float dpi_scale = ddpi / 96.0f;
    if (dpi_scale < 1.0f) dpi_scale = 1.0f; // minimum 1.0

    return dpi_scale;
}


 Parent new_window(char* title, int w, int h) {
    // Enable DPI scaling hint for Windows
  SDL_SetHint(SDL_HINT_WINDOWS_DPI_SCALING, "1");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
    }

    if (TTF_Init() == -1) {
        printf("TTF initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
    }
    
    // init sdl image
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP; // we can add more format support in the future
	int initted = IMG_Init(imgFlags);
	if ((initted & imgFlags) != imgFlags) {
    printf("Warning: Not all image formats were initialized! IMG_Error: %s\n", IMG_GetError());
	}
//

    SDL_Window* sdl_win = SDL_CreateWindow(title,
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           w, h,
                                           SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);  // Enable high DPI
    if (!sdl_win) {
        printf("Window creation failed: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
    }

    SDL_Renderer* sdl_ren = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED);
    if (!sdl_ren) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(sdl_win);
        TTF_Quit();
        SDL_Quit();
    }


    Parent parent;

    parent.base.sdl_window = sdl_win;
    parent.base.sdl_renderer = sdl_ren;
    // Compute DPI scale using the same helper
    parent.base.dpi_scale = get_display_dpi(0);
    parent.is_window = 1;
    parent.w = w;
    parent.h = h;
    parent.color = COLOR_GRAY;
    parent.is_open = true;
    parent.title_height=0;

    return parent;
}


void destroy_parent(Parent* parent) {
    if (!parent) return;

    if (parent->is_window) {
        if (parent->base.sdl_renderer) {
            SDL_DestroyRenderer(parent->base.sdl_renderer);
        }
        if (parent->base.sdl_window) {
            SDL_DestroyWindow(parent->base.sdl_window);
        }
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
    }

}

//=============== children helper func ====================
SDL_Rect  get_parent_rect(Parent *p){
	SDL_Rect rect ={p->x, p->y,p->w,p->h};
	return rect;
}
