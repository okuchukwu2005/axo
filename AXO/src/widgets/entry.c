/* entry.c --------------------------------------------------------------- */
#include "../../include/widgets/entry.h"
#include "../../include/core/theme.h"
#include "../../include/core/graphics.h"
#include "../../include/core/app.h"
#include "../../include/core/parent.h"   // for Rect
#include "../../include/core/backends/sdl2/clip.h"     // for clip_begin/end

#include <stdlib.h>
#include <string.h>
#include <math.h>
/* --------------------------------------------------------------------- */
/* Entry definition (opaque to the rest of the program) */
/* --------------------------------------------------------------------- */
struct Entry {
    Parent* parent;
    char*   place_holder;
    int     x, y, w, h;
    int     max_length;
    char*   text;
    int     is_active;
    int     cursor_pos;
    int     selection_start;
    int     visible_text_start;
    int     is_mouse_selecting;
};

/* --------------------------------------------------------------------- */
/* Helper: measure a UTF-8 substring with the *global* font */
/* --------------------------------------------------------------------- */
static int measure_utf8(const char* txt, int* w, int* h)
{
    if (!global_font) return -1;
    return ttf_size_utf8(global_font, txt, w, h);
}

/* --------------------------------------------------------------------- */
/* new_entry – no font loading any more */
/* --------------------------------------------------------------------- */
Entry new_entry(Parent* parent, int x, int y, int w, int max_length)
{
    if (!parent || !parent->base.sdl_renderer) {
        printf("Invalid parent or renderer\n");
    }
    if (!current_theme) current_theme = (Theme*)&THEME_LIGHT;

    int logical_font_size = current_theme->default_font_size;
    int logical_padding   = current_theme->padding;

    Entry e = {0};
    e.parent       = parent;
    e.place_holder = strdup(" ");
    e.x = x;  e.y = y;  e.w = w;
    e.h = logical_font_size + 2 * logical_padding;
    e.max_length = max_length;
    e.text = malloc(max_length + 1);
    if (e.text) e.text[0] = '\0';
    return e;
}

/* --------------------------------------------------------------------- */
void set_entry_placeholder(Entry* e, const char* placeholder)
{
    if (!e) return;
    free(e->place_holder);
    e->place_holder = placeholder ? strdup(placeholder) : strdup(" ");
}

