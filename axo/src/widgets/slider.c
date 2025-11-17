/* slider.c --------------------------------------------------------------- */
#include "../../include/widgets/slider.h"
#include "../../include/core/theme.h"
#include "../../include/core/app.h"
#include "../../include/core/parent.h"   // for Rect

#include <stdlib.h>
#include <string.h>
#include <math.h>

/* --------------------------------------------------------------------- */
/* Slider definition (opaque) */
/* --------------------------------------------------------------------- */
struct Slider {
    Parent* parent;
    int     x, y, w, h;           // logical coordinates
    int     min, max, value;
    char*   label;
    bool    dragging;
    bool    is_hovered;

    Color*  custom_track_color;
    Color*  custom_thumb_color;
    Color*  custom_label_color;
};

/* --------------------------------------------------------------------- */
Slider new_slider(Parent* parent, int x, int y, int w, int h,
                  int min, int max, int start_value, const char* label)
{
    if (!parent || !parent->base.sdl_renderer) {
        printf("Invalid parent or renderer\n");
        Slider s = {0};
        return s;
    }
    if (!current_theme) current_theme = (Theme*)&THEME_LIGHT;

    Slider s = {0};
    s.parent     = parent;
    s.x          = x;
    s.y          = y;
    s.w          = w;
    s.h          = h;
    s.min        = min;
    s.max        = max;
    s.value      = start_value;
    s.label      = label ? strdup(label) : NULL;
    return s;
}

/* --------------------------------------------------------------------- */
/* Setters – unchanged */
void set_slider_track_color(Slider* s, Color c) {
    if (!s) return;
    if (!s->custom_track_color) s->custom_track_color = malloc(sizeof(Color));
    if (s->custom_track_color) *s->custom_track_color = c;
}
void set_slider_thumb_color(Slider* s, Color c) {
    if (!s) return;
    if (!s->custom_thumb_color) s->custom_thumb_color = malloc(sizeof(Color));
    if (s->custom_thumb_color) *s->custom_thumb_color = c;
}
void set_slider_label_color(Slider* s, Color c) {
    if (!s) return;
    if (!s->custom_label_color) s->custom_label_color = malloc(sizeof(Color));
    if (s->custom_label_color) *s->custom_label_color = c;
}

/* --------------------------------------------------------------------- */
void render_slider(Slider* s)
{
    if (!s || !s->parent || !s->parent->base.sdl_renderer || !s->parent->is_open) return;
    if (!global_font) { printf("global_font missing\n"); return; }

    Base* base = &s->parent->base;
    float dpi  = base->dpi_scale;

    /* ---------- DPI-SCALED VALUES ---------- */
    int abs_x = s->x + s->parent->x;
    int abs_y = s->y + s->parent->y + s->parent->title_height;

    int sx = (int)roundf(abs_x * dpi);
    int sy = (int)roundf(abs_y * dpi);
    int sw = (int)roundf(s->w * dpi);
    int sh = (int)roundf(s->h * dpi);

    int track_h     = (int)roundf(4 * dpi);
    int thumb_w     = (int)roundf(10 * dpi);
    int pad         = (int)roundf(current_theme->padding * dpi);
    int label_pad   = (int)roundf(8 * dpi);
    int label_v_offset = (int)roundf(2 * dpi);

    /* ---------- PARENT CLIPPING (for containers) ---------- */
    if (!s->parent->is_window) {
        Rect pr = get_parent_rect(s->parent);           // our own Rect
        pr.x = (int)roundf(pr.x * dpi);
        pr.y = (int)roundf(pr.y * dpi);
        pr.w = (int)roundf(pr.w * dpi);
        pr.h = (int)roundf(pr.h * dpi);
        clip_begin(base, &pr);
    } else {
        clip_begin(base, NULL);  // no clipping for root window
    }

    /* ---------- COLORS ---------- */
    Color track = s->custom_track_color ? *s->custom_track_color : current_theme->bg_secondary;
    Color thumb = s->custom_thumb_color ? *s->custom_thumb_color : current_theme->accent;
    if (s->is_hovered || s->dragging) {
        thumb = s->custom_thumb_color ? lighten_color(*s->custom_thumb_color, 0.1f)
                                      : current_theme->button_hovered;
    }

    /* ---------- TRACK (centered vertically) ---------- */
    int track_y = sy + (sh - track_h) / 2;
    draw_rect(base, sx, track_y, sw, track_h, track);

    /* ---------- THUMB POSITION ---------- */
    float range = s->max - s->min;
    float ratio = (s->value - s->min) / range;
    int thumb_x = sx + (int)(ratio * sw);
    int thumb_cx = thumb_x - thumb_w / 2;

    draw_rect(base, thumb_cx, sy, thumb_w, sh, thumb);

    /* ---------- LABEL (if any) ---------- */
    if (s->label) {
        Color label_col = s->custom_label_color ? *s->custom_label_color : current_theme->text_secondary;
        int label_x = sx + sw + label_pad;
        int label_y = sy + (sh / 2) - label_v_offset;
        draw_text_from_font(base, global_font, s->label, label_x, label_y, label_col, ALIGN_LEFT);
    }

    /* ---------- RESTORE CLIP ---------- */
    clip_end(base);
}

