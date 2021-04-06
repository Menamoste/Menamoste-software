#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include "gui.h"
#include "matrix.h"
#include "tools.h"
#include "resize.h"

const size_t window_width = 1920;
const size_t window_height = 1080;

void cleanResources(SDL_Window *window, SDL_Renderer *renderer,
 SDL_Texture *texture) {
	if (texture  != NULL)
		SDL_DestroyTexture(texture);
	if (renderer != NULL)
		SDL_DestroyRenderer(renderer);
	if (window   != NULL)
		SDL_DestroyWindow(window);
	SDL_Quit();
}

int main () {

	//Data

    	//Box Blur Matrix
    	matrix *convo = matrix_zero(3, 3);
   	matrix_set(convo, 0, 0, 0.1111111);
   	matrix_set(convo, 0, 1, 0.1111111);
    	matrix_set(convo, 0, 2, 0.1111111);
    	matrix_set(convo, 1, 0, 0.1111111);
    	matrix_set(convo, 1, 1, 0.1111111);
    	matrix_set(convo, 1, 2, 0.1111111);
    	matrix_set(convo, 2, 0, 0.1111111);
    	matrix_set(convo, 2, 1, 0.1111111);
    	matrix_set(convo, 2, 2, 0.1111111);

   	//Contrast Matrix
    	matrix *convo2 = matrix_zero(3, 3);
    	matrix_set(convo2, 0, 0, 0);
    	matrix_set(convo2, 0, 1, -1);
    	matrix_set(convo2, 0, 2, 0);
    	matrix_set(convo2, 1, 0, -1);
    	matrix_set(convo2, 1, 1, 5);
    	matrix_set(convo2, 1, 2, -1);
    	matrix_set(convo2, 2, 0, 0);
    	matrix_set(convo2, 2, 1, -1);
    	matrix_set(convo2, 2, 2, 0);

	//Colors for icons
	SDL_Color colors[9];
	set_color(colors, 0, 255, 0  , 0  , 255);
	set_color(colors, 1, 0  , 255, 0  , 255);
	set_color(colors, 2, 0  , 0  , 255, 255);
	set_color(colors, 3, 255, 0  , 255, 255);
	set_color(colors, 4,   0, 255, 255, 255);
	set_color(colors, 5, 255, 255, 0  , 255);
	set_color(colors, 6, 255, 255, 255, 255);
	set_color(colors, 7, 0  , 0  , 255, 255);
	set_color(colors, 8, 127, 127, 255, 255);

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

	//Icons
 	SDL_Rect rects[9]; 
    	size_t x = 1100;
    	for (size_t i  = 0; i < 9; i++)
    	{
		set_rect(&rects[i], x, 100, 64, 64);
		draw_rect(renderer, rects[i], colors[i]);
        	x += 100;
    	}
	
    	size_t y = 300;
    	for (size_t i = 0; i < 4; i++)
    	{
		set_rect(&rects[i], y, 100, 64, 64);
		draw_rect(renderer, rects[i], colors[i]);
        	y += 64;     
    	}

	//Image
	SDL_Surface *image_surface = SDL_LoadBMP("../res/Images/Lenna.bmp");
	if (!image_surface) {
		SDL_Log("Erreur : %s\n", SDL_GetError());
		cleanResources(window, renderer, NULL);
		return -1;
	}

	SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer,
	image_surface);
	if (!image_texture) {
		SDL_Log("Erreur : %s\n", SDL_GetError());
		cleanResources(window, renderer, NULL);
		return -1;
	}

	SDL_FreeSurface(image_surface);
	
	//Set surfaces on renderer.
	//First the image.
	SDL_Rect image_rect = {window_width / 3, window_height / 4, 0, 0};
	SDL_QueryTexture(image_texture, NULL, NULL, &image_rect.w, &image_rect.h);
	SDL_RenderCopy(renderer, image_texture, NULL, &image_rect);

	//Then the icons : 
	//Load the icons on surfaces.
	SDL_Surface *pencil_surface = SDL_LoadBMP("../res/Icons/pensil.bmp");
	SDL_Surface *cursor_surface = SDL_LoadBMP("../res/Icons/cursor.bmp");
	SDL_Surface *eraser_surface = SDL_LoadBMP("../res/Icons/eraser.bmp");
	SDL_Surface *bucket_surface = SDL_LoadBMP("../res/Icons/bucket.bmp");

	//Then put them in textures.
	SDL_Texture *pencil_texture = SDL_CreateTextureFromSurface(renderer,
	pencil_surface);
	SDL_Texture *cursor_texture = SDL_CreateTextureFromSurface(renderer,
	cursor_surface);
	SDL_Texture *eraser_texture = SDL_CreateTextureFromSurface(renderer,
	eraser_surface);
	SDL_Texture *bucket_texture = SDL_CreateTextureFromSurface(renderer,
	bucket_surface);
	//Free the surfaces.
	SDL_FreeSurface(pencil_surface);
	SDL_FreeSurface(cursor_surface);
	SDL_FreeSurface(eraser_surface);
	SDL_FreeSurface(bucket_surface);
	//And finally copy them where they belong.
	SDL_RenderCopy(renderer, pencil_texture, NULL, &rects[0]);
	SDL_RenderCopy(renderer, cursor_texture, NULL, &rects[1]);
	SDL_RenderCopy(renderer, eraser_texture, NULL, &rects[2]);
	SDL_RenderCopy(renderer, bucket_texture, NULL, &rects[3]);
	//Show the result.
	SDL_RenderPresent(renderer);
	//Detection of mouse click in rect_select
	char opened = 1;
	SDL_Event events;
	int mouse_x = 0;
	int mouse_y = 0;

	//TODO: Fixed icons positions.
        //The display of the rectangle is just a bit above about his
        //real position. So the position needs to be fixed.

        //Event Management
	while (opened) {
		while(SDL_PollEvent(&events)) {
			switch (events.type) {
				case SDL_QUIT:
					opened--;
					break;
				case SDL_MOUSEBUTTONDOWN:
					SDL_GetGlobalMouseState(&mouse_x, 
					&mouse_y);
					printf("%i and %i\n", mouse_x, 
					mouse_y);
					//Change detection collision
					/*SDL_Point mouse_pos = {mouse_x, 
					mouse_y};
					if (SDL_PointInRect(&mouse_pos, 
						&rect_select)) {
						printf("PIXEL MODE\n");
					}*/
					break;
			}
		}
	}
	cleanResources(window, renderer, image_texture);
	return 0;	
}
