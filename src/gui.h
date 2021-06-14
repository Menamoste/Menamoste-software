#include <stdio.h>
#include <SDL2/SDL.h>
#include "matrix.h"

#ifndef GUI_H
#define GUI_H

//Set a color on a SDL_Color array.
void set_color(SDL_Color *colors, size_t i, unsigned char r, unsigned char
g, unsigned char b, unsigned char a);

//Set the propries on a rectangle.
void set_rect(SDL_Rect *rect, int x, int y, int h, int w);

//Place the correct coordinates on the rects structure.
void place_rects(SDL_Rect *rects);

//Draw a rectangle on the renderer.
void draw_rect(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color);

//Load the icons on the renderer.
void load_rects(SDL_Renderer *renderer, SDL_Rect *rects);

//Clean the ressources to avoid memory leaks.
void cleanResources(SDL_Window *window, SDL_Renderer *renderer, 
SDL_Texture *texture);

//Resize the image.
matrix_pack *resize_image(matrix_pack *mat_pack, int new_w, int new_y);

//Modify the image according to the mode chosen.
matrix_pack *modify_image(matrix_pack *mat_pack, matrix *convo, int mode);

//Print the image on the renderer
void print_image(SDL_Renderer *renderer, SDL_Rect *rect, SDL_Surface *sur,
 matrix_pack *mat_pack); 

#endif
