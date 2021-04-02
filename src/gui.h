#include <stdio.h>

#ifndef GUI_H
#define GUI_H

//Set the propries on a rectangle.
void set_rect(SDL_Rect *rect, int x, int y, int h, int w);

//Draw a rectangle on the renderer.
void draw_rect(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color);

//Set a color on a SDL_Color array.
void set_color(SDL_Color *colors, size_t i, unsigned char r, unsigned char
g, unsigned char b, unsigned char a);
#endif
