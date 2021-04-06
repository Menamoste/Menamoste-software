#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include "gui.h"

//Constants
const size_t window_width  = 1920;
const size_t window_height = 1080;
const size_t icon_size     = 64;
const size_t nb_rects      = 18;
const size_t nb_colors     = 9;
const size_t nb_icons      = 8;

//SDL Variables
//Stores all the colors.
SDL_Color colors[9];

int main () 
{
	//Data
	//Set colors
	set_color(colors, 0, 255, 0  , 0  , 255);
	set_color(colors, 1, 0  , 255, 0  , 255);
	set_color(colors, 2, 0  , 0  , 255, 255);
	set_color(colors, 3, 255, 0  , 255, 255);
	set_color(colors, 4,   0, 255, 255, 255);
	set_color(colors, 5, 255, 255, 0  , 255);
	set_color(colors, 6, 255, 255, 255, 255);
	set_color(colors, 7, 0  , 0  , 255, 255);
	set_color(colors, 8, 127, 127, 255, 255);

	//Matrices
   	//Contrast Matrix
    	matrix *convo = matrix_zero(3, 3);
    	matrix_set(convo, 0, 0, 0);
    	matrix_set(convo, 0, 1, -1);
    	matrix_set(convo, 0, 2, 0);
    	matrix_set(convo, 1, 0, -1);
    	matrix_set(convo, 1, 1, 5);
    	matrix_set(convo, 1, 2, -1);
    	matrix_set(convo, 2, 0, 0);
    	matrix_set(convo, 2, 1, -1);
    	matrix_set(convo, 2, 2, 0);

    	//Box Blur Matrix
    	matrix *convo2 = matrix_zero(3, 3);
   	matrix_set(convo2, 0, 0, 0.1111111);
   	matrix_set(convo2, 0, 1, 0.1111111);
    	matrix_set(convo2, 0, 2, 0.1111111);
    	matrix_set(convo2, 1, 0, 0.1111111);
    	matrix_set(convo2, 1, 1, 0.1111111);
    	matrix_set(convo2, 1, 2, 0.1111111);
    	matrix_set(convo2, 2, 0, 0.1111111);
    	matrix_set(convo2, 2, 1, 0.1111111);
    	matrix_set(convo2, 2, 2, 0.1111111);


	//Program
	//Initialisation
	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		SDL_Log("Erreur > %s\n", SDL_GetError());
		cleanResources(NULL, NULL, NULL);
		return -1;
	}

	SDL_Window *window = SDL_CreateWindow("Menamoste Image Editor", 
	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width,
	window_height, 0);
	if (!window) {
		SDL_Log("Erreur : %s\n", SDL_GetError());
		cleanResources(NULL, NULL, NULL);
		return -1;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 
	SDL_RENDERER_SOFTWARE);
	if (!renderer) {
		SDL_Log("Erreur : %s\n", SDL_GetError());
		cleanResources(window, NULL, NULL);
		return -1;
	}

	//BG
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	//Stores all the surfaces.
	SDL_Rect rects[nb_rects];

	//Icons	
	//Icons management
	place_rects(rects); 

	//Draw the color icons
    	for (size_t i = nb_icons; i < nb_rects; i++)
		draw_rect(renderer, rects[i], colors[i - nb_icons]);

	SDL_Rect *image_rect = &rects[nb_rects -  1];
	//Print the screen.
	print_image(window, renderer, image_rect, convo, 0);

	//Then the icons.
	load_rects(renderer, rects);

	//Show the result.
	SDL_RenderPresent(renderer);
	
	//Event Management
	char opened = 1;
	char is_pencil = 0;
	SDL_Event events;
	int mouse_x = 0;
	int mouse_y = 0;
	while (opened) {
		while(SDL_PollEvent(&events)) {
			switch (events.type) {
				case SDL_QUIT:
					opened--;
					break;
				case SDL_MOUSEBUTTONDOWN:
					SDL_GetGlobalMouseState(&mouse_x, 
					&mouse_y);
					//Change detection collision
					SDL_Point mouse_pos = {mouse_x, 
					mouse_y};
					//Pencil
					if (SDL_PointInRect(&mouse_pos, 
						&rects[0])) {
						is_pencil++;
					}
					//Bucket
					//Clear board
					if (SDL_PointInRect(&mouse_pos, 
						&rects[3])) {
						print_image(window, renderer, 
						image_rect, convo, 0);
						is_pencil = 0;
					}
					//Filter
					if (SDL_PointInRect(&mouse_pos, 
					        &rects[4])) {
						print_image(window, renderer, 
						image_rect, convo, 1);
						is_pencil = 0;
					}
					//Resize
					if (SDL_PointInRect(&mouse_pos, 
						&rects[6])) {
						print_image(window, renderer, 
						image_rect, convo, 2);
						is_pencil = 0;
					}
					//Rotate
					if (SDL_PointInRect(&mouse_pos, 
						&rects[7])) {
						print_image(window, renderer, 
						image_rect, convo2, 3);
						is_pencil = 0;
					}
					//Image
					if (SDL_PointInRect(&mouse_pos, 
						image_rect)) {
						printf("test\n");
						is_pencil++;
					}
					break;
			}
		}
	}
	cleanResources(window, renderer, NULL);
	return 0;	
}
