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
#include <SDL2/SDL.h> // for SDL_Event, etc.

typedef struct {
    Parent* parent;            // Pointer to the parent window or container
    int x, y;                  // Position of the progress bar (logical)
    int w, h;                  // Width and height of the progress bar (logical)
    int min, max;              // Range (default 0-100)
    int value;                 // Current value (clamped between min and max)
    bool show_percentage;      // Whether to display percentage text
    Color* custom_bg_color;    // Optional override for background color (NULL = use theme)
    Color* custom_fill_color;  // Optional override for fill color (NULL = use theme)
    Color* custom_text_color;  // Optional override for text color (NULL = use theme)
} ProgressBar;



// -------- Create --------
ProgressBar new_progress_bar(Parent* parent, int x, int y, int w, int h, int min, int max, int start_value, bool show_percentage);

void set_progress_bar_bg_color(ProgressBar* progress_bar, Color color);

void set_progress_bar_fill_color(ProgressBar* progress_bar, Color color);

void set_progress_bar_text_color(ProgressBar* progress_bar, Color color);
// Setter for value
void set_progress_bar_value(ProgressBar* progress_bar, int value);

// -------- Render --------
void render_progress_bar(ProgressBar* progress_bar);
// -------- Update --------
void update_progress_bar(ProgressBar* progress_bar, Event* event);

// -------- Free --------
void free_progress_bar(ProgressBar* progress_bar);


#define MAX_PROGRESS_BARS 100
extern ProgressBar* progress_bar_widgets[MAX_PROGRESS_BARS];
extern int progress_bars_count;

// -------- Helpers for all Progress Bars --------
void register_progress_bar(ProgressBar* progress_bar);

void render_all_registered_progress_bars(void);

void update_all_registered_progress_bars(Event* event);

void free_all_registered_progress_bars(void);

#endif // PROGRESS_H
