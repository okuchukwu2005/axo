/* sdl2_ttf.c – only this file includes the real SDL_ttf.h */
#include "../../../include/backends/sdl2/sdl2_ttf.h"
#include "../../../include/core/theme.h"
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <string.h>

struct Font_ttf {
    TTF_Font* font;
};

/* ------------------------------------------------------------------ */
Font_ttf* load_font_ttf(const char* path, int ptsize)
{
    TTF_Font* raw = TTF_OpenFont(path, ptsize);
    if (!raw) return NULL;
    Font_ttf* f = malloc(sizeof *f);
    if (!f) { TTF_CloseFont(raw); return NULL; }
    f->font = raw;
    return f;
}

void free_font_ttf(Font_ttf* f)
{
    if (!f) return;
    if (f->font) TTF_CloseFont(f->font);
    free(f);
}

/* ------------------------------------------------------------------ */
static SDL_Surface* render_surface(TTF_Font* font,
                                   const char* text,
                                   TTF_RenderMode mode,
                                   SDL_Color fg,
                                   SDL_Color bg)
{
    switch (mode) {
        case TTF_RENDER_SOLID:   return TTF_RenderUTF8_Solid  (font, text, fg);
        case TTF_RENDER_SHADED:  return TTF_RenderUTF8_Shaded (font, text, fg, bg);
        case TTF_RENDER_BLENDED: return TTF_RenderUTF8_Blended(font, text, fg);
        default:                 return NULL;
    }
}

/* ------------------------------------------------------------------ */
TTF_TextResult render_text_ttf(SDL_Renderer*    ren,
                               Font_ttf*        font,
                               const char*      utf8_text,
                               TTF_RenderMode   mode,
                               SDL_Color        fg,
                               SDL_Color        bg)
{
    TTF_TextResult res = { .texture = NULL, .w = 0, .h = 0 };

    if (!ren || !font || !font->font || !utf8_text || utf8_text[0] == '\0')
        return res;

    SDL_Surface* surf = render_surface(font->font, utf8_text, mode, fg, bg);
    if (!surf) return res;

    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    if (tex) {
        res.texture = tex;
        res.w       = surf->w;
        res.h       = surf->h;
    }

    SDL_FreeSurface(surf);
    return res;
}

/* ------------------------------------------------------------------ */
int ttf_text_width(Font_ttf* f, const char* text)
{
    int w = 0;
    if (f && f->font && text) TTF_SizeUTF8(f->font, text, &w, NULL);
    return w;
}

int ttf_text_height(Font_ttf* f, const char* text)
{
    int h = 0;
    if (f && f->font && text) TTF_SizeUTF8(f->font, text, NULL, &h);
    return h;
}

int ttf_line_skip(Font_ttf* f)
{
    return f && f->font ? TTF_FontLineSkip(f->font) : 0;
}

/* ------------------------------------------------------------------ */
/* Extra measurement helpers – thin wrappers around the real SDL_ttf   */
/* ------------------------------------------------------------------ */

int ttf_font_height(Font_ttf* f)
{
    return f && f->font ? TTF_FontHeight(f->font) : 0;
}

int ttf_font_ascent(Font_ttf* f)
{
    return f && f->font ? TTF_FontAscent(f->font) : 0;
}

int ttf_font_descent(Font_ttf* f)
{
    return f && f->font ? TTF_FontDescent(f->font) : 0;
}

/* TTF_SizeText – works with any 8-bit string (Latin-1 or UTF-8 if the font supports it) */
int ttf_size_text(Font_ttf* f, const char* text, int* w, int* h)
{
    if (!f || !f->font || !text) {
        if (w) *w = 0;
        if (h) *h = 0;
        return -1;
    }
    return TTF_SizeText(f->font, text, w, h);
}

/* TTF_SizeUTF8 – guaranteed UTF-8, the same call you already use for width/height */
int ttf_size_utf8(Font_ttf* f, const char* text, int* w, int* h)
{
    if (!f || !f->font || !text) {
        if (w) *w = 0;
        if (h) *h = 0;
        return -1;
    }
    return TTF_SizeUTF8(f->font, text, w, h);
}

//--------------------  drawing ------------------
void draw_text_from_font(Base* base, void* font_ptr, const char* text,
                                 int x, int y, Color color, TextAlign align)
{
	Font_ttf* font = (Font_ttf*)font_ptr;
    if (!font || !text || text[0] == '\0') {
        printf("Invalid font or empty text\n");
        return;
    }

    SDL_Color fg = { color.r, color.g, color.b, color.a };
    SDL_Color bg = { 0, 0, 0, 0 };  // not used in SOLID

    // Use SOLID for speed (aliased), or BLENDED for smooth
    TTF_TextResult result = render_text_ttf(
        base->sdl_renderer,
        font,
        text,
        TTF_RENDER_BLENDED,   // or TTF_RENDER_SOLID
        fg,
        bg
    );

    if (!result.texture) {
        printf("Failed to render text (TTF error hidden in wrapper)\n");
        return;
    }

    int draw_x = x;
    switch (align) {
        case ALIGN_CENTER:
            draw_x = x - result.w / 2;
            break;
        case ALIGN_RIGHT:
            draw_x = x - result.w;
            break;
        case ALIGN_LEFT:
        default:
            break;
    }

    SDL_Rect dst = { draw_x, y, result.w, result.h };
    SDL_RenderCopy(base->sdl_renderer, result.texture, NULL, &dst);

    // Clean up texture
    free_ttf_result(&result);
}

void draw_text(Base* base, const char* text, int font_size, int x, int y, Color color) {
    Font_ttf* font = load_font_ttf(current_theme->font_file, font_size);
    if (!font) {
        printf("Failed to load font '%s': %s\n",current_theme->font_file , TTF_GetError());
        return;
    }

    draw_text_from_font(base, font, text, x, y, color, ALIGN_LEFT);
    free_font_ttf(font);
}
