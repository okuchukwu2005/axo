
#include "../../include/widgets/progress.h"
#include "../../include/core/theme.h"
#include "../../include/core/graphics.h"
#include <stdlib.h> // for malloc
#include <SDL2/SDL.h> // for SDL_Event, etc.
#include <SDL2/SDL_ttf.h> // for SDL_Event, etc.
#include <math.h>   // For roundf in scaling



// -------- Create --------
ProgressBar new_progress_bar(Parent* parent, int x, int y, int w, int h, int min, int max, int start_value, bool show_percentage) {
    if (!parent || !parent->base.sdl_renderer) {
        printf("Invalid parent or renderer\n");
    }

    ProgressBar progress_bar;
    progress_bar.parent = parent;
    progress_bar.x = x;
    progress_bar.y = y;
    progress_bar.w = w;
    progress_bar.h = h;
    progress_bar.min = min;
    progress_bar.max = max;
    progress_bar.value = start_value;
    progress_bar.show_percentage = show_percentage;
    progress_bar.custom_bg_color = NULL;
    progress_bar.custom_fill_color = NULL;
    progress_bar.custom_text_color = NULL;

    return progress_bar;
}

// Setters for overrides
void set_progress_bar_bg_color(ProgressBar* progress_bar, Color color) {
    if (progress_bar) {
        if (!progress_bar->custom_bg_color) {
            progress_bar->custom_bg_color = (Color*)malloc(sizeof(Color));
        }
        *progress_bar->custom_bg_color = color;
    }
}

void set_progress_bar_fill_color(ProgressBar* progress_bar, Color color) {
    if (progress_bar) {
        if (!progress_bar->custom_fill_color) {
            progress_bar->custom_fill_color = (Color*)malloc(sizeof(Color));
        }
        *progress_bar->custom_fill_color = color;
    }
}

void set_progress_bar_text_color(ProgressBar* progress_bar, Color color) {
    if (progress_bar) {
        if (!progress_bar->custom_text_color) {
            progress_bar->custom_text_color = (Color*)malloc(sizeof(Color));
        }
        *progress_bar->custom_text_color = color;
    }
}

// Setter for value
void set_progress_bar_value(ProgressBar* progress_bar, int value) {
    if (progress_bar) {
        if (value < progress_bar->min) value = progress_bar->min;
        if (value > progress_bar->max) value = progress_bar->max;
        progress_bar->value = value;
    }
}

// -------- Render --------
void render_progress_bar(ProgressBar* progress_bar) {
    if (!progress_bar || !progress_bar->parent || !progress_bar->parent->base.sdl_renderer || !progress_bar->parent->is_open) {
        printf("Invalid progress bar, renderer, or parent is not open\n");
        return;
    }
	//set container clipping
	if(progress_bar->parent->is_window == false){
	SDL_Rect parent_bounds = get_parent_rect(progress_bar->parent);
	SDL_RenderSetClipRect(progress_bar->parent->base.sdl_renderer, &parent_bounds);
	}

    // Fallback if no theme set
    if (!current_theme) {
        current_theme = (Theme*)&THEME_LIGHT;  // Or set a static fallback
    }

    float dpi = progress_bar->parent->base.dpi_scale;
    // Calculate absolute position relative to parent (logical), then scale
    int abs_x = progress_bar->x + progress_bar->parent->x;
    int abs_y = progress_bar->y + progress_bar->parent->y + progress_bar->parent->title_height;
    int sx = (int)roundf(abs_x * dpi);
    int sy = (int)roundf(abs_y * dpi);
    int sw = (int)roundf(progress_bar->w * dpi);
    int sh = (int)roundf(progress_bar->h * dpi);
    int font_size = (int)roundf(current_theme->default_font_size * dpi);
    float roundness = current_theme->roundness;  // Roundness is a ratio (0-1), no scaling needed

    Base* base = &progress_bar->parent->base;

    // Get colors from theme/overrides
    Color bg_color = progress_bar->custom_bg_color ? *progress_bar->custom_bg_color : current_theme->bg_secondary;
    Color fill_color = progress_bar->custom_fill_color ? *progress_bar->custom_fill_color : current_theme->accent;
    Color text_color = progress_bar->custom_text_color ? *progress_bar->custom_text_color : current_theme->text_primary;

    // Draw background (full bar)
    draw_rounded_rect(base, sx, sy, sw, sh, roundness, bg_color);

    // Draw fill (progress portion)
    float progress_ratio = (float)(progress_bar->value - progress_bar->min) / (progress_bar->max - progress_bar->min);
    int fill_width = (int)roundf(sw * progress_ratio);
    draw_rounded_rect(base, sx, sy, fill_width, sh, roundness, fill_color);

    // Draw percentage text if enabled (centered)
    if (progress_bar->show_percentage) {
        char percentage_text[16];
        snprintf(percentage_text, sizeof(percentage_text), "%d%%", (int)(progress_ratio * 100));
        char* font_file = current_theme->font_file ? current_theme->font_file : "FreeMono.ttf";
        TTF_Font* font = TTF_OpenFont(font_file, font_size);
        if (font) {
            int text_w, text_h;
            TTF_SizeText(font, percentage_text, &text_w, &text_h);
            int text_x = sx + (sw - text_w) / 2;
            int text_y = sy + (sh - text_h) / 2;
            draw_text_from_font(base, font, percentage_text, text_x, text_y, text_color, ALIGN_LEFT);
            TTF_CloseFont(font);
        }
    }
    // Reset clipping
    SDL_RenderSetClipRect(progress_bar->parent->base.sdl_renderer, NULL);
}

// -------- Update --------
void update_progress_bar(ProgressBar* progress_bar, Event* event) {
    // Progress bar is non-interactive, so no event handling needed.
    // If you add interactive features (e.g., clickable to pause), implement here.
}

// -------- Free --------
void free_progress_bar(ProgressBar* progress_bar) {
    if (progress_bar) {
        if (progress_bar->custom_bg_color) free(progress_bar->custom_bg_color);
        if (progress_bar->custom_fill_color) free(progress_bar->custom_fill_color);
        if (progress_bar->custom_text_color) free(progress_bar->custom_text_color);
    }
}


ProgressBar* progress_bar_widgets[MAX_PROGRESS_BARS];
int progress_bars_count = 0;

// -------- Helpers for all Progress Bars --------
void register_progress_bar(ProgressBar* progress_bar) {
    if (progress_bars_count < MAX_PROGRESS_BARS) {
        progress_bar_widgets[progress_bars_count++] = progress_bar;
    }
}

void render_all_registered_progress_bars(void) {
    for (int i = 0; i < progress_bars_count; i++) {
        if (progress_bar_widgets[i]) {
            render_progress_bar(progress_bar_widgets[i]);
        }
    }
}

void update_all_registered_progress_bars(Event* event) {
    for (int i = 0; i < progress_bars_count; i++) {
        if (progress_bar_widgets[i]) {
            update_progress_bar(progress_bar_widgets[i], event);
        }
    }
}

void free_all_registered_progress_bars(void) {
    for (int i = 0; i < progress_bars_count; i++) {
        if (progress_bar_widgets[i]) {
            free_progress_bar(progress_bar_widgets[i]);
            progress_bar_widgets[i] = NULL;
        }
    }
    progress_bars_count = 0;
}