/* --------------------------------------------------------------------- */
/* render_entry – uses only wrapper functions, clipping fixed */
/* --------------------------------------------------------------------- */
void render_entry(Entry* e)
{
    if (!e || !e->parent || !e->parent->base.sdl_renderer || !e->parent->is_open) return;

    float dpi = e->parent->base.dpi_scale;

    /* ---------- PARENT CLIP (if any) ---------- */
    Rect parent_clip = {0,0,0,0};
    bool has_parent_clip = false;
    if (!e->parent->is_window) {
        parent_clip = get_parent_rect(e->parent);
        parent_clip.x = (int)roundf(parent_clip.x * dpi);
        parent_clip.y = (int)roundf(parent_clip.y * dpi);
        parent_clip.w = (int)roundf(parent_clip.w * dpi);
        parent_clip.h = (int)roundf(parent_clip.h * dpi);
        has_parent_clip = SDL_TRUE;
    }

    /* ---------- PHYSICAL BOUNDS ---------- */
    int abs_x = e->x + e->parent->x;
    int abs_y = e->y + e->parent->y + e->parent->title_height;
    int sx = (int)roundf(abs_x * dpi);
    int sy = (int)roundf(abs_y * dpi);
    int sw = (int)roundf(e->w * dpi);
    int sh = (int)roundf(e->h * dpi);
    int border = (int)roundf(2 * dpi);
    int pad    = (int)roundf(current_theme->padding * dpi);

    if (!global_font) { return; }

    int font_h = ttf_font_height(global_font);   /* wrapper for TTF_FontHeight */

    Rect parent_r = { parent_clip.x, parent_clip.y, parent_clip.w, parent_clip.h };
    if (has_parent_clip) {
        clip_begin(&e->parent->base, &parent_r);
    }

    /* ---------- BACKGROUND / BORDER ---------- */
    draw_rect(&e->parent->base, sx, sy, sw, sh, current_theme->accent);
    draw_rect(&e->parent->base,
              sx + border, sy + border,
              sw - 2*border, sh - 2*border,
              current_theme->bg_secondary);

    if (has_parent_clip) {
        clip_end(&e->parent->base);
    }

    /* ---------- TEXT CLIP – now includes the padding area ---------- */
    Rect text_clip = {
        sx + border,               /* left   = border */
        sy + border,               /* top    = border */
        sw - 2*border,             /* width  = full inner width  */
        sh - 2*border              /* height = full inner height */
    };
    Rect effective_text;
    if (has_parent_clip) {
        if (!rect_intersect(&parent_clip, &text_clip, &effective_text))
            return;
    } else {
        effective_text = text_clip;
    }
    if (effective_text.w <= 0 || effective_text.h <= 0) return;
    Rect text_r = { effective_text.x, effective_text.y, effective_text.w, effective_text.h };
    clip_begin(&e->parent->base, &text_r);

    /* ---------- TEXT DRAWING AREA (inside padding) ---------- */
    int text_x = sx + border + pad;
    int text_y = sy + (sh - font_h) / 2;               /* vertical centre */

    const char* display = (e->is_active || e->text[0])
                          ? e->text + e->visible_text_start
                          : e->place_holder;
    Color txt_col = (display == e->place_holder) ? current_theme->text_secondary
                                                 : current_theme->text_primary;

    /* ---------- SELECTION ---------- */
    if (e->selection_start != -1 && e->is_active) {
        int sel_start = (e->selection_start < e->cursor_pos ? e->selection_start : e->cursor_pos)
                        - e->visible_text_start;
        int sel_end   = (e->selection_start < e->cursor_pos ? e->cursor_pos : e->selection_start)
                        - e->visible_text_start;

        if (sel_start < 0) sel_start = 0;
        if (sel_end   > (int)strlen(display)) sel_end = (int)strlen(display);
        if (sel_start < sel_end) {
            char tmp[256];
            int offset = 0, w = 0;

            /* left part */
            strncpy(tmp, display, sel_start); tmp[sel_start] = '\0';
            measure_utf8(tmp, &offset, NULL);

            /* selected part */
            strncpy(tmp, display + sel_start, sel_end - sel_start);
            tmp[sel_end - sel_start] = '\0';
            measure_utf8(tmp, &w, NULL);

            draw_rect(&e->parent->base,
                      text_x + offset, text_y,
                      w, font_h,
                      current_theme->accent_hovered);
        }
    }

    /* ---------- TEXT ---------- */
    draw_text_from_font(&e->parent->base,
                        global_font, display,
                        text_x, text_y,
                        txt_col, ALIGN_LEFT);

    /* ---------- CURSOR ---------- */
    if (e->is_active) {
        int cur_off = 0;
        if (e->cursor_pos - e->visible_text_start > 0) {
            char tmp[256];
            int len = e->cursor_pos - e->visible_text_start;
            strncpy(tmp, display, len); tmp[len] = '\0';
            measure_utf8(tmp, &cur_off, NULL);
        }
        int cur_w = (int)roundf(2 * dpi);
        draw_rect(&e->parent->base,
                  text_x + cur_off, text_y,
                  cur_w, font_h,
                  current_theme->accent);
    }

    clip_end(&e->parent->base);
}

