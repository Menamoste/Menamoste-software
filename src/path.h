#ifndef TTF_H
#define TTF_H

#include <string.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//Initializes the TTF.
void clean_init();

//Fill a SDL_Rect with a color on the renderer.
void fill_text_box(SDL_Renderer *renderer, SDL_Rect bar);

// Writes the text.
void print_text(char *text, SDL_Renderer *renderer, TTF_Font *font);

//Manages the input and output of text.
void text_handeling(SDL_Renderer *renderer);

//Draw and fill the text box with text.
void text_box(SDL_Renderer *renderer);
#endif
