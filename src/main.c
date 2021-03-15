#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include "matrix.h"
#include "tools.h"

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
	//Initialisation
	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		SDL_Log("Erreur > %s\n", SDL_GetError());
		cleanResources(NULL, NULL, NULL);
		return -1;
	}

	SDL_Window *window = SDL_CreateWindow("Affichage", 
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
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_Rect rect_select = {100, 100, 20, 20};
	SDL_RenderFillRect(renderer, &rect_select);
	SDL_RenderPresent(renderer);

	//Image
	SDL_Surface *image_surface = SDL_LoadBMP("../res/Lenna.bmp");
	if (!image_surface) {
		SDL_Log("Erreur : %s\n", SDL_GetError());
		//cleanResources(window, renderer, NULL);
		return -1;
	}
	
	//Test Matrices
	//matrix_pack *mat_pack = sur_to_mat_pack(image_surface);
	//mat_pack_to_sur(image_surface, mat_pack);
	//Fin test matrices

	SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer,
	image_surface);
	if (!image_texture) {
		SDL_Log("Erreur : %s\n", SDL_GetError());
		cleanResources(window, renderer, NULL);
		return -1;
	}

	size_t image_width = image_surface->w;
	size_t image_height = image_surface->h;

	SDL_FreeSurface(image_surface);
	SDL_Rect rect = {window_width / 3, window_height / 4, image_width,
	image_height};
	SDL_QueryTexture(image_texture, NULL, NULL, &rect.w, &rect.h);
	SDL_RenderCopy(renderer, image_texture, NULL, &rect);
	SDL_RenderPresent(renderer);

	//Detection of mouse click in rect_select
	char opened = 1;
	SDL_Event events;
	int mouse_x = 0;
	int mouse_y = 0;
        //The display of the rectangle is just a bit above about his
        //real position. So the position needs to be fixed.
        rect_select.y -= 20;
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
					SDL_Point mouse_pos = {mouse_x, 
					mouse_y};
					if (SDL_PointInRect(&mouse_pos, 
						&rect_select)) {
						printf("PIXEL MODE\n");
					}
					break;
			}
		}
	}
	cleanResources(window, renderer, image_texture);
	return 0;	
}
