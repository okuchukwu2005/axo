#include "../../include/core/color.h"
#include <SDL2/SDL.h>
#include <stdint.h>  // For uint8_t (ensures compatibility)

// Color manipulation helpers
/**
 * @brief Darkens a color by a factor (0.0 = no change, 1.0 = black)
 * @param c The original color
 * @param factor The darkening amount
 * @return The darkened color
 */
Color darken_color(Color c, float factor) {
    Color darkened = {
        .r = (uint8_t)(c.r * (1.0f - factor)),
        .g = (uint8_t)(c.g * (1.0f - factor)),
        .b = (uint8_t)(c.b * (1.0f - factor)),
        .a = c.a
    };
    return darkened;
}

/**
 * @brief Lightens a color by a factor (0.0 = no change, 1.0 = white)
 * @param c The original color
 * @param factor The lightening amount
 * @return The lightened color
 */

 Color lighten_color(Color c, float factor) {
    Color lightened = {
        .r = (uint8_t)(c.r + (255 - c.r) * factor),
        .g = (uint8_t)(c.g + (255 - c.g) * factor),
        .b = (uint8_t)(c.b + (255 - c.b) * factor),
        .a = c.a
    };
    return lightened;
}
