#include "../../include/widgets/radio.h"
#include "../../include/core/theme.h"
#include "../../include/core/graphics.h"
#include "../../include/core/app.h"
/* radio.c --------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* --------------------------------------------------------------------- */
/*  Radio definition (opaque)                                            */
/* --------------------------------------------------------------------- */
struct Radio {
    Parent*    parent;
    int        x, y, w, h;          /* logical coordinates */
    char*      label;
    int        group_id;
    bool       selected;
    bool       is_hovered;

    Color*     custom_outer_color;
    Color*     custom_inner_color;
    Color*     custom_label_color;
};

/* --------------------------------------------------------------------- */
Radio new_radio_button(Parent* parent, int x, int y, int w, int h,
                       const char* label, int group_id)
{
    if (!parent || !parent->base.sdl_renderer) {
        printf("Invalid parent or renderer\n");
    }
    if (!current_theme) current_theme = (Theme*)&THEME_LIGHT;

    Radio r = {0};
    r.parent     = parent;
    r.x          = x;
    r.y          = y;
    r.w          = w;
    r.h          = h;
    r.label      = label ? strdup(label) : NULL;
    r.group_id   = group_id;
    return r;
}

/* --------------------------------------------------------------------- */
/*  Setters – unchanged (just copy)                                      */
/* --------------------------------------------------------------------- */
void set_radio_outer_color(Radio* r, Color c) {
    if (!r) return;
    if (!r->custom_outer_color) r->custom_outer_color = malloc(sizeof(Color));
    if (r->custom_outer_color) *r->custom_outer_color = c;
}
void set_radio_inner_color(Radio* r, Color c) {
    if (!r) return;
    if (!r->custom_inner_color) r->custom_inner_color = malloc(sizeof(Color));
    if (r->custom_inner_color) *r->custom_inner_color = c;
}
void set_radio_label_color(Radio* r, Color c) {
    if (!r) return;
    if (!r->custom_label_color) r->custom_label_color = malloc(sizeof(Color));
    if (r->custom_label_color) *r->custom_label_color = c;
}

/* --------------------------------------------------------------------- */
void render_radio_(Radio* r)
{
    if (!r || !r->parent || !r->parent->base.sdl_renderer || !r->parent->is_open) return;
    if (!global_font) { printf("global_font missing\n"); return; }

    SDL_Renderer* ren = r->parent->base.sdl_renderer;
    float dpi         = r->parent->base.dpi_scale;

    /* ---------- SAVE CLIP ---------- */
    SDL_Rect saved_clip; SDL_RenderGetClipRect(ren, &saved_clip);
    SDL_bool clip_was = SDL_RenderIsClipEnabled(ren);

    /* ---------- PARENT CLIP (if not window) ---------- */
    if (!r->parent->is_window) {
        SDL_Rect pr = get_parent_rect(r->parent);
        pr.x = (int)roundf(pr.x * dpi);
        pr.y = (int)roundf(pr.y * dpi);
        pr.w = (int)roundf(pr.w * dpi);
        pr.h = (int)roundf(pr.h * dpi);
        SDL_RenderSetClipRect(ren, &pr);
    }

    /* ---------- DPI-SCALED VALUES ---------- */
    int abs_x = r->x + r->parent->x;
    int abs_y = r->y + r->parent->y + r->parent->title_height;

    int sx        = (int)roundf(abs_x * dpi);
    int sy        = (int)roundf(abs_y * dpi);
    int sh        = (int)roundf(r->h * dpi);           // use height as size
    int radius    = sh / 2;
    int pad       = (int)roundf(current_theme->padding * dpi);
    int inner_m   = (int)roundf(4 * dpi);              // inner margin
    int inner_r   = radius - inner_m;

    Base* base = &r->parent->base;

    /* ---------- COLORS ---------- */
    Color outer = r->custom_outer_color ? *r->custom_outer_color : current_theme->bg_secondary;
    if (r->is_hovered) {
        outer = r->custom_outer_color ? lighten_color(*r->custom_outer_color, 0.1f)
                                      : current_theme->button_hovered;
    }
    Color inner = r->custom_inner_color ? *r->custom_inner_color : current_theme->accent;
    Color label = r->custom_label_color ? *r->custom_label_color : current_theme->text_primary;

    /* ---------- DRAW CIRCLE (outer) ---------- */
    draw_circle(base, sx, sy, radius, outer);

    /* ---------- INNER DOT (if selected) ---------- */
    if (r->selected && inner_r > 0) {
        draw_circle(base, sx, sy, inner_r, inner);
    }

    /* ---------- LABEL (to the right, vertically centered) ---------- */
    if (r->label) {
        int label_x = sx + sh + pad / 2;
        int label_y = sy - (sh / 6);   // fine-tuned centering (logical → physical via dpi)

        draw_text_from_font(base, global_font, r->label,
                            label_x, label_y,
                            label, ALIGN_LEFT);
    }

    /* ---------- RESTORE CLIP ---------- */
    if (clip_was) SDL_RenderSetClipRect(ren, &saved_clip);
    else          SDL_RenderSetClipRect(ren, NULL);
}

