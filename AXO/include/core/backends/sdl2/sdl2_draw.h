/**
 * @file sdl2_backend.h
 * @brief Contains all rendering logic for SDL2-based drawing operations
 */

#ifndef SDL2_DRAW_H
#define SDL2_DRAW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../../graphics.h" // for text Align struct
#include "../../parent.h" // Access Base
#include "../../color.h"   // Access Color struct
#include "sdl2_ttf.h"   // Access Color struct

/**
 * @brief Clears the screen to the specified color
 * @param base Pointer to the Base struct containing the renderer
 * @param color The background color to set
 */
void backend_clear_screen(Base* base, Color color);
/**
 * @brief Presents the rendered content to the screen
 * @param base Pointer to the Base struct containing the renderer
 */
void backend_present(Base* base);

// ______________DRAW FUNCTIONS_____________

/**
 * @brief Draws a filled rectangle at the specified position and size
 * @param base Pointer to the Base struct containing the renderer
 * @param x X-coordinate of the top-left corner
 * @param y Y-coordinate of the top-left corner
 * @param w Width of the rectangle
 * @param h Height of the rectangle
 * @param color The fill color of the rectangle
 */
void backend_draw_rect(Base* base, int x, int y, int w, int h, Color color);

/**
 * @brief Draws a filled circle at the specified center with the given radius
 * @param base Pointer to the Base struct containing the renderer
 * @param x X-coordinate of the circle's center
 * @param y Y-coordinate of the circle's center
 * @param radius Radius of the circle
 * @param color The fill color of the circle
 */
void backend_draw_circle(Base* base, int x, int y, int radius, Color color);

/**
 * @brief Draws a filled triangle with the specified vertices
 * @param base Pointer to the Base struct containing the renderer
 * @param x1 X-coordinate of the first vertex
 * @param y1 Y-coordinate of the first vertex
 * @param x2 X-coordinate of the second vertex
 * @param y2 Y-coordinate of the second vertex
 * @param x3 X-coordinate of the third vertex
 * @param y3 Y-coordinate of the third vertex
 * @param color The fill color of the triangle
 */
void backend_draw_triangle(Base* base, int x1, int y1, int x2, int y2, int x3, int y3, Color color);
/**
 * @brief Draws a filled rounded rectangle (similar to raylib's DrawRectangleRounded)
 * @param base Pointer to the Base struct containing the renderer
 * @param x X-coordinate of the top-left corner
 * @param y Y-coordinate of the top-left corner
 * @param w Width of the rectangle
 * @param h Height of the rectangle
 * @param roundness Rounding factor (0.0f = rectangle, 1.0f = full rounded)
 * @param color The fill color of the rounded rectangle
 */
void backend_draw_rounded_rect(Base* base, int x, int y, int w, int h, float roundness, Color color);


/**
 * @brief Draws text at the specified position using a provided TTF font
 * @param base Pointer to the Base struct containing the renderer
 * @param font Pointer to the loaded TTF_Font
 * @param text The text string to render
 * @param x X-coordinate for the text (depends on align)
 * @param y Y-coordinate for the top of the text
 * @param color The color of the text
 * @param align Text alignment (ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT)
 */
void backend_draw_text_from_font(Base* base, void* font, const char* text, int x, int y, Color color, TextAlign align);
/**
 * @brief Draws text at the specified position with the given font size and color
 * @param base Pointer to the Base struct containing the renderer
 * @param text The text string to render
 * @param font_size The size of the font in points
 * @param x X-coordinate for the top-left corner of the text
 * @param y Y-coordinate for the top-left corner of the text
 * @param color The color of the text
 */
void backend_draw_text(Base* base, const char* text, int font_size, int x, int y, Color color);

/**
 * @brief Draw an image from file (loads and destroys texture each call).
 * @param base Base struct with valid SDL_Renderer.
 * @param file Path to image file.
 * @param x    X coordinate.
 * @param y    Y coordinate.
 * @param w    Width (0 to use texture width).
 * @param h    Height (0 to use texture height).
 */
void backend_draw_image(Base * base, const char * file, int x, int y, int w, int h);

/**
* @brief Draws an image using sdl texture
* @param base for renderer
* @param texture it contains file and renderer
* @param x
* @param y
* @param w
* @param h 
*/
void backend_draw_image_from_texture(Base *base, void * texture, int x, int y, int w, int h);

#endif // SDL2_DRAW_H
