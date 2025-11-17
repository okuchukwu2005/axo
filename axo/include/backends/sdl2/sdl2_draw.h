/**
 * @file sdl2_backend.h
 * @brief Contains all rendering logic for SDL2-based drawing operations
 */

#ifndef SDL2_DRAW_H
#define SDL2_DRAW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../../core/color.h"   // Access Color struct
#include "sdl2.h" 


/**
 * @brief Clears the screen to the specified color
 * @param base Pointer to the Base struct containing the renderer
 * @param color The background color to set
 */
void clear_screen(Base* base, Color color);
/**
 * @brief Presents the rendered content to the screen
 * @param base Pointer to the Base struct containing the renderer
 */
void present_screen(Base* base);

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
void draw_rect(Base* base, int x, int y, int w, int h, Color color);

/**
 * @brief Draws a filled circle at the specified center with the given radius
 * @param base Pointer to the Base struct containing the renderer
 * @param x X-coordinate of the circle's center
 * @param y Y-coordinate of the circle's center
 * @param radius Radius of the circle
 * @param color The fill color of the circle
 */
void draw_circle(Base* base, int x, int y, int radius, Color color);

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
void draw_triangle(Base* base, int x1, int y1, int x2, int y2, int x3, int y3, Color color);
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
void draw_rounded_rect(Base* base, int x, int y, int w, int h, float roundness, Color color);

void draw_image(Base * base, const char * file, int x, int y, int w, int h);

/**
* @brief Draws an image using sdl texture
* @param base for renderer
* @param texture it contains file and renderer
* @param x
* @param y
* @param w
* @param h 
*/
void draw_image_from_texture(Base *base, void * texture, int x, int y, int w, int h);
void draw_icon(Base* base, const char* icon, int font_size, int x, int y, Color color);

#endif // SDL2_DRAW_H
