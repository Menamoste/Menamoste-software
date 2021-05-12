#ifndef MAIN_H_
#define MAIN_H_

#include <SDL2/SDL.h>
#include <math.h>
#include "matrix.h"
#include "tools.h"

//Black and white image.
void Black_and_white(matrix_pack *mat_pack, SDL_Surface* image_surface, triplet trip);

//Grayscale image.
void Grayscale(matrix_pack *mat_pack, SDL_Surface* image_surface, triplet trip);

//The color are reversed.
void Negative(matrix_pack *mat_pack, SDL_Surface* image_surface, triplet trip);

//Lighten the image. 
void Lighten(matrix_pack *mat_pack, SDL_Surface* image_surface, triplet trip);

//Give a peach effect to the image.
void Peach(matrix_pack *mat_pack, SDL_Surface* image_surface, triplet trip);

//Give a vintage effect to the image.
void Vintage(matrix_pack *mat_pack, SDL_Surface* image_surface, triplet trip);

//Darken the image.
void Darken(matrix_pack *mat_pack, SDL_Surface* image_surface, triplet trip);

//Light pixels are ligthen and dark ones are darken.
void Contrast(matrix_pack *mat_pack, SDL_Surface* image_surface, triplet trip);

//Blur the image.
void Blur(matrix_pack *mat_pack, SDL_Surface* image_surface, triplet trip);

//Cut out the image.
void Outline(matrix_pack *mat_pack, SDL_Surface* image_surface, triplet trip);

#endif