/* --------------------------------------------------------------------- */
/* update_visible_text – uses wrapper size functions, fixed scrolling */
/* --------------------------------------------------------------------- */
void update_visible_text(Entry* e)
{
    if (!e || !e->parent || !global_font) return;
    if (!current_theme) current_theme = (Theme*)&THEME_LIGHT;

    int logical_padding = current_theme->padding;
    int max_vis_w = e->w - 2 * logical_padding;   /* logical pixels */

    /* ---- how many characters fit in the visible area? ---- */
    int fit_w = 0, fit_chars = 0;
    const char* txt = e->text;
    for (int i = 0; i < (int)strlen(txt); ++i) {
        char ch[2] = { txt[i], '\0' };
        int cw = 0;
        if (measure_utf8(ch, &cw, NULL) != 0) break;
        if (fit_w + cw > max_vis_w) break;
        fit_w += cw;
        ++fit_chars;
    }

    /* ---- cursor pixel position (logical) ---- */
    int cursor_px = 0;
    if (e->cursor_pos > 0) {
        char tmp[256];
        int len = e->cursor_pos;
        strncpy(tmp, txt, len); tmp[len] = '\0';
        measure_utf8(tmp, &cursor_px, NULL);
    }

    /* ---- scroll right ---- */
    if (cursor_px > max_vis_w) {
        while (cursor_px > max_vis_w && e->visible_text_start < (int)strlen(txt)) {
            char ch[2] = { txt[e->visible_text_start], '\0' };
            int cw = 0;
            measure_utf8(ch, &cw, NULL);
            cursor_px -= cw;
            ++e->visible_text_start;
        }
    }
    /* ---- scroll left ---- */
    else if (e->cursor_pos < e->visible_text_start) {
        e->visible_text_start = e->cursor_pos;
    }

    /* ---- keep selection visible (optional) ---- */
    if (e->selection_start != -1) {
        int sel_end = (e->selection_start > e->cursor_pos) ? e->selection_start : e->cursor_pos;
        if (sel_end < e->visible_text_start) {
            e->visible_text_start = (sel_end > fit_chars) ? sel_end - fit_chars : 0;
        }
    }

    /* ---- clamp ---- */
    int max_start = (int)strlen(txt) - fit_chars;
    if (max_start < 0) max_start = 0;
    if (e->visible_text_start > max_start) e->visible_text_start = max_start;
}

