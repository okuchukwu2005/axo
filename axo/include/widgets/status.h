/**
 * @file progress_bar.h
 * @brief Contains logic for progress bar widgets using SDL2
 */

#ifndef STATUS_H
#define STATUS_H

#include "../core/parent.h"
#include "../core/color.h"
#include "../core/backend_interface.h"


#include <stdbool.h>

typedef struct {
    axParent* parent;            // Pointer to the parent window or container
    int x, y;                  // Position of the progress bar (logical)
    int w, h;                  // Width and height of the progress bar (logical)
    int min, max;              // Range (default 0-100)
    int value;                 // Current value (clamped between min and max)
    bool show_percentage;
    bool has_custom_bg_color;    // Optional override for background color (NULL = use theme)
    bool has_custom_fill_color;  // Optional override for fill color (NULL = use theme)
    bool has_custom_text_color;        // Whether to display percentage text
    Color custom_bg_color;    // Optional override for background color (NULL = use theme)
    Color custom_fill_color;  // Optional override for fill color (NULL = use theme)
    Color custom_text_color;  // Optional override for text color (NULL = use theme)
} axStatusBar;



// -------- Create --------
axStatusBar* axCreateStatusBar(axParent* parent, int x, int y, int w, int h, int min, int max, int start_value, bool show_percentage);

// -------- Render --------
void axRenderStatusBar(axStatusBar* progress_bar);
// -------- Update --------
void axUpdateStatusBar(axStatusBar* progress_bar, axEvent* event);

// -------- Free --------
void axFreeStatusBar(axStatusBar* progress_bar);


#define MAX_STATUS_BARS 100
extern axStatusBar* status_bar_widgets[MAX_STATUS_BARS];
extern int status_bars_count;

// -------- Helpers for all Progress Bars --------
void axRegisterStatusBar(axStatusBar* progress_bar);

void axRenderAllRegisteredStatusBar(void);

void axUpdateAllRegisteredStatusBar(axEvent* event);

void axFreeAllRegisteredStatusBar(void);

#endif // STATUS_H
