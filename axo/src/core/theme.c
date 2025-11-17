/**
 * @file theme.c
 * @brief Implementation for theme module
 */

#include "../../include/core/theme.h"

const Theme* current_theme = NULL;

void set_theme(const Theme* theme) {
    current_theme = theme;
}