/* --------------------------------------------------------------------- */
void update_slider(Slider* s, Event* ev)
{
    if (!s || !s->parent || !s->parent->is_open) return;

    float dpi = s->parent->base.dpi_scale;

    /* ---------- DPI-SCALED ABSOLUTE POSITION ---------- */
    int abs_x = s->x + s->parent->x;
    int abs_y = s->y + s->parent->y + s->parent->title_height;
    int sx    = (int)roundf(abs_x * dpi);
    int sy    = (int)roundf(abs_y * dpi);
    int sw    = (int)roundf(s->w  * dpi);
    int sh    = (int)roundf(s->h  * dpi);
    int thumb_w = (int)roundf(10 * dpi);

    /* ---------- THUMB PHYSICAL BOUNDS ---------- */
    float range = s->max - s->min;
    float ratio = (s->value - s->min) / range;
    int thumb_x = sx + (int)(ratio * sw);
    int thumb_left  = thumb_x - thumb_w / 2;
    int thumb_right = thumb_x + thumb_w / 2;

    /* ---------- MOUSE STATE (physical pixels) ---------- */
    int mouse_x, mouse_y;
    input_get_mouse(&mouse_x, &mouse_y);

    /* ---------- HOVER ---------- */
    bool over_thumb = (mouse_x >= thumb_left && mouse_x <= thumb_right &&
                       mouse_y >= sy && mouse_y <= sy + sh);
    s->is_hovered = over_thumb;

    /* ---------- DRAG LOGIC ---------- */
    if (ev->type == EVENT_MOUSEMOTION) {
        if (s->dragging) {
            int rel_x = mouse_x - sx;
            if (rel_x < 0) rel_x = 0;
            if (rel_x > sw) rel_x = sw;
            float logical_ratio = rel_x / (float)sw;
            int new_val = s->min + (int)(logical_ratio * range);
            s->value = new_val;
        }
    }
    else if (ev->type == EVENT_MOUSEBUTTONDOWN && ev->mouseButton.button == MOUSE_LEFT) {
        if (over_thumb) {
            s->dragging = true;
        }
    }
    else if (ev->type == EVENT_MOUSEBUTTONUP && ev->mouseButton.button == MOUSE_LEFT) {
        s->dragging = false;
    }
}

/* --------------------------------------------------------------------- */
void free_slider(Slider* s)
{
    if (!s) return;
    free(s->label);
    free(s->custom_track_color);
    free(s->custom_thumb_color);
    free(s->custom_label_color);
}

/* --------------------------------------------------------------------- */
/* Global registration (unchanged) */
Slider* sliders[MAX_SLIDERS];
int     sliders_count = 0;

void register_slider(Slider* s)
{
    if (sliders_count < MAX_SLIDERS) sliders[sliders_count++] = s;
}
void render_all_registered_sliders(void)
{
    for (int i = 0; i < sliders_count; ++i)
        if (sliders[i]) render_slider(sliders[i]);
}
void update_all_registered_sliders(Event* ev)
{
    for (int i = 0; i < sliders_count; ++i)
        if (sliders[i]) update_slider(sliders[i], ev);
}
void free_all_registered_sliders(void)
{
    for (int i = 0; i < sliders_count; ++i) {
        if (sliders[i]) {
            free_slider(sliders[i]);
            sliders[i] = NULL;
        }
    }
    sliders_count = 0;
}
