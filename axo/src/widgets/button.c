/* button.c --------------------------------------------------------------- */
#include "../../include/widgets/button.h"
#include "../../include/core/theme.h"
#include "../../include/core/app.h"
#include "../../include/core/parent.h"    // <-- for Rect

#include <stdlib.h>
#include <string.h>
#include <math.h>

void function_callback_override (){
    DEBUG_PRINT("Button was clicked!\n");
    // Add custom logic, e.g., open a dialog, submit a form, etc.
}

/* --------------------------------------------------------------------- */
axButton axCreateButton(axParent* parent, int x, int y, int w, int h,
                  const char* label, void (*callback)(void))
{
    if (!parent || !parent->base.sdl_renderer) {
        DEBUG_PRINT("Error: Invalid parent or renderer\n");
        axButton b = {0};
        return b;
    }
    if (!current_theme) current_theme = (Theme*)&THEME_LIGHT;

    axButton b = {0};
    b.parent = parent;
    b.x = x; b.y = y; b.w = w; b.h = h;
    b.label = label ? strdup(label) : NULL;
    b.callback = callback;
    return b;
}

/* --------------------------------------------------------------------- */
void axSetButtonBgColor(axButton* b, Color c) {
    if (!b) return;
    if (!b->custom_bg_color) b->custom_bg_color = malloc(sizeof(Color));
    if (b->custom_bg_color) *b->custom_bg_color = c;
}

void axSetButtonTextColor(axButton* b, Color c) {
    if (!b) return;
    if (!b->custom_text_color) b->custom_text_color = malloc(sizeof(Color));
    if (b->custom_text_color) *b->custom_text_color = c;
}

/* --------------------------------------------------------------------- */
void axRenderButton(axButton* b)
{
    if (!b || !b->parent || !b->parent->base.sdl_renderer || !b->parent->is_open) return;
    if (!global_font) return;

    Base* base = &b->parent->base;
    float dpi = base->dpi_scale;

    /* ---------- DPI-SCALED BUTTON BOUNDS ---------- */
    int abs_x = b->x + b->parent->x;
    int abs_y = b->y + b->parent->y + b->parent->title_height;

    int sx = (int)roundf(abs_x * dpi);
    int sy = (int)roundf(abs_y * dpi);
    int sw = (int)roundf(b->w * dpi);
    int sh = (int)roundf(b->h * dpi);

    // Rect button_rect = { sx, sy, sw, sh };

    /* ---------- PARENT CLIPPING (for containers) ---------- */
    if (!b->parent->is_window) {
        Rect pr = get_parent_rect(b->parent);           // our own Rect
        pr.x = (int)roundf(pr.x * dpi);
        pr.y = (int)roundf(pr.y * dpi);
        pr.w = (int)roundf(pr.w * dpi);
        pr.h = (int)roundf(pr.h * dpi);
        clip_begin(base, &pr);
    } else {
        clip_begin(base, NULL);  // no clipping for root window
    }

    /* ---------- BACKGROUND COLOR (state-aware) ---------- */
    Color bg = b->custom_bg_color ? *b->custom_bg_color : current_theme->button_normal;
    if (b->is_pressed) {
        bg = b->custom_bg_color ? darken_color(*b->custom_bg_color, 0.2f)
                                : current_theme->button_pressed;
    } else if (b->is_hovered) {
        bg = b->custom_bg_color ? lighten_color(*b->custom_bg_color, 0.1f)
                                : current_theme->button_hovered;
    }

    /* ---------- DRAW ROUNDED RECT ---------- */
    draw_rounded_rect(base, sx, sy, sw, sh, current_theme->roundness, bg);

    /* ---------- TEXT (centered) ---------- */
    if (b->label) {
        int text_w = ttf_text_width(global_font, b->label);
        if (text_w < 0) text_w = 0;
        int text_h = current_theme->default_font_size;
        int text_x = sx + (sw - text_w) / 2;
        int text_y = sy + (sh - text_h) / 2;
        Color txt_col = b->custom_text_color ? *b->custom_text_color : current_theme->button_text;
        draw_text_from_font(base, global_font, b->label, text_x, text_y, txt_col, ALIGN_LEFT);
    }

    /* ---------- RESTORE CLIP ---------- */
    clip_end(base);
}

/* --------------------------------------------------------------------- */
void axUpdateButton(axButton* b, axEvent* ev)
{
    if (!b || !b->parent || !b->parent->is_open) return;

    float dpi = b->parent->base.dpi_scale;

    /* ---------- PHYSICAL BOUNDS FOR MOUSE ---------- */
    int abs_x = b->x + b->parent->x;
    int abs_y = b->y + b->parent->y + b->parent->title_height;
    int sx = (int)roundf(abs_x * dpi);
    int sy = (int)roundf(abs_y * dpi);
    int sw = (int)roundf(b->w * dpi);
    int sh = (int)roundf(b->h * dpi);

    int mouse_x, mouse_y;
    input_get_mouse(&mouse_x, &mouse_y);
    bool over = (mouse_x >= sx && mouse_x < sx + sw &&
                 mouse_y >= sy && mouse_y < sy + sh);

    if (ev->type == EVENT_MOUSEMOTION) {
        b->is_hovered = over;
    }
    else if (ev->type == EVENT_MOUSEBUTTONDOWN && ev->mouseButton.button == MOUSE_LEFT) {
        if (over) b->is_pressed = true;
    }
    else if (ev->type == EVENT_MOUSEBUTTONUP && ev->mouseButton.button == MOUSE_LEFT) {
        if (b->is_pressed && over && b->callback) {
            b->callback();
        }
        b->is_pressed = false;
    }
}

/* --------------------------------------------------------------------- */
void axFreeButton(axButton* b)
{
    if (!b) return;
    free(b->label);
    free(b->custom_bg_color);
    free(b->custom_text_color);
}

/* --------------------------------------------------------------------- */
/* Registration (unchanged) */
/* --------------------------------------------------------------------- */
axButton* button_widgets[MAX_BUTTONS];
int buttons_count = 0;

void axRegisterButton(axButton* b)
{
    if (buttons_count < MAX_BUTTONS)
        button_widgets[buttons_count++] = b;
}

void axRenderAllRegisteredButtons(void)
{
    for (int i = 0; i < buttons_count; ++i)
        if (button_widgets[i]) axRenderButton(button_widgets[i]);
}

void axUpdateAllRegisteredButtons(axEvent* ev)
{
    for (int i = 0; i < buttons_count; ++i)
        if (button_widgets[i]) axUpdateButton(button_widgets[i], ev);
}

void axFreeAllRegisteredButtons(void)
{
    for (int i = 0; i < buttons_count; ++i) {
        if (button_widgets[i]) {
            axFreeButton(button_widgets[i]);
            button_widgets[i] = NULL;
        }
    }
    buttons_count = 0;
}
