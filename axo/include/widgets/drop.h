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
#include <SDL2/SDL.h> // for SDL_Event, etc.

typedef struct {
    Parent* parent;         // Parent window or container
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
} Drop;

#define MAX_DROPS 100
extern Drop* drop_widgets[MAX_DROPS];
extern int drops_count;


Drop new_drop_down(Parent* parent, int x, int y, int w, int h, char** options, int option_count);

// Setters for overrides
void set_drop_bg_color(Drop* drop, Color color);
void set_drop_button_color(Drop* drop, Color color);

void set_drop_text_color(Drop* drop, Color color);

void set_drop_highlight_color(Drop* drop, Color color);

void set_drop_font_size(Drop* drop, int size);

void draw_upside_down_triangle_(Base* base, int x1, int y1, int x2, int y2, int x3, int y3, Color color);

void render_drop_down_(Drop* drop);
void update_drop_down_(Drop* drop, Event *event);
void free_drop_(Drop* drop);

void register_drop(Drop* drop);

void render_all_registered_drops(void);

void update_all_registered_drops(Event *event);

void free_all_registered_drops(void);
#endif // DROP_H
