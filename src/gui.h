#include <stdio.h>
#include <SDL2/SDL.h>
#include "matrix.h"

#ifndef GUI_H
#define GUI_H

//Set the propries on a rectangle.
void set_rect(SDL_Rect *rect, int x, int y, int h, int w);

//Draw a rectangle on the renderer.
void draw_rect(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color);

//Set a color on a SDL_Color array.
void set_color(SDL_Color *colors, size_t i, unsigned char r, unsigned char
g, unsigned char b, unsigned char a);

//Load the icons on the renderer.
void load_rects(SDL_Renderer *renderer, SDL_Rect *rects);

//Clean the ressources to avoid memory leaks.
void cleanResources(SDL_Window *window, SDL_Renderer *renderer,
 SDL_Texture *texture);

//Print the image depending of the button the user pressed.
int print_image(SDL_Window *window, SDL_Renderer *renderer, 
 SDL_Rect *rect, matrix *convo, int mode);

//Place the correct coordinates on the rects structure.
void place_rects(SDL_Rect *rects); 
#endif
