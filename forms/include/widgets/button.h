/**
 * @file button.h
 * @brief Contains logic for button widgets using SDL2
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h> // for SDL_Event, etc.
#include <math.h>   // For roundf in scaling

#include "color.h"
#include "parent.h"

#define MAX_BUTTONS 100

typedef struct {
    Parent* parent;            // Pointer to the parent window or container
    int x, y;                  // Position of the button (logical)
    int w, h;                  // Width and height of the button (logical)
    char* label;               // Button label text
    void (*callback)(void);    // Callback function on click
    int is_hovered;            // Is the mouse hovering over the button?
    int is_pressed;            // Is the button pressed?
    Color* custom_bg_color;    // Optional override for bg color (NULL = use theme)
    Color* custom_text_color;  // Optional override for text color (NULL = use theme)
} Button;


Button new_button(Parent* parent, int x, int y, int w, int h, const char* label, void (*callback)(void));
// Setter for bg color override
void set_button_bg_color(Button* button, Color color);
void set_button_text_color(Button* button, Color color);

void render_button(Button* button);
void function_callback_override ();
void update_button(Button* button, SDL_Event event);
void free_button(Button* button);

#define OVERRIDE function_callback_override


// Registration

extern Button* button_widgets[MAX_BUTTONS];
extern int buttons_count;

void register_button(Button* button);

void render_all_registered_buttons(void);

void update_all_registered_buttons(SDL_Event event);
void free_all_registered_buttons(void);

#endif // BUTTON_H
