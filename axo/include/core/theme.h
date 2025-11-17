/**
 * @file theme.h
 * @brief Defines themes for the GUI library, including color palettes and styles
 */

#ifndef THEME_H
#define THEME_H

#include "color.h"  // For Color struct

/**
 * @brief Theme structure holding visual properties for the GUI
 */
typedef struct {
    // Background colors
    Color bg_primary;       // Main background (e.g., window bg)
    Color bg_secondary;     // Secondary backgrounds (e.g., panels, containers)

    // Text colors
    Color text_primary;     // Default text color
    Color text_secondary;   // Muted or secondary text (e.g., hints)

    // Accent and interactive colors
    Color accent;           // Highlights, borders, or active elements
    Color accent_hovered;   // Hover state for accents
    Color accent_pressed;   // Pressed state for accents

    // Button-specific colors
    Color button_normal;    // Default button background
    Color button_hovered;   // Button hover background
    Color button_pressed;   // Button pressed background
    Color button_text;      // Button text color

    // Container-specific colors
    Color container_bg;     // Background for containers
    Color container_title_bg; // Background for container title bars

    // Metrics and styles
    int default_font_size;  // Default font size in points
    char* font_file;        // Path to the default font file
    int padding;            // Default padding for widgets
    float roundness;        // Rounding factor for rounded rectangles (0.0 to 1.0)
} Theme;

// Predefined themes

/**
 * @brief Light theme preset
 */
static const Theme THEME_LIGHT = {
    .bg_primary = {255, 255, 255, 255},      // White
    .bg_secondary = {240, 240, 240, 255},    // Light gray
    .text_primary = {0, 0, 0, 255},          // Black
    .text_secondary = {100, 100, 100, 255},  // Dark gray
    .accent = {0, 122, 255, 255},            // Blue
    .accent_hovered = {0, 142, 255, 255},    // Lighter blue
    .accent_pressed = {0, 102, 255, 255},    // Darker blue
    .button_normal = {200, 200, 200, 255},   // Gray
    .button_hovered = {220, 220, 220, 255},  // Lighter gray
    .button_pressed = {180, 180, 180, 255},  // Darker gray
    .button_text = {0, 0, 0, 255},           // Black
    .container_bg = {250, 250, 250, 255},    // Slightly whiter for contrast
    .container_title_bg = {220, 220, 220, 255}, // Lighter gray for title bar
    .default_font_size = 16,
    .font_file = "../axo/asset/fonts/FiraCode-Regular.ttf",
    .padding = 10,
    .roundness = 0.2f
};

/**
 * @brief Dark theme preset
 */
static const Theme THEME_DARK = {
    .bg_primary = {30, 30, 30, 255},         // Dark gray
    .bg_secondary = {45, 45, 45, 255},       // Slightly lighter dark gray
    .text_primary = {255, 255, 255, 255},    // White
    .text_secondary = {170, 170, 170, 255},  // Light gray
    .accent = {0, 122, 255, 255},            // Blue (same as light for consistency)
    .accent_hovered = {0, 142, 255, 255},    // Lighter blue
    .accent_pressed = {0, 102, 255, 255},    // Darker blue
    .button_normal = {60, 60, 60, 255},      // Dark gray
    .button_hovered = {80, 80, 80, 255},     // Lighter dark gray
    .button_pressed = {40, 40, 40, 255},     // Darker dark gray
    .button_text = {255, 255, 255, 255},     // White
    .container_bg = {35, 35, 35, 255},       // Slightly darker for contrast
    .container_title_bg = {50, 50, 50, 255}, // Lighter dark gray for title bar
    .default_font_size = 16,
    .font_file = "AXO/include/core/fonts/FreeMono.ttf",
    .padding = 10,
    .roundness = 0.2f
};

/**
 * @brief Hacker theme preset (green on black, for fun)
 */
