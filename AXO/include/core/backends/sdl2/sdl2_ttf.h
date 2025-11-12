/* sdl2_ttf.h  –  public API, never includes SDL_ttf.h */
#ifndef SDL2_TTF_H
#define SDL2_TTF_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

/* Opaque font handle */
typedef struct Font_ttf Font_ttf;

/* ------------------------------------------------------------------ */
/*  Font lifecycle                                                    */
/* ------------------------------------------------------------------ */
Font_ttf* load_font_ttf(const char* path, int ptsize);
void      free_font_ttf(Font_ttf* f);

/* ------------------------------------------------------------------ */
/*  Render modes                                                      */
/* ------------------------------------------------------------------ */
typedef enum {
    TTF_RENDER_SOLID,      /* fast, aliased */
    TTF_RENDER_SHADED,     /* background colour */
    TTF_RENDER_BLENDED     /* slow, anti-aliased */
} TTF_RenderMode;

/* Result – owns an SDL_Texture that must be destroyed */
typedef struct {
    SDL_Texture* texture;   /* NULL on failure */
    int          w;
    int          h;
} TTF_TextResult;

/* ------------------------------------------------------------------ */
/*  Core rendering – renderer is supplied by the caller               */
/* ------------------------------------------------------------------ */
TTF_TextResult render_text_ttf(
        SDL_Renderer*    ren,
        Font_ttf*        font,
        const char*      utf8_text,
        TTF_RenderMode   mode,
        SDL_Color        fg,          /* used for SOLID/BLENDED */
        SDL_Color        bg);         /* used only for SHADED */

/* Helper to free the texture */
static inline void free_ttf_result(TTF_TextResult* r)
{
    if (r && r->texture) SDL_DestroyTexture(r->texture);
}

/* ------------------------------------------------------------------ */
/*  Measurement helpers                                               */
/* ------------------------------------------------------------------ */
int ttf_text_width (Font_ttf* f, const char* text);
int ttf_text_height(Font_ttf* f, const char* text);
int ttf_line_skip  (Font_ttf* f);

/* ------------------------------------------------------------------ */
/* Extra measurement helpers (mirrors the raw SDL_ttf API)           */
/* ------------------------------------------------------------------ */
int ttf_font_height   (Font_ttf* f);                /* TTF_FontHeight   */
int ttf_font_ascent   (Font_ttf* f);                /* TTF_FontAscent   */
int ttf_font_descent  (Font_ttf* f);                /* TTF_FontDescent  */

int ttf_size_text     (Font_ttf* f, const char* text,
                       int* w, int* h);            /* TTF_SizeText     */
int ttf_size_utf8     (Font_ttf* f, const char* text,
                       int* w, int* h);            /* TTF_SizeUTF8     */
#endif /* SDL2_TTF_H */

/* EXAMPLE USAGE
#include "sdl2_ttf.h"

void draw_hello(SDL_Renderer* ren)
{
    Font_ttf* f = load_font_ttf("assets/Roboto-Regular.ttf", 32);
    if (!f) return;   // handle error

    SDL_Color white = {255,255,255,255};
    SDL_Color black = {0,0,0,255};

    TTF_TextResult txt = render_text_ttf(ren, f,
                                         "Hello, world!",
                                         TTF_RENDER_BLENDED,
                                         white, black);
    if (txt.texture) {
        SDL_Rect dst = { 100, 100, txt.w, txt.h };
        SDL_RenderCopy(ren, txt.texture, NULL, &dst);
        free_ttf_result(&txt);
    }

    free_font_ttf(f);
}
*/
