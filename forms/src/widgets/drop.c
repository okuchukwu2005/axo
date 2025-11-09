
#include "../../include/widgets/drop.h"
#include "../../include/core/theme.h"
#include "../../include/core/graphics.h"
#include "../../include/core/app.h"


/* drop.c --------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* --------------------------------------------------------------------- */
/*  Drop definition (opaque)                                             */
/* --------------------------------------------------------------------- */
struct Drop {
    Parent*    parent;
    int        x, y, w, h;          /* logical coordinates */
    char**     options;
    int        option_count;
    int        selected_index;
    bool       is_expanded;
    bool       is_hovered;
    int        font_size;           /* 0 = use theme default */
    char*      place_holder;

    Color*     custom_bg_color;
    Color*     custom_button_color;
    Color*     custom_text_color;
    Color*     custom_highlight_color;
};

/* --------------------------------------------------------------------- */
Drop new_drop_down(Parent* parent, int x, int y, int w, int h,
                   char** options, int option_count)
{
    if (!parent || !parent->base.sdl_renderer) {
        printf("Invalid parent or renderer\n");
    }
    if (!current_theme) current_theme = (Theme*)&THEME_LIGHT;

    Drop d = {0};
    d.parent         = parent;
    d.x              = x;
    d.y              = y;
    d.w              = w;
    d.h              = h;
    d.options        = options;
    d.option_count   = option_count;
    d.selected_index = -1;
    d.place_holder   = strdup("select option");
    return d;
}

/* --------------------------------------------------------------------- */
/*  Setters – unchanged (just copy)                                      */
/* --------------------------------------------------------------------- */
void set_drop_bg_color(Drop* d, Color c) {
    if (!d) return;
    if (!d->custom_bg_color) d->custom_bg_color = malloc(sizeof(Color));
    if (d->custom_bg_color) *d->custom_bg_color = c;
}
void set_drop_button_color(Drop* d, Color c) {
    if (!d) return;
    if (!d->custom_button_color) d->custom_button_color = malloc(sizeof(Color));
    if (d->custom_button_color) *d->custom_button_color = c;
}
void set_drop_text_color(Drop* d, Color c) {
    if (!d) return;
    if (!d->custom_text_color) d->custom_text_color = malloc(sizeof(Color));
    if (d->custom_text_color) *d->custom_text_color = c;
}
void set_drop_highlight_color(Drop* d, Color c) {
    if (!d) return;
    if (!d->custom_highlight_color) d->custom_highlight_color = malloc(sizeof(Color));
    if (d->custom_highlight_color) *d->custom_highlight_color = c;
}
void set_drop_font_size(Drop* d, int size) {
    if (d) d->font_size = size;
}

/* --------------------------------------------------------------------- */
static void draw_upside_down_triangle(Base* base, int x, int y, int size, Color col)
{
    // Down-pointing triangle: base at top, point at bottom
    int half = size / 2;
    draw_triangle(base,
                  x,           y,           // top-left
                  x + size,    y,           // top-right
                  x + half,    y + size,    // bottom point
                  col);
}

/* --------------------------------------------------------------------- */
void render_drop_down_(Drop* d)
{
    if (!d || !d->parent || !d->parent->base.sdl_renderer || !d->parent->is_open) return;
    if (!global_font) { printf("global_font missing\n"); return; }

    SDL_Renderer* ren = d->parent->base.sdl_renderer;
    float dpi         = d->parent->base.dpi_scale;

    /* ---------- SAVE CLIP ---------- */
    SDL_Rect saved_clip; SDL_RenderGetClipRect(ren, &saved_clip);
    SDL_bool clip_was = SDL_RenderIsClipEnabled(ren);

    /* ---------- PARENT CLIP (if not window) ---------- */
    if (!d->parent->is_window) {
        SDL_Rect pr = get_parent_rect(d->parent);
        pr.x = (int)roundf(pr.x * dpi);
        pr.y = (int)roundf(pr.y * dpi);
        pr.w = (int)roundf(pr.w * dpi);
        pr.h = (int)roundf(pr.h * dpi);
        SDL_RenderSetClipRect(ren, &pr);
    }

    /* ---------- DPI-SCALED VALUES ---------- */
    int abs_x = d->x + d->parent->x;
    int abs_y = d->y + d->parent->y + d->parent->title_height;

    int sx = (int)roundf(abs_x * dpi);
    int sy = (int)roundf(abs_y * dpi);
    int sw = (int)roundf(d->w * dpi);
    int sh = (int)roundf(d->h * dpi);
    int pad = (int)roundf(current_theme->padding * dpi);
    int font_sz = (int)roundf((d->font_size > 0 ? d->font_size : current_theme->default_font_size) * dpi);

    Base* base = &d->parent->base;

    /* ---------- COLORS ---------- */
    Color button = d->custom_button_color ? *d->custom_button_color : current_theme->button_normal;
    if (d->is_hovered) {
        button = d->custom_button_color ? lighten_color(*d->custom_button_color, 0.1f)
                                        : current_theme->button_hovered;
    }
    Color bg       = d->custom_bg_color       ? *d->custom_bg_color       : current_theme->bg_secondary;
    Color text     = d->custom_text_color     ? *d->custom_text_color     : current_theme->text_primary;
    Color highlight= d->custom_highlight_color? *d->custom_highlight_color: current_theme->accent;

    /* ---------- MAIN BUTTON ---------- */
    draw_rect(base, sx, sy, sw, sh, button);

    /* ---------- TEXT (placeholder or selected) ---------- */
    const char* display = (d->selected_index >= 0 && d->selected_index < d->option_count)
                          ? d->options[d->selected_index]
                          : d->place_holder;
    int text_y = sy + (sh - font_sz) / 2;
    draw_text_from_font(base, global_font, display, sx + pad, text_y, text, ALIGN_LEFT);

    /* ---------- ARROW (down/up) ---------- */
    int arrow_size = (int)roundf((d->h / 3.0f) * dpi);
    int arrow_x = sx + sw - arrow_size - pad;
    int arrow_y = sy + (sh - arrow_size) / 2;

    if (d->is_expanded) {
        // Up-pointing triangle
        draw_triangle(base,
                      arrow_x,               arrow_y + arrow_size,  // bottom-left
                      arrow_x + arrow_size,  arrow_y + arrow_size,  // bottom-right
                      arrow_x + arrow_size/2, arrow_y,              // top point
                      text);
    } else {
        draw_upside_down_triangle(base, arrow_x, arrow_y, arrow_size, text);
    }

    /* ---------- EXPANDED OPTIONS ---------- */
    if (d->is_expanded) {
        for (int i = 0; i < d->option_count; ++i) {
            int oy_logical = abs_y + d->h * (i + 1);
            int oy = (int)roundf(oy_logical * dpi);

            Color opt_bg = (i == d->selected_index) ? highlight : bg;
            draw_rect(base, sx, oy, sw, sh, opt_bg);

            int opt_text_y = oy + (sh - font_sz) / 2;
            draw_text_from_font(base, global_font, d->options[i],
                                sx + pad, opt_text_y, text, ALIGN_LEFT);
        }
    }

    /* ---------- RESTORE CLIP ---------- */
    if (clip_was) SDL_RenderSetClipRect(ren, &saved_clip);
    else          SDL_RenderSetClipRect(ren, NULL);
}

