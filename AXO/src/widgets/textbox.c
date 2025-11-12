#include "../../include/widgets/textbox.h"
#include "../../include/core/theme.h"
#include "../../include/core/color.h"
#include "../../include/core/graphics.h"
#include "../../include/core/app.h"
#include "../../include/core/parent.h"   // for Rect
#include "../../include/core/backends/sdl2/clip.h"     // for clip_begin/end
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Forward declaration for line computation using wrapper
Line* compute_visual_lines(const char* text, int max_width, Font_ttf* font, int* num_lines);

TextBox new_textbox(Parent* parent, int x, int y, int w, int max_length) {
    if (!parent || !parent->base.sdl_renderer) {
        printf("Invalid parent or renderer\n");
    }
    if (!current_theme) {
        current_theme = (Theme*)&THEME_LIGHT;
    }

    TextBox new_textbox;
    new_textbox.parent = parent;
    new_textbox.place_holder = strdup(" ");
    if (!new_textbox.place_holder) {
        printf("Failed to allocate memory for placeholder\n");
    }
    new_textbox.x = x;
    new_textbox.y = y;
    new_textbox.w = w;
    new_textbox.h = 10 * (current_theme->default_font_size + current_theme->padding / 2);
    new_textbox.max_length = max_length;
    new_textbox.text = (char*)malloc(max_length + 1);
    if (!new_textbox.text) {
        printf("Failed to allocate memory for textbox text\n");
        free(new_textbox.place_holder);
    }
    new_textbox.text[0] = '\0';
    new_textbox.is_active = 0;
    new_textbox.cursor_pos = 0;
    new_textbox.selection_start = -1;
    new_textbox.visible_line_start = 0;
    new_textbox.is_mouse_selecting = 0;

    // Use global_font (already DPI-scaled)
    if (global_font) {
        new_textbox.line_height = ttf_line_skip(global_font);
        if (new_textbox.line_height <= 0) {
            new_textbox.line_height = (int)roundf(current_theme->default_font_size * parent->base.dpi_scale) + current_theme->padding / 2;
        }
    } else {
        new_textbox.line_height = (int)roundf(current_theme->default_font_size * parent->base.dpi_scale) + current_theme->padding / 2;
    }

    return new_textbox;
}

