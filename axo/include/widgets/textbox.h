#ifndef TEXTBOX_H
#define  TEXTBOX_H

#include "../core/parent.h"
#include "../core/backend_interface.h"


typedef struct {
    int start;
    int len;
} Line;

Line* compute_visual_lines(const char* text, int max_width, Font_ttf* font, int* num_lines);

typedef struct {
    axParent* parent;            // Pointer to the parent window or container
    int x, y;                  // Position of the textbox (logical)
    int w, h;                  // Width and height of the textbox (logical, taller by default)
    char* place_holder;        // Placeholder text
    int max_length;            // Maximum text length
    char* text;                // Input text (supports \n for lines)
    int is_active;             // Is the textbox active?
    int cursor_pos;            // Cursor position (character index)
    int selection_start;       // Selection anchor (-1 if no selection)
    int visible_line_start;    // Index of first visible line
    int line_height;           // Height of each line (logical, computed from font)
    int is_mouse_selecting;    // Flag to track if mouse is being used to select text
} axTextBox;


axTextBox axCreateTextBox(axParent* parent, int x, int y, int w, int max_length);

// Renders the textbox widget to the screen
// Parameters:
// - textbox: The TextBox widget to render
/* --------------------------------------------------------------
   RENDER TEXTBOX – CLIPPING FIXED
   -------------------------------------------------------------- */
void axRenderTextBox(axTextBox* textbox) ;
void update_visible_lines(axTextBox* textbox);

// Updates the textbox widget based on SDL events (mouse, keyboard, text input)
// Parameters:
// - textbox: The TextBox widget to update
// - event: The SDL event to process
void axUpdateTextBox(axTextBox* textbox, axEvent *event);

void axFreeTextBox(axTextBox* textbox);

// Registration
#define MAX_TEXTBOXS 100

extern axTextBox* textbox_widgets[MAX_TEXTBOXS];
extern int textboxs_count;

void axRegisterTextBox(axTextBox* textbox);

void axRenderAllRegisteredTextBoxes(void);

void axUpdateAllRegisteredTextBoxes(axEvent* event);

void axFreeAllRegisteredTextBoxes(void);

#endif // TEXTBOX_H
