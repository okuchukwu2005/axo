/* drop.c --------------------------------------------------------------- */
#include "../../include/widgets/drop.h"
#include "../../include/core/theme.h"
#include "../../axo.h"
#include "../../include/core/parent.h"   // for Rect

#include <stdlib.h>
#include <string.h>
#include <math.h>


/* --------------------------------------------------------------------- */
axDropDown axCreateDropDown(axParent* parent, int x, int y, int w, int h,
                   char** options, int option_count)
{
    if (!parent || !parent->base.sdl_renderer) {
        printf("Invalid parent or renderer\n");
        axDropDown d = {0};
        return d;
    }
    if (!current_theme) current_theme = (Theme*)&THEME_LIGHT;

    axDropDown d = {0};
    d.parent         = parent;
    d.x = x; d.y = y; d.w = w; d.h = h;
    d.options        = options;
    d.option_count   = option_count;
    d.selected_index = -1;
    d.place_holder   = strdup("select option");
    return d;
}

/* --------------------------------------------------------------------- */
/* Setters – unchanged (just copy) */
void axSetDropDownBgColor(axDropDown* d, Color c) {
    if (!d) return;
    if (!d->custom_bg_color) d->custom_bg_color = malloc(sizeof(Color));
    if (d->custom_bg_color) *d->custom_bg_color = c;
}
void axSetDropDownButtonColor(axDropDown* d, Color c) {
    if (!d) return;
    if (!d->custom_button_color) d->custom_button_color = malloc(sizeof(Color));
    if (d->custom_button_color) *d->custom_button_color = c;
}
void axSetDropDownTextColor(axDropDown* d, Color c) {
    if (!d) return;
    if (!d->custom_text_color) d->custom_text_color = malloc(sizeof(Color));
    if (d->custom_text_color) *d->custom_text_color = c;
}
void axSetDropDownHighLightColor(axDropDown* d, Color c) {
    if (!d) return;
    if (!d->custom_highlight_color) d->custom_highlight_color = malloc(sizeof(Color));
    if (d->custom_highlight_color) *d->custom_highlight_color = c;
}
void axSetDropFontSize(axDropDown* d, int size) {
    if (d) d->font_size = size;
}

/* --------------------------------------------------------------------- */
static void draw_upside_down_triangle(Base* base, int x, int y, int size, Color col)
{
    int half = size / 2;
    draw_triangle(base,
                  x,           y,           // top-left
                  x + size,    y,           // top-right
                  x + half,    y + size,    // bottom point
                  col);
}

/* --------------------------------------------------------------------- */
void axRenderDropDown(axDropDown* d)
{
    if (!d || !d->parent || !d->parent->base.sdl_renderer || !d->parent->is_open) return;
    if (!global_font) { printf("global_font missing\n"); return; }

    Base* base = &d->parent->base;
    float dpi  = base->dpi_scale;

    /* ---------- DPI-SCALED VALUES ---------- */
    int abs_x = d->x + d->parent->x;
    int abs_y = d->y + d->parent->y + d->parent->title_height;

    int sx = (int)roundf(abs_x * dpi);
    int sy = (int)roundf(abs_y * dpi);
    int sw = (int)roundf(d->w * dpi);
    int sh = (int)roundf(d->h * dpi);
    int pad = (int)roundf(current_theme->padding * dpi);
    int font_sz = (int)roundf((d->font_size > 0 ? d->font_size : current_theme->default_font_size) * dpi);

    /* ---------- PARENT CLIPPING (for containers) ---------- */
    if (!d->parent->is_window) {
        Rect pr = get_parent_rect(d->parent);           // our own Rect
        pr.x = (int)roundf(pr.x * dpi);
        pr.y = (int)roundf(pr.y * dpi);
        pr.w = (int)roundf(pr.w * dpi);
        pr.h = (int)roundf(pr.h * dpi);
        clip_begin(base, &pr);
    } else {
        clip_begin(base, NULL);  // no clipping for root window
    }

    /* ---------- COLORS ---------- */
    Color button = d->custom_button_color ? *d->custom_button_color : current_theme->button_normal;
    if (d->is_hovered) {
        button = d->custom_button_color ? lighten_color(*d->custom_button_color, 0.1f)
                                        : current_theme->button_hovered;
    }
    Color bg        = d->custom_bg_color        ? *d->custom_bg_color        : current_theme->bg_secondary;
    Color text      = d->custom_text_color      ? *d->custom_text_color      : current_theme->text_primary;
    Color highlight = d->custom_highlight_color ? *d->custom_highlight_color : current_theme->accent;

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
    clip_end(base);
}

/* --------------------------------------------------------------------- */
void axUpdateDropDown(axDropDown* d, axEvent* ev)
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
    input_get_mouse(&mouse_x, &mouse_y);
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
    }
}

/* --------------------------------------------------------------------- */
void axFreeDropDown(axDropDown* d)
{
    if (!d) return;
    free(d->place_holder);
    free(d->custom_bg_color);
    free(d->custom_button_color);
    free(d->custom_text_color);
    free(d->custom_highlight_color);
}

/* --------------------------------------------------------------------- */
/* Global registration (unchanged) */
axDropDown* drop_widgets[MAX_DROPS];
int   drops_count = 0;

void axRegisterDropDown(axDropDown* d)
{
    if (drops_count < MAX_DROPS) drop_widgets[drops_count++] = d;
}
void axRenderAllRegisteredDropDown(void)
{
    for (int i = 0; i < drops_count; ++i)
        if (drop_widgets[i]) axRenderDropDown(drop_widgets[i]);
}
void axUpdateAllRegisteredDropDown(axEvent* ev)
{
    for (int i = 0; i < drops_count; ++i)
        if (drop_widgets[i]) axUpdateDropDown(drop_widgets[i], ev);
}
void axFreeAllRegisteredDropDown(void)
{
    for (int i = 0; i < drops_count; ++i) {
        if (drop_widgets[i]) {
            axFreeDropDown(drop_widgets[i]);
            drop_widgets[i] = NULL;
        }
    }
    drops_count = 0;
}