/* --------------------------------------------------------------------- */
/* update_entry – same logic, only size calls go through the wrapper */
/* --------------------------------------------------------------------- */
void update_entry(Entry* e, Event* ev)
{
    if (!e || !e->parent || !e->parent->is_open || !global_font) return;

    float dpi = e->parent->base.dpi_scale;
    Uint16 mod = SDL_GetModState();

    int abs_x = e->x + e->parent->x;
    int abs_y = e->y + e->parent->y + e->parent->title_height;
    int sx = (int)roundf(abs_x * dpi);
    int sy = (int)roundf(abs_y * dpi);
    int sw = (int)roundf(e->w * dpi);
    int sh = (int)roundf(e->h * dpi);
    int pad = (int)roundf(current_theme->padding * dpi);

    /* ---------- MOUSE DOWN ---------- */
    if (ev->type == EVENT_MOUSEBUTTONDOWN && ev->mouseButton.button == MOUSE_LEFT) {
        int mx = ev->mouseButton.x;
        int my = ev->mouseButton.y;
        if (mx >= sx && mx <= sx+sw && my >= sy && my <= sy+sh) {
            e->is_active = 1;
            e->is_mouse_selecting = 1;
            e->selection_start = -1;

            int logical_mx = (int)roundf(mx / dpi);
            int click_off  = logical_mx - (abs_x + current_theme->padding);
            int cum = 0, pos = 0;
            for (int i = 0; i < (int)strlen(e->text); ++i) {
                char ch[2] = { e->text[i], '\0' };
                int cw = 0;
                measure_utf8(ch, &cw, NULL);
                if (cum + cw/2 > click_off) break;
                cum += cw;
                pos = i+1;
            }
            e->cursor_pos = pos;
            update_visible_text(e);
        } else {
            e->is_active = 0;
            e->is_mouse_selecting = 0;
            e->selection_start = -1;
        }
    }
    /* ---------- MOUSE UP ---------- */
    else if (ev->type == EVENT_MOUSEBUTTONUP && ev->mouseButton.button == MOUSE_LEFT) {
        e->is_mouse_selecting = 0;
    }
    /* ---------- MOUSE DRAG ---------- */
    else if (ev->type == EVENT_MOUSEMOTION && e->is_mouse_selecting &&
             (ev->mouseMove.button_state & MOUSE_BUTTON_LEFT_MASK)) {
        int mx = ev->mouseMove.x;
        int my = ev->mouseMove.y;
        if (mx >= sx && mx <= sx+sw && my >= sy && my <= sy+sh) {
            if (e->selection_start == -1) e->selection_start = e->cursor_pos;

            int logical_mx = (int)roundf(mx / dpi);
            int click_off  = logical_mx - (abs_x + current_theme->padding);
            int cum = 0, pos = 0;
            for (int i = 0; i < (int)strlen(e->text); ++i) {
                char ch[2] = { e->text[i], '\0' };
                int cw = 0;
                measure_utf8(ch, &cw, NULL);
                if (cum + cw/2 > click_off) break;
                cum += cw;
                pos = i+1;
            }
            e->cursor_pos = pos;
            update_visible_text(e);
        }
    }
    /* ---------- TEXT INPUT ---------- */
    else if (ev->type == EVENT_TEXTINPUT && e->is_active) {
        if (e->selection_start != -1) {
            int start = (e->selection_start < e->cursor_pos) ? e->selection_start : e->cursor_pos;
            int end   = (e->selection_start < e->cursor_pos) ? e->cursor_pos : e->selection_start;
            memmove(e->text + start, e->text + end, strlen(e->text) - end + 1);
            e->cursor_pos = start;
            e->selection_start = -1;
        }
        int len = strlen(e->text);
        int add = strlen(ev->text.text);
        if (len + add < e->max_length) {
            memmove(e->text + e->cursor_pos + add,
                    e->text + e->cursor_pos,
                    len - e->cursor_pos + 1);
            memcpy(e->text + e->cursor_pos, ev->text.text, add);
            e->cursor_pos += add;
            update_visible_text(e);
        }
    }
    /* ---------- KEYBOARD ---------- */
    else if (ev->type == EVENT_KEYDOWN && e->is_active) {
        #define DEL_SEL() do { \
            int s = (e->selection_start < e->cursor_pos) ? e->selection_start : e->cursor_pos; \
            int d = (e->selection_start < e->cursor_pos) ? e->cursor_pos : e->selection_start; \
            memmove(e->text + s, e->text + d, strlen(e->text) - d + 1); \
            e->cursor_pos = s; e->selection_start = -1; \
        } while(0)

        if (ev->key.key == KEY_BACKSPACE) {
            if (e->selection_start != -1) { DEL_SEL(); }
            else if (e->cursor_pos > 0) {
                memmove(e->text + e->cursor_pos - 1,
                        e->text + e->cursor_pos,
                        strlen(e->text) - e->cursor_pos + 1);
                --e->cursor_pos;
            }
            update_visible_text(e);
        }
        else if (ev->key.key == KEY_DELETE) {
            if (e->selection_start != -1) { DEL_SEL(); }
            else if (e->cursor_pos < (int)strlen(e->text)) {
                memmove(e->text + e->cursor_pos,
                        e->text + e->cursor_pos + 1,
                        strlen(e->text) - e->cursor_pos);
            }
            update_visible_text(e);
        }
        else if (ev->key.key == KEY_LEFT) {
            if (e->cursor_pos > 0) {
                if (mod & KMOD_SHIFT) {
                    if (e->selection_start == -1) e->selection_start = e->cursor_pos;
                    --e->cursor_pos;
                } else {
                    --e->cursor_pos;
                    e->selection_start = -1;
                }
                update_visible_text(e);
            }
        }
        else if (ev->key.key == KEY_RIGHT) {
            if (e->cursor_pos < (int)strlen(e->text)) {
                if (mod & KMOD_SHIFT) {
                    if (e->selection_start == -1) e->selection_start = e->cursor_pos;
                    ++e->cursor_pos;
                } else {
                    ++e->cursor_pos;
                    e->selection_start = -1;
                }
                update_visible_text(e);
            }
        }
        else if (ev->key.key == KEY_RETURN) {
            e->is_active = 0;
            e->selection_start = -1;
        }
        else if (ev->key.key == KEY_A && (ev->key.mod & KEY_MOD_CTRL)) {
            if (e->text[0]) {
                e->selection_start = 0;
                e->cursor_pos = (int)strlen(e->text);
            }
        }
        else if (ev->key.key == KEY_C && (ev->key.mod & KEY_MOD_CTRL)) {
            if (e->selection_start != -1) {
                int s   = (e->selection_start < e->cursor_pos) ? e->selection_start : e->cursor_pos;
                int len = abs(e->cursor_pos - e->selection_start);
                char* sel = malloc(len + 1);
                if (sel) {
                    strncpy(sel, e->text + s, len); sel[len] = '\0';
                    clipboard_set_text(sel);
                    free(sel);
                }
            }
        }
        else if (ev->key.key == KEY_X && (ev->key.mod & KEY_MOD_CTRL)) {
            if (e->selection_start != -1) {
                int s   = (e->selection_start < e->cursor_pos) ? e->selection_start : e->cursor_pos;
                int len = abs(e->cursor_pos - e->selection_start);
                char* sel = malloc(len + 1);
                if (sel) {
                    strncpy(sel, e->text + s, len); sel[len] = '\0';
                    clipboard_set_text(sel);
                    free(sel);
                }
                DEL_SEL();
                update_visible_text(e);
            }
        }
        else if (ev->key.key == KEY_V && (ev->key.mod & KEY_MOD_CTRL)) {
            if (clipboard_has_text()) {
                char* paste = clipboard_get_text();
                if (paste) {
                    int plen = strlen(paste);
                    int len  = strlen(e->text);
                    if (e->selection_start != -1) { DEL_SEL(); len = strlen(e->text); }
                    if (len + plen < e->max_length) {
                        memmove(e->text + e->cursor_pos + plen,
                                e->text + e->cursor_pos,
                                len - e->cursor_pos + 1);
                        memcpy(e->text + e->cursor_pos, paste, plen);
                        e->cursor_pos += plen;
                        update_visible_text(e);
                    }
                    clipboard_free(paste);
                }
            }
        }
        #undef DEL_SEL
    }
}

/* --------------------------------------------------------------------- */
void free_entry(Entry* e)
{
    if (e) {
        free(e->text);
        free(e->place_holder);
    }
}

/* --------------------------------------------------------------------- */
/* Global registration (unchanged) */
/* --------------------------------------------------------------------- */
Entry* entry_widgets[MAX_ENTRYS];
int entrys_count = 0;

void register_entry(Entry* e) {
    if (entrys_count < MAX_ENTRYS) entry_widgets[entrys_count++] = e;
}
void render_all_registered_entrys(void) {
    for (int i = 0; i < entrys_count; ++i)
        if (entry_widgets[i]) render_entry(entry_widgets[i]);
}
void update_all_registered_entrys(Event* ev) {
    for (int i = 0; i < entrys_count; ++i)
        if (entry_widgets[i]) update_entry(entry_widgets[i], ev);
}
void free_all_registered_entrys(void) {
    for (int i = 0; i < entrys_count; ++i) {
        if (entry_widgets[i]) { free_entry(entry_widgets[i]); entry_widgets[i] = NULL; }
    }
    entrys_count = 0;
}