static const Theme THEME_HACKER = {
    .bg_primary = {0, 0, 0, 255},            // Black
    .bg_secondary = {20, 20, 20, 255},       // Very dark gray
    .text_primary = {0, 255, 0, 255},        // Green
    .text_secondary = {0, 200, 0, 255},      // Muted green
    .accent = {0, 255, 0, 255},              // Green
    .accent_hovered = {100, 255, 100, 255},  // Lighter green
    .accent_pressed = {0, 200, 0, 255},      // Darker green
    .button_normal = {0, 50, 0, 255},        // Dark green
    .button_hovered = {0, 100, 0, 255},      // Medium green
    .button_pressed = {0, 30, 0, 255},       // Darker green
    .button_text = {0, 255, 0, 255},         // Green
    .container_bg = {10, 10, 10, 255},       // Even darker gray for contrast
    .container_title_bg = {30, 30, 30, 255}, // Lighter dark for title bar
    .default_font_size = 16,
    .font_file = "AXO/include/core/fonts/FreeMono.ttf",
    .padding = 10,
    .roundness = 0.1f                        // Less rounded for a "techy" feel
};

/* --------------------------- 2. DEAR IMGUI --------------------------- */

static const Theme THEME_IMGUI_DARK = {
    .bg_primary          = {30, 30, 30, 255},
    .bg_secondary        = {40, 40, 40, 255},
    .text_primary        = {220,220,220,255},
    .text_secondary      = {140,140,140,255},
    .accent              = {0, 120, 215, 255},
    .accent_hovered      = {0, 140, 235, 255},
    .accent_pressed      = {0, 100, 195, 255},
    .button_normal       = {70, 70, 70, 255},
    .button_hovered      = {90, 90, 90, 255},
    .button_pressed      = {50, 50, 50, 255},
    .button_text         = {255,255,255,255},
    .container_bg        = {35, 35, 35, 255},
    .container_title_bg  = {55, 55, 55, 255},
    .default_font_size   = 16,
    .font_file           = "AXO/include/core/fonts/FreeMono.ttf",
    .padding             = 8,
    .roundness           = 0.15f
};


/* --------------------------- 3. NUKLEAR --------------------------- */
static const Theme THEME_NUKLEAR = {
    .bg_primary          = {50, 50, 50, 255},
    .bg_secondary        = {60, 60, 60, 255},
    .text_primary        = {220,220,220,255},
    .text_secondary      = {140,140,140,255},
    .accent              = {175, 95, 35, 255},
    .accent_hovered      = {195,115, 55, 255},
    .accent_pressed      = {155, 75, 15, 255},
    .button_normal       = {70, 70, 70, 255},
    .button_hovered      = {90, 90, 90, 255},
    .button_pressed      = {50, 50, 50, 255},
    .button_text         = {255,255,255,255},
    .container_bg        = {55, 55, 55, 255},
    .container_title_bg  = {75, 75, 75, 255},
    .default_font_size   = 14,
    .font_file           = "AXO/include/core/fonts/FreeMono.ttf",
    .padding             = 6,
    .roundness           = 0.0f
};


/* --------------------------- 6. MATERIAL DESIGN --------------------------- */

static const Theme THEME_MATERIAL_DARK = {
    .bg_primary          = {18, 18, 18, 255},
    .bg_secondary        = {30, 30, 30, 255},
    .text_primary        = {255,255,255,255},
    .text_secondary      = {255,255,255,180},
    .accent              = {66, 165, 245, 255},
    .accent_hovered      = {86, 185, 255, 255},
    .accent_pressed      = {46, 145, 225, 255},
    .button_normal       = {50, 50, 50, 255},
    .button_hovered      = {70, 70, 70, 255},
    .button_pressed      = {30, 30, 30, 255},
    .button_text         = {255,255,255,255},
    .container_bg        = {25, 25, 25, 255},
    .container_title_bg  = {66, 165, 245, 255},
    .default_font_size   = 14,
    .font_file           = "AXO/include/core/fonts/FreeMono.ttf",
    .padding             = 16,
    .roundness           = 0.3f
};