/* --------------------------------------------------------------------- */
void update_radio_(Radio* r, Event* ev)
{
    if (!r || !r->parent || !r->parent->is_open) return;

    float dpi = r->parent->base.dpi_scale;

    /* ---------- DPI-SCALED ABSOLUTE POSITION ---------- */
    int abs_x = r->x + r->parent->x;
    int abs_y = r->y + r->parent->y + r->parent->title_height;
    int sx    = (int)roundf(abs_x * dpi);
    int sy    = (int)roundf(abs_y * dpi);
    int sh    = (int)roundf(r->h * dpi);
    int radius = sh / 2;

    /* ---------- HOVER (mouse position is in physical pixels) ---------- */
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    // Distance from center
    int dx = mouse_x - sx;
    int dy = mouse_y - sy;
    r->is_hovered = (dx*dx + dy*dy <= radius*radius);

    /* ---------- CLICK ---------- */
    if (ev->type == EVENT_MOUSEBUTTONDOWN && ev->mouseButton.button == MOUSE_LEFT) {
        int mx = ev->mouseButton.x;
        int my = ev->mouseButton.y;
        int dx = mx - sx;
        int dy = my - sy;
        if (dx*dx + dy*dy <= radius*radius) {
            // Deselect others in group
            for (int i = 0; i < radios_count; ++i) {
                if (radio_widgets[i] && radio_widgets[i]->group_id == r->group_id) {
                    radio_widgets[i]->selected = false;
                }
            }
            r->selected = true;
        }
    }
}

/* --------------------------------------------------------------------- */
void free_radio_(Radio* r)
{
    if (!r) return;
    free(r->label);
    free(r->custom_outer_color);
    free(r->custom_inner_color);
    free(r->custom_label_color);
}

/* --------------------------------------------------------------------- */
/*  Global registration (unchanged)                                      */
/* --------------------------------------------------------------------- */
Radio* radio_widgets[MAX_RADIOS];
int    radios_count = 0;

void register_radio(Radio* r)
{
    if (radios_count < MAX_RADIOS) radio_widgets[radios_count++] = r;
}
void render_all_registered_radios(void)
{
    for (int i = 0; i < radios_count; ++i)
        if (radio_widgets[i]) render_radio_(radio_widgets[i]);
}
void update_all_registered_radios(Event* ev)
{
    for (int i = 0; i < radios_count; ++i)
        if (radio_widgets[i]) update_radio_(radio_widgets[i], ev);
}
void free_all_registered_radios(void)
{
    for (int i = 0; i < radios_count; ++i) {
        if (radio_widgets[i]) { free_radio_(radio_widgets[i]); radio_widgets[i] = NULL; }
    }
    radios_count = 0;
}