void render_textbox(TextBox* textbox) {
    if (!textbox || !textbox->parent || !textbox->parent->base.sdl_renderer || !textbox->parent->is_open) {
        printf("Invalid textbox, renderer, or parent is not open\n");
        return;
    }

    float dpi = textbox->parent->base.dpi_scale;

    // Parent clip
    Rect parent_clip = {0,0,0,0};
    int has_parent_clip = 0;
    if (!textbox->parent->is_window) {
        Rect pr = get_parent_rect(textbox->parent);
        parent_clip.x = (int)roundf(pr.x * dpi);
        parent_clip.y = (int)roundf(pr.y * dpi);
        parent_clip.w = (int)roundf(pr.w * dpi);
        parent_clip.h = (int)roundf(pr.h * dpi);
        has_parent_clip = 1;
    }

    // Physical bounds
    int abs_x = textbox->x + textbox->parent->x;
    int abs_y = textbox->y + textbox->parent->y + textbox->parent->title_height;
    int sx = (int)roundf(abs_x * dpi);
    int sy = (int)roundf(abs_y * dpi);
    int sw = (int)roundf(textbox->w * dpi);
    int sh = (int)roundf(textbox->h * dpi);
    int border_width = (int)roundf(2 * dpi);
    int padding = (int)roundf(current_theme->padding * dpi);

    // Use global_font
    Font_ttf* font = global_font;
    if (!font) {
        printf("global_font is NULL — cannot render textbox\n");
        return;
    }

    int font_height = ttf_line_skip(font);

    if (has_parent_clip) {
        clip_begin(&textbox->parent->base, &parent_clip);
    }

    // Background & border
    draw_rect(&textbox->parent->base, sx, sy, sw, sh, current_theme->accent);
    draw_rect(&textbox->parent->base,
               sx + border_width, sy + border_width,
               sw - 2*border_width, sh - 2*border_width,
               current_theme->bg_secondary);

    if (has_parent_clip) {
        clip_end(&textbox->parent->base);
    }

    // Text clip
    Rect text_clip = {
        sx + border_width + padding,
        sy + border_width + padding,
        sw - 2*(border_width + padding),
        sh - 2*(border_width + padding)
    };
    Rect effective_text;
    if (has_parent_clip) {
        if (!rect_intersect(&parent_clip, &text_clip, &effective_text)) {
            return;
        }
    } else {
        effective_text = text_clip;
    }
    if (effective_text.w <= 0 || effective_text.h <= 0) {
        return;
    }
    clip_begin(&textbox->parent->base, &effective_text);

    // Text to display
    char* display_text = (textbox->is_active || textbox->text[0] != '\0') ? textbox->text : textbox->place_holder;
    Color text_color = (display_text == textbox->place_holder) ? current_theme->text_secondary : current_theme->text_primary;

    int text_x = sx + border_width + padding;
    int text_y = sy + border_width + padding;
    int max_text_width = sw - 2*(border_width + padding);

    int num_lines = 0;
    Line* lines = compute_visual_lines(display_text, max_text_width, font, &num_lines);
    int visible_lines_count = (sh - 2*(border_width + padding)) / font_height;

    // Selection
    int sel_min = -1, sel_max = -1;
    if (textbox->selection_start != -1 && textbox->is_active) {
        sel_min = textbox->selection_start < textbox->cursor_pos ? textbox->selection_start : textbox->cursor_pos;
        sel_max = textbox->selection_start < textbox->cursor_pos ? textbox->cursor_pos : textbox->selection_start;
    }

    // Render visible lines
    for (int i = textbox->visible_line_start;
         i < num_lines && i < textbox->visible_line_start + visible_lines_count;
         ++i) {
        Line l = lines[i];
        char* line_text = (char*)malloc(l.len + 1);
        if (!line_text) continue;
        strncpy(line_text, display_text + l.start, l.len);
        line_text[l.len] = '\0';

        int draw_y = text_y + (i - textbox->visible_line_start) * font_height;

        // Selection highlight
        if (sel_min != -1) {
            int line_start = l.start;
            int line_end = l.start + l.len;
            if (sel_max > line_start && sel_min < line_end) {
                int ov_start = sel_min > line_start ? sel_min : line_start;
                int ov_end = sel_max < line_end ? sel_max : line_end;

                char* prefix = (char*)malloc((ov_start - line_start) + 1);
                if (prefix) {
                    strncpy(prefix, display_text + line_start, ov_start - line_start);
                    prefix[ov_start - line_start] = '\0';
                    int offset = ttf_text_width(font, prefix);
                    free(prefix);

                    char* sel_part = (char*)malloc((ov_end - ov_start) + 1);
                    if (sel_part) {
                        strncpy(sel_part, display_text + ov_start, ov_end - ov_start);
                        sel_part[ov_end - ov_start] = '\0';
                        int w = ttf_text_width(font, sel_part);
                        free(sel_part);

                        draw_rect(&textbox->parent->base, text_x + offset, draw_y, w, font_height, current_theme->accent_hovered);
                    }
                }
            }
        }

        // Draw text using wrapper
        draw_text_from_font(&textbox->parent->base, font, line_text, text_x, draw_y, text_color, ALIGN_LEFT);
        free(line_text);
    }

    // Cursor
    if (textbox->is_active && font) {
        for (int i = 0; i < num_lines; ++i) {
            Line l = lines[i];
            if (textbox->cursor_pos >= l.start && textbox->cursor_pos <= l.start + l.len) {
                if (i < textbox->visible_line_start || i >= textbox->visible_line_start + visible_lines_count)
                    break;

                int rel_line = i - textbox->visible_line_start;
                int draw_y = text_y + rel_line * font_height;

                char* prefix = (char*)malloc((textbox->cursor_pos - l.start) + 1);
                if (prefix) {
                    strncpy(prefix, display_text + l.start, textbox->cursor_pos - l.start);
                    prefix[textbox->cursor_pos - l.start] = '\0';
                    int cursor_offset = ttf_text_width(font, prefix);
                    free(prefix);

                    int cursor_x = text_x + cursor_offset;
                    int cursor_w = (int)roundf(2 * dpi);
                    draw_rect(&textbox->parent->base, cursor_x, draw_y, cursor_w, font_height, current_theme->accent);
                }
                break;
            }
        }
    }

    free(lines);

    clip_end(&textbox->parent->base);
}

