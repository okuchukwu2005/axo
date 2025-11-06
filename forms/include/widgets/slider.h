#ifndef SLIDER_H
#define SLIDER_H

#include "../core/parent.h"
#include "../core/color.h"
#include "../core/interface.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    Parent* parent;      // Parent container or window
    int x, y;            // Position (relative to parent) (logical)
    int w, h;            // Width and height (assume horizontal slider, w > h) (logical)
    int min, max;        // Range of values
    int value;           // Current value
    char* label;         // Optional label text
    bool dragging;       // Flag to track if thumb is being dragged
    bool is_hovered;     // Flag for hover state (for color variants)
    Color* custom_track_color;   // Optional override for track color (NULL = use theme)
    Color* custom_thumb_color;   // Optional override for thumb color (NULL = use theme)
    Color* custom_label_color;   // Optional override for label color (NULL = use theme)
} Slider;


// -------- Create --------
Slider new_slider(Parent* parent, int x, int y, int w, int h, int min, int max, int start_value, const char* label);

// Setters for overrides
void set_slider_track_color(Slider* slider, Color color);

void set_slider_thumb_color(Slider* slider, Color color);

void set_slider_label_color(Slider* slider, Color color);

// -------- Render --------
void render_slider(Slider* slider);

// -------- Update --------
void update_slider(Slider* slider, Event *event);
// -------- Free --------
void free_slider(Slider* slider);


#define MAX_SLIDERS 100
extern Slider* sliders[MAX_SLIDERS];
extern int sliders_count;

// -------- Register --------
void register_slider(Slider* slider);

// -------- Helpers for all Sliders --------
void render_all_registered_sliders(void);

void update_all_registered_sliders(Event* event);
void free_all_registered_sliders(void);
#endif // SLIDER_H
