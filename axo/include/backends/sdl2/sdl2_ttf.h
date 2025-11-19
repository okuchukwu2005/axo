/* sdl2_ttf.h  –  public API, never includes SDL_ttf.h */
#ifndef SDL2_TTF_H
#define SDL2_TTF_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include "sdl2.h"
#include "../../core/color.h"
/**
 * @brief Text alignment options for draw_text_from_font_
 */
typedef enum {
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
} TextAlign;

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

// ------------------- Drawing -----------------

/**
 * @brief Draws text at the specified position using a provided TTF font
 * @param base Pointer to the Base struct containing the renderer
 * @param font Pointer to the loaded TTF_Font
 * @param text The text string to render
 * @param x X-coordinate for the text (depends on align)
 * @param y Y-coordinate for the top of the text
 * @param color The color of the text
 * @param align Text alignment (ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT)
 */
void draw_text_from_font(Base* base, void* font, const char* text, int x, int y, Color color, TextAlign align);
/**
 * @brief Draws text at the specified position with the given font size and color
 * @param base Pointer to the Base struct containing the renderer
 * @param text The text string to render
 * @param font_size The size of the font in points
 * @param x X-coordinate for the top-left corner of the text
 * @param y Y-coordinate for the top-left corner of the text
 * @param color The color of the text
 */
void draw_text(Base* base, const char* text, int font_size, int x, int y, Color color);

/**
 * @brief Draw an image from file (loads and destroys texture each call).
 * @param base Base struct with valid SDL_Renderer.
 * @param file Path to image file.
 * @param x    X coordinate.
 * @param y    Y coordinate.
 * @param w    Width (0 to use texture width).
 * @param h    Height (0 to use texture height).
 */

//----------------------------------------------------------
// to open compiled c binary font
typedef struct {
    SDL_RWops* handle;
} RWops;
RWops* open_font_rw(const void *mem, int size);
Font_ttf* load_font_rw(RWops * rw, int font_size);
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
