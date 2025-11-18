/**
 * @file text.h
 * @brief Contains logic for text widgets (labels) using SDL2
 */

#ifndef TEXT_H
#define TEXT_H

#include "../core/parent.h"
#include "../core/backend_interface.h"



#include <SDL2/SDL.h> // for SDL_Event, etc.


typedef struct {
    axParent* parent;            // Pointer to the parent window or container
    int x, y;                  // Position of the text
    char* content;             // Text content
    int font_size;             // Font size in points
    Color* color;               // Text color
    TextAlign align;           // Alignment (LEFT, CENTER, RIGHT)
} axText;

axText axCreateText(axParent* parent, int x, int y, const char* content, int font_size, TextAlign align);
void axRenderText(axText* text);
// Setters for overrides
void axSetTextColor(axText* text, Color color);

void axUpdateText(axText* text, axEvent *event);

void axFreeText(axText *text);


// Registration
#define MAX_TEXTS 100

extern axText* text_widgets[MAX_TEXTS];
extern int texts_count;

void axRegisterText(axText* text);

void axRenderAllRegisteredTexts(void);

void axUpdateAllRegisteredTexts(axEvent* event);

void axFreeAllRegisteredTexts(void);
#endif // TEXT_H