void update_visible_lines(TextBox* textbox) {
    if (!textbox || !textbox->parent || !global_font) return;

    float dpi = textbox->parent->base.dpi_scale;
    int logical_padding = current_theme->padding;
    int max_text_width = textbox->w - 2 * logical_padding;

    int num_lines = 0;
    Line* lines = compute_visual_lines(textbox->text, max_text_width, global_font, &num_lines);
    int visible_lines = (textbox->h - 2 * logical_padding) / textbox->line_height;

    int cursor_line = -1;
    for (int i = 0; i < num_lines; i++) {
        Line l = lines[i];
        if (textbox->cursor_pos >= l.start && textbox->cursor_pos <= l.start + l.len) {
            cursor_line = i;
            break;
        }
    }

    if (cursor_line != -1) {
        if (cursor_line < textbox->visible_line_start) {
            textbox->visible_line_start = cursor_line;
        } else if (cursor_line >= textbox->visible_line_start + visible_lines) {
            textbox->visible_line_start = cursor_line - visible_lines + 1;
        }
    }
    if (textbox->visible_line_start > num_lines - visible_lines && num_lines > visible_lines) {
        textbox->visible_line_start = num_lines - visible_lines;
    }
    if (textbox->visible_line_start < 0) textbox->visible_line_start = 0;

    free(lines);
}

