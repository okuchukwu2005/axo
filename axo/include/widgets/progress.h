/**
 * @file progress_bar.h
 * @brief Contains logic for progress bar widgets using SDL2
 */

#ifndef PROGRESS_H
#define PROGRESS_H

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
    bool show_percentage;      // Whether to display percentage text
    Color* custom_bg_color;    // Optional override for background color (NULL = use theme)
    Color* custom_fill_color;  // Optional override for fill color (NULL = use theme)
    Color* custom_text_color;  // Optional override for text color (NULL = use theme)
} axProgressBar;



// -------- Create --------
axProgressBar axCreateProgressBar(axParent* parent, int x, int y, int w, int h, int min, int max, int start_value, bool show_percentage);

void axSetProgressBarBgColor(axProgressBar* progress_bar, Color color);

void axSetProgressBarFillColor(axProgressBar* progress_bar, Color color);

void axSetProgressBarTextColor(axProgressBar* progress_bar, Color color);
// Setter for value
void axSetProgressBarValue(axProgressBar* progress_bar, int value);

// -------- Render --------
void axRenderProgressBar(axProgressBar* progress_bar);
// -------- Update --------
void axUpdateProgressBar(axProgressBar* progress_bar, axEvent* event);

// -------- Free --------
void axFreeProgressBar(axProgressBar* progress_bar);


#define MAX_PROGRESS_BARS 100
extern axProgressBar* progress_bar_widgets[MAX_PROGRESS_BARS];
extern int progress_bars_count;

// -------- Helpers for all Progress Bars --------
void axRegisterProgressBar(axProgressBar* progress_bar);

void axRenderAllRegisteredProgressBars(void);

void axUpdateAllRegisteredProgressBars(axEvent* event);

void axFreeAllRegisteredProgressBars(void);

#endif // PROGRESS_H
