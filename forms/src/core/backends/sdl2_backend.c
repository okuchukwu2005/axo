#include "../../../include/core/backends/sdl2_backend.h"
#include <stdio.h>
#include <math.h>

void backend_clear_screen(Base* base, Color color) {
    SDL_SetRenderDrawColor(base->sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(base->sdl_renderer);
}

void backend_present(Base* base) {
    SDL_RenderPresent(base->sdl_renderer);
}

// ______________DRAW FUNCTIONS_____________

void backend_draw_rect(Base* base, int x, int y, int w, int h, Color color) {
    SDL_SetRenderDrawColor(base->sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(base->sdl_renderer, &rect);
}

void backend_draw_circle(Base* base, int x, int y, int radius, Color color) {
    SDL_SetRenderDrawColor(base->sdl_renderer, color.r, color.g, color.b, color.a);
    // Midpoint circle algorithm (approximation for filled circle)
    for (int w = -radius; w <= radius; w++) {
        for (int h = -radius; h <= radius; h++) {
            if (w * w + h * h <= radius * radius) {
                SDL_RenderDrawPoint(base->sdl_renderer, x + w, y + h);
            }
        }
    }
}

void backend_draw_triangle(Base* base, int x1, int y1, int x2, int y2, int x3, int y3, Color color) {
    SDL_SetRenderDrawColor(base->sdl_renderer, color.r, color.g, color.b, color.a);

    // Sort vertices by y-coordinate (v1 at top, v3 at bottom)
    int temp_x, temp_y;
    if (y1 > y2) {
        temp_x = x1; temp_y = y1;
        x1 = x2; y1 = y2;
        x2 = temp_x; y2 = temp_y;
    }
    if (y2 > y3) {
        temp_x = x2; temp_y = y2;
        x2 = x3; y2 = y3;
        x3 = temp_x; y3 = temp_y;
    }
    if (y1 > y2) {
        temp_x = x1; temp_y = y1;
        x1 = x2; y1 = y2;
        x2 = temp_x; y2 = temp_y;
    }

    // Handle degenerate cases (same y-coordinates)
    if (y1 == y3) {
        return;
    }

    // Linear interpolation for edges
    float dx12 = (y2 != y1) ? (float)(x2 - x1) / (y2 - y1) : 0;
    float dx13 = (y3 != y1) ? (float)(x3 - x1) / (y3 - y1) : 0;
    float dx23 = (y3 != y2) ? (float)(x3 - x2) / (y3 - y2) : 0;

    // Top to middle (y1 to y2)
    for (int y = y1; y <= y2 && y <= y3; y++) {
        int x_start = x1 + (int)((y - y1) * dx13);
        int x_end = (y < y2) ? x1 + (int)((y - y1) * dx12) : x2 + (int)((y - y2) * dx23);
        if (x_start > x_end) {
            int temp = x_start;
            x_start = x_end;
            x_end = temp;
        }
        SDL_RenderDrawLine(base->sdl_renderer, x_start, y, x_end, y);
    }

    // Middle to bottom (y2 to y3)
    for (int y = y2 + 1; y <= y3; y++) {
        int x_start = x1 + (int)((y - y1) * dx13);
        int x_end = x2 + (int)((y - y2) * dx23);
        if (x_start > x_end) {
            int temp = x_start;
            x_start = x_end;
            x_end = temp;
        }
        SDL_RenderDrawLine(base->sdl_renderer, x_start, y, x_end, y);
    }
}

void backend_draw_rounded_rect(Base* base, int x, int y, int w, int h, float roundness, Color color) {
    SDL_SetRenderDrawColor(base->sdl_renderer, color.r, color.g, color.b, color.a);

    if (w <= 0 || h <= 0) {
        return;
    }

    if (roundness <= 0.0f) {
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderFillRect(base->sdl_renderer, &rect);
        return;
    }

    float min_dim = (w < h) ? w : h;
    int radius = (int)(roundness * min_dim / 2.0f);
    if (radius <= 0) {
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderFillRect(base->sdl_renderer, &rect);
        return;
    }

    // Clamp radius to avoid over-rounding
    if (2 * radius > w) radius = w / 2;
    if (2 * radius > h) radius = h / 2;

    int radius_sq = radius * radius;

    for (int dy = 0; dy < h; ++dy) {
        for (int dx = 0; dx < w; ++dx) {
            bool inside = false;

            // Inner rectangle
            if (dx >= radius && dy >= radius && dx < w - radius && dy < h - radius) {
                inside = true;
            }
            // Left strip
            else if (dx < radius && dy >= radius && dy < h - radius) {
                inside = true;
            }
            // Right strip
            else if (dx >= w - radius && dy >= radius && dy < h - radius) {
                inside = true;
            }
            // Top strip
            else if (dy < radius && dx >= radius && dx < w - radius) {
                inside = true;
            }
            // Bottom strip
            else if (dy >= h - radius && dx >= radius && dx < w - radius) {
                inside = true;
            }
            // Top-left corner
            else if (dx < radius && dy < radius) {
                int dx_corner = dx - radius;
                int dy_corner = dy - radius;
                if (dx_corner * dx_corner + dy_corner * dy_corner <= radius_sq) {
                    inside = true;
                }
            }
            // Top-right corner
            else if (dx >= w - radius && dy < radius) {
                int dx_corner = dx - (w - radius);
                int dy_corner = dy - radius;
                if (dx_corner * dx_corner + dy_corner * dy_corner <= radius_sq) {
                    inside = true;
                }
            }
            // Bottom-left corner
            else if (dx < radius && dy >= h - radius) {
                int dx_corner = dx - radius;
                int dy_corner = dy - (h - radius);
                if (dx_corner * dx_corner + dy_corner * dy_corner <= radius_sq) {
                    inside = true;
                }
            }
            // Bottom-right corner
            else if (dx >= w - radius && dy >= h - radius) {
                int dx_corner = dx - (w - radius);
                int dy_corner = dy - (h - radius);
                if (dx_corner * dx_corner + dy_corner * dy_corner <= radius_sq) {
                    inside = true;
                }
            }

            if (inside) {
                SDL_RenderDrawPoint(base->sdl_renderer, x + dx, y + dy);
            }
        }
    }
}

void backend_draw_text_from_font(Base* base, void * font_ptr, const char* text, int x, int y, Color color, void * align_ptr) {
	TTF_Font *font = (TTF_Font *) font_ptr; // type cast
	TextAlign *align = (TextAlign *) align_ptr; // type cast
    if (!font) {
        printf("No font provided for text rendering\n");
        return;
    }

    SDL_Color sdl_color = {color.r, color.g, color.b, color.a};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, sdl_color);
    if (!surface) {
        printf("Failed to render text: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(base->sdl_renderer, surface);
    if (!texture) {
        printf("Failed to create texture from text surface: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    int text_width = surface->w;
    int text_height = surface->h;
    int adjusted_x = x;

    // Adjust x-coordinate based on alignment
    switch (align) {
        case ALIGN_CENTER:
            adjusted_x = x - text_width / 2;
            break;
        case ALIGN_RIGHT:
            adjusted_x = x - text_width;
            break;
        case ALIGN_LEFT:
        default:
            // No adjustment for left alignment
            break;
    }

    SDL_Rect dst_rect = {adjusted_x, y, text_width, text_height};
    SDL_RenderCopy(base->sdl_renderer, texture, NULL, &dst_rect);

    // Clean up
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

static char *FONT_FILE = "forms/core/FreeMono.ttf";

void backend_draw_text(Base* base, const char* text, int font_size, int x, int y, Color color) {
    TTF_Font* font = TTF_OpenFont(FONT_FILE, font_size);
    if (!font) {
        printf("Failed to load font '%s': %s\n", FONT_FILE, TTF_GetError());
        return;
    }

    backend_draw_text_from_font(base, font, text, x, y, color, ALIGN_LEFT);
    TTF_CloseFont(font);
}


void backend_draw_image(Base * base, const char * file, int x, int y, int w, int h){
	SDL_Texture * texture = IMG_LoadTexture(base->sdl_renderer, file);
	if(!texture){
		printf("Failed to load img %s : %s\n",file, IMG_GetError());
		return;
	}
	 // If width/height are 0, use the texture's actual size
    if (w <= 0 || h <= 0) {
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    }

    SDL_Rect img_rect = {x, y, w, h}; // x, y, w, h
    SDL_RenderCopy(base->sdl_renderer, texture, NULL, &img_rect);
    
	SDL_DestroyTexture(texture);
}

void backend_draw_image_from_texture(Base *base, void * texture_ptr, int x, int y, int w, int h){
	SDL_Texture *texture = (SDL_Texture *) texture_ptr;
	if(!texture){
		printf("Failed to load img: %s\n", IMG_GetError());
		return;
	}
	 // If width/height are 0, use the texture's actual size
    if (w <= 0 || h <= 0) {
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    }

    SDL_Rect img_rect = {x, y, w, h}; // x, y, w, h
    SDL_RenderCopy(base->sdl_renderer, texture, NULL, &img_rect);
}