void update_textbox(TextBox* textbox, Event* event) {
    if (!textbox || !textbox->parent || !textbox->parent->is_open || !global_font) return;

    float dpi = textbox->parent->base.dpi_scale;
    Uint16 mod = input_get_mod_state();
    int abs_x = textbox->x + textbox->parent->x;
    int abs_y = textbox->y + textbox->parent->y + textbox->parent->title_height;
    int s_abs_x = (int)roundf(abs_x * dpi);
    int s_abs_y = (int)roundf(abs_y * dpi);
    int s_w = (int)roundf(textbox->w * dpi);
    int s_h = (int)roundf(textbox->h * dpi);
    int logical_padding = current_theme->padding;

    Font_ttf* font = global_font;

    if (event->type == EVENT_MOUSEBUTTONDOWN && event->mouseButton.button == MOUSE_LEFT) {
        int mouseX = event->mouseButton.x;
        int mouseY = event->mouseButton.y;
        if (mouseX >= s_abs_x && mouseX <= s_abs_x + s_w && mouseY >= s_abs_y && mouseY <= s_abs_y + s_h) {
            textbox->is_active = 1;
            textbox->is_mouse_selecting = 1;
            textbox->selection_start = -1;

            int logical_mouse_x = (int)roundf(mouseX / dpi);
            int logical_mouse_y = (int)roundf(mouseY / dpi);
            int click_y = logical_mouse_y - (abs_y + logical_padding);
            int clicked_line = textbox->visible_line_start + click_y / textbox->line_height;
            int max_text_width = textbox->w - 2 * logical_padding;

            int num_lines = 0;
            Line* lines = compute_visual_lines(textbox->text, max_text_width, font, &num_lines);
            if (clicked_line < num_lines) {
                Line l = lines[clicked_line];
                int click_x = logical_mouse_x - (abs_x + logical_padding);
                int cum_width = 0;
                textbox->cursor_pos = l.start;
                for (int j = 0; j < l.len; j++) {
                    char ch[2] = {textbox->text[l.start + j], '\0'};
                    int char_w = ttf_text_width(font, ch);
                    if (cum_width + char_w / 2 > click_x) break;
                    cum_width += char_w;
                    textbox->cursor_pos = l.start + j + 1;
                }
            } else {
                textbox->cursor_pos = strlen(textbox->text);
            }
            free(lines);
            update_visible_lines(textbox);
        } else {
            textbox->is_active = 0;
            textbox->is_mouse_selecting = 0;
            textbox->selection_start = -1;
        }
    }
    else if (event->type == EVENT_MOUSEBUTTONUP && event->mouseButton.button == MOUSE_LEFT) {
        textbox->is_mouse_selecting = 0;
    }
    else if (event->type == EVENT_MOUSEMOTION && textbox->is_mouse_selecting && (event->mouseMove.button_state & MOUSE_BUTTON_LEFT_MASK)) {
        int mouseX = event->mouseMove.x;
        int mouseY = event->mouseMove.y;
        if (mouseX >= s_abs_x && mouseX <= s_abs_x + s_w && mouseY >= s_abs_y && mouseY <= s_abs_y + s_h) {
            if (textbox->selection_start == -1) textbox->selection_start = textbox->cursor_pos;

            int logical_mouse_x = (int)roundf(mouseX / dpi);
            int logical_mouse_y = (int)roundf(mouseY / dpi);
            int click_y = logical_mouse_y - (abs_y + logical_padding);
            int clicked_line = textbox->visible_line_start + click_y / textbox->line_height;
            int max_text_width = textbox->w - 2 * logical_padding;

            int num_lines = 0;
            Line* lines = compute_visual_lines(textbox->text, max_text_width, font, &num_lines);
            if (clicked_line < num_lines) {
                Line l = lines[clicked_line];
                int click_x = logical_mouse_x - (abs_x + logical_padding);
                int cum_width = 0;
                textbox->cursor_pos = l.start;
                for (int j = 0; j < l.len; j++) {
                    char ch[2] = {textbox->text[l.start + j], '\0'};
                    int char_w = ttf_text_width(font, ch);
                    if (cum_width + char_w / 2 > click_x) break;
                    cum_width += char_w;
                    textbox->cursor_pos = l.start + j + 1;
                }
            } else {
                textbox->cursor_pos = strlen(textbox->text);
            }
            free(lines);
            update_visible_lines(textbox);
        }
    }
    else if (event->type == EVENT_TEXTINPUT && textbox->is_active) {
        if (textbox->selection_start != -1) {
            int sel_start = textbox->selection_start < textbox->cursor_pos ? textbox->selection_start : textbox->cursor_pos;
            int sel_end = textbox->selection_start < textbox->cursor_pos ? textbox->cursor_pos : textbox->selection_start;
            memmove(textbox->text + sel_start, textbox->text + sel_end, strlen(textbox->text) - sel_end + 1);
            textbox->cursor_pos = sel_start;
            textbox->selection_start = -1;
        }
        int len = strlen(textbox->text);
        int input_len = strlen(event->text.text);
        if (len + input_len < textbox->max_length) {
            memmove(textbox->text + textbox->cursor_pos + input_len, textbox->text + textbox->cursor_pos, len - textbox->cursor_pos + 1);
            strncpy(textbox->text + textbox->cursor_pos, event->text.text, input_len);
            textbox->cursor_pos += input_len;
            update_visible_lines(textbox);
        }
    }
    else if (event->type == EVENT_KEYDOWN && textbox->is_active) {
        // All key handling uses global_font via compute_visual_lines and update_visible_lines
        // (No direct TTF calls — all through wrapper)
        if (event->key.key == KEY_BACKSPACE) {
            if (textbox->selection_start != -1) {
                int sel_start = textbox->selection_start < textbox->cursor_pos ? textbox->selection_start : textbox->cursor_pos;
                int sel_end = textbox->selection_start < textbox->cursor_pos ? textbox->cursor_pos : textbox->selection_start;
                memmove(textbox->text + sel_start, textbox->text + sel_end, strlen(textbox->text) - sel_end + 1);
                textbox->cursor_pos = sel_start;
                textbox->selection_start = -1;
            } else if (textbox->cursor_pos > 0) {
                memmove(textbox->text + textbox->cursor_pos - 1, textbox->text + textbox->cursor_pos, strlen(textbox->text) - textbox->cursor_pos + 1);
                textbox->cursor_pos--;
            }
            update_visible_lines(textbox);
        }
        else if (event->key.key == KEY_DELETE) {
            if (textbox->selection_start != -1) {
                int sel_start = textbox->selection_start < textbox->cursor_pos ? textbox->selection_start : textbox->cursor_pos;
                int sel_end = textbox->selection_start < textbox->cursor_pos ? textbox->cursor_pos : textbox->selection_start;
                memmove(textbox->text + sel_start, textbox->text + sel_end, strlen(textbox->text) - sel_end + 1);
                textbox->cursor_pos = sel_start;
                textbox->selection_start = -1;
            } else if (textbox->cursor_pos < strlen(textbox->text)) {
                memmove(textbox->text + textbox->cursor_pos, textbox->text + textbox->cursor_pos + 1, strlen(textbox->text) - textbox->cursor_pos);
            }
            update_visible_lines(textbox);
        }
        else if (event->key.key == KEY_LEFT || event->key.key == KEY_RIGHT) {
            int delta = (event->key.key == KEY_RIGHT) ? 1 : -1;
            if (mod & KMOD_SHIFT) {
                if (textbox->selection_start == -1) textbox->selection_start = textbox->cursor_pos;
                textbox->cursor_pos = fmax(0, fmin((int)strlen(textbox->text), textbox->cursor_pos + delta));
            } else {
                textbox->cursor_pos = fmax(0, fmin((int)strlen(textbox->text), textbox->cursor_pos + delta));
                textbox->selection_start = -1;
            }
            update_visible_lines(textbox);
        }
        else if (event->key.key == KEY_UP || event->key.key == KEY_DOWN) {
            int max_text_width = textbox->w - 2 * logical_padding;
            int num_lines = 0;
            Line* lines = compute_visual_lines(textbox->text, max_text_width, font, &num_lines);

            int curr_line_idx = -1;
            int curr_offset_chars = 0;
            int old_cursor_pos = textbox->cursor_pos;
            for (int i = 0; i < num_lines; i++) {
                Line l = lines[i];
                if (textbox->cursor_pos >= l.start && textbox->cursor_pos <= l.start + l.len) {
                    curr_line_idx = i;
                    curr_offset_chars = textbox->cursor_pos - l.start;
                    break;
                }
            }

            if (curr_line_idx != -1) {
                char* temp = (char*)malloc(curr_offset_chars + 1);
                if (temp) {
                    strncpy(temp, textbox->text + lines[curr_line_idx].start, curr_offset_chars);
                    temp[curr_offset_chars] = '\0';
                    int preferred_width = ttf_text_width(font, temp);
                    free(temp);

                    int delta = (event->key.key == KEY_DOWN) ? 1 : -1;
                    int target_line_idx = curr_line_idx + delta;
                    if (target_line_idx >= 0 && target_line_idx < num_lines) {
                        Line target_l = lines[target_line_idx];
                        int accum_w = 0;
                        int target_offset = 0;
                        for (int j = 0; j < target_l.len; j++) {
                            char ch[2] = {textbox->text[target_l.start + j], '\0'};
                            int char_w = ttf_text_width(font, ch);
                            if (accum_w + char_w / 2 > preferred_width) break;
                            accum_w += char_w;
                            target_offset++;
                        }
                        if (accum_w < preferred_width) target_offset = target_l.len;
                        textbox->cursor_pos = target_l.start + target_offset;
                    } else if (target_line_idx < 0) {
                        textbox->cursor_pos = 0;
                    } else {
                        textbox->cursor_pos = strlen(textbox->text);
                    }
                }
            }
            free(lines);
            if (mod & KMOD_SHIFT) {
                if (textbox->selection_start == -1) textbox->selection_start = old_cursor_pos;
            } else {
                textbox->selection_start = -1;
            }
            update_visible_lines(textbox);
        }
        else if (event->key.key == KEY_RETURN) {
            int len = strlen(textbox->text);
            if (len + 1 < textbox->max_length) {
                memmove(textbox->text + textbox->cursor_pos + 1, textbox->text + textbox->cursor_pos, len - textbox->cursor_pos + 1);
                textbox->text[textbox->cursor_pos] = '\n';
                textbox->cursor_pos++;
                update_visible_lines(textbox);
            }
        }
        // Ctrl+A, C, X, V unchanged — no font calls
        else if (event->key.key == KEY_A && (event->key.mod & KEY_MOD_CTRL)) {
            if (strlen(textbox->text) > 0) {
                textbox->selection_start = 0;
                textbox->cursor_pos = strlen(textbox->text);
            }
        }
        else if (event->key.key == KEY_C && (event->key.mod & KEY_MOD_CTRL)) {
            if (textbox->selection_start != -1) {
                int sel_start = textbox->selection_start < textbox->cursor_pos ? textbox->selection_start : textbox->cursor_pos;
                int sel_len = abs(textbox->cursor_pos - textbox->selection_start);
                char* sel_text = malloc(sel_len + 1);
                if (sel_text) {
                    strncpy(sel_text, textbox->text + sel_start, sel_len);
                    sel_text[sel_len] = '\0';
                    clipboard_set_text(sel_text);
                    free(sel_text);
                }
            }
        }
        else if (event->key.key == KEY_X && (event->key.mod & KEY_MOD_CTRL)) {
            if (textbox->selection_start != -1) {
                int sel_start = textbox->selection_start < textbox->cursor_pos ? textbox->selection_start : textbox->cursor_pos;
                int sel_len = abs(textbox->cursor_pos - textbox->selection_start);
                char* sel_text = malloc(sel_len + 1);
                if (sel_text) {
                    strncpy(sel_text, textbox->text + sel_start, sel_len);
                    sel_text[sel_len] = '\0';
                    clipboard_set_text(sel_text);
                    free(sel_text);
                }
                memmove(textbox->text + sel_start, textbox->text + sel_start + sel_len, strlen(textbox->text) - (sel_start + sel_len) + 1);
                textbox->cursor_pos = sel_start;
                textbox->selection_start = -1;
                update_visible_lines(textbox);
            }
        }
        else if (event->key.key == KEY_V && (event->key.mod & KEY_MOD_CTRL)) {
            if (clipboard_has_text()) {
                char* paste_text = clipboard_get_text();
                if (paste_text) {
                    int paste_len = strlen(paste_text);
                    int len = strlen(textbox->text);
                    if (textbox->selection_start != -1) {
                        int sel_start = textbox->selection_start < textbox->cursor_pos ? textbox->selection_start : textbox->cursor_pos;
                        int sel_len = abs(textbox->cursor_pos - textbox->selection_start);
                        memmove(textbox->text + sel_start, textbox->text + sel_start + sel_len, len - (sel_start + sel_len) + 1);
                        textbox->cursor_pos = sel_start;
                        textbox->selection_start = -1;
                        len -= sel_len;
                    }
                    if (len + paste_len < textbox->max_length) {
                        memmove(textbox->text + textbox->cursor_pos + paste_len, textbox->text + textbox->cursor_pos, len - textbox->cursor_pos + 1);
                        strncpy(textbox->text + textbox->cursor_pos, paste_text, paste_len);
                        textbox->cursor_pos += paste_len;
                        update_visible_lines(textbox);
                    }
                    clipboard_free(paste_text);
                }
            }
        }
    }
}

