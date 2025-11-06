/**
 * @file text.h
 * @brief Contains logic for text widgets (labels) using SDL2
 */

#ifndef TEXT_H
#define TEXT_H

#include "../core/parent.h"
#include "../core/graphics.h"
#include "../core/interface.h"


#include <SDL2/SDL.h> // for SDL_Event, etc.


typedef struct {
    Parent* parent;            // Pointer to the parent window or container
    int x, y;                  // Position of the text
    char* content;             // Text content
    int font_size;             // Font size in points
    Color* color;               // Text color
    TextAlign align;           // Alignment (LEFT, CENTER, RIGHT)
} Text;

Text new_text(Parent* parent, int x, int y, const char* content, int font_size, TextAlign align);
void render_text(Text* text);
// Setters for overrides
void set_text_color(Text* text, Color color);

void update_text(Text* text, Event *event);

void free_text(Text *text);


// Registration
#define MAX_TEXTS 100

extern Text* text_widgets[MAX_TEXTS];
extern int texts_count;

void register_text(Text* text);

void render_all_registered_texts(void);

void update_all_registered_texts(Event* event);

void free_all_registered_texts(void);
#endif // TEXT_H