/* --------------------------- 8. DRACULA --------------------------- */
static const Theme THEME_DRACULA = {
    .bg_primary          = {40, 42, 54, 255},
    .bg_secondary        = {52, 56, 71, 255},
    .text_primary        = {248, 248, 242, 255},
    .text_secondary      = {150, 152, 161, 255},
    .accent              = {255, 121, 198, 255},
    .accent_hovered      = {255, 141, 218, 255},
    .accent_pressed      = {235, 101, 178, 255},
    .button_normal       = {68, 71, 90, 255},
    .button_hovered      = {88, 91, 110, 255},
    .button_pressed      = {48, 51, 70, 255},
    .button_text         = {248,248,242,255},
    .container_bg        = {45, 47, 59, 255},
    .container_title_bg  = {68, 71, 90, 255},
    .default_font_size   = 14,
    .font_file           = "AXO/include/core/fonts/FreeMono.ttf",
    .padding             = 16,
    .roundness           = 0.2f
};

/* --------------------------- 9. NORD --------------------------- */
static const Theme THEME_NORD = {
    .bg_primary          = {46, 52, 64, 255},
    .bg_secondary        = {59, 66, 82, 255},
    .text_primary        = {216, 222, 233, 255},
    .text_secondary      = {136, 142, 156, 255},
    .accent              = {136, 192, 208, 255},
    .accent_hovered      = {156, 212, 228, 255},
    .accent_pressed      = {116, 172, 188, 255},
    .button_normal       = {67, 76, 94, 255},
    .button_hovered      = {87, 96, 114, 255},
    .button_pressed      = {47, 56, 74, 255},
    .button_text         = {216,222,233,255},
    .container_bg        = {51, 57, 69, 255},
    .container_title_bg  = {76, 86, 106, 255},
    .default_font_size   = 14,
    .font_file           = "AXO/include/core/fonts/FreeMono.ttf",
    .padding             = 10,
    .roundness           = 0.15f
};

/* --------------------------- 10. SOLARIZED --------------------------- */

static const Theme THEME_SOLARIZED_DARK = {
    .bg_primary          = {0, 43, 54, 255},
    .bg_secondary        = {7, 54, 66, 255},
    .text_primary        = {131, 148, 150, 255},
    .text_secondary      = {101, 123, 131, 255},
    .accent              = {38, 139, 210, 255},
    .accent_hovered      = {58, 159, 230, 255},
    .accent_pressed      = {18, 119, 190, 255},
    .button_normal       = {20, 63, 74, 255},
    .button_hovered      = {40, 83, 94, 255},
    .button_pressed      = {0, 43, 54, 255},
    .button_text         = {131,148,150,255},
    .container_bg        = {5, 49, 60, 255},
    .container_title_bg  = {30, 73, 84, 255},
    .default_font_size   = 14,
    .font_file           = "AXO/include/core/fonts/FreeMono.ttf",
    .padding             = 10,
    .roundness           = 0.0f
};


/* --------------------------- 15. WINDOWS 95 --------------------------- */
static const Theme THEME_WIN95 = {
    .bg_primary          = {192, 192, 192, 255},
    .bg_secondary        = {168, 168, 168, 255},
    .text_primary        = {0, 0, 0, 255},
    .text_secondary      = {100,100,100,255},
    .accent              = {0, 0, 128, 255},
    .accent_hovered      = {0, 0, 148, 255},
    .accent_pressed      = {0, 0, 108, 255},
    .button_normal       = {212, 208, 200, 255},
    .button_hovered      = {232, 228, 220, 255},
    .button_pressed      = {192, 188, 180, 255},
    .button_text         = {0, 0, 0, 255},
    .container_bg        = {212, 208, 200, 255},
    .container_title_bg  = {0, 0, 128, 255},
    .default_font_size   = 11,
    .font_file           = "AXO/include/core/fonts/FreeMono.ttf",
    .padding             = 3,
    .roundness           = 0.0f
};

extern const Theme* current_theme;
void set_theme(const Theme* theme);

#endif // THEME_H
