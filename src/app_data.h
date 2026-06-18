#ifndef APP_DATA_H
#define APP_DATA_H

#include <SDL2/SDL.h>
#include "matrix.h"

// Color palette initialization
void app_init_colors(SDL_Color* colors);

// Filter matrices initialization
void app_init_filter_matrices(matrix** contrast_matrix, matrix** blur_matrix);

// Cleanup filter matrices
void app_cleanup_filter_matrices(matrix* contrast_matrix, matrix* blur_matrix);

// Color conversion helper
void color_to_trip(SDL_Color color, triplet *trip);

#endif
