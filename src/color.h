#ifndef COLOR_H
#define COLOR_H

#include <SDL2/SDL.h>
#include "matrix.h"
#include "tools.h"

//Color a pixel chosen by the user.
void color_pixel(matrix_pack *mat_pack, SDL_Surface *sur, triplet trip, int x,
int y);

#endif
