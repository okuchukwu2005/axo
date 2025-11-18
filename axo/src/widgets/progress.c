/* progress.c --------------------------------------------------------------- */
#include "../../include/widgets/progress.h"
#include "../../include/core/theme.h"
#include "../../include/core/app.h"
#include "../../include/core/parent.h"   // for Rect

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/* --------------------------------------------------------------------- */
axProgressBar axCreateProgressBar(axParent* parent, int x, int y, int w, int h,
                             int min, int max, int start_value, bool show_percentage)
{
    if (!parent || !parent->base.sdl_renderer) {
        printf("Invalid parent or renderer\n");
        axProgressBar p = {0};
        return p;
    }
    if (!current_theme) current_theme = (Theme*)&THEME_LIGHT;

    axProgressBar p = {0};
    p.parent           = parent;
    p.x                = x;
    p.y                = y;
    p.w                = w;
    p.h                = h;
    p.min              = min;
    p.max              = max;
    p.value            = start_value;
    p.show_percentage  = show_percentage;
    return p;
}

/* --------------------------------------------------------------------- */
void axSetProgressBarBgColor(axProgressBar* p, Color c) {
    if (!p) return;
    if (!p->custom_bg_color) p->custom_bg_color = malloc(sizeof(Color));
    if (p->custom_bg_color) *p->custom_bg_color = c;
}
void axSetProgressBarFillColor(axProgressBar* p, Color c) {
    if (!p) return;
    if (!p->custom_fill_color) p->custom_fill_color = malloc(sizeof(Color));
    if (p->custom_fill_color) *p->custom_fill_color = c;
}
void axSetProgressBarTextColor(axProgressBar* p, Color c) {
    if (!p) return;
    if (!p->custom_text_color) p->custom_text_color = malloc(sizeof(Color));
    if (p->custom_text_color) *p->custom_text_color = c;
}
void axSetProgressBarValue(axProgressBar* p, int value) {
    if (!p) return;
    if (value < p->min) value = p->min;
    if (value > p->max) value = p->max;
    p->value = value;
}

/* --------------------------------------------------------------------- */
void axRenderProgressBar(axProgressBar* p)
{
    if (!p || !p->parent || !p->parent->base.sdl_renderer || !p->parent->is_open) return;
    if (!global_font) return;

    Base* base = &p->parent->base;
    float dpi  = base->dpi_scale;

    /* ---------- DPI-SCALED VALUES ---------- */
    int abs_x = p->x + p->parent->x;
    int abs_y = p->y + p->parent->y + p->parent->title_height;

    int sx = (int)roundf(abs_x * dpi);
    int sy = (int)roundf(abs_y * dpi);
    int sw = (int)roundf(p->w * dpi);
    int sh = (int)roundf(p->h * dpi);
    float roundness = current_theme->roundness;

    /* ---------- PARENT CLIPPING (for containers) ---------- */
    if (!p->parent->is_window) {
        Rect pr = get_parent_rect(p->parent);           // our own Rect
        pr.x = (int)roundf(pr.x * dpi);
        pr.y = (int)roundf(pr.y * dpi);
        pr.w = (int)roundf(pr.w * dpi);
        pr.h = (int)roundf(pr.h * dpi);
        clip_begin(base, &pr);
    } else {
        clip_begin(base, NULL);  // no clipping for root window
    }

    /* ---------- COLORS ---------- */
    Color bg   = p->custom_bg_color   ? *p->custom_bg_color   : current_theme->bg_secondary;
    Color fill = p->custom_fill_color ? *p->custom_fill_color : current_theme->accent;
    Color txt  = p->custom_text_color ? *p->custom_text_color : current_theme->text_primary;

    /* ---------- DRAW BACKGROUND ---------- */
    draw_rounded_rect(base, sx, sy, sw, sh, roundness, bg);

    /* ---------- DRAW FILL ---------- */
    float ratio = (float)(p->value - p->min) / (p->max - p->min);
    int fill_w = (int)roundf(sw * ratio);
    if (fill_w > 0) {
        draw_rounded_rect(base, sx, sy, fill_w, sh, roundness, fill);
    }

    /* ---------- PERCENTAGE TEXT (centered) ---------- */
    if (p->show_percentage) {
        char txt_buf[16];
        snprintf(txt_buf, sizeof(txt_buf), "%d%%", (int)(ratio * 100));

        int text_w = ttf_text_width(global_font, txt_buf);
        if (text_w < 0) text_w = 0;
        int text_h = current_theme->default_font_size;

        int tx = sx + (sw - text_w) / 2;
        int ty = sy + (sh - text_h) / 2;

        draw_text_from_font(base, global_font, txt_buf, tx, ty, txt, ALIGN_LEFT);
    }

    /* ---------- RESTORE CLIP ---------- */
    clip_end(base);
}

/* --------------------------------------------------------------------- */
void axUpdateProgressBar(axProgressBar* p, axEvent* ev)
{
    /* Non-interactive – nothing to do */
    (void)p; (void)ev;
}

/* --------------------------------------------------------------------- */
void axFreeProgressBar(axProgressBar* p)
{
    if (!p) return;
    free(p->custom_bg_color);
    free(p->custom_fill_color);
    free(p->custom_text_color);
}

/* --------------------------------------------------------------------- */
/* Registration (unchanged) */
axProgressBar* progress_bar_widgets[MAX_PROGRESS_BARS];
int          progress_bars_count = 0;

void axRegisterProgressBar(axProgressBar* p)
{
    if (progress_bars_count < MAX_PROGRESS_BARS)
        progress_bar_widgets[progress_bars_count++] = p;
}

void axRenderAllRegisteredProgressBars(void)
{
    for (int i = 0; i < progress_bars_count; ++i)
        if (progress_bar_widgets[i]) axRenderProgressBar(progress_bar_widgets[i]);
}

void axUpdateAllRegisteredProgressBars(axEvent* ev)
{
    for (int i = 0; i < progress_bars_count; ++i)
        if (progress_bar_widgets[i]) axUpdateProgressBar(progress_bar_widgets[i], ev);
}

void axFreeAllRegisteredProgressBars(void)
{
    for (int i = 0; i < progress_bars_count; ++i) {
        if (progress_bar_widgets[i]) {
            axFreeProgressBar(progress_bar_widgets[i]);
            progress_bar_widgets[i] = NULL;
        }
    }
    progress_bars_count = 0;
}