/* --------------------------------------------------------------------- */
void update_drop_down_(Drop* d, Event* ev)
{
    if (!d || !d->parent || !d->parent->is_open) return;

    float dpi = d->parent->base.dpi_scale;

    /* ---------- DPI-SCALED ABSOLUTE POSITION ---------- */
    int abs_x = d->x + d->parent->x;
    int abs_y = d->y + d->parent->y + d->parent->title_height;
    int sx    = (int)roundf(abs_x * dpi);
    int sy    = (int)roundf(abs_y * dpi);
    int sw    = (int)roundf(d->w  * dpi);
    int sh    = (int)roundf(d->h  * dpi);

    /* ---------- HOVER (mouse in physical pixels) ---------- */
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    d->is_hovered = (mouse_x >= sx && mouse_x <= sx + sw &&
                     mouse_y >= sy && mouse_y <= sy + sh);

    /* ---------- CLICK ---------- */
    if (ev->type != EVENT_MOUSEBUTTONDOWN || ev->mouseButton.button != MOUSE_LEFT) return;

    int mx = ev->mouseButton.x;
    int my = ev->mouseButton.y;

    // Click on main button → toggle
    if (mx >= sx && mx <= sx + sw && my >= sy && my <= sy + sh) {
        d->is_expanded = !d->is_expanded;
        return;
    }

    // Click on option
    if (d->is_expanded) {
        bool clicked_option = false;
        for (int i = 0; i < d->option_count; ++i) {
            int oy = (int)roundf((abs_y + d->h * (i + 1)) * dpi);
            if (mx >= sx && mx <= sx + sw && my >= oy && my <= oy + sh) {
                d->selected_index = i;
                d->is_expanded = false;
                clicked_option = true;
                break;
            }
        }
        // Click outside → close
        if (!clicked_option) {
            d->is_expanded = false;
        }
    } else {
        // Click outside when collapsed → close (redundant but safe)
        d->is_expanded = false;
    }
}

/* --------------------------------------------------------------------- */
void free_drop_(Drop* d)
{
    if (!d) return;
    free(d->place_holder);
    free(d->custom_bg_color);
    free(d->custom_button_color);
    free(d->custom_text_color);
    free(d->custom_highlight_color);
}

/* --------------------------------------------------------------------- */
/*  Global registration (unchanged)                                      */
/* --------------------------------------------------------------------- */
Drop* drop_widgets[MAX_DROPS];
int   drops_count = 0;

void register_drop(Drop* d)
{
    if (drops_count < MAX_DROPS) drop_widgets[drops_count++] = d;
}
void render_all_registered_drops(void)
{
    for (int i = 0; i < drops_count; ++i)
        if (drop_widgets[i]) render_drop_down_(drop_widgets[i]);
}
void update_all_registered_drops(Event* ev)
{
    for (int i = 0; i < drops_count; ++i)
        if (drop_widgets[i]) update_drop_down_(drop_widgets[i], ev);
}
void free_all_registered_drops(void)
{
    for (int i = 0; i < drops_count; ++i) {
        if (drop_widgets[i]) { free_drop_(drop_widgets[i]); drop_widgets[i] = NULL; }
    }
    drops_count = 0;
}