// compute_visual_lines using wrapper
Line* compute_visual_lines(const char* text, int max_width, Font_ttf* font, int* num_lines) {
    *num_lines = 0;
    if (!text || !font) return NULL;

    int text_len = strlen(text);
    int max_lines = text_len * 2 + 2;
    Line* lines = (Line*)malloc(sizeof(Line) * max_lines);
    if (!lines) return NULL;

    int line_start = 0;
    for (int pos = 0; pos <= text_len; pos++) {
        if (pos == text_len || text[pos] == '\n') {
            int seg_start = line_start;
            int seg_len = pos - line_start;
            int seg_pos = 0;
            while (seg_pos < seg_len) {
                int line_start_local = seg_pos;
                int last_space = -1;
                int current_width = 0;
                while (seg_pos < seg_len) {
                    char ch[2] = {text[seg_start + seg_pos], '\0'};
                    int char_w = ttf_text_width(font, ch);
                    if (current_width + char_w > max_width) {
                        if (current_width == 0) {
                            current_width += char_w;
                            if (isspace(ch[0])) last_space = seg_pos;
                            seg_pos++;
                            continue;
                        }
                        int add_len = (last_space != -1) ? (last_space - line_start_local) : (seg_pos - line_start_local);
                        if (add_len > 0) {
                            lines[*num_lines].start = seg_start + line_start_local;
                            lines[*num_lines].len = add_len;
                            (*num_lines)++;
                        }
                        line_start_local = seg_pos = last_space != -1 ? last_space + 1 : seg_pos;
                        current_width = 0;
                        last_space = -1;
                        continue;
                    }
                    current_width += char_w;
                    if (isspace(text[seg_start + seg_pos])) last_space = seg_pos;
                    seg_pos++;
                }
                int add_len = seg_pos - line_start_local;
                if (add_len > 0 || (add_len == 0 && seg_len == 0)) {
                    lines[*num_lines].start = seg_start + line_start_local;
                    lines[*num_lines].len = add_len;
                    (*num_lines)++;
                }
            }
            if (seg_len == 0) {
                lines[*num_lines].start = seg_start;
                lines[*num_lines].len = 0;
                (*num_lines)++;
            }
            line_start = pos + 1;
        }
    }
    return lines;
}

// Registration functions unchanged
TextBox* textbox_widgets[MAX_TEXTBOXS];
int textboxs_count = 0;
void register_textbox(TextBox* textbox) {
    if (textboxs_count < MAX_TEXTBOXS) {
        textbox_widgets[textboxs_count++] = textbox;
    }
}
void render_all_registered_textboxs(void) {
    for (int i = 0; i < textboxs_count; i++) {
        if (textbox_widgets[i]) render_textbox(textbox_widgets[i]);
    }
}
void update_all_registered_textboxs(Event *event) {
    for (int i = 0; i < textboxs_count; i++) {
        if (textbox_widgets[i]) update_textbox(textbox_widgets[i], event);
    }
}
void free_all_registered_textboxes(void) {
    for (int i = 0; i < textboxs_count; i++) {
        if (textbox_widgets[i]) {
            free_textbox(textbox_widgets[i]);
            textbox_widgets[i] = NULL;
        }
    }
    textboxs_count = 0;
}
void free_textbox(TextBox* textbox) {
    if (textbox) {
        free(textbox->text);
        free(textbox->place_holder);
    }
}
