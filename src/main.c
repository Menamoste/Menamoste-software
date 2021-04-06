#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include "gui.h"
#include "matrix.h"
#include "tools.h"
#include "resize.h"

const size_t window_width = 1920;
const size_t window_height = 1080;
const size_t nb_rects = 17;
const size_t nb_colors = 9;

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

int print_screen(SDL_Window *window, SDL_Renderer *renderer, 
 SDL_Rect *rects, matrix *convo, int mode) {
	//Data
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
	//BG
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	//Draw the color icons
    	for (size_t i = 8; i < nb_rects; i++)
		draw_rect(renderer, rects[i], colors[i + 1 - nb_colors]);

	//Image
	//Mode normal
	SDL_Surface *image_surface = SDL_LoadBMP("../res/Images/Lenna.bmp");
	if (!image_surface) {
		SDL_Log("Erreur : %s\n", SDL_GetError());
		cleanResources(window, renderer, NULL);
		return -1;
	}
	//Mode filter
	if (mode == 1) {
		matrix_pack *mat_pack = sur_to_mat_pack(image_surface);
		convolution(mat_pack, convo);
		mat_pack_to_sur(image_surface, mat_pack);
		mat_pack_free(mat_pack);
	}
	//Mode resize
	//size_t new_size = 512;
	if (mode == 2) {
		image_surface = SDL_LoadBMP("../res/Images/Lenna_resized.bmp");
		matrix_pack *mat_pack  = sur_to_mat_pack(image_surface);
		matrix_pack *mat_pack2 = resize(mat_pack, 768, 768);
		mat_pack_free(mat_pack);
		mat_pack_free(mat_pack2);
	}

	//Mode rotate
	if (mode == 3) {
		matrix_pack *mat_pack  = sur_to_mat_pack(image_surface);
		matrix_pack *mat_pack2 = rotation(mat_pack, 45);
		convolution(mat_pack2, convo);
		mat_pack_to_sur(image_surface, mat_pack2);
		mat_pack_free(mat_pack);
		mat_pack_free(mat_pack2);
	}
	//Mode color
	if (mode == 4) {
		matrix_pack *mat_pack = sur_to_mat_pack(image_surface);
		mat_pack_to_sur(image_surface, mat_pack);
		mat_pack_free(mat_pack);
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
	set_rect(&rects[nb_rects], window_width / 3, window_height / 4, 0, 0);
	SDL_QueryTexture(image_texture, NULL, NULL, &rects[nb_rects].w,
	 &rects[nb_rects].h);
	if (mode == 2) {
		//rects[nb_rects].w = new_size;
		//rects[nb_rects].h = new_size;
	}
	SDL_RenderCopy(renderer, image_texture, NULL, &rects[nb_rects]);

	//Then the icons :
	//Load the icons on surfaces.
	SDL_Surface *pencil_surface = SDL_LoadBMP("../res/Icons/pensil.bmp");
	SDL_Surface *cursor_surface = SDL_LoadBMP("../res/Icons/cursor.bmp");
	SDL_Surface *eraser_surface = SDL_LoadBMP("../res/Icons/eraser.bmp");
	SDL_Surface *bucket_surface = SDL_LoadBMP("../res/Icons/bucket.bmp");
	SDL_Surface *filter_surface = SDL_LoadBMP("../res/Icons/filter.bmp");
	SDL_Surface *group_surface  = SDL_LoadBMP("../res/Icons/group.bmp");
	SDL_Surface *resize_surface = SDL_LoadBMP("../res/Icons/resize.bmp");
	SDL_Surface *rotate_surface = SDL_LoadBMP("../res/Icons/rotate.bmp");
	//Then put them in textures.
	SDL_Texture *pencil_texture = SDL_CreateTextureFromSurface(renderer,
	pencil_surface);
	SDL_Texture *cursor_texture = SDL_CreateTextureFromSurface(renderer,
	cursor_surface);
	SDL_Texture *eraser_texture = SDL_CreateTextureFromSurface(renderer,
	eraser_surface);
	SDL_Texture *bucket_texture = SDL_CreateTextureFromSurface(renderer,
	bucket_surface);
	SDL_Texture *filter_texture = SDL_CreateTextureFromSurface(renderer,
	filter_surface);
	SDL_Texture *group_texture  = SDL_CreateTextureFromSurface(renderer,
	group_surface);
        SDL_Texture *resize_texture = SDL_CreateTextureFromSurface(renderer,
	resize_surface);
	SDL_Texture *rotate_texture = SDL_CreateTextureFromSurface(renderer,
	rotate_surface);
	//Free the surfaces.
	SDL_FreeSurface(pencil_surface);
	SDL_FreeSurface(cursor_surface);
	SDL_FreeSurface(eraser_surface);
	SDL_FreeSurface(bucket_surface);
	SDL_FreeSurface(filter_surface);
	SDL_FreeSurface(group_surface) ;
	SDL_FreeSurface(resize_surface);
	SDL_FreeSurface(rotate_surface);
	//And finally copy them where they belong.
	SDL_RenderCopy(renderer, pencil_texture, NULL, &rects[0]);
	SDL_RenderCopy(renderer, cursor_texture, NULL, &rects[1]);
	SDL_RenderCopy(renderer, eraser_texture, NULL, &rects[2]);
	SDL_RenderCopy(renderer, bucket_texture, NULL, &rects[3]);
	SDL_RenderCopy(renderer, filter_texture, NULL, &rects[4]);
	SDL_RenderCopy(renderer, group_texture , NULL, &rects[5]);
	SDL_RenderCopy(renderer, resize_texture, NULL, &rects[6]);
	SDL_RenderCopy(renderer, rotate_texture, NULL, &rects[7]);
	//Show the result.
	SDL_RenderPresent(renderer);
	return 0;
}

int main () {
	//Data

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
	//Icons management
 	SDL_Rect rects[nb_rects + 1]; 
        size_t x = 50;

	//Icons images
    	for (size_t i = 0; i < 8; i++)
    	{
		set_rect(&rects[i], x, 100, 64, 64);
        	x += 85;     
    	}
        
        x += 250;
        //Icons colors
    	for (size_t i = 8; i < nb_rects; i++)
    	{
		set_rect(&rects[i], x, 100, 64, 64);
        	x += 100;
    	}

	//Print the screen
	print_screen(window, renderer, rects, convo, 0);
	
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
					if (SDL_PointInRect(&mouse_pos, 
						&rects[3])) {
						print_screen(window, renderer, 
						rects, convo, 0);
					}
					//Filter
					if (SDL_PointInRect(&mouse_pos, 
					        &rects[4])) {
						print_screen(window, renderer, 
						rects, convo, 1);
						is_pencil = 0;
					}
					//Resize
					if (SDL_PointInRect(&mouse_pos, 
						&rects[6])) {
						print_screen(window, renderer, 
						rects, convo, 2);
						is_pencil = 0;
					}
					//Rotate
					if (SDL_PointInRect(&mouse_pos, 
						&rects[7])) {
						print_screen(window, renderer, 
						rects, convo2, 3);
						is_pencil = 0;
					}
					break;
			}
		}
	}
	cleanResources(window, renderer, NULL);
	return 0;	
}
