/**
 * @file radio.h
 * @brief Contains logic for radio button widgets using SDL2
 */

#ifndef RADIO_H
#define RADIO_H

#include "parent.h"
#include "color.h"
#include <stdbool.h>
#include <SDL2/SDL.h> // for SDL_Event, etc.

typedef struct {
    Parent* parent;      // Parent container or window
    int x, y;            // Position (relative to parent) (logical)
    int w, h;            // Size (logical)
    char* label;         // Label text
    bool selected;       // Is selected?
    int group_id;        // Group ID (1 group → only 1 selected)
    bool is_hovered;     // For potential hover effects
    // Theme overrides (NULL = use theme)
    Color* custom_outer_color;   // Outer circle color
    Color* custom_inner_color;   // Inner circle color when selected
    Color* custom_label_color;   // Label text color
} Radio;


// -------- Create --------
Radio new_radio_button(Parent* parent, int x, int y, int w, int h, const char* label, int group_id);
void set_radio_outer_color(Radio* radio, Color color);

void set_radio_inner_color(Radio* radio, Color color);

void set_radio_label_color(Radio* radio, Color color);

// -------- Render --------
void render_radio_(Radio* radio);

#define MAX_RADIOS 100
extern Radio* radio_widgets[MAX_RADIOS];
extern int radios_count;

// -------- Update --------
void update_radio_(Radio* radio, SDL_Event event);
// -------- Free --------
void free_radio_(Radio* radio);

// -------- Register --------
void register_radio(Radio* radio);

// -------- Helpers for all radios --------
void render_all_registered_radios(void);

void update_all_registered_radios(SDL_Event event);

void free_all_registered_radios(void);

#endif // RADIO_H

/**
 * RADIO CREATION STYLES (user side)
 * ---------------------------------
 *
 * METHOD 1: simple stack locals (most common)
 *
 *     Radio r1 = new_radio_button(parent, 40, 40, 24, 24, "Option A", 1);
 *     Radio r2 = new_radio_button(parent, 40, 80, 24, 24, "Option B", 1);
 *     register_radio(&r1);
 *     register_radio(&r2);
 *
 *
 * METHOD 3: bulk creation array pattern (settings / wizard panes)
 *
 *     Radio radios[3];
 *     radios[0] = new_radio_button(parent, 50, 40, 24, 24, "Low",    1);
 *     radios[1] = new_radio_button(parent, 50, 80, 24, 24, "Medium", 1);
 *     radios[2] = new_radio_button(parent, 50,120, 24, 24, "High",   1);
 *
 *     for (int i = 0; i < 3; i++) {
 *         register_radio(&radios[i]);
 *     }
 */
