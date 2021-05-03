#ifndef TTF_H
#define TTF_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Text structure containing text's string, rect, texture, font, color and positions
typedef struct
{
        char *str;
        SDL_Rect rect;
        SDL_Texture *texture;
        TTF_Font *font;
        SDL_Color color;
        int x, y;
} Text;

// Initializes TTF
void init_ttf();

// wait
void wait(int ms);

// Initializes Text 
void init_text(Text *text, char *label, int ptsize, SDL_Color col, int style);

// Sets position for text
void text_set_pos(Text *text, int x, int y);

// Updates position for text1, text2, text3 and path
void update_text_pos(SDL_Renderer *renderer, Text *text1, Text *text2, Text *text3, SDL_Rect rect, int *x, int *y);

// Gets texture and rectangle for char *text
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect, SDL_Color color);

// Adds character c to string txt
void input_txt(char *txt, SDL_Color *color, char c);

// Removes last character from string txt
void rm_last_char(char *txt);

// Manages the uppercases
void put_letter(SDL_Event event, char * path, SDL_Color *color,int capslock, char l);

// Main loop for the path input window
void path_input(SDL_Renderer *renderer0, char *path);

#endif 
