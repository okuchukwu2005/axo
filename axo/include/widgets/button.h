/**
 * @file button.h
 * @brief Contains logic for button widgets using SDL2
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h> // for SDL_Event, etc.
#include <math.h>   // For roundf in scaling

#include "../core/color.h"
#include "../core/parent.h"
#include "../core/backend_interface.h"


#define MAX_BUTTONS 100

typedef struct {
    axParent* parent;            // Pointer to the parent window or container
    int x, y;                  // Position of the button (logical)
    int w, h;                  // Width and height of the button (logical)
    char* label;               // Button label text
    void (*callback)(void);    // Callback function on click
    int is_hovered;            // Is the mouse hovering over the button?
    int is_pressed;            // Is the button pressed?
    Color* custom_bg_color;    // Optional override for bg color (NULL = use theme)
    Color* custom_text_color;  // Optional override for text color (NULL = use theme)
}axButton;


axButton axCreateButton(axParent* parent, int x, int y, int w, int h, const char* label, void (*callback)(void));
// Setter for bg color override
void axSetButtonBgColor(axButton* button, Color color);
void axSetButtonTextColor(axButton* button, Color color);

void axRenderButton(axButton* button);
void function_callback_override ();
void axUpdateButton(axButton* button, axEvent* event);
void axFreeButton(axButton* button);

#define OVERRIDE function_callback_override


// Registration

extern axButton* button_widgets[MAX_BUTTONS];
extern int buttons_count;

void axRegisterButton(axButton* button);

void axRenderAllRegisteredButtons(void);

void axUpdateAllRegisteredButtons(axEvent* event);
void axFreeAllRegisteredButtons(void);

#endif // BUTTON_H
