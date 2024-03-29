#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "gui.h"
#include "tools.h"
#include "color.h"
#include "path.h"
#include "selection.h"

#define PATH_MAX_LENGTH 35

//Constants
const size_t window0_width = 600;
const size_t window0_height = 200;
const size_t window_width = 1920;
const size_t window_height = 1080;
const size_t icon_size = 64;
const size_t nb_rects = 18;
const size_t nb_colors = 9;
const size_t nb_icons = 8;
const size_t path_max_length = PATH_MAX_LENGTH;

//SDL Variables
//Stores all the colors.
SDL_Color colors[9];

void color_to_trip(SDL_Color color, triplet *trip)
{
	float r_val = (float)color.r;
	r_val /= 255.0;
	float g_val = (float)color.g;
	g_val /= 255.0;
	float b_val = (float)color.b;
	b_val /= 255.0;
	trip->r = r_val;
	trip->g = g_val;
	trip->b = b_val;
}
int main()
{
	//Data
	//Set colors
	set_color(colors, 0, 255, 0, 0, 255);
	set_color(colors, 1, 0, 255, 0, 255);
	set_color(colors, 2, 0, 0, 255, 255);
	set_color(colors, 3, 255, 0, 255, 255);
	set_color(colors, 4, 0, 255, 255, 255);
	set_color(colors, 5, 255, 255, 0, 255);
	set_color(colors, 6, 255, 255, 255, 255);
	set_color(colors, 7, 127, 127, 127, 255);
	set_color(colors, 8, 0, 0, 0, 255);

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
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		SDL_Log("Erreur > %s\n", SDL_GetError());
		cleanResources(NULL, NULL, NULL);
		return -1;
	}

	//Initialisation of the TTF
	clean_init();

	SDL_Window *window = SDL_CreateWindow("Menamoste Image Editor",
										  SDL_WINDOWPOS_CENTERED, 
										  SDL_WINDOWPOS_CENTERED, window_width,
										  window_height, 0);
	if (!window)
	{
		SDL_Log("Erreur : %s\n", SDL_GetError());
		cleanResources(NULL, NULL, NULL);
		return -1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
												SDL_RENDERER_SOFTWARE);
	if (!renderer)
	{
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
	for (size_t i = nb_icons; i < nb_rects - 1; i++)
		draw_rect(renderer, rects[i], colors[i - nb_icons]);

	//Then the icons.
	load_rects(renderer, rects);

	//Print the result on the renderer.
	SDL_RenderPresent(renderer);

    //Input image
	SDL_Rect path_rect = {0, 20, 1000, 60};
	SDL_Surface *image_surface = NULL;
    SDL_Rect bar = {1100, 10, 800, 60};
    char text[] = "Impossible de charger l'image";
	char text2[] = "L'image a bien pu etre chargee";
	while (!image_surface)
	{
	    //Get the image's surface.
	    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	    char *path = text_box(renderer, path_rect, 100);
	    image_surface = SDL_LoadBMP(path);
            free(path);
	    if (!image_surface)
        	print_message(text, renderer, bar, 1);
	}

	//Get the matrix
	matrix_pack *mat_pack = sur_to_mat_pack(image_surface);

	//Set the image coordinates.
	SDL_Rect *image_rect = &rects[nb_rects - 1];
	set_rect(image_rect, window_width / 3, window_height / 4, 0, 0);

	//Print the image.
	print_image(renderer, image_rect, image_surface, mat_pack);

        //Print Successful message.
        print_message(text2, renderer, bar, 0);
	SDL_Delay(2000);
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        fill_text_box(renderer, bar);
        fill_text_box(renderer, bar);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	//Event Management

	char opened = 1;
	char is_resized = 0;
	char is_pencil = 0;
	char is_selection = 0;
	size_t is_selected = 0;
	int mouse_x = 0;
	int mouse_y = 0;
	SDL_Event events;
	//Color of the pencil
	triplet trip = {1.0f, 0.0f, 0.0f};
	//Coordinates
	size_t x1 = 0;
	size_t y1 = 0;
	size_t x2 = 0;
	size_t y2 = 0; 
	while (opened)
	{
		while (SDL_PollEvent(&events))
		{
			switch (events.type)
			{
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
									&rects[0]))
				{
					is_pencil = 1;
				}
				//Filter
				if (SDL_PointInRect(&mouse_pos,
									&rects[4]))
				{
					if (is_resized == 0)
					{
						matrix_pack *mat_pack2 =
							modify_image(mat_pack,
										 convo, 1);
						print_image(renderer,
									image_rect,
									image_surface,
									mat_pack2);
						is_pencil = 0;
					}
				}
				//Selection
				if (SDL_PointInRect(&mouse_pos,
									&rects[5]))
				{
					is_selection = 1;
				}
				//Resize
				if (SDL_PointInRect(&mouse_pos,
									&rects[6]))
				{
					if (is_resized == 0)
					{
						SDL_Rect arg1_rect = {1700, 400, 200, 50};	
						SDL_Rect arg2_rect = {1700, 600, 200, 50};
                                                fill_text_box(renderer, arg2_rect);

						char *arg1 = text_box(renderer, arg1_rect, 5);
						char *arg2 = text_box(renderer, arg2_rect, 5);
                                                int new_h = atoi(arg1);
                                                int new_w = atoi(arg2);

						matrix_pack *mat_pack2 = 
                                                resize_image(mat_pack, new_w, new_h);

						SDL_Color color[1];
						set_color(color, 0, 100, 100, 100, 0);

						draw_rect(renderer, *image_rect, color[0]);

						image_surface =
							SDL_CreateRGBSurface(0,
												 mat_pack2->r->cols,
												 mat_pack2->r->rows,
												 32, 0, 0, 0, 0);
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
									&rects[7]))
				{
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
									image_rect))
				{
					if (is_pencil == 1)
					{
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

					if (is_selection)
					{
						x1 = 0;
						y1 = 0;

						SDL_GetGlobalMouseState(&mouse_x, 
						&mouse_y);
						while ((!x1 && !y1))
						{
							if (SDL_MOUSEBUTTONDOWN)
							{
									x1 = mouse_x - window_width/3;
									y1 = mouse_y - window_height/4;
							}
						}
					}
				}
				//Color red
				if (SDL_PointInRect(&mouse_pos,
									&rects[nb_icons]))
				{
					color_to_trip(colors[0], &trip);
				}
				//Color green
				if (SDL_PointInRect(&mouse_pos,
									&rects[nb_icons + 1]))
				{
					color_to_trip(colors[1], &trip);
				}
				//Color dark blue
				if (SDL_PointInRect(&mouse_pos,
									&rects[nb_icons + 2]))
				{
					color_to_trip(colors[2], &trip);
				}
				//Color magenta
				if (SDL_PointInRect(&mouse_pos,
									&rects[nb_icons + 3]))
				{
					color_to_trip(colors[3], &trip);
				}
				//Color light blue
				if (SDL_PointInRect(&mouse_pos,
									&rects[nb_icons + 4]))
				{
					color_to_trip(colors[4], &trip);
				}
				//Color yellow
				if (SDL_PointInRect(&mouse_pos,
									&rects[nb_icons + 5]))
				{
					color_to_trip(colors[5], &trip);
				}
				//Color white
				if (SDL_PointInRect(&mouse_pos,
									&rects[nb_icons + 6]))
				{
					color_to_trip(colors[6], &trip);
				}
				//Color dark blue
				if (SDL_PointInRect(&mouse_pos,
									&rects[nb_icons + 7]))
				{
					color_to_trip(colors[7], &trip);
				}
				//Color black
				if (SDL_PointInRect(&mouse_pos,
									&rects[nb_icons + 8]))
				{
					color_to_trip(colors[8], &trip);
				}
				break;
			
			case SDL_MOUSEBUTTONUP:
					SDL_GetGlobalMouseState(&mouse_x, 
					&mouse_y);
					//Image
					if (SDL_PointInRect(&mouse_pos, 
						image_rect)) {                        
						if (is_selection)
						{
							x2 = 0;
							y2 = 0;
							while ((!x2 && !y2))
							{	
								if (SDL_MOUSEBUTTONUP)
								{
									x2 = mouse_x - window_width/3;
									y2 = mouse_y - window_height/4;
									
									SDL_Color color[1];
									set_color(color, 0, 
																	100, 100, 100, 0);

									draw_rect(renderer, 
																		*image_rect, 
																		color[0]);
									matrix_pack* mat_pack2 = select(mat_pack, x1, y1, x2, y2, &is_selected);

									image_surface =
									SDL_CreateRGBSurface(0,
																mat_pack2->r->cols,
																mat_pack2->r->rows,
																32, 0, 0, 0, 0);

									print_image(renderer,
												image_rect,
												image_surface,
												mat_pack2);

									is_pencil = 0;
									is_selection = 0;
								}
							}
						}						
					}
				break;
			}

		}
	}
	cleanResources(window, renderer, NULL);
	TTF_Quit();
	return 0;
}
