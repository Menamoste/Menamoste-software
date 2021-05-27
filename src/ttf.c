#include "ttf.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <err.h>
#include "gui.h"
#include <ctype.h>

extern const size_t window_width;
extern const size_t window_height;
extern const size_t window0_width;
extern const size_t window0_height;
extern const size_t path_max_length;

void init_ttf()
{
    TTF_Init();

    if (TTF_Init() == -1)
    {
        fprintf(stderr, "Couldn't initialize TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

void wait(int ms)
{
	SDL_Delay(ms);
}

void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect, SDL_Color color) {
    int text_w;
    int text_h;
    SDL_Surface *surface;
    
    if (strlen(text) < 1)
        surface = TTF_RenderText_Blended(font, " ", color);
    else
        surface = TTF_RenderText_Blended(font, text, color);

    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_w = surface->w;
    text_h = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_w;
    rect->h = text_h;
}

void input_txt(char *txt, SDL_Color *color, char c)
{
    if (strlen(txt) < path_max_length)
    {
        SDL_Color black = {0, 0, 0, 0};
        *color = black;
        txt[strlen(txt)] = c;
        txt[strlen(txt) + 1] = '\0';
    }
}

void rm_last_char(char *txt)
{
    if (strlen(txt))
        txt[strlen(txt) - 1] = '\0';
}


void put_letter(SDL_Event event, char *path, SDL_Color *color, int capslock, char l)
{
    if (event.key.keysym.mod & KMOD_LSHIFT || capslock)         	
        l = toupper(l);
    input_txt(path, color, l);
}

void init_text(Text *text, char *label, int ptsize, SDL_Color col, int style)
{	
	char *font_path = "../res/Fonts/arial.ttf";

	text->str = label;
	text->font = TTF_OpenFont(font_path, ptsize);
	text->color = col;

	if (!text->font)
	{
		fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	TTF_SetFontStyle(text->font, style);
}

void text_set_pos(Text *text, int x, int y)
{
	text->x = x;
	text->y = y;
}

void update_text_pos(SDL_Renderer *renderer, Text *text1, Text *text2, 
Text *text3, SDL_Rect rect, int *x, int *y)
{
	// PATH
	*x = (window0_width - rect.w)/2;
	*y = (window0_height - rect.h)/2;

	// TITLE
	text_set_pos(text1, 
	/*x*/		(window0_width - text1->rect.w)/2,      
	/*y*/		0);				

	// FILE NOT FOUND						 
	text_set_pos(text2, 
	/*x*/		(window0_width - text2->rect.w)/2,    
	/*y*/		(.9*window0_height - text2->rect.h));   

	// OPENING FILE
	text_set_pos(text3,
	/*x*/		(window0_width - text3->rect.w)/2,    
	/*y*/		(.9*window0_height - text3->rect.h));

	get_text_and_rect(renderer, text1->x, text1->y, text1->str, text1->font,
	 							&text1->texture, &text1->rect, text1->color);
	get_text_and_rect(renderer, text2->x, text2->y, text2->str, text2->font,
								&text2->texture, &text2->rect, text2->color);
	get_text_and_rect(renderer, text3->x, text3->y, text3->str, text3->font,
								&text3->texture, &text3->rect, text3->color);
}


void path_input(SDL_Renderer *renderer, char *path)
{
	int quit, capslock;
	// Used colors
    SDL_Color black = {0  , 0  , 0  , 0};
	SDL_Color   red = {200, 0  , 0  , 0};
	SDL_Color green = {0  , 200, 0  , 0};
    SDL_Color  gray = {100, 100, 100, 0};
    
	SDL_Surface *image;
    SDL_Event event;

	// path data
	SDL_Rect rect_path;
	SDL_Texture *t_path;
	SDL_Color col_path;
	TTF_Font *f_path;
	int x_path, y_path;

	// texts
	Text title, file_nf, opening_f;
    
	// initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
	// initialize TTF
	init_ttf();

	f_path = TTF_OpenFont("../res/Fonts/arial.ttf", 20);

	// text data
	init_text(&title, "Adresse du fichier (Enter pour valider)",
	 							24, black, TTF_STYLE_UNDERLINE);
	init_text(&file_nf, "fichier introuvable !",
								20, black, TTF_STYLE_ITALIC);
	init_text(&opening_f, "Ouverture du fichier...",
								20, black, TTF_STYLE_ITALIC);

	// BG
	SDL_SetRenderDrawColor(renderer, gray.r, gray.g, gray.b, gray.a);
	
	// loop data
	capslock = 0;
	quit = 0;

	// calling update_text_pos twice to prevent title misplacement
	update_text_pos(renderer, &title, &file_nf,&opening_f,rect_path, &x_path, &y_path);
	update_text_pos(renderer, &title, &file_nf, &opening_f, rect_path, &x_path, &y_path);
	get_text_and_rect(renderer, x_path, y_path, path, f_path, &t_path, &rect_path, col_path);

	while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
                case SDL_QUIT:
                    exit(EXIT_SUCCESS);
                    break;
                
                case SDL_KEYDOWN:
                    // CTRL to paste text from clipboard
                    if (SDL_HasClipboardText() && event.key.keysym.mod && 
							KMOD_CTRL)
                    {
                        path = SDL_GetClipboardText();
                        get_text_and_rect(renderer, x_path, y_path, path, 
						f_path, &t_path, &rect_path, col_path);
                    }
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:	//ESC
                            exit(EXIT_SUCCESS);
                            break;

						case SDLK_CAPSLOCK: 
							if (!capslock)
								capslock++; 
							else	
								capslock--;
							break;
								
						case SDLK_RETURN:	//ENTER

							SDL_RenderClear(renderer); 
							
							// to display title
							SDL_RenderCopy(renderer, title.texture, NULL, 
							&title.rect);
							
							// load SDL_Surface* at path
							image = SDL_LoadBMP(path); 

							if(image)	// path correct
							{
								// set text color to green
								col_path = green;
								// update texts 
								update_text_pos(renderer, &title, &file_nf, 
								&opening_f, rect_path, &x_path, &y_path);
								get_text_and_rect(renderer, x_path, y_path, 
								path, f_path, &t_path, &rect_path, col_path);
								// to display path
								SDL_RenderCopy(renderer, t_path, NULL, 
								&rect_path);
								// to display "Opening file..."
								SDL_RenderCopy(renderer, opening_f.texture, 
								NULL, &opening_f.rect);
								// display on renderer
								SDL_RenderPresent(renderer);
								// waits for 2.5 secs
								wait(2500);
								// exit main loop
								quit++;
							}
							else	// path incorrect
							{
								// set text color to red
								col_path = red;
								
								// update texts positions
								update_text_pos(renderer, &title, &file_nf, 
								&opening_f, rect_path, &x_path, &y_path);
								get_text_and_rect(renderer, x_path, y_path, 
								path, f_path, &t_path, &rect_path, col_path);
								// to display path
								SDL_RenderCopy(renderer, t_path, NULL, 
								&rect_path);
								// to display "file not found !"
								SDL_RenderCopy(renderer, file_nf.texture, 
								NULL, &file_nf.rect);
								// display on renderer
								SDL_RenderPresent(renderer);
								// waits for 1.5 secs
								wait(1500);
							}
						
							// free image
							SDL_FreeSurface(image);
							break;

						case SDLK_BACKSPACE:
							rm_last_char(path);
							col_path = black;
							break;
                    }


					
					// updates text positions
					update_text_pos(renderer, &title, &file_nf, &opening_f, 
					rect_path, &x_path, &y_path);
					get_text_and_rect(renderer, x_path, y_path, path, f_path, 
					&t_path, &rect_path, col_path);
					break;
            }
        }

		// displays texts
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, title.texture, NULL, &title.rect);
		SDL_RenderCopy(renderer, t_path, NULL, &rect_path);		
		SDL_RenderPresent(renderer);
    }

	cleanResources(NULL, NULL, title.texture);
	cleanResources(NULL, NULL, t_path);
	cleanResources(NULL, NULL, file_nf.texture);
	cleanResources(NULL, NULL, opening_f.texture);
	
	TTF_CloseFont(title.font);
	TTF_CloseFont(f_path);
	TTF_CloseFont(file_nf.font);
    
	TTF_Quit();
}
