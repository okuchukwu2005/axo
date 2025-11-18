#ifndef SLIDER_H
#define SLIDER_H

#include "../core/parent.h"
#include "../core/color.h"
#include "../core/backend_interface.h"


#include <stdbool.h>

typedef struct {
    axParent* parent;      // Parent container or window
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
} axSlider;


// -------- Create --------
axSlider axCreateSlider(axParent* parent, int x, int y, int w, int h, int min, int max, int start_value, const char* label);

// Setters for overrides
void axSetSliderTrackColor(axSlider* slider, Color color);

void axSetSliderThumbColor(axSlider* slider, Color color);

void axSetSliderLabelColor(axSlider* slider, Color color);

// -------- Render --------
void axRenderSlider(axSlider* slider);

// -------- Update --------
void axUpdateSlider(axSlider* slider, axEvent *event);
// -------- Free --------
void axFreeSlider(axSlider* slider);


#define MAX_SLIDERS 100
extern axSlider* sliders[MAX_SLIDERS];
extern int sliders_count;

// -------- Register --------
void axRegisterSlider(axSlider* slider);

// -------- Helpers for all Sliders --------
void axRenderAllRegisteredSliders(void);

void axUpdateAllRegisteredSliders(axEvent* event);
void axFreeAllRegisteredSliders(void);
#endif // SLIDER_H
