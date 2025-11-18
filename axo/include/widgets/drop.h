/**
 * @file drop.h
 * @brief Contains logic for dropdown widgets using SDL2
 */

#ifndef DROP_H
#define DROP_H

#include "../core/parent.h"
#include "../core/color.h"
#include "../core/backend_interface.h"



#include<stdbool.h>

typedef struct {
    axParent* parent;         // Parent window or container
    int x, y;               // Position relative to parent (logical)
    int w, h;               // Width and height of the dropdown button (logical)
    char** options;         // Array of option strings (caller-managed)
    int option_count;       // Number of options
    int selected_index;     // Index of currently selected option (-1 = none)
    bool is_expanded;       // Whether the dropdown is open
    bool is_hovered;        // Whether mouse is over the dropdown button
    int font_size;          // Font size (overridable, defaults to theme) (logical)
    char* place_holder;     // Placeholder text when no option is selected
    // Theme overrides (NULL = use theme)
    Color* custom_bg_color;       // Background for options
    Color* custom_button_color;   // Background for dropdown button
    Color* custom_text_color;     // Text color
    Color* custom_highlight_color; // Highlight for selected/hovered option
} axDropDown;

#define MAX_DROPS 100
extern axDropDown* drop_widgets[MAX_DROPS];
extern int drops_count;


axDropDown axCreateDropDown(axParent* parent, int x, int y, int w, int h, char** options, int option_count);

// Setters for overrides
void axSetDropDownBgColor(axDropDown* drop, Color color);
void axSetDropDownButtonColor(axDropDown* drop, Color color);

void axSetDropDownTextColor(axDropDown* drop, Color color);

void axSetDropDownHighlightColor(axDropDown* drop, Color color);

void axSetDropDownFontSize(axDropDown* drop, int size);

void draw_upside_down_triangle_(Base* base, int x1, int y1, int x2, int y2, int x3, int y3, Color color);

void axRenderDropDown(axDropDown* drop);
void axUpdateDropDown(axDropDown* drop, axEvent *event);
void axFreeDropDown(axDropDown* drop);

void axRegisterDropDown(axDropDown* drop);

void axRenderAllRegisteredDropDown(void);

void axUpdateAllRegisteredDropDown(axEvent *event);

void axFreeAllRegisteredDropDown(void);
#endif // DROP_H
