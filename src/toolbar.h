#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <SDL2/SDL.h>

// Toolbar rendering functions
void toolbar_render_background(SDL_Renderer* renderer, size_t window_width);
void toolbar_render_separator(SDL_Renderer* renderer, size_t window_width);

// Tool label rendering
void toolbar_render_tool_labels(SDL_Renderer* renderer, SDL_Rect* rects, size_t nb_icons);

#endif
