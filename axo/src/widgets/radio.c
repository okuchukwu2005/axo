/* radio.c --------------------------------------------------------------- */
#include "../../include/widgets/radio.h"
#include "../../include/core/theme.h"
#include "../../axo.h"
#include "../../include/core/parent.h"   // for Rect

#include <stdlib.h>
#include <string.h>
#include <math.h>


/* --------------------------------------------------------------------- */
axRadioButton *axCreateRadioButton(axParent* parent, int x, int y, int w, int h,
                       const char* label, int group_id)
{
    if (!parent || !parent->base.sdl_renderer) {
        printf("Invalid parent or renderer\n");
        return NULL;
    }
    if (!current_theme) current_theme = (Theme*)&THEME_LIGHT; // if no default theme set; use light

    axRadioButton* r = (axRadioButton*)calloc(1,sizeof(axRadioButton)); // allocate memory using calloc
    
        if(!r){
        	DEBUG_PRINT("failed to allocate memory for radio button\n");
        	return NULL;
        }
        //--------- initialize radio obj --------
    r->parent     = parent;
    r->x          = x;
    r->y          = y;
    r->w          = w;
    r->h          = h;
    r->label      = label ? strdup(label) : NULL; // ternary operator to store text label or set to null
    r->group_id   = group_id;
    return r;
}


/* --------------------------------------------------------------------- */
void axRenderRadioButton(axRadioButton* r)
{
    if (!r || !r->parent || !r->parent->base.sdl_renderer || !r->parent->is_open) return;
    if (!global_font) { printf("global_font missing\n"); return; }

    Base* base = &r->parent->base;
    float dpi  = base->dpi_scale;

    /* ---------- abs x and y,  ---------- */
    /*
    adding parent x and y to child x and y, so if for example a child is rendered in a container, if the container moves in real time it's child will move in relation to it 
    */
    int abs_x = r->x + r->parent->x;
    int abs_y = r->y + r->parent->y + r->parent->title_height; /* sometimes if parent (window, container) does not have a title bar then title bar height will be zero when the window is created, so adding zero to the abs_y does nothing. but if it has title bar then it must be accounted for, for precise rendering*/
 /* ---------- DPI-SCALED VALUES ---------- */
    int sx     = (int)roundf(abs_x * dpi);
    int sy     = (int)roundf(abs_y * dpi);
    int sh     = (int)roundf(r->h * dpi);           // use height as size
    int radius = sh / 2;
    int pad    = (int)roundf(current_theme->padding * dpi);
    int inner_m = (int)roundf(4 * dpi);             // inner margin
    int inner_r = radius - inner_m;

    /* ---------- PARENT CLIPPING (for containers) ---------- */
    if (!r->parent->is_window) {
        Rect pr = get_parent_rect(r->parent);       // our own Rect: like use a new rect pr (parent rect), to get the rect properties from r->parent
        pr.x = (int)roundf(pr.x * dpi); // x of rect 
        pr.y = (int)roundf(pr.y * dpi);
        pr.w = (int)roundf(pr.w * dpi);
        pr.h = (int)roundf(pr.h * dpi);
        clip_begin(base, &pr);
    } else {
        clip_begin(base, NULL);  // no clipping for root window
    }

    /* ---------- COLORS ---------- */
    Color outer = r->has_custom_outer_color ? r->custom_outer_color : current_theme->bg_secondary;
    if (r->is_hovered) {
        outer = r->has_custom_outer_color ? lighten_color(r->custom_outer_color, 0.1f)
                                      : current_theme->button_hovered;
    }
    Color inner = r->has_custom_inner_color ? r->custom_inner_color : current_theme->accent;
    Color label = r->has_custom_label_color ? r->custom_label_color : current_theme->text_primary;

    /* ---------- DRAW CIRCLE (outer) ---------- */
    draw_circle(base, sx, sy, radius, outer);

    /* ---------- INNER DOT (if selected) ---------- */
    if (r->selected && inner_r > 0) {
        draw_circle(base, sx, sy, inner_r, inner);
    }

    /* ---------- LABEL (to the right, vertically centered) ---------- */
    if (r->label) {
        int radius  = sh / 2;
		int label_x = sx + radius + pad;
        int text_height = ttf_font_height(global_font); // this function is a wrapper over TTF_FontHeight() from SDL, it code is in backend/sdl2_ttf.h
       int label_y = sy - text_height / 2;

        draw_text_from_font(base, global_font, r->label,
                            label_x, label_y, label, ALIGN_LEFT);
    }

    /* ---------- RESTORE CLIP ---------- */
    clip_end(base);
}

/* --------------------------------------------------------------------- */
void axUpdateRadioButton(axRadioButton* r, axEvent* ev)
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
    input_get_mouse(&mouse_x, &mouse_y);

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
void axFreeRadioButton(axRadioButton* r)
{
    if (!r) return;
    free(r->label);
    free(r);
}

/* --------------------------------------------------------------------- */
/* Global registration (unchanged) */
axRadioButton* radio_widgets[MAX_RADIOS];
int    radios_count = 0;

void axRegisterRadioButton(axRadioButton* r)
{
    if (radios_count < MAX_RADIOS) radio_widgets[radios_count++] = r;
}
void axRenderAllRegisteredRadioButtons(void)
{
    for (int i = 0; i < radios_count; ++i)
        if (radio_widgets[i]) axRenderRadioButton(radio_widgets[i]);
}
void axUpdateAllRegisteredRadioButtons(axEvent* ev)
{
    for (int i = 0; i < radios_count; ++i)
        if (radio_widgets[i]) axUpdateRadioButton(radio_widgets[i], ev);
}
void axFreeAllRegisteredRadioButtons(void)
{
    for (int i = 0; i < radios_count; ++i) {
        if (radio_widgets[i]) {
            axFreeRadioButton(radio_widgets[i]);
            radio_widgets[i] = NULL;
        }
    }
    radios_count = 0;
}
