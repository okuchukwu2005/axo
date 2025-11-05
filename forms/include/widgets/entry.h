#ifndef ENTRY_H
#define ENTRY_H

#include"../core/parent.h"
#include <SDL2/SDL.h> // SDL library for handling graphics, events (e.g., SDL_Event, SDLK_* for key codes)

// Defines a struct for a text entry widget, representing an input field in a GUI
typedef struct {
    Parent* parent;            // Pointer to the parent window/container holding this entry
    int x, y;                  // Logical (unscaled) position of the entry relative to parent
    int w, h;                  // Logical width and height of the entry
    char* place_holder;        // Placeholder text shown when the entry is empty
    int max_length;            // Maximum number of characters allowed in the input
    char* text;                // Buffer storing the user-entered text
    int is_active;             // Flag indicating if the entry is currently focused (1 = active, 0 = inactive)
    int cursor_pos;            // Index of the cursor's position in the text (character index)
    int selection_start;       // Starting index of text selection (-1 if no selection)
    int visible_text_start;    // Index of the first visible character (for scrolling text)
    int is_mouse_selecting;    // Flag to track if mouse is being used to select text
} Entry;


// Creates a new text entry widget with specified properties
// Parameters:
// - parent: The parent window/container
// - x, y: Logical position relative to parent
// - w: Logical width of the entry
// - max_length: Maximum number of characters allowed
// Returns: Pointer to the new Entry or NULL on failure
Entry new_entry(Parent* parent, int x, int y, int w, int max_length);

void set_entry_placeholder(Entry* entry, const char* placeholder);
// Renders the text entry widget to the screen
// Parameters:
// - entry: The Entry widget to render
// Renders the text entry widget to the screen
// Parameters:
// - entry: The Entry widget to render
void render_entry(Entry* entry);
// Updates the visible portion of the text when the cursor moves or text changes
// Parameters:
// - entry: The Entry widget to update
void update_visible_text(Entry* entry);
// Updates the text entry widget based on SDL events (mouse, keyboard, text input)
// Parameters:
// - entry: The Entry widget to update
// - event: The SDL event to process
void update_entry(Entry* entry, SDL_Event event);
// Frees the memory allocated for an Entry widget
// Parameters:
// - entry: The Entry widget to free
void free_entry(Entry* entry);

// ___________________
// Global array to store all registered entry widgets (up to MAX_ENTRYS)
#define MAX_ENTRYS 100
extern Entry* entry_widgets[MAX_ENTRYS];
extern int entrys_count; // Tracks the number of registered entries

// Registers an entry widget in the global array
// Parameters:
// - entry: The Entry widget to register
void register_entry(Entry* entry);

// Renders all registered entry widgets
void render_all_registered_entrys(void);

// Updates all registered entry widgets based on an SDL event
// Parameters:
// - event: The SDL event to process
void update_all_registered_entrys(SDL_Event event);

void free_all_registered_entrys(void);

#endif // ENTRY_H
