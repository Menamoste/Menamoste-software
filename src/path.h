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

// Writes a message.
void print_message(char *text, SDL_Renderer *renderer, SDL_Rect bar, int error);

// Writes the text.
void print_text(char *text, SDL_Renderer *renderer, TTF_Font *font,
SDL_Rect bar);

//Manages the input and output of text.
char *text_handeling(SDL_Renderer *renderer, SDL_Rect bar, size_t len_max);

//Draw and fill the text box with text.
char *text_box(SDL_Renderer *renderer, SDL_Rect bar, size_t len_max);
#endif
