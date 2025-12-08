/* progress.c --------------------------------------------------------------- */
#include "../../include/widgets/status.h"
#include "../../include/core/theme.h"
#include "../../axo.h"
#include "../../include/core/parent.h"   // for Rect

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/* --------------------------------------------------------------------- */
axStatusBar *axCreateStatusBar(axParent* parent, int x, int y, int w, int h,
                             int min, int max, int start_value, bool show_percentage)
{
    if (!parent || !parent->base.sdl_renderer) {
        printf("Invalid parent or renderer\n");
        return NULL;
    }
    if (!current_theme) current_theme = (Theme*)&THEME_LIGHT;

    axStatusBar *p = (axStatusBar*)calloc(1,sizeof(axStatusBar));
    
        if(!p){
        	DEBUG_PRINT("failed to allocate memory for progress bar\n");
        	return NULL;
        }
    p->parent           = parent;
    p->x                = x;
    p->y                = y;
    p->w                = w;
    p->h                = h;
    p->min              = min;
    p->max              = max;
    p->value            = start_value;
    p->show_percentage  = show_percentage;
    return p;
}

/* --------------------------------------------------------------------- */
void axRenderStatusBar(axStatusBar* p)
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
    Color bg   = p->has_custom_bg_color   ? p->custom_bg_color   : current_theme->bg_secondary;
    Color fill = p->has_custom_fill_color ? p->custom_fill_color : current_theme->accent;
    Color txt  = p->has_custom_text_color ? p->custom_text_color : current_theme->text_primary;

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
void axUpdateStatusBar(axStatusBar* p, axEvent* ev)
{
    /* Non-interactive – nothing to do */
    (void)p; (void)ev;
}

/* --------------------------------------------------------------------- */
void axFreeStatusBar(axStatusBar* p)
{
    if (!p) return;
    free(p);
}

/* --------------------------------------------------------------------- */
/* Registration (unchanged) */
axStatusBar* status_bar_widgets[MAX_STATUS_BARS];
int status_bars_count = 0;

void axRegisterStatusBar(axStatusBar* p)
{
    if (status_bars_count < MAX_STATUS_BARS)
        status_bar_widgets[status_bars_count++] = p;
}

void axRenderAllRegisteredStatusBar(void)
{
    for (int i = 0; i < status_bars_count; ++i)
        if (status_bar_widgets[i]) axRenderStatusBar(status_bar_widgets[i]);
}

void axUpdateAllRegisteredStatusBar(axEvent* ev)
{
    for (int i = 0; i < status_bars_count; ++i)
        if (status_bar_widgets[i]) axUpdateStatusBar(status_bar_widgets[i], ev);
}

void axFreeAllRegisteredStatusBar(void)
{
    for (int i = 0; i < status_bars_count; ++i) {
        if (status_bar_widgets[i]) {
            axFreeStatusBar(status_bar_widgets[i]);
            status_bar_widgets[i] = NULL;
        }
    }
    status_bars_count = 0;
}
