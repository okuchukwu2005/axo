/**
 * @file drop.h
 * @brief Dropdown widget - safe, no malloc'd colors
 */

#ifndef DROP_H
#define DROP_H

#include "../core/parent.h"
#include "../core/color.h"
#include "../core/backend_interface.h"
#include <stdbool.h>

#define MAX_DROPS 100

typedef struct {
    axParent* parent;
    int x, y;
    int w, h;
    char** options;
    int option_count;
    int selected_index;
    bool is_expanded;
    bool is_hovered;
    int font_size;                    // logical, defaults to theme
    char* place_holder;

    // Custom colors — user sets directly, no malloc!
    bool has_custom_bg_color;
    bool has_custom_button_color;
    bool has_custom_text_color;
    bool has_custom_highlight_color;

    Color custom_bg_color;
    Color custom_button_color;
    Color custom_text_color;
    Color custom_highlight_color;
} axDropDown;

// Public API
axDropDown* axCreateDropDown(axParent* parent, int x, int y, int w, int h,
                             char** options, int option_count);


void axRenderDropDown(axDropDown* drop);
void axUpdateDropDown(axDropDown* drop, axEvent* event);
void axFreeDropDown(axDropDown* drop);

// Registration
extern axDropDown* drop_widgets[MAX_DROPS];
extern int drops_count;

void axRegisterDropDown(axDropDown* drop);
void axRenderAllRegisteredDropDown(void);
void axUpdateAllRegisteredDropDown(axEvent* event);
void axFreeAllRegisteredDropDown(void);

#endif // DROP_H
