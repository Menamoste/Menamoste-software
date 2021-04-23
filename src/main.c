#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include "gui.h"
#include "tools.h"
#include "color.h"

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

	//WARNING


	//Then the icons.
	load_rects(renderer, rects);

	//Print the result on the renderer.
	SDL_RenderPresent(renderer);

	//Get the image's surface.
	SDL_Surface *image_surface = SDL_LoadBMP("../res/Images/Lenna.bmp");
	if (!image_surface) 
	{
		SDL_Log("Erreur : %s\n", SDL_GetError());
		cleanResources(window, renderer, NULL);
		return -1;
	}

	//Get the matrix
	matrix_pack *mat_pack = sur_to_mat_pack(image_surface);

	//Set the image coordinates.
	SDL_Rect *image_rect = &rects[nb_rects - 1];
	set_rect(image_rect, window_width / 3, window_height / 4, 0, 0);

	//Print the image.
	print_image(renderer, image_rect, image_surface, mat_pack);



	//END OF WARNING

	//Event Management
	char opened = 1;
	char is_resized = 0;
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
						printf("pencil\n");
						is_pencil++;
					}
					//Bucket
					//Clear board
					if (SDL_PointInRect(&mouse_pos, 
						&rects[3])) 
						{
						printf("bucket\n");
						/*matrix_pack *mat_pack2 = 
						modify_image(mat_pack, convo, 
						4);
						print_image(renderer, 
						image_rect, image_surface, 
						mat_pack2);
						is_resized = 0;
						is_pencil = 0;*/
					}
					//Filter
					if (SDL_PointInRect(&mouse_pos, 
					        &rects[4])) {
						printf("filter\n");
						if (is_resized == 0) 
						{
							matrix_pack *mat_pack2= 
							modify_image(mat_pack, 
							convo, 1);
							print_image(renderer, 
							image_rect, 
							image_surface, 
							mat_pack2);
							is_pencil = 0;
						}
					}
					//Resize
					if (SDL_PointInRect(&mouse_pos, 
						&rects[6])) {
						printf("resize\n");
						if (is_resized == 0)
						{
							matrix_pack *mat_pack2 
							= modify_image(mat_pack
							, convo, 2);
							
							SDL_Color color[1];
							set_color(color, 0, 100, 100, 100, 0);

							draw_rect(renderer, *image_rect, color[0]);

							image_surface = 
							SDL_CreateRGBSurface(0, mat_pack2->r->cols, mat_pack2->r->rows, 32, 0, 0, 0, 0);
							print_image(renderer, 
							image_rect, 
							image_surface, 
							mat_pack2);
							is_pencil = 0;
							is_resized++;
						}
					}
					//Rotate
					if (SDL_PointInRect(&mouse_pos, 
						&rects[7])) {
						printf("rotate\n");
						matrix_pack *mat_pack2 = 
						modify_image(mat_pack, convo, 
						3);
						print_image(renderer, 
						image_rect, image_surface, 
						mat_pack2);
						is_pencil = 0;
					}
					//Image
					if (SDL_PointInRect(&mouse_pos, 
						image_rect)) {
						printf("Try to color\n");
						triplet trip = {1.0f, 0.0f, 0.0f};
						int rel_x = mouse_x - 
						window_width / 3;
						int rel_y = mouse_y - 
						window_height / 4;
						color_pixel(mat_pack, 
						image_surface, trip, rel_x, 
						rel_y);
						print_image(renderer, 
						image_rect, image_surface,
						mat_pack);
					}
					break;
			}
		}
	}
	cleanResources(window, renderer, NULL);
	return 0;	
}
