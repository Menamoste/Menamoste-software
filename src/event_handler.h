#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL2/SDL.h>
#include "matrix.h"

// Event handling context containing all state
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Color *colors;
    SDL_Rect *rects;
    SDL_Surface *image_surface;
    matrix_pack *mat_pack;
    matrix *convo;      // Filter matrices
    matrix *convo2;
    int active_tool;
    int active_color_idx;
    size_t nb_icons;
    size_t nb_rects;
    size_t window_width;
    size_t window_height;
} EventContext;

// Main event loop
int event_loop_run(EventContext* ctx);

// Color conversion helper
void color_to_trip(SDL_Color color, triplet *trip);

#endif
