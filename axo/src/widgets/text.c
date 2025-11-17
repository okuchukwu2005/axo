/* text.c --------------------------------------------------------------- */
#include "../../include/widgets/text.h"
#include "../../include/core/theme.h"
#include "../../include/core/app.h"
#include "../../include/core/parent.h"   // for Rect

#include <stdlib.h>
#include <string.h>
#include <math.h>

/* --------------------------------------------------------------------- */
/* Text definition (opaque) */
/* --------------------------------------------------------------------- */
struct Text {
    Parent*    parent;
    int        x, y;               // logical coordinates
    char*      content;
    int        font_size;          // logical font size
    Color*     color;              // NULL → use theme
    TextAlign  align;
};

/* --------------------------------------------------------------------- */
Text new_text(Parent* parent, int x, int y, const char* content,
              int font_size, TextAlign align)
{
    if (!parent || !parent->base.sdl_renderer) {
        printf("Invalid parent or renderer\n");
        Text t = {0};
        return t;
    }
    if (!current_theme) current_theme = (Theme*)&THEME_LIGHT;

    Text t = {0};
    t.parent     = parent;
    t.x          = x;
    t.y          = y;
    t.content    = content ? strdup(content) : NULL;
    t.font_size  = font_size;
    t.align      = align;
    return t;
}

/* --------------------------------------------------------------------- */
void render_text(Text* t)
{
    if (!t || !t->parent || !t->parent->base.sdl_renderer || !t->parent->is_open) return;
    if (!t->content) return;
    if (!global_font) { printf("global_font missing\n"); return; }

    Base* base = &t->parent->base;
    float dpi  = base->dpi_scale;

    /* ---------- DPI-SCALED VALUES ---------- */
    int abs_x = t->x + t->parent->x;
    int abs_y = t->y + t->parent->y + t->parent->title_height;

    int sx = (int)roundf(abs_x * dpi);
    int sy = (int)roundf(abs_y * dpi);

    /* ---------- PARENT CLIPPING (for containers) ---------- */
    if (!t->parent->is_window) {
        Rect pr = get_parent_rect(t->parent);           // our own Rect
        pr.x = (int)roundf(pr.x * dpi);
        pr.y = (int)roundf(pr.y * dpi);
        pr.w = (int)roundf(pr.w * dpi);
        pr.h = (int)roundf(pr.h * dpi);
        clip_begin(base, &pr);
    } else {
        clip_begin(base, NULL);  // no clipping for root window
    }

    /* ---------- COLOR ---------- */
    Color col = t->color ? *t->color : current_theme->text_primary;

    /* ---------- RENDER TEXT (physical pixels, DPI-scaled font) ---------- */
    draw_text_from_font(base,
                        global_font,
                        t->content,
                        sx, sy,
                        col,
                        t->align);

    /* ---------- RESTORE CLIP ---------- */
    clip_end(base);
}

/* --------------------------------------------------------------------- */
void set_text_color(Text* t, Color c)
{
    if (!t) return;
    if (!t->color) {
        t->color = malloc(sizeof(Color));
        if (!t->color) { printf("malloc failed for text color\n"); return; }
    }
    *t->color = c;
}

/* --------------------------------------------------------------------- */
void update_text(Text* t, Event* ev)
{
    (void)t; (void)ev;   // static widget – no interaction
}

/* --------------------------------------------------------------------- */
void free_text(Text* t)
{
    if (!t) return;
    free(t->content);
    free(t->color);
}

/* --------------------------------------------------------------------- */
/* Global registration (unchanged) */
Text* text_widgets[MAX_TEXTS];
int   texts_count = 0;

void register_text(Text* t)
{
    if (texts_count < MAX_TEXTS) text_widgets[texts_count++] = t;
}
void render_all_registered_texts(void)
{
    for (int i = 0; i < texts_count; ++i)
        if (text_widgets[i]) render_text(text_widgets[i]);
}
void update_all_registered_texts(Event* ev)
{
    for (int i = 0; i < texts_count; ++i)
        if (text_widgets[i]) update_text(text_widgets[i], ev);
}
void free_all_registered_texts(void)
{
    for (int i = 0; i < texts_count; ++i) {
        if (text_widgets[i]) {
            free_text(text_widgets[i]);
            text_widgets[i] = NULL;
        }
    }
    texts_count = 0;
}
