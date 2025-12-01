/**
 * @file button.h
 * @brief Contains logic for button widgets using SDL2
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <math.h>

#include "../core/color.h"
#include "../core/parent.h"
#include "../core/backend_interface.h"

#define MAX_BUTTONS 100

typedef struct {
    axParent* parent;
    int x, y;
    int w, h;
    char* label;
    void (*callback)(void);

    // State
    bool is_hovered;
    bool is_pressed;

    // Custom colors — user can set directly (no malloc!)
    bool has_custom_bg_color;
    bool has_custom_text_color;
    Color custom_bg_color;
    Color custom_text_color;
} axButton;

// Creation / lifecycle
axButton* axCreateButton(axParent* parent, int x, int y, int w, int h,
                         const char* label, void (*callback)(void));

void axRenderButton(axButton* button);
void axUpdateButton(axButton* button, axEvent* event);
void axFreeButton(axButton* button);

// Dummy override (optional)
void function_callback_override(void);
#define OVERRIDE function_callback_override

// Registration system
extern axButton* button_widgets[MAX_BUTTONS];
extern int buttons_count;

void axRegisterButton(axButton* button);
void axRenderAllRegisteredButtons(void);
void axUpdateAllRegisteredButtons(axEvent* event);
void axFreeAllRegisteredButtons(void);

#endif // BUTTON_H
