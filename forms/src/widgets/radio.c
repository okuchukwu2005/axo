#include "radio.h"
#include "theme.h"
#include <stdlib.h> // for malloc
#include <string.h> // for strdup
#include <math.h>   // For roundf in scaling



// -------- Create --------
Radio new_radio_button(Parent* parent, int x, int y, int w, int h, const char* label, int group_id) {
    if (!parent || !parent->base.sdl_renderer) {
        printf("Invalid parent or renderer\n");
    }

    Radio radio;

    radio.parent = parent;
    radio.x = x;
    radio.y = y;
    radio.w = w;
    radio.h = h;
    radio.label = strdup(label);
    if (!radio.label) {
        printf("Failed to allocate memory for label\n");
    }
    radio.selected = false;
    radio.group_id = group_id;
    radio.is_hovered = false;
    // Init overrides to NULL (use theme)
    radio.custom_outer_color = NULL;
    radio.custom_inner_color = NULL;
    radio.custom_label_color = NULL;

    return radio;
}

// Setters for overrides
void set_radio_outer_color(Radio* radio, Color color) {
    if (radio) {
        if (!radio->custom_outer_color) {
            radio->custom_outer_color = (Color*)malloc(sizeof(Color));
        }
        *radio->custom_outer_color = color;
    }
}

void set_radio_inner_color(Radio* radio, Color color) {
    if (radio) {
        if (!radio->custom_inner_color) {
            radio->custom_inner_color = (Color*)malloc(sizeof(Color));
        }
        *radio->custom_inner_color = color;
    }
}

void set_radio_label_color(Radio* radio, Color color) {
    if (radio) {
        if (!radio->custom_label_color) {
            radio->custom_label_color = (Color*)malloc(sizeof(Color));
        }
        *radio->custom_label_color = color;
    }
}

// -------- Render --------
void render_radio_(Radio* radio) {
    if (!radio || !radio->parent || !radio->parent->base.sdl_renderer || !radio->parent->is_open) {
        printf("Invalid radio, renderer, or parent is not open\n");
        return;
    }
	//set container clipping
	if(radio->parent->is_window == false){
	SDL_Rect parent_bounds = get_parent_rect(radio->parent);
	SDL_RenderSetClipRect(radio->parent->base.sdl_renderer, &parent_bounds);
	}

    // Fallback if no theme set
    if (!current_theme) {
        current_theme = (Theme*)&THEME_LIGHT;  // Or set a static fallback
    }

    float dpi = radio->parent->base.dpi_scale;
    // Calculate absolute position relative to parent (logical), then scale
    int abs_x = radio->x + radio->parent->x;
    int abs_y = radio->y + radio->parent->y + radio->parent->title_height;
    int sx = (int)roundf(abs_x * dpi);
    int sy = (int)roundf(abs_y * dpi);
    int sh = (int)roundf(radio->h * dpi);  // Assuming square, w ignored or = h
    int radius = (int)roundf((radio->h / 2) * dpi);
    int inner_margin = (int)roundf(4 * dpi);  // Scale the inner margin
    int inner_radius = radius - inner_margin;
    int font_size = (int)roundf(current_theme->default_font_size * dpi);  // Scaled from theme
    int pad = (int)roundf(current_theme->padding * dpi);  // Scaled padding for label

    Base* base = &radio->parent->base;

    // Get colors from theme/overrides
    Color outer_color = radio->custom_outer_color ? *radio->custom_outer_color : current_theme->bg_secondary;
    if (radio->is_hovered) {
        outer_color = radio->custom_outer_color ? lighten_color(*radio->custom_outer_color, 0.1f) : current_theme->button_hovered;
    }
    Color inner_color = radio->custom_inner_color ? *radio->custom_inner_color : current_theme->accent;
    Color label_color = radio->custom_label_color ? *radio->custom_label_color : current_theme->text_primary;

    // Outer circle (centered at sx, sy)
    draw_circle_(base, sx, sy, radius, outer_color);

    // Inner circle if selected
    if (radio->selected) {
        draw_circle_(base, sx, sy, inner_radius, inner_color);
    }

    // Label text (to the right, vertically centered)
    int label_y = sy - (int)roundf((radio->h / 6) * dpi);  // Adjusted for better centering (was /3)
    draw_text_(base, radio->label, font_size,
               sx + sh + pad / 2, label_y,
               label_color);

    // Reset clipping
    SDL_RenderSetClipRect(radio->parent->base.sdl_renderer, NULL);
}

Radio* radio_widgets[MAX_RADIOS];
int radios_count = 0;

// -------- Update --------
void update_radio_(Radio* radio, SDL_Event event) {
    if (!radio || !radio->parent || !radio->parent->is_open) {
        printf("Invalid radio, parent, or parent is not open\n");
        return;
    }

    // Calculate absolute position relative to parent (logical)
    int abs_x = radio->x + radio->parent->x;
    int abs_y = radio->y + radio->parent->y + radio->parent->title_height;

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    // Check hover (bounding box around circle)
    bool over = (mouse_x >= abs_x - radio->h/2 && mouse_x <= abs_x + radio->h/2 &&
                 mouse_y >= abs_y - radio->h/2 && mouse_y <= abs_y + radio->h/2);
    radio->is_hovered = over;

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mx = event.button.x;
        int my = event.button.y;

        // Hitbox: circle bounding box
        if (mx >= abs_x - radio->h/2 && mx <= abs_x + radio->h/2 &&
            my >= abs_y - radio->h/2 && my <= abs_y + radio->h/2) {

            // Deselect others in same group
            for (int i = 0; i < radios_count; i++) {
                if (radio_widgets[i] && radio_widgets[i]->group_id == radio->group_id) {
                    radio_widgets[i]->selected = false;
                }
            }
            radio->selected = true;
        }
    }
}

// -------- Free --------
void free_radio_(Radio* radio) {
    if (radio) {
        free(radio->label);
        if (radio->custom_outer_color) free(radio->custom_outer_color);
        if (radio->custom_inner_color) free(radio->custom_inner_color);
        if (radio->custom_label_color) free(radio->custom_label_color);
    }
}

// -------- Register --------
void register_radio(Radio* radio) {
    if (radios_count < MAX_RADIOS) {
        radio_widgets[radios_count++] = radio;
    }
}

// -------- Helpers for all radios --------
void render_all_registered_radios(void) {
    for (int i = 0; i < radios_count; i++) {
        if (radio_widgets[i]) {
            render_radio_(radio_widgets[i]);
        }
    }
}

void update_all_registered_radios(SDL_Event event) {
    for (int i = 0; i < radios_count; i++) {
        if (radio_widgets[i]) {
            update_radio_(radio_widgets[i], event);
        }
    }
}

void free_all_registered_radios(void) {
    for (int i = 0; i < radios_count; i++) {
        free_radio_(radio_widgets[i]);
        radio_widgets[i] = NULL;
    }
    radios_count = 0;
}
