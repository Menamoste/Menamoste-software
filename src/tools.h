#ifndef TOOLS_H
#define TOOLS_H

#include <SDL2/SDL.h>
#include "matrix.h"

//Set a pixel with the x and y and pixel is the color.
void set_pixel(SDL_Surface *sur, int x, int y, Uint32 pixel);

//Take the color of a pixel with the x and y.
Uint32 get_pixel(SDL_Surface *sur, int x, int y);

//Return a matrix_pack of the RGB pixels of the surface.
matrix_pack *sur_to_mat_pack(SDL_Surface *sur);

//Changes directly the surface *sur with the matrix_pack *mat_pack.
void mat_pack_to_sur(SDL_Surface *sur, matrix_pack *mat_pack);

//Rotate the matrix by the angle and returns a new matrix_pack.
matrix_pack *rotation(matrix_pack *mat_pack, unsigned char angle);
#endif
